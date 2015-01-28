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
#include "AliJDiJetAnalysis.h"
#include "AliJJet.h"

void AliJDiJetAnalysis::UserCreateObject(){
    // comment needed
    fHistos = new AliJHistosDiJetAnalysis(fCard);
    fHistos->CreateHistos();
    fJetListOfList.Add( new TObjArray );
    fJetListOfList.Add( new TObjArray );
    fJetListOfList.Add( new TObjArray );
    fJetListOfList.Add( new TObjArray );
    fJetPtBin = fCard->GetVector("Jet:PtBins");
    fPtPairBins = fCard->GetVector("Jet:PtPairBins");
}
void AliJDiJetAnalysis::UserExec(){
    // comment needed
    for( int i=0;i<4;i++ ){
        fJetListOfList[i]->Clear();
    }
    if( GetNJets() < 2 ) return;
    SelectDiJet();
    FillHistos();
    FillAtalasDPhi();
    FillJetMultVsDPhi();
}
void AliJDiJetAnalysis::Terminate() const{
    // comment needed
}

void AliJDiJetAnalysis::FillAtalasDPhi(){
    // 1102.2696v2
    int njet = 0;
    for( int ij=0;ij<GetNJets();ij++ ){
        if( fabs(Jet(ij).Rapidity()) < 2.8 && Jet(ij).Pt() > 100 ) { njet++; }
    }

    if(njet < 2) return;
    TObjArray & dijets = *GetDiJetList(kJDiJetAtlas); // Apply eta cut |eta|<0.8
    if( dijets.GetEntries() < 2 ) return;
    AliJJet *j1 = ((AliJJet*)dijets[0]);
    AliJJet *j2 = ((AliJJet*)dijets[1]);
    if(j1->Pt()>110) {
        double dphi = j1->DeltaPhi(*j2);
        // QESTION ? : 1. what is dphi? 2. which ptbin? ptMax>110 then jetptbin is alwasy last one (100~150)
        if( njet >= 2 ) fHistos->hLeadingDiJetDeltaPhiNjetsAtlas[0]->Fill(dphi);
        if( njet >= 3 ) fHistos->hLeadingDiJetDeltaPhiNjetsAtlas[1]->Fill(dphi);
        if( njet >= 4 ) fHistos->hLeadingDiJetDeltaPhiNjetsAtlas[2]->Fill(dphi);
        if( njet >= 5 ) fHistos->hLeadingDiJetDeltaPhiNjetsAtlas[3]->Fill(dphi);
    }
}

void AliJDiJetAnalysis::FillJetMultVsDPhi(){
    //comment needed
    int njet = 0;
    for( int ij=0;ij<GetNJets();ij++ ){
        if( fabs(Jet(ij).Rapidity()) < 0.5 && Jet(ij).Pt() > 20 ) { njet++; }
        else break;
    }


    if( njet < 2 ) return;
    TObjArray & dijets = *GetDiJetList(kJDiJetInAliceAcceptance); // Apply eta cut |eta|<0.8
    if( dijets.GetEntries() < 2 ) return;
    AliJJet *j1 = ((AliJJet*)dijets[0]);
    AliJJet *j2 = ((AliJJet*)dijets[1]);
    double dphi = j1->DeltaPhi(*j2);

    int jetPtBin1 = TMath::BinarySearch(fJetPtBin->GetNrows(), fJetPtBin->GetMatrixArray(), float(j1->Pt()) );
    if( jetPtBin1 < 0 || jetPtBin1 > fJetPtBin->GetNrows()-2 ) jetPtBin1 = -1;
    if( jetPtBin1 < 0 ) return;
    // QESTION ? : 1. what is dphi? 2. which ptbin? ptMax>110 then jetptbin is alwasy last one (100~150)
    if( njet >= 2 ) fHistos->hLeadingDiJetDeltaPhiNjets[0][jetPtBin1]->Fill(dphi);
    if( njet >= 3 ) fHistos->hLeadingDiJetDeltaPhiNjets[1][jetPtBin1]->Fill(dphi);
    if( njet >= 4 ) fHistos->hLeadingDiJetDeltaPhiNjets[2][jetPtBin1]->Fill(dphi);
    if( njet >= 5 ) fHistos->hLeadingDiJetDeltaPhiNjets[3][jetPtBin1]->Fill(dphi);
}


void SelectorEta(TObjArray * src, TObjArray * dst, double x1, double x2 ){

}

TObjArray * GetDiJets(int type, int js, int djs){
    if( fDiJets[type][js][djs] ) return fDijets[type][js][djs];
    fDiJets[type][js][djs] = DiJetSelector( GetJets( type, js ), djs );
    return fDiJets[type][js][djs];
}

TObjArray * GetJets(int type, int js){
    if( fJets[type][js] ) return fjets[type][js];
    TObjArray * array = (TObjArray*) fJetListOfList[i]; 
    fJets[type][js] = JetSelector( array, js );
    return fJets[type][js];
}

TObjArray * JetSelector( TObjArray * jets, int js ){
    if( jets==NULL || jets->GetEntriFast()<1 ) return NULL;
    TObjArray * na = new TObjArray;
    for( int i=0;i<jets->GEtEntriFast();i++ ){
        AliJBaseTrack * jet = (AliJBaseTrack*)  jets->At(i);
        if( js == kEtaAll && fabs(jet->Eta())>5 ) continue;
        if( js == kEtaAlice && fabs(jet->Eta())->0.4 ) continue;
        na->Add(js);
    }
    if( na->GetEntriFast() < 1 ){
        delete na;
        return NULL;
    }
    return na;

}

TObjArray * DiJetSelector( TObjArray * jets, int djs ){
    //==== DIJET DiJet In Full Acceptance
    TObjArray * dijet = new TObjArray();
    if( djs = kJDiJetInFullAcceptance ){
        AliJJet * jet = (AliJJet*) jets->At(ij);
        for( int ij=0;ij<jets->GetEntriesFast();ij++ ){
            dijet->Add( jet );
            if( dijet->GetEntries() >= 2 ) break;
        }
    }
    //==== DIJET DiJet In Alice Acceptance
    if( djs = kJDiJetInAliceAcceptance ){
        for( int ij=0;ij<jets->GetEntriesFast();ij++ ){
            AliJJet * jet = (AliJJet*) jets->At(ij);
            if( jet->EtaAbs() > fJetEtaRange ) continue;
            dijet->Add(jet);
            if( dijet->GetEntries() >= 2 ) break;
        }
    }
    //==== DIJET 2 : Marta - Find DiJet in Eta
    if( djs = kJDiJetMarta ){
        for( int ij=0;ij<jets->GetEntriesFast();ij++ ){
            AliJJet * jet = (AliJJet*) jets->At(ij);
            if( jet.Area() < 0.3 ) continue;
            if( jet.EtaAbs() > fJetEtaRange ) continue;
            // TODO
            dijet->Add( jet );
        }
    }

}



void AliJDiJetAnalysis::SelectJets(){

    // pythia Incomming
    //jets = fPythiaTool->GetIncommingGroupjet();
    //jets = fPythiaTool->GetOutGoingGroupjet();

    // kJChargedJet, kJFullJet, kJChargedJet08, kJFullJet08, kJIncommingJet, kJOutgoingJet, kJIdealJet, kJNJetType
    // kJEtaAll, kEtaAlice, kNJetSelection
    for( int i=0;i<kNJetType;i++ ){
        jets[i][kJEtaAll] = GetJets(i);
        jets[i][kJEtaAclie] = SelectorEta(-0.4, 0.4, jets[i][kJEtaAll] );
        for( int j=0;j<kNJetSelection;j++ ){
            TObjArray * js = jets[i][j];
            if( !js ) continue;
            for( int ij=0;ij<js;ij++ ){
                AliJJet * jet = (AliJJet*)(jet->At(ij));
                hJetPt->Fill( jet->Pt() );
            }
        }
    }

    // dijet[jettype][jetSelection][dijetselection]
    for( int i=0;i<kNJetType;i++ ){
        for( int j=0;j<kNJetSelection;j++ ){
            for( int d=0;d<kNDijetSelection;d++ ){
                dijet[i][j][d] = GetDiJet( i,j,d );
            }
        }
    }

}



void AliJDiJetAnalysis::SelectDiJet(){
    //comment needed
    int nDiJetType = 0;

    //==== DIJET DiJet In Full Acceptance
    dijet = GetDiJetList( kJDiJetInFullAcceptance );
    for( int ij=0;ij<GetNJets();ij++ ){
        dijet->Add( &Jet(ij) );
        if( dijet->GetEntries() >= 2 ) break;
    }

    //==== DIJET DiJet In Alice Acceptance
    dijet = GetDiJetList( kJDiJetInAliceAcceptance );
    for( int ij=0;ij<GetNJets();ij++ ){
        if( Jet(ij).EtaAbs() > fJetEtaRange ) continue;
        dijet->Add(&Jet(ij));
        if( dijet->GetEntries() >= 2 ) break;
    }

    //==== DIJET 2 : Marta - Find DiJet in Eta
    dijet = GetDiJetList( kJDiJetMarta );
    for(  int i=0 ; i< GetNJets() ; i++ ){
        if( Jet(i).Area() < 0.3 ) continue;
        if( Jet(i).EtaAbs() > fJetEtaRange ) continue;
        dijet->Add( &Jet(i) );
        break;
    }
    // == ATLAS dijet Only in |eta|<0.8
    dijet = GetDiJetList( kJDiJetAtlas );
    for(  int i=0 ; i< GetNJets() ; i++ ){
        if( Jet(i).EtaAbs() > 0.8 ) continue;
        dijet->Add( &Jet(i) );
        break;
    }

    if( dijet->GetEntries() > 0 ){
        AliJJet * leadingJet = (AliJJet*) dijet->At(0);
        for( unsigned int i=0;i< fJetList->GetEntriesFast();i++ ){
            if( Jet(i).Area() < 0.3 ) continue; //TODO
            if( Jet(i).EtaAbs() > fJetEtaRange ) continue;
            double dPhi = leadingJet->DeltaPhi( Jet(i));
            if( fabs(dPhi) < TMath::Pi()/2 ) continue;
            dijet->Add( &Jet(i) );
            break;
        }
    }
    // Check Marta's dijet
    fIsMartasDiJet = 0;
    if( dijet->GetEntriesFast() == 2){
        for( int i = 0; i<2 ; i++ ){
            if( Jet(i).Pt() > 20 ){
                int leadingConPt = 0;
                for( unsigned int ic=0;ic<Jet(i).GetConstituents()->GetEntriesFast();ic++ ){
                    double pt = Jet(i).GetConstituent(ic)->Pt();
                    if( leadingConPt < pt ) leadingConPt = pt;
                    if( leadingConPt > 100 ) break;
                }
                if( leadingConPt >5 && leadingConPt<100 ) fIsMartasDiJet |=1<<i;
            }
        }
    }
    if( fIsMartasDiJet != 3 ) fIsMartasDiJet=0;
}


void AliJDiJetAnalysis::FillHistos(){
    // comment needed

    int nJetPtBin = fJetPtBin->GetNrows()-1;
    int nPtPairBin = fPtPairBins->GetNrows()-1;

    for( int id=0;id<fJetListOfList.GetEntries();id++ ){
        TObjArray & dijets = *GetDiJetList(id);
        if( dijets.GetEntries() < 2 ) continue;
        for( int i1=0;i1<dijets.GetEntriesFast();i1++ ){
            AliJJet *j1 = ((AliJJet*)dijets[i1]);
            double jpt1 = j1->Pt();
            int jetPtBin1 = TMath::BinarySearch(fJetPtBin->GetNrows(), fJetPtBin->GetMatrixArray(), float(jpt1) );
            if( jetPtBin1 < 0 || jetPtBin1 > fJetPtBin->GetNrows()-2 ) jetPtBin1 = -1;
            for( int i2=i1+1;i2<dijets.GetEntries();i2++ ){
                AliJJet *j2 = ((AliJJet*)dijets[i2]);
                double deltaphi = j1->DeltaPhi(*j2);
                double deltaeta = TMath::Abs(j1->Eta()-j2->Eta());
                double jpt2 = j2->Pt();
                int jetPtBin2 = TMath::BinarySearch(fJetPtBin->GetNrows(), fJetPtBin->GetMatrixArray(), float(jpt2) );
                if( jetPtBin2 < 0 || jetPtBin2 > fJetPtBin->GetNrows()-2 ) jetPtBin2 = -1;
                if( jetPtBin1 < 0 || jetPtBin2 < 0 ) continue;
                double dijetmass = TMath::Sqrt(j1->GetLorentzVector()*j2->GetLorentzVector());
                fHistos->fhRecoDiJetM->Fill(dijetmass);
                //if(dijetmass<243 || dijetmass >323 ) continue;
                double ptpair[3];
                ptpair[0] = ( (*j1)+(*j2) ).Pt() ;
                ptpair[1] = fabs(jpt1*TMath::Sin(deltaphi));
                ptpair[2] = fabs(jpt2*TMath::Sin(deltaphi));

                TLorentzVector j2random( *j2 ) ;
                double dPhiRandom = gRandom->Uniform( (2./3.)*TMath::Pi(), (4./3.)*TMath::Pi() );
                j2random.SetPhi( j1->Phi()+dPhiRandom );
                double ptpairRandom[3];
                ptpairRandom[0] = ( (*j1)+j2random).Pt();
                ptpairRandom[1] = fabs( jpt1*TMath::Sin( dPhiRandom ) );
                ptpairRandom[2] = fabs( jpt2*TMath::Sin( dPhiRandom ) );

                int iptpairbin = TMath::BinarySearch( nPtPairBin+1, fPtPairBins->GetMatrixArray(), float(ptpair[0]) );
                if( iptpairbin < 0 || iptpairbin > nPtPairBin - 1 ) iptpairbin = -1;

                int jetid1 = j1->GetUniqueID();
                int jetid2 = j2->GetUniqueID();

                int selDeltaPhi = (fabs(deltaphi)>2./3.*TMath::Pi()) ? 1 : 0;
                int selPtCut = (jpt1>20 && jpt2>20 )? 1 : 0;

                int jetSelection[10]= { 0,0,0,0,0,0,0,0,0,0};
                jetSelection[0] = 1;
                if( id==0 ){ jetSelection[1] = fIsMartasDiJet;}
                else if( selPtCut )  { jetSelection[1] = 1; }
                jetSelection[2] = jetSelection[1] ;
                jetSelection[3] = jetSelection[2] && jpt2/jpt1>0.9;
                jetSelection[4] = jetSelection[2] && jpt2/jpt1>0.7;
                jetSelection[5] = jetSelection[2] && jpt2/jpt1>0.5;

                for( int i=0;i<6;i++ ){
                    int ind = id*6+i;
                    if( jetSelection[i] < 1) continue; 
                    fHistos->hDiJetDeltPhi[ind]->Fill( deltaphi, ptpair[0] );
                    fHistos->fhJetDphi[ind][jetPtBin1]->Fill( deltaphi );
                    if( jetPtBin1 >=0 && iptpairbin >= 0 ){
                        fHistos->hDiJetDeltaPhi[ind][jetPtBin1][iptpairbin]->Fill( deltaphi );
                    }
                }
                jetSelection[2] = jetSelection[2] && selDeltaPhi;
                jetSelection[3] = jetSelection[3] && selDeltaPhi;
                jetSelection[4] = jetSelection[4] && selDeltaPhi;
                jetSelection[5] = jetSelection[5] && selDeltaPhi;


                for( int i=0;i<6;i++ ){
                    int ind = id*6+i;
                    if( jetSelection[i] < 1) continue; 
                    fHistos->hDiJetPtPair[ind][0]->Fill( ptpair[0], jpt1 );
                    fHistos->hDiJetPtPair[ind][1]->Fill( ptpair[1], jpt1 );
                    fHistos->hDiJetPtPair[ind][2]->Fill( ptpair[2], jpt1 );
                    fHistos->hDiJetPtPairRandom[ind][0]->Fill( ptpairRandom[0], jpt1 );
                    fHistos->hDiJetPtPairRandom[ind][1]->Fill( ptpairRandom[0], jpt1 );
                    fHistos->hDiJetPtPairRandom[ind][2]->Fill( ptpairRandom[0], jpt1 );

                    fHistos->hDiJetTriggPt[ind]->Fill( jpt1 );
                    fHistos->hDiJetAssocPt[ind]->Fill( jpt2 );
                    fHistos->hDiJetPt[ind]->Fill( jpt1 );
                    fHistos->hDiJetPt[ind]->Fill( jpt2 );
                    fHistos->hDiJetptRatio[ind]->Fill( jpt2/jpt1 );

                    fHistos->fhJetDeta[ind][jetPtBin1]->Fill( deltaeta );
                    fHistos->fhJetDPt[ind][jetPtBin1]->Fill( jpt1-jpt2 );
                    fHistos->fhJetAssymDiPt[ind][jetPtBin1]->Fill( (jpt1-jpt2)/(jpt1+jpt2) );

                    TLorentzVector sumOfTrack[2];
                    for(int ii=0;ii<fInputList->GetEntriesFast();ii++){ // trigger loop 
                        AliJBaseTrack * track = (AliJBaseTrack*)fInputList->At(ii);
                        int jetid = fTrackJetMap->at( ii );
                        if( jetid==jetid1 || jetid==jetid2 ){
                            fHistos->hDiJetHadronPt[ind]->Fill( track->Pt() ); 
                            sumOfTrack[0] += *track;
                        }else{
                            sumOfTrack[1] += *track;
                        }
                    }
                    fHistos->hDiJetTrackESum[ind][0] -> Fill( sumOfTrack[0].E() );
                    fHistos->hDiJetTrackESum[ind][1] -> Fill( sumOfTrack[1].E() );
                    fHistos->hDiJetTrackESumRatio[ind] -> Fill( sumOfTrack[0].E()/(sumOfTrack[0].E()+sumOfTrack[1].E()));
                }
            }
        }
    }
}

void AliJDiJetAnalysis::FillJetHistos( int it ){
    TObjectArray * jets = GetJetList(it);
    for( int i=0;i<jets->GetEntriesFast();i++ ){
        AliJJet * jet = (AliJJet*) jets->At(i);

    }
}

void  AliJDiJetAnalysis::FillEbyEHistos(int ref){

    for( int i=0;i<nDiJetType;i++ ){

        fEbyEDiJetDPt

    }

}



