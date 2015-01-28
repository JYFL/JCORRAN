// $Id: jtkt_phenix.C,v 1.29 2008/02/18 10:09:32 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file jtkt_phenix.h
  \brief
  \author J. Rak, D.J.Kim, B.S Chang (University of Jyvaskyla)
  \email: djkim@cc.jyu.fi
  \version $Revision: 1.29 $
  \date $Date: 2008/02/18 10:09:32 $
  */
////////////////////////////////////////////////////

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <TROOT.h>
#include <TSystem.h>
#include <TStopwatch.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TF1.h>
#include <TProfile.h>
#include <TFile.h>
#include <TChain.h>
#include <TObject.h>
#include <TTree.h>
#include <TRandom.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TPRegexp.h>

#include "AliJCard.h"
#include "AliJDiJetAnalysis.h"
#include "AliJJetJtAnalysis.h"
#include "AliJJetCORRAnalysis.h"
TString MemoryStatus(){
    int pid = getpid();
    TString key;
    TString v1;
    ifstream ins(Form("/proc/%d/status",pid ));
    double VmSwap;
    double VmSize;
    double VmPeak;
    double nom = TMath::Power( 2, 20 );
    TPMERegexp split("\\s+");
    TString line;
    while( ins.good() ){
        line.ReadLine(ins);
        split.Split(line);
        if( split.NMatches() < 2 ) continue;
        key = split[0];
        v1 = split[1];
        if( key == "VmSwap:" ) VmSwap = v1.Atof()/nom;
        if( key == "VmSize:" ) VmSize = v1.Atof()/nom;
        if( key.BeginsWith("VmPeak") ) VmPeak = v1.Atof()/nom;
    }
    TString res = Form("VmPeak:%10.2f VmSize:%10.2f VmSwap:%10.2f", VmPeak, VmSize, VmSwap);
    ins.close();
    return res;
}

int main(int argc, char **argv) {

    //==== Read arguments ===== 
    TROOT root("nanoDST","nanoDST analysis");
    if ( argc<6 ) {
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout<<"+  "<<argv[0]<<" [inputFilelist] [outputFile] [inCard.input] [trigger] [assoc] [pythiaConfig file] <inclusiveSpectra>"<<endl;
        cout<<"+  InclusiveSpectra for k_perp bck simulation. If not given, sampling from flat #Delta#eta"<<endl;
        cout<<"+  InclusiveSpectra is just the output file with the same binning"<<endl;
        cout<<"+  trigger/assoc: "; for(int i=0;i<kNumberOfParticleTypes;i++)
            cout<<kParticleTypeStrName[i]<<" "; cout<<"+"<<endl<<"+"<<endl;
        cout<<"+  1) for pizer0 mass distribs trigger must be pizero"<<endl;
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout << endl << endl;
        exit(1);
    }

    char *outFile, *inputFile, *cardName, *inclusFileName=NULL, *jtrigg, *jassoc;
    inputFile = argv[1];
    outFile   = argv[2];
    cardName  = argv[3];
    jtrigg = argv[4];
    jassoc = argv[5];
    TString pythiaConfig = argv[6];
    TString pythiaSettingAll = argv[7];
    Int_t random_seed = atoi(argv[8]);


    char inFile[200];
    TChain * fChain = new TChain("JetTree");
    ifstream infiles( inputFile );
    while( infiles >> inFile ){
        fChain->Add(inFile);
    }

    if(fChain->GetEntriesFast()<=0){
        cout<<"Empty chain from "<<inputFile<<endl;
        exit(0);
    }

    TClonesArray * fClonesArray[8] ;

    for( int i=0;i<sizeof(fClonesArray)/sizeof(fClonesArray[0]);i++ ) fClonesArray[i]=NULL;
    TString branchesStr[8] = { "pythiaOrgParton", "pythiaJet" };
    for( int i=2;i<8;i++ ){
        branchesStr[i] = Form("Jet%02d",i-2);
        cout<<Form("Jet%02d",i-2)<<endl;
    }
    for( int i=0;i<8;i++ ){
        int res = -10;
        if( fChain->FindBranch(branchesStr[i]) ) res = fChain->SetBranchAddress(branchesStr[i], &fClonesArray[i]);
        cout<<res<<endl;
    }
    TClonesArray * fTracksArray=NULL;
    fChain->SetBranchAddress( "InputList", &fTracksArray );



    TFile *fout = new TFile(outFile,"RECREATE","Final analysis");//, *inclusFile = NULL;
    cout <<"=================================================="<<endl;
    cout <<"* inp. file list \t"<<inputFile<<endl;
    cout <<"* out file name \t"<<outFile<<endl;
    cout <<"* card name \t"<<cardName<<endl;
    cout <<"* Trigg Particle = "<< argv[4] <<" x "<< "Assoc = "<< argv[5] << endl;
    cout <<"* Pythia config file = "<< pythiaConfig.Data() << endl;
    cout <<"* random_seed = "<< random_seed << endl;
    cout <<"=================================================="<<endl;

    // === Create all what you need ====
    AliJCard *card = new AliJCard(cardName);
    card->PrintOut();

    TObjArray IncomingJets;
    TObjArray OutgoingJets;
    TObjArray MultiPartonAllJets;

    AliJDiJetAnalysis dijetAna( card );
    dijetAna.UserCreateOutputObjects();


    dijetAna.AddJets( &IncomingJets );
    dijetAna.AddJets( &OutgoingJets );
    for( int i=2;i<8;i++ ){
        if( !fClonesArray[i] ){
            continue;
        }
        dijetAna.AddJets( fClonesArray[i] );
    }

    dijetAna.AddJets( &MultiPartonAllJets );

    vector<TString> JetFinderName( 6 );
    //Caution cone size name should be for example "R040" form 
    JetFinderName[0] = "FullR040";
    JetFinderName[1] = "FullR050";
    JetFinderName[2] = "FullR060";
    JetFinderName[3] = "ChargedR040";
    JetFinderName[4] = "ChargedR050";
    JetFinderName[5] = "ChargedR060";

    AliJJetJtAnalysis jetJtAna( card ); 
    jetJtAna.SetJetFinderName( JetFinderName );
    jetJtAna.SetNumberOfJetFinders( 6 );
    fout->cd();
    jetJtAna.UserCreateOutputObjects();
    for( int i=2;i<8;i++ ){
        if( !fClonesArray[i] ) continue;
        jetJtAna.AddJets( fClonesArray[i] );
    }
    jetJtAna.SetJTracks(fTracksArray);

    // Add AliJJetCORRAnalysis from DJ
    cout <<"Adding AliJJetCORRAnalysis..."<< endl;
    AliJJetCORRAnalysis fJJetCORRAnalysis( card );
    fout->cd();
    fJJetCORRAnalysis.UserCreateOutputObjects();
    // Add jets into the fJJetCORRAnalysis-
    cout <<"JJetCORRAnalysis.AddJets ..."<< endl;
    for( int i=2;i< 8;i++ ){
        if( !fClonesArray[i] ) continue;
        fJJetCORRAnalysis.AddJets( fClonesArray[i] );
    }
    int cBin = 0;
    int zBin = 0;
    double fcent=0;
    double zVert=0;
    int fTargetJetIndex = 2;

    fJJetCORRAnalysis.SetTargetJetIndex( fTargetJetIndex );

    Long64_t numberEvents = fChain->GetEntries();


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Loop over all events
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TStopwatch timer;
    timer.Start();
    for(Long64_t evt=0; evt<numberEvents; evt++) {
        if( evt%10000 == 0 ){
            cout<<Form("--Event %10d - %5.3f",evt, evt*100./numberEvents)<<" "<<MemoryStatus()<<endl;
        }
        fChain->GetEntry( evt );

        IncomingJets.Clear();
        IncomingJets.Add( fClonesArray[1]->At(0) );
        IncomingJets.Add( fClonesArray[1]->At(1) );


        OutgoingJets.Clear();
        OutgoingJets.Add( fClonesArray[1]->At(2) );
        OutgoingJets.Add( fClonesArray[1]->At(3) );

        MultiPartonAllJets.Clear();
        for( int i=4;i<fClonesArray[1]->GetEntriesFast();i++ ){
            MultiPartonAllJets.Add( fClonesArray[1]->At(i) );
        }

        fJJetCORRAnalysis.SetCentralityBin( cBin );
        fJJetCORRAnalysis.SetCentrality( fcent );
        fJJetCORRAnalysis.SetZVertexBin( zBin );
        fJJetCORRAnalysis.SetZVertex( zVert );


        dijetAna.ClearBeforeEvent();
        dijetAna.UserExec();
        //cout<< fClonesArray[0]->GetEntriesFast()<<endl;

        // Apply acceptance for eta cut
        jetJtAna.UserExec();
        fJJetCORRAnalysis.UserExec();

    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // End of Event Loop
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    dijetAna.Terminate();

    //Int_t EventCounter = fJCORRAN->GetEventCounter();

    fout->cd();
    jetJtAna.WriteHistograms();
    //write Trigg ID and Assoc ID
    card->WriteCard(fout);
    fout->Write(); 
    fout->Close();
    timer.Print();

    cout << "Total Number of Event scanned from input = " << numberEvents << endl;
    //cout << "Total Number of Event used for analysis  = " << EventCounter << endl;
    cout<<"All files properly closed. Good Bye!"<<endl;
}

