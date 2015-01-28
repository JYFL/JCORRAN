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

//===========================================================
// Dummy comment, should be replaced by a real one
// comment
// comment
//===========================================================

#include <TRandom.h>
#include <TMath.h>
#include "AliJJet.h"
#include "AliJDiJetAnalysis.h"
#include "AliJHistManager.h"


void AliJDiJetAnalysis::UserCreateObject(){
  // comment needed
  CreateHistos();
  fJetPtBins = fCard->GetVector("Jet:PtBins");
  fJetPtPairBins = fCard->GetVector("Jet:PtPairBins");

  for( int i=0;i<kJNJetType;i++ ){
    for( int j=0;j<kJNJetSelection;j++ ){
      fJets[i][j] = NULL;
      for( int k=0;k<kJNDiJetSelection;k++ ){
        fDiJets[i][j][k]=NULL;
        fDiJetBin[i][j][k][0] = -1; 
        fDiJetBin[i][j][k][1] = -1; 
      }
    }
  }
}

void AliJDiJetAnalysis::ClearBeforeEvent(){
  fJetListOfList.Clear();
  for( int i=0;i<kJNJetType;i++ ){
    for( int j=0;j<kJNJetSelection;j++ ){
      if( fJets[i][j] ){
        delete fJets[i][j];
        fJets[i][j] = NULL;
      }
      for( int k=0;k<kJNDiJetSelection;k++ ){
        if( fDiJets[i][j][k] ){
          delete fDiJets[i][j][k];
          fDiJets[i][j][k] = NULL;
        }
      }
    }
  }
}

void AliJDiJetAnalysis::UserExec(){
  //cout<<"DEBUG_B1 "<<fJetListOfList.GetEntries()<<endl;
  for( int i=0;i<fJetListOfList.GetEntries();i++ ){
    TObjArray * ar = (TObjArray*) fJetListOfList[i];
    if(!ar) {
      cout<<"DEBUG_B2 no array at "<<i<<endl;
      continue;
    }
  }
  FillHistosJets();
  FillHistosDiJet();

}
void AliJDiJetAnalysis::Terminate() const{
  // comment needed
  fHMG->Write();
  fHMG->WriteConfig();
}


AliJDiJet * AliJDiJetAnalysis::GetDiJets(int type, int js, int djs){
  if( fDiJets[type][js][djs] ) return fDiJets[type][js][djs];
  TObjArray * jets = GetJets( type, js );
  AliJDiJet * dijet = fDiJets[type][js][djs] = new AliJDiJet();
  if( !jets ) return dijet;

  fDiJets[type][js][djs] = dijet;
  if( type==kJIncomingJet && js!=0 && djs!=0 ){
    return dijet;
  }
  //==== DIJET DiJet In Full Acceptance
  if( djs == kJDiJetLeadingSubLeading ){
    for( int ij=0;ij<jets->GetEntriesFast();ij++ ){
      AliJJet * jet = (AliJJet*) jets->At(ij);
      dijet->Add( jet );
      if( dijet->GetEntries() >= 2 ) break;
    }
  }
  //==== DIJET DiJet In Alice Acceptance
  if( djs == kLeadingSubLeadingOpposite ){
    for( int ij=0;ij<jets->GetEntriesFast();ij++ ){
      AliJJet * jet = (AliJJet*) jets->At(ij);
      dijet->Add(jet);
      if( dijet->GetEntries() >= 2 ) break;
    }
    if( dijet->GetEntries() > 1){
      double dphi = dijet->DPhi();
      if( fabs(dphi) < TMath::Pi()/2 ) dijet->Clear();
    }

  }
  //==== DIJET 2 : Marta - Find DiJet in Eta
  if( djs == kJDiJetMarta ){
    if( type == kJIncomingJet ) return dijet;
    TObjArray *jetsFull = NULL;
    switch ( type ){
      case kJChargedJet: jetsFull = GetJets( kJFullJet, js );break;
      case kJChargedJet08: jetsFull = GetJets( kJFullJet08, js );break;
      default: jetsFull = jets;
    }

    if( !jetsFull || !jets ) return dijet;
    AliJJet * trigJet = NULL;
    for( int ij=0;ij<jetsFull->GetEntriesFast();ij++ ){
      AliJJet * jet = (AliJJet*) jetsFull->At(ij);
      if( jet->Area() < 0.3 ) continue;
      if( jet->Pt() < 20 ) break;
      if( jet->LeadingParticlePt() < 5 || jet->LeadingParticlePt() > 100 ) continue;
      trigJet = jet;break;
    }
    if( trigJet == NULL ) return dijet;
    double ptt = trigJet->Pt();
    AliJJet * asocJet = NULL;
    for( int ij=0;ij<jets->GetEntriesFast();ij++ ){
      AliJJet * jet = (AliJJet*) jets->At(ij);
      if( !jet ){
        cout<<"JWARN_GETDIJET13: "<<dijet->GetEntries()<<endl;
        continue;
      }
      if( jet->Area() < 0.3 ) continue;
      double pta = jet->Pt();
      if( pta < 20 ) break;
      if( pta > ptt ) break;
      if( jet->LeadingParticlePt() < 5 || jet->LeadingParticlePt() > 100 ) continue;
      if( fabs(jet->DeltaPhi( *trigJet ))<TMath::Pi()/2 ) continue;
      asocJet = jet;break;
    }
    if( asocJet ){
      dijet->SetJets( trigJet,asocJet );
    }
  }
  return fDiJets[type][js][djs];
}

TObjArray * AliJDiJetAnalysis::GetJets(int type, int js){
  if( fJets[type][js] ) return fJets[type][js];
  TObjArray * na = new TObjArray;
  //=========================================
  //== Jet Selection
  //=========================================
  TObjArray * jets =  (TObjArray*)fJetListOfList[type];
  for( int i=0;i<jets->GetEntriesFast();i++ ){
    AliJJet * jet = (AliJJet*)  jets->At(i);
    if( type == kJIncomingJet ){
      //=========================================
      //== Incoming jets
      //=========================================
      if( js != kJEtaAll ) continue;
      na->Add(jet);
    }else if( js == kJEtaAll ){
      //=========================================
      //== kJEtaAll
      //=========================================
      if( type == kJOutgoingJet ){
        //cout<<"DEBUG_G1 "<<jet->Eta()<<endl;
      }
      if( fabs(jet->Eta()) < 5 ){
        na->Add(jet);
      }
    }else if( js == kJEtaAlice ){
      //=========================================
      //== kJEtaAlice
      //=========================================
      if( fabs(jet->Eta()) < 0.4 ){
        na->Add(jet);
      }
    }
  }// jets
  //=========================================
  //== NOJET
  //=========================================
  if( na->GetEntries() < 1 ){
    delete na;
    na=NULL;
  }
  return fJets[type][js]=na;
}

void AliJDiJetAnalysis::FillHistosJets(){
  //==== TYPE 
  for( int type=0;type<kJNJetType;type++ ){
    //==== Jet Selection
    for( int js=0;js<kJNJetSelection;js++ ){
      //cout<<"DEBUG_F0: type="<< type <<"\tjs="<<js<<endl;
      TObjArray *jets = GetJets( type, js );
      if( !jets ) {
        //cout<<"DEBUG_F1: type="<< type <<"\tjs="<<js<<"\t"<<jets<<endl;
        continue;
      }else{
        //cout<<"DEBUG_F2: type="<< type <<"\tjs="<<js<<"\t"<<jets<<endl;
      }
      //==== jets
      for( int i=0;i<jets->GetEntriesFast();i++ ){
        AliJJet * jet = dynamic_cast<AliJJet*>( jets->At(i) );
        if( !jet ) { 
          //cout<<"DEBUG_F15 type="<<type<<"\tjs="<<js<<"\ti="<<i<<endl;
          continue;
        }else{
          //cout<<"DEBUG_F16 type="<<type<<"\tjs="<<js<<"\ti="<<i<<endl;

        }
        fhJetPt[type][js]->Fill( jet->Pt() );
        fhJetPhi[type][js]->Fill( jet->Phi() );
        fhJetEta[type][js]->Fill( jet->Eta() );
        fhJetNConst[type][js]->Fill( jet->GetNConstituents() );
        fhJetEnergy[type][js]->Fill( jet->E() );
        //== DiJet Multiplicity
        for( int j=i+1;j<jets->GetEntriesFast();j++ ){
            AliJJet * jet1 = dynamic_cast<AliJJet*>( jets->At(j) );
            AliJDiJet dijet(jet,jet1);
            fhDiJetMultiplicity[type][js]->Fill( dijet.InvM() );
        }
      }
    }
  }
}

void AliJDiJetAnalysis::FillHistosDiJet(){
    //==== TYPE 
    for( int type=0;type<kJNJetType;type++ ){
        //==== Jet Selection
        for( int js=0;js<kJNJetSelection;js++ ){
            //==== DiJet Selection
            for( int djs=0;djs<kJNDiJetSelection;djs++ ){
                AliJDiJet & dijet = *GetDiJets( type, js, djs );
                if( dijet.GetEntries() < 2 ) continue;
                //TODO error check
                double pt0 = dijet(0).Pt();
                double pt1 = dijet(1).Pt();
                double invm = dijet.InvM();
                double lpt  = dijet.LeadingPt();
                int iInvM = fInvMBin.GetBin( invm );
                int iLPt  = fJetPtBin.GetBin( lpt );
                fDiJetBin[type][js][djs][0] = iInvM;
                fDiJetBin[type][js][djs][1] = iLPt;
            }
        }
    }
    //==== TYPE 
    for( int type=0;type<kJNJetType;type++ ){
        //==== Jet Selection
        for( int js=0;js<kJNJetSelection;js++ ){
            //==== DiJet Selection
            for( int djs=0;djs<kJNDiJetSelection;djs++ ){
                AliJDiJet & dijet = *GetDiJets( type, js, djs );
                if( dijet.GetEntries() < 2 ) continue;
                double ptpair = dijet.PtPair();
                double invm   = dijet.InvM();
                double dR     = dijet.DR();
                double dPhi   = dijet.DPhi();
                double dEta   = dijet.DEta();
                double eAsymm = dijet.EAsymm();
                double ptAsymm= dijet.PtAsymm();
                double kTA    = dijet.kTA();

                for( int i=0; i<6 ; i++ ){
                    int bin = 0;
                    if( i==0 ) bin = fDiJetBin[kJChargedJet][js][djs][0];
                    if( i==1 ) bin = fDiJetBin[kJOutgoingJet][js][0][1];
                    if( i==2 ) bin = fDiJetBin[kJChargedJet][js][djs][0];
                    if( i==3 ) bin = fDiJetBin[kJOutgoingJet][js][0][1];
                    if( i==4 ) bin = fDiJetBin[type][js][djs][0];
                    if( i==5 ) bin = fDiJetBin[type][js][djs][1];

                    if( bin < 0 ) continue;

                    fhDiJetPtPair[type][js][djs][i][bin]->Fill( ptpair );
                    fhDiJetInvM[type][js][djs][i][bin]->Fill( invm );
                    fhDiJetDeltaR[type][js][djs][i][bin]->Fill( dR );
                    fhDiJetDeltaPhi[type][js][djs][i][bin]->Fill( dPhi );
                    fhDiJetDeltaEta[type][js][djs][i][bin]->Fill( dEta );
                    fhDiJetPtAsymm[type][js][djs][i][bin]->Fill( ptAsymm );
                    fhDiJetEAsymm[type][js][djs][i][bin]->Fill( eAsymm );

                }
                if( type==kJChargedJet ){
                    int bin = -1;
                    AliJDiJet & dijet0 = * GetDiJets(1, 0, 0);
                    AliJDiJet & dijet1 = * GetDiJets(2, js, djs);
                    for( int j0=0;j0<2;j0++){
                        int bin=-1;
                        if( js==1 ) { bin = fJetPtBin.GetBin( dijet(j0).Pt() );}
                        if( js==0 ) { bin = fJetPtBin.GetBin( dijet(j0).E() ); }
                        if( bin < 0 ) continue;
                        for( int j1=0;j1<dijet0.GetEntries();j1++ ){
                            if( dijet(j0).DeltaR(dijet0(j1)) < 1 ){
                                fhJetEnergyComp[1][js][djs][0][bin]->Fill( dijet(j0).E(), dijet0(j1).E() );
                            }else{
                                fhJetEnergyComp[1][js][djs][1][bin]->Fill( dijet(j0).E(), dijet0(j1).E() );
                            }
                        }
                        for( int j1=0;j1<dijet1.GetEntries();j1++ ){
                            if( dijet(j0).DeltaR(dijet1(j1)) < 1 ){
                                fhJetEnergyComp[2][js][djs][0][bin]->Fill( dijet(j0).E(), dijet1(j1).E() );
                            }else{
                                fhJetEnergyComp[2][js][djs][1][bin]->Fill( dijet(j0).E(), dijet1(j1).E() );
                            }
                        }
                    }
                }
            }
        }
    }
}

void AliJDiJetAnalysis::FillPythiaDiJet(int js ){

    AliJDiJet * jetsIn = GetDiJets( 0, 0, 0 );
    AliJDiJet * jetsOut = GetDiJets( 1, js, 0 );


    /* TODO
       if( jetsIn->GetEntriesFast() < 2 || jetsOut->GetEntriesFast() < 2 ) return;

       AliJJet*  jet00 = (AliJJet*) jetsIn->At(0);
       AliJJet*  jet01 = (AliJJet*) jetsIn->At(1);
       AliJJet*  jet10 = (AliJJet*) jetsOut->At(0);
       AliJJet*  jet11 = (AliJJet*) jetsOut->At(1);

       AliJJet  jet0 = *jet00+*jet01;
       AliJJet  jet1 = *jet10+*jet11;

       AliJJet jetsum = jet0+jet1;
       int ipt = TMath::BinarySearch(fJetPtBins->GetNrows(), fJetPtBins->GetMatrixArray(), float(fPythiaTool->GetParton23Pt()));

       if( ipt > -1 ){
       fHistos->fPythiaJetPtPair[0][ipt]->Fill( (*jet00+*jet01).Pt() );
       fHistos->fPythiaJetPtPair[1][ipt]->Fill( (*jet10+*jet11).Pt() );
       fHistos->fPythiaJetSum[0]->Fill( jetsum.P() );
       fHistos->fPythiaJetSum[1]->Fill( jetsum.E() );
       }
       */

}

void AliJDiJetAnalysis::CreateHistos(){
    // Create Histograms

    //==== Hist Manager
    fHMG = new AliJHistManager( "AliJDiJetAnalysisHistManager");

    //==== BIN
    fJetPtPairBin   .Set("JetPtPair",   "P", "p_{Tpair}:%2.0f-%2.0f").SetBin( fCard->GetVector("Jet:PtPairBins") );
    fJetPtBin       .Set("JetPt",       "P", "p_{T}:%2.0f-%2.0f").SetBin( fCard->GetVector("Jet:PtBins") );
    fInvMBin        .Set("InvMB",       "M", "M:%2.0f-%2.0f").SetBin( fCard->GetVector("InvMBin"));
    fJetTypeBin     .Set("JetType",     "T", "T%1.0f", AliJBin::kSingle).SetBin( kJNJetType );
    fJetSelectionBin.Set("JetSelection","J", "J%1.0f", AliJBin::kSingle).SetBin( kJNJetSelection );
    fDiJetSelectionBin.Set("DiJetSelection","J", "J%1.0f",AliJBin::kSingle).SetBin( kJNDiJetSelection);

    fJetRBin        .Set("JetR",        "R", "R:%2.1f",AliJBin::kSingle ).SetBin( "0.4, 0.5");
    fDiJetBinTypeBin.Set("BinType",     "B", "B:%1.f", AliJBin::kSingle ).SetBin( 6 );

    //==== Log Bins
    int nBINS2=300;
    double logBinsX2[nBINS2+2], limL2=0.1, LimH2=1000;
    double logBW2 = (log(LimH2)-log(limL2))/nBINS2;
    for(int ij=0;ij<=nBINS2;ij++) logBinsX2[ij+1]=limL2*exp(ij*logBW2);
    logBinsX2[0]=0;

    //==== Histogram
    int nDR = 1000;double xDR0= -10; double xDR1 = 10;
    int nDPhi=1000;double xDPhi0=-TMath::Pi(); double xDPhi1=-xDPhi0;
    //== Jets QA
    fhJetPt 
        << TH1D("hJetPt","",nBINS2, logBinsX2 )
        << fJetTypeBin << fJetSelectionBin  <<"END";
    fhJetPhi 
        << TH1D("hJetPhi","",nDPhi, xDPhi0, xDPhi1 )
        << fJetTypeBin << fJetSelectionBin  <<"END";
    fhJetEta 
        << TH1D("hJetEta","",nDR, xDR0, xDR1 )
        << fJetTypeBin << fJetSelectionBin  <<"END";
    fhJetNConst 
        << TH1D("hJetNConst","",100, 0-.5, 100-.5 )
        << fJetTypeBin << fJetSelectionBin  <<"END";
    fhJetNConst 
        << TH1D("hJetNConst","",nBINS2, logBinsX2 )
        << fJetTypeBin << fJetSelectionBin  <<"END";
    fhJetEnergy 
        << TH1D("hJetEnergy","",nBINS2, logBinsX2 )
        << fJetTypeBin << fJetSelectionBin  <<"END";

    //== Jet Comparision
    fhJetEnergyComp
        << TProfile("hJetEnergyComp","",nBINS2, logBinsX2 )
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fJetRBin << fJetPtBin <<"END";
    fhJetDRToRef
        << TH1D("hJetDRToRef","", nDR, xDR0, xDR1 )
        << fJetTypeBin << fJetSelectionBin << fJetPtBin <<"END";
    fhJetDPhiToRef
        << TH1D("hJetDPhiToRef","", nDPhi, xDPhi0, xDPhi1 )
        << fJetTypeBin << fJetSelectionBin << fJetPtBin <<"END";



    //== DiJets
    fhDiJetPtPair // Inclusive PtPair
        << TH1D("hDiJetPtPair","",nBINS2, logBinsX2 )
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetInvM
        << TH1D("hDiJetInvM","",nBINS2, logBinsX2 )
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetKtA
        << TH1D("hDiJetKtA","",nBINS2, logBinsX2 ) // TODO change bins
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetDeltaR
        << TH1D("hDiJetDeltaR","",nDR, xDR0, xDR1 ) // TODO change bins
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetDeltaPhi
        << TH1D("hDiJetDeltaPhi","",nDPhi, xDPhi0, xDPhi1) // TODO change bins
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetDeltaEta
        << TH1D("hDiJetDeltaEta","",nDR, xDR0, xDR1 ) // TODO change bins
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetPtAsymm
        << TH1D("hDiJetPtAsymm","",100,0,1 ) // TODO change bins
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetEAsymm
        << TH1D("hDiJetEAsymm","",100,0,1 ) // TODO change bins
        << fJetTypeBin << fJetSelectionBin << fDiJetSelectionBin << fDiJetBinTypeBin << fInvMBin<<"END";
    fhDiJetMultiplicity
        << TH1D("hDiJetMultiplicity","",nBINS2, logBinsX2)// TODO change bins
        << fJetTypeBin << fJetSelectionBin << "END";
}


