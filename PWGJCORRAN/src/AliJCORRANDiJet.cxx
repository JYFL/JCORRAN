/**************************************************************************
 * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// Comment describing what this class does needed!

// jcorran main class
// used in local and grid execution
// blah
// blah
// blah

#include "AliJCORRANDiJet.h"

#include "AliJTrackCounter.h"
#include <TClonesArray.h>

#include "AliJCard.h"
#include "AliJHistos.h"
#include "AliJCorrelations.h"
#include "AliJEventPool.h"
#include "AliJDataManager.h"

#include  "AliJEventHeader.h"
#include  "AliJRunHeader.h"
#include  "AliJTrack.h"
#include  "AliJPhoton.h"
#include  "AliJCaloCell.h"
#include  "AliJMCTrack.h"
#include  "AliJPiZero.h"
#include  "AliJConst.h"




#include "AliJEfficiency.h"
#include <iostream>

//ClassImp(AliJCORRANDiJet)

AliJCORRANDiJet::AliJCORRANDiJet() :
    //TObject(),
    fExecLocal(kTRUE),
    fFirstEvent(kTRUE),
    fcard(0),
    finputFile(0), 
    fevt(0),
    fhistos(0), 
    fcorrelations(0), 
    fphotonPool(0), 
    fassocPool(0),  
    fphotonList(0),  
    fchargedHadronList(0), 
    fpizeroList(0), 
    ftriggList(0),  
    fassocList(0), 
    fpairList(0), 
    fpairCounterList(0), 
    finputList(0), 
    fInclusiveFile(""),
    fdmg(0), 
    feventHeader(0), 
    frunHeader(0), 
    fnumberEvents(0), 
    fieout(0), 
    fEventCounter(0), 
    fnoMB(0), 
    fnoERT(0), 
    fcent(0), 
    fncBin(0), 
    fnPttBin(0), 
    fbTriggCorrel(0), 
    fbLPCorrel(0), 
    fbLPpairCorrel(0), 
    fTrackEtaRange(0), 
    flowerPtAssocBoarder(0), 
    fCentMultLow(0),  
    fCentMultHigh(0),
    fmixMB(0),  
    fmixMBMB(0),  
    fhasOfflineTrigger(0),
    fEfficiency(0),
    fRunTable(0)
{
    // constructor
}

AliJCORRANDiJet::AliJCORRANDiJet(Bool_t execLocal) :
    //TObject(),
    fExecLocal(execLocal),
    fFirstEvent(kTRUE),
    fcard(0),
    finputFile(0), 
    fevt(0),
    fhistos(0), 
    fcorrelations(0), 
    fphotonPool(0), 
    fassocPool(0),  
    fphotonList(0),  
    fchargedHadronList(0), 
    fpizeroList(0), 
    ftriggList(0),  
    fassocList(0), 
    fpairList(0), 
    fpairCounterList(0), 
    finputList(0), 
    fdmg(0), 
    feventHeader(0), 
    frunHeader(0), 
    fnumberEvents(0), 
    fieout(0), 
    fEventCounter(0), 
    fnoMB(0), 
    fnoERT(0), 
    fcent(0), 
    fncBin(0), 
    fnPttBin(0), 
    fbTriggCorrel(0), 
    fbLPCorrel(0), 
    fbLPpairCorrel(0), 
    fTrackEtaRange(0), 
    flowerPtAssocBoarder(0), 
    fCentMultLow(0),  
    fCentMultHigh(0),
    fmixMB(0),  
    fmixMBMB(0),  
    fhasOfflineTrigger(0),
    fEfficiency(0),
    fRunTable(0)
{
    // constructor
}

AliJCORRANDiJet::~AliJCORRANDiJet(){
    // destructor
}

AliJCORRANDiJet::AliJCORRANDiJet(const AliJCORRANDiJet& obj){
    // copy constructor
}

AliJCORRANDiJet& AliJCORRANDiJet::operator=(const AliJCORRANDiJet& obj){
    // copy constructor
    return *this;
}


void AliJCORRANDiJet::Initialize() const{
    // init

}

void AliJCORRANDiJet::UserCreateOutputObjects(){
    // local init

    fJetFinder = new AliJJetFinder( fcard );
    fAnaDiJet = new AliJDiJetAnalysis(fcard);
    cout<<"DEBUG 1 fAnaDiJet->UserCreateObject();"<<endl;
    fAnaDiJet->UserCreateObject();
    fAnaDiJet->SetJetPtBin( fcard->GetVector("Jet:PtBins"));

    char jemcabuf[20];

    cout << "jcorran user create output objects ----------------" << endl;

    fHadronSelectionCut =int ( fcard->Get("HadronSelectionCut"));
    fIsolationR = fcard->Get("IsolationR");

    fhistos = new AliJHistos( fcard );          
    fhistos->CreateEventTrackHistos();
    fhistos->CreateAzimuthCorrHistos();
    fhistos->CreateIAAMoons();
    fhistos->CreateXEHistos();
    fhistos->CreateJetHistos();
    //fhistos->CreateXtHistos();



    //TODO: inclusive fhistos shipping along

    fcorrelations = new AliJCorrelations( fcard, fhistos);
    cout<<endl<< " -----" <<endl; 
    if( fInclusiveFile.Length() ) {
        fhistos->ReadInclusiveHistos(fInclusiveFile);
        fcorrelations->SetSampligInclusive(); //kperp background and triangle. Default is flat
        cout<<"Sampling kperp and triangle from " << fInclusiveFile <<endl; 
    } else cout << "Sampling kperp and triangle from flat" <<endl; 
    cout<< " -----" <<endl <<endl;  

    //==================================

    // EventPool for Mixing
    fassocPool   = new AliJEventPool( fcard, fhistos, fcorrelations, fjassoc);

    fchargedHadronList  = new TClonesArray(kParticleProtoType[kHadron],1500);      
    ftriggList  = new TObjArray;
    fassocList  = new TObjArray;
    finputList = NULL;
    //TClonesArray *isolPizeroList  = new TClonesArray("AliPhJPiZero",1500);

    fdmg = new AliJDataManager(fcard, fhistos, fcorrelations, fExecLocal);
    fdmg->SetExecLocal( fExecLocal );

    //==== Read the Data files =====
    if( fExecLocal ){
        fdmg->ChainInputStream(finputFile);
        // TODO: run header is not supposed to be here
        frunHeader = fdmg-> GetRunHeader();
        // doesn't work fSQRTS = 2.*frunHeader->GetBeamEnergy();

        // for grid running, numberEvents is filled by the encapsulating
        // grid task, which has access to the input handlers and can
        // extract event numbers out of it
        fnumberEvents = fdmg->GetNEvents();
        fieout = fnumberEvents/20;

        if (fieout<1) fieout=1;
        cout<<"RunID = "<<frunHeader->GetRunNumber()<< " Looping over "<<fnumberEvents<<" events"<<endl;

        //==== RunTable
        fRunTable = & AliJRunTable::GetSpecialInstance();
        fRunTable->SetRunNumber( frunHeader->GetRunNumber() );
        fSQRTS = fRunTable->GetBeamEnergy(fRunTable->GetPeriod());
        cout << "sqrts = "<< fSQRTS << endl;
        //==== Efficiency ====
        fEfficiency = new AliJEfficiency;
        fEfficiency->SetMode( fcard->Get("EfficiencyMode") ); // 0:NoEff, 1:Period 2:RunNum 3:Auto
        fEfficiency->SetRunNumber( frunHeader->GetRunNumber() );
        TString effTag = fcard->GetStr("EfficiencyTag");

        if( effTag.Length() > 0 ){
            fEfficiency->SetTag( effTag );
        }
        fEfficiency->Load();

        fhistos->fhEventPerRun->Fill(fnumberEvents>0?log(fnumberEvents):1);


    }

    fJetPtBinsVector = fcard->GetVector("Jet:PtBins");
    fDoJetHadronCorrelation = fcard->Get("DoJetHadronCorrelation");
    fJetEtaCut = fcard->Get("Jet:EtaRange");

    //-------------------------------------------------------------------
    fEventCounter=0;
    fnoMB=0;
    fnoERT=0;

    fcent = -1;
    fncBin = fcard->GetNoOfBins( kCentrType );
    fnPttBin = fcard->GetNoOfBins( kTriggType );

    fTrackEtaRange = fcard->Get("EtaRange");

    fFirstEvent = kTRUE;
    fevt = -1;

    cout << "end of jcorran user create output objects ----------------" << endl;

}

void AliJCORRANDiJet::UserExec(){
    // event loop
    fevt++;

    if( !fExecLocal && fFirstEvent ){
        fdmg->ChainInputStream("");
        frunHeader = fdmg->GetRunHeader();

        fieout = fnumberEvents/20;

        if (fieout<1) fieout=1;
        cout<<"RunID = "<<frunHeader->GetRunNumber()<< " Looping over "<<fnumberEvents<<" events"<<endl;

        //==== Efficiency ====
        // TODO run can be different in a job?
        fhistos->fhEventPerRun->Fill(fnumberEvents>0?log(fnumberEvents):1);
        fFirstEvent = kFALSE;
    }

    if(fevt % fieout == 0) cout << fevt << "\t" << int(float(fevt)/fnumberEvents*100) << "%" << endl ;
    fdmg->LoadEvent(fevt);

    fhistos->fhEvents->Fill( 0 );

    if(!fdmg->IsGoodEvent()) return;  // Vertex cut applied in IsGoodEvent and histo saved there too

    feventHeader  = fdmg->GetEventHeader();

    double zVert    = feventHeader->GetZVertex();
    //----------------------------------------------------------


    UInt_t triggermaskJCorran = feventHeader->GetTriggerMaskJCorran();

    if( fdmg->IsSelectedTrigger((int) triggermaskJCorran))
        fhistos->fhEvents->Fill( 5 );

    // select only some BC%4
    if( feventHeader->GetBunchCrossNumber() % 4 != fEventBC &&
            fEventBC > -1 )
        return;

    if( fdmg->IsSelectedTrigger((int) triggermaskJCorran))
        fhistos->fhEvents->Fill( 6 );

    if(fRunTable->IsHeavyIon() || fRunTable->IsPA()){
        fcent = feventHeader->GetCentrality();
    }else  {
        fcent = 1; //ntracks;
    }

    //cout<<"evt="<<fevt <<" c="<<  fcent <<endl;
    int cBin        = fcard->GetBin(kCentrType, fcent);
    if(cBin<0) return;

    if( fdmg->IsSelectedTrigger((int) triggermaskJCorran))
        fhistos->fhEvents->Fill( 7 );

    int zBin        = fcard->GetBin(kZVertType, zVert); //should be alway >0; checked in fdmg->IsGoodEvent()

    // do not fill MB in case of MB mixing
    if( fdmg->IsSelectedTrigger((int) triggermaskJCorran))
        fhistos->fhZVert[cBin]->Fill(zVert);

    //temporary fevent skip
    //Trigger to be selected from the JCorran trigger mask is specified in the fCard
    //         if(! fdmg->IsSelectedTrigger((int) triggermaskJCorran))
    //           continue;

    fEventCounter++;

    //==== QA Event
    fhistos->fhV0AMult[cBin]->Fill( feventHeader->GetV0AMult() );

    const float * jetPtBins = fJetPtBinsVector->GetMatrixArray();
    int nJetptBins = fJetPtBinsVector->GetNrows();

    //------------------------------------------------------------------
    // Triggers and associated
    //----------------------ooooo---------------------------------------

    if(fjtrigg==kHadron || fjassoc==kHadron){
        fchargedHadronList->Clear();
        fdmg->RegisterList(fchargedHadronList, NULL, cBin, zBin, kHadron);
        // apply efficiencies

        for( int i = 0; i < fchargedHadronList->GetEntries(); i++ ){

            AliJBaseTrack *triggTr = (AliJBaseTrack*)fchargedHadronList->At(i);
            double ptt = triggTr->Pt();

            double effCorr = 1./fEfficiency->GetCorrection(ptt, fHadronSelectionCut, fcent);  // here you generate warning if ptt>30
            fhistos->fhTrackingEfficiency[cBin]->Fill( ptt, 1./effCorr );
            triggTr->SetTrackEff( 1./effCorr );
        }
    }

    //---- assign input list ---- 
    if(fjtrigg==kHadron) finputList = fchargedHadronList;
    int noAllTriggTracks = finputList->GetEntries();
    int noAllChargedTracks = fchargedHadronList->GetEntries();
    fhistos->fhChargedMult[cBin]->Fill(noAllChargedTracks);
    fhistos->fhChargedMultCent->Fill(fcent, noAllChargedTracks>0 ? log(noAllChargedTracks) : 0);

    //---------------------------------------------
    //--    Check fcentrality outliers           ---
    //--    and count enevents only here        ---
    fhistos->fhChargedMultCut[cBin]->Fill(noAllChargedTracks);
    fhistos->fhCentr->Fill(fcent);
    fhistos->fhiCentr->Fill(cBin);

    fJetFinder->RecoJets((TObjArray*)finputList,NULL);
    TObjArray * jetList = fJetFinder->GetJetList();
    vector<PseudoJet> fJets = fJetFinder->get_jets();

    // TODO nJetptBins, jetPtBins,  fDoJetHadronCorrelation
    for(int i=0;i<jetList->GetEntriesFast();i++){
        fhistos->fhJetPt[cBin]->Fill(fJets[i].perp());
        AliJBaseTrack * jet = (AliJBaseTrack*) jetList->At(i);
        jet->SetTrackEff(1);
        float pt = jet->Pt();
        int jetPtBin = TMath::BinarySearch( nJetptBins, jetPtBins, pt );
        if( jetPtBin >= nJetptBins - 1  ) jetPtBin = -1;
        jet->SetTriggBin(jetPtBin);
        if ( jetPtBin < 0 ) continue;
        // cout<<"DEBUG A : "<<jetPtBin<<"\t"<<pt<<"\t"<<nJetptBins<<endl;
        fhistos->fhJetPartMult[jetPtBin]->Fill( fJets[i].constituents().size() );
    }
    fhistos->fhLeadingJetPt[cBin]->Fill(fJets[0].perp());
	int leadingJetPtBin = ((AliJBaseTrack*)jetList->At(0))->GetTriggBin();
    fhistos->fhJetMultPt[leadingJetPtBin]->Fill( fJets.size() );

    //------------------------------------------------------------------
    //==== First Particle Loop
    //------------------------------------------------------------------
    ftriggList->Clear();
    //ftriggListDijet->Clear();
    fassocList->Clear();
    //vector<int> triggered( noTriggTracs, 0);
    //vector<int> assocated( noAssoc, 0);
    double leadingParticlePt = 0;
    if( fDoJetHadronCorrelation ){
        for(int i=0;i<jetList->GetEntriesFast();i++) {
            AliJBaseTrack *track = (AliJBaseTrack*)jetList->At(i);
            //cout<<"DEBUG jet:(eta/phi/pt) : "<< Form("%5.2f %5.2f %5.2f", track->Eta(), track->Phi(), track->Pt())<<endl;
            if( fabs(track->Eta())  > fJetEtaCut ) continue;
            //if( track->Phi()  < jetPhiCut0 || track->Phi() > jetPhiCut1 ) continue;
            //if( !card->IsInEtaRange(track->Eta())) continue;
            track->SetTriggBin( fcard->GetBin(kTriggType, track->Pt()) );
            if( track->GetTriggBin() > -1 ) ftriggList->Add( track );
        }
        for(int i=0;i<finputList->GetEntriesFast();i++) {
            AliJBaseTrack *track = (AliJBaseTrack*)finputList->At(i);
            if( fabs(track->GetCharge()) < 1e-4 ) continue;
            if( !fcard->IsInEtaRange(track->Eta())) continue;
            track->SetAssocBin( fcard->GetBin(kAssocType, track->Pt()) );
            if( track->GetAssocBin() > -1 ) fassocList->Add( track );
        }
    }else{
        for(int i=0;i<finputList->GetEntriesFast();i++) {
            AliJBaseTrack *track = (AliJBaseTrack*)finputList->At(i);
            if( fabs(track->GetCharge()) < 1e-4 ) continue;
            if( !fcard->IsInEtaRange(track->Eta())) continue;
            track->SetTriggBin( fcard->GetBin(kTriggType, track->Pt()) );
            track->SetAssocBin( fcard->GetBin(kAssocType, track->Pt()) );
            if( track->GetTriggBin() > -1 ) ftriggList->Add( track );
            if( track->GetAssocBin() > -1 ) fassocList->Add( track );

            //if( track->GetTriggBin() > -1 ) triggered[track->GetTriggBin()]++;
            // if( track->GetAssocBin() > -1 ) assocated[track->GetAssocBin()]++
            if( track->Pt() > leadingParticlePt ) leadingParticlePt = track->Pt();
        }
    }

    int noTriggTracs = ftriggList->GetEntries();
    int noAssocs = fassocList->GetEntries();

    int centBin = 0;
    fAnaDiJet->SetJetList( fJetFinder->GetJetList() );
    fAnaDiJet->SetInputList( finputList );
    fAnaDiJet->SetTrackJetMap( fJetFinder->GetJetTrackMap() );
    //fAnaDiJet->SetHistos( histos );
    //fAnaDiJet->SetPyHistos( pyhistos );
    fAnaDiJet->SetJetEtaRange( 0.4 ); 

    //cout<<"DEBUG 5 "<<finputList->GetEntriesFast()<<"\t"<<fJetFinder->GetJetList()->GetEntriesFast()<<endl;

    fAnaDiJet->UserExec();

    //------------------------------------------------------------------
    //==== Correlation Loop
    //------------------------------------------------------------------
    if( fDoJetHadronCorrelation ) {
        for(int ii=0;ii<noTriggTracs;ii++){ // trigger loop 
            AliJBaseTrack * triggTr = (AliJBaseTrack*)ftriggList->At(ii);
            double ptt = triggTr->Pt();
            int iptt   = triggTr->GetTriggBin(); 
            //if( ptt > 20 ) cout<<"DEBUG 0"<<endl;
            if( iptt < 0 ) continue;
            //cout<<"\tDEBUG1"<<endl;
            for(int jj=0;jj<noAssocs;jj++){ // assoc loop
                AliJBaseTrack  *assocTr = (AliJBaseTrack*)fassocList->At(jj);
                //cout<<"\t\tDEBUG2"<<endl;
                if( assocTr->GetCharge() ==0 ) continue;
                if( fDoJetHadronCorrelation && (fJetFinder->GetMotherJet( assocTr ) != triggTr )) continue;
                //cout<<"\t\t\tDEBUG"<<endl;

                //-------------------------------------------------------------
                fcorrelations->FillAzimuthHistos(kReal, 0, 0, triggTr, assocTr);
                //-------------------------------------------------------------
            }
        }
    }



}

void AliJCORRANDiJet::Terminate() {
    // termination

    for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++){
        ScaleNotEquidistantHisto( fhistos->fhChargedPt[hic], 1);
        ScaleNotEquidistantHisto( fhistos->fhChargedPtNoCorr[hic], 1);
        ScaleNotEquidistantHisto( fhistos->fhChargedPtJacek[hic], 1);
    }
    ScaleNotEquidistantHisto( fhistos->fhLPpt, 1);
    ScaleNotEquidistantHisto( fhistos->fhLPpairPt, 1);
    ScaleNotEquidistantHisto( fhistos->fhIsolatedLPpt, 1);
    ScaleNotEquidistantHisto( fhistos->fhChargedPtFiete, 1);

    //    cout<<"MB's="<<noMB<<" "<<" ERT's="<<noERT<<endl;
    fcorrelations->PrintOut();
    fassocPool->PrintOut();
    fEfficiency->Write();
}

particleType  AliJCORRANDiJet::GetParticleType(char *inchar){
    // part type
    for(int i=0;i<kNumberOfParticleTypes;i++) {
        //cout<<"<"<<inchar<<"> <"<<particleTypeStr[i]<<"> "<<strcmp(inchar,particleTypeStr[i])<<" "<<(particleType)i<<endl;
        if(strcmp(inchar,kParticleTypeStrName[i])==0) return (particleType)i;
    }
    std::cout<<"Particle type <"<<inchar<<"> not recognized"<<std::endl;
    exit(1);
}

double AliJCORRANDiJet::DeltaPhi(double phi1, double phi2) {
    // dphi
    double res =  atan2(sin(phi1-phi2), cos(phi1-phi2));
    //return res>-kJPi/3.0 ? res : kJTwoPi+res ; 
    return res>-kJPi*9./20. ? res : kJTwoPi+res ; 
}

void AliJCORRANDiJet::ScaleNotEquidistantHisto(TH1D *hid, const double sc=1){
    // histo scaler
    for(int i=1;i<= hid->GetNbinsX();i++){
        hid->SetBinContent(i,hid->GetBinContent(i)*sc/hid->GetBinWidth(i));
        hid->SetBinError(i,hid->GetBinError(i)*sc/hid->GetBinWidth(i));
    }   
}

// Fill xT histograms (Esko)
void AliJCORRANDiJet::FillXtHistos(TClonesArray *finputList, AliJTrackCounter *lpTrackCounter){
    // Comment to describe the method needed 

    enum xTtype { kInclusive=0, kIsolated=1, kIsolatedLP=2} ;
    double lowerPtCut = 0.2;
    double isolationR = 0.4;
    double fCutOnBkgActivity = 0.10;
    int cBin  = fcard->GetBin(kCentrType, fcent);
    double sqrts = fSQRTS;
    int triggCounter = 0, assocCounter = 0;
    int noAllTriggTracks = finputList->GetEntries();
    //cout << "Entering Esko's analysis loop" << endl;
    //cout << "Sqrts = " << sqrts << " pT = " << ptt << " xT = " << xtt << endl;
    for(int itrack=0; itrack<noAllTriggTracks; itrack++){
        AliJBaseTrack *triggTr = (AliJBaseTrack*)finputList->At(itrack);
        double  sumPtAroundTrigger = 0;
        double ptt = triggTr->Pt();
        double xtt = 2.0 * ptt / (1.0 * sqrts);
        double effCorr = 1.0/triggTr->GetTrackEff();
        fhistos->fhPtForXt[kInclusive][cBin]->Fill(ptt, ptt>0 ? 1./ptt*effCorr : 0);
        fhistos->fhXt[kInclusive][cBin]->Fill(xtt, effCorr );
        fhistos->fhXtWeighted[kInclusive][cBin]->Fill(xtt, effCorr*1.0/xtt );

        for(int jj=0;jj<finputList->GetEntriesFast();jj++){ // assoc loop
            AliJBaseTrack *assocTr = (AliJBaseTrack*)finputList->At(jj);
            if(triggTr->GetID()==assocTr->GetID()) continue;
            double pta = assocTr->Pt();
            // determine the activity in the cone around trigger (Esko)
            if( lowerPtCut < pta ){
                double dPhi = DeltaPhi( assocTr->Phi(), triggTr->Phi() );
                double dEta = assocTr->Eta() - triggTr->Eta();
                if(isolationR > sqrt(dPhi*dPhi+dEta*dEta))  sumPtAroundTrigger += assocTr->Pt();//FK//
            }
            //cout << "  Assoc number " << assocCounter++ << endl;
        }
        // If pT sum is below the limit, fill to the isolated histos
        if( sumPtAroundTrigger/ptt < fCutOnBkgActivity &&     ///relative activity
                ( fabs(triggTr->Eta()) < (fTrackEtaRange - isolationR) )   ){ //fiducial cut
            // pT and xT
            // kInclusive kIsolated, kLPIsolated
            fhistos->fhPtForXt[kIsolated][cBin]->Fill(ptt,effCorr*1.0/ptt);
            fhistos->fhXt[kIsolated][cBin]->Fill(xtt, effCorr );
            fhistos->fhXtWeighted[kIsolated][cBin]->Fill(xtt,effCorr*1.0/xtt);
        }
        //cout<<"pT sum around trigger = " << sumPtAroundTrigger << endl;
    }

} // end FillXtHistos
