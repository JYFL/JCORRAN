#include "TH1D.h"
#include "TProfile.h"
#include "TMath.h"
#include "TFile.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <Pythia.h>
#include <TStopwatch.h>

#include <TClonesArray.h>

#include "src/AliJConst.h"
#include "src/AliJBaseTrack.h"
#include "src/AliJCard.h"
#include "src/JParticleTools.h"
#include "src/JPythiaDataManager.h"
#include "src/JPythiaHistos.h"
#include "src/AliJHistos.h"
#include "src/AliJCorrelations.h"
#include "src/AliJDiJetAnalysis.h"
#include "src/Sigma1GenRes.h"
#include "src/AliJPythiaTool.h"
#include <TTree.h>

using namespace Pythia8; 

void scaleNotEquidistantHisto(TH1D *hid, double sc);
double DeltaPhi(double dphi);

int main(int argc, char **argv) {

    if(argc<4){
        cout<<"usage: iaa_pythia <card.input> <jet.config> <outdile.root> [random_seed]"<<endl;
        cout<<"example 1) Normal pythia     : ./pythiaJet cardAlice_pp.input card_jet.config config.cmnd t.root"<<endl;
        cout<<"example 2) Resornance pythia : ./pythiaJet cardAlice_pp.input card_jet.config SimpleGluonDijet_200GeV.cmnd t.root"<<endl;
        cout<<"\"DoResonancePythia\" in card should be managed"<<endl;
        exit(1);
    }
    TStopwatch timer; // BS timer
    timer.Start();    // BS

    char* cardName  = argv[1];
    char* jetconfig = argv[2];
    char* pythiaconfig = argv[3];
    AliJCard *card = new AliJCard(cardName);
    card->PrintOut();
    int fDoJetHadronCorrelation = card->Get("DoJetHadronCorrelation");

    AliJBaseCard *jetcard = new AliJBaseCard(jetconfig);
    jetcard->PrintOut();

    double jetEtaCut = jetcard->Get("Jet:EtaRange");


    TH1D::StatOverflows();
    TH2D::StatOverflows();
    TH1D::SetDefaultSumw2();
    TH2D::SetDefaultSumw2();

    TString outputs = argv[4];
    Int_t random_seed = argc>5 ? atoi(argv[5]) : 0;

    TFile * foutTree = new TFile( outputs+".tree.root", "RECREATE");
    TTree * tree = new TTree("JetTree","Jets");

    TFile *fout = new TFile(outputs.Data(),"RECREATE");
    fout->cd();


    //---------------------
    //Pythia initialization 
    //---------------------
    Pythia pythia;   // Generator.
    Event& event      = pythia.event;
    ParticleData& pdt = pythia.particleData;
    Sigma1GenRes *fsigma1GenRes;
    fsigma1GenRes = NULL;


    // Read in commands from external file.
    if(card->Get("DoResonancePythia")==1) {
        // A class to generate the fictitious resonance initial state.
        fsigma1GenRes = new Sigma1GenRes();
        // Hand pointer to Pythia.
        pythia.setSigmaPtr( fsigma1GenRes );
    }
    pythia.readFile(pythiaconfig);   

    // Extract settings to be used in the main program.
    int    nEvent  = pythia.mode("Main:numberOfEvents");
    int    nShow   = pythia.mode("Main:timesToShow");
    bool   showCS  = pythia.flag("Main:showChangedSettings");
    bool   showCPD = pythia.flag("Main:showChangedParticleData");

    cout<<"Events="<<nEvent <<" RNDM seed "<< random_seed << endl;
    int ieout = nEvent/20;
    if (ieout<1) ieout=1;

    pythia.readString(Form("Random:seed=%02d",random_seed));

    // Initialize. Beam parameters set in .cmnd file.
    pythia.init();

    // List changed data.
    if (showCS)  pythia.settings.listChanged();
    if (showCPD) pdt.listChanged();

    //-------------------------------------------------------
    // Histograms and tools
    //-------------------------------------------------------
    AliJHistos *histos = new AliJHistos( card );
    histos->CreateEventTrackHistos();
    //histos->CreateAzimuthCorrHistos();
    //histos->CreateXEHistos();
    //histos->CreateXtHistos();
    //histos->CreateIAAMoons();
    histos->CreateJetHistos();

    JPythiaHistos *pyhistos = new JPythiaHistos(card);
    pyhistos->CreatePythiaHistos();
    JPythiaDataManager *dmg  = new JPythiaDataManager(card, pyhistos);

    //-------------------------------------------------------
    // Correlation
    //-------------------------------------------------------
    AliJCorrelations * fcorrelations = new AliJCorrelations( card, histos);
    fcorrelations->SetPyHistos( pyhistos );


    TClonesArray *inputList  = new TClonesArray("AliJBaseTrack",1500);
    //------------------------------------------------------------------
    // Define jet reconstruction
    //------------------------------------------------------------------

    const int Njetfinder = 6;
    AliJJetFinder *triggJet[Njetfinder]; // 0:Full 1:Full08 2:Charged 3:Charged08

    int nPrintCount = 0;
    for( int i=0;i<Njetfinder;i++ ){
        triggJet[i] = new AliJJetFinder(jetcard);
        triggJet[i]->PrintConfig("Jet Config");
        tree->Branch(Form("Jet%02d",i),triggJet[i]->GetJetList(),32000,2 );
    }
    triggJet[0]->SetRCharged( 0.4, 0);
    triggJet[1]->SetRCharged( 0.5, 0);
    triggJet[2]->SetRCharged( 0.6, 0);
    triggJet[3]->SetRCharged( 0.4, 1);
    triggJet[4]->SetRCharged( 0.5, 1);
    triggJet[5]->SetRCharged( 0.6, 1);

    //--------------------------------------------------------
    //         B e g i n    e v e n t    l o o p.
    //--------------------------------------------------------
    cout<<"Let's start" <<endl; 
    TObjArray * ftriggList = new TObjArray;
    TObjArray * ftriggListDijet = new TObjArray;
    TObjArray * fassocList = new TObjArray;

    TVector * jetPtBinsVector = jetcard->GetVector("Jet:PtBins");
    const float * jetPtBins = jetPtBinsVector->GetMatrixArray();
    int nJetptBins = jetPtBinsVector->GetNrows();


    int pythiaMode = card->Get("DoResonancePythia") ? AliJPythiaTool::kJResornancePythia : 0;
    AliJPythiaTool pythiaTool(event, pythiaMode);
    foutTree->cd();
    TBranch * pythiaJetBranch = tree->Branch("pythiaJet",pythiaTool.GetPartonGroup(), 32000, 2 );
    TBranch * pythiaOrgBranch = tree->Branch("pythiaOrgParton",pythiaTool.GetPartonOrgGroup(), 32000, 2 );
	TBranch * inputBranch	  = tree->Branch("InputList", &inputList, 32000, 2 );
    fout->cd();
    int fEventCounter = 0;
    for(int iEvent = 0; iEvent < nEvent; ++iEvent) {
        //cout<<"DEBUG 0 "<<iEvent<<endl;
        if(!pythia.next()) cout<<"Pruser"<<endl;


        dmg->SetEvent(event);
        int iC=0;
        histos->fhiCentr->Fill(iC);
        inputList->Clear();
        Int_t nParticles = dmg->RegisterList(inputList);
        if(nShow > 0 && iEvent%ieout == 0) cout << " Event: " << iEvent << " " << iEvent*100.0/nEvent <<"% ,nParticles="<<nParticles<<endl;

        if( nParticles < 1 ) continue;
        pythiaTool.ScanPartons();
        //cout<<"DEBUG C1 "<<pythiaTool.GetPartonGroup()->GetEntriesFast()<<endl;

        if(inputList->GetEntriesFast()<1) continue;
        fEventCounter++;
        //------------------------------------------------------------------
        // Jet Reco and Getting the jet which contains the leading particle
        // Many differnt jet algorithm or parameters can be set in JParticleTool(ReconstructJets)
        //------------------------------------------------------------------
        TObjArray * JetLists[10];
        vector<fastjet::PseudoJet> *fJets[Njetfinder];
        for( int ijt=0;ijt<Njetfinder;ijt++ ){
            triggJet[ijt]->RecoJets(inputList, NULL);
            fJets[ijt] = & triggJet[ijt]->get_jets();
            JetLists[ijt] = triggJet[ijt]->GetJetList();
            //for( int ij=0;ij<JetLists[ijt]->GetEntriesFast();ij++ ){
            //    AliJJet* jet = (AliJJet*)(JetLists[ijt]->At(ij));
            //    jet->ReSum();
           // }
        }

        tree->Fill();

    }//event loop
    cout <<"Number of events = "<< fEventCounter << endl;
    foutTree->Write();
    foutTree->Close();
    //pythia.statistics();
    pythia.settings.listAll();
    card->WriteCard(fout);
    jetcard->WriteCard(fout);
    fout->Write();
    fout->Close();
    if(fsigma1GenRes) delete fsigma1GenRes;
    pythia.settings.writeFile(Form("%s.config",outputs.Data()), true);

    timer.Print(); 


}

double DeltaPhi(double dphi){
    if(dphi < -kJPi*9./20.) dphi += 2*kJPi;
    return dphi;
}


bool SetPythiaProcess( Pythia& pythia, int ptype ){
    switch (ptype){
        case 0: //all
            pythia.readString("HardQCD:all = on");
            break;
        case 1: // ->gg
            pythia.readString("HardQCD:all = off");
            pythia.readString("HardQCD:gg2gg = on");
            pythia.readString("HardQCD:qqbar2gg = on");
            break;
        case 2: //  ->qq
            pythia.readString("HardQCD:all = off");
            pythia.readString("HardQCD:gg2qqbar = on");
            pythia.readString("HardQCD:qqbar2qqbarNew = on");
            pythia.readString("HardQCD:qq2qq = on");
            break;
        case 3: // ->qg
            pythia.readString("HardQCD:all = off");
            pythia.readString("HardQCD:qg2qg = on");
            break;
    }
    return true;
}
