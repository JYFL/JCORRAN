#include "JPythiaDataManager.h"
#include <set>
#include <map>
#include <vector>
#include <list>

using namespace std;

Int_t JPythiaDataManager::RegisterList(TClonesArray *inputList){
    int counter = 0;
    TVector3 boost;
    boost.SetPtEtaPhi(0, fEtaBoost, 0 );
    vector<int> seen(event.size(),0 );
    int leadingPt = -1;
    
    // Fill parton 23's, note that resonance pythia is a special case treated here
    if( card->Get("DoResonancePythia") ){
        // check that parton 23 pair is at the fiducial acceptance
        double eta23 = event[6].eta();
		double etaFiducial = 0.4;
		/*Solution is pT = M / ( 2*cosh(eta) ).
			For the jets 45 < pt < 50 GeV, i.e. M = 100 GeV, and for 90 < pT < 100 GeV, i.e. M = 200, this leads into a precisely the same condition;
			|eta| < arc cosh(10/9) ~ 0.4671.
        */
        if( abs(eta23) > etaFiducial ){ // 
        //if( abs(eta23) > card->Get("EtaRange") - card->Get("TriggerFiducialEtaCut") ){
            inputList->Clear();
            return 0;
        }
        pyHistos->fhIncEtaParton23->Fill(eta23);
        double pT23 = event[6].pT();
        pyHistos->fhIncPtParton23->Fill(pT23);
        pyHistos->fhIncInvPtParton23->Fill(pT23, pT23>0?1/pT23:1.);
    }
    
    for(int i = 0; i < event.size(); i++){
        
        // parton 23, if not resonance pythia. Note: pT is back-to-back but pz not => untrivial eta
        if( !card->Get("DoResonancePythia") ){
            if( abs( event[i].status() ) == 23 ){
                pyHistos->fhIncEtaParton23->Fill(event[i].eta());
                double pT23 = event[i].pT();
                pyHistos->fhIncPtParton23->Fill(pT23);
                pyHistos->fhIncInvPtParton23->Fill(pT23, pT23>0?1/pT23:1.);
            }
        }
        
        int isSelected = -1;
        switch ( fTrackSelection ){
            case kJCharged:
                if( event[i].isFinal() && event[i].isCharged() && event[i].isHadron() ) isSelected=i;
                break;
            case kJChargedAndNeutral:
                if( event[i].isFinal() && event[i].isCharged() && event[i].isHadron() ) isSelected=i;
                if( event[i].isFinal() &&  event[i].id()==22 ) {
                    int mother = event[i].mother1();
                    if( (! seen[mother]) && event[mother].isHadron()){
                        isSelected = mother;
                        for( int ip=event[mother].daughter1();ip<=event[mother].daughter2();ip++ ){
                            if( event[ip].id()!=22 ) { isSelected=-1;break; }
                        }
                    }
                }
                break;
            case kJChargedAndPhoton:
                if( event[i].isFinal() && event[i].isCharged() && event[i].isHadron() ) isSelected=i;
                if( event[i].isFinal() && event[i].id()==22) isSelected=i;
                break;
            case kJParton71:
                if( event[i].statusAbs()==71 ) isSelected=i;
                break;
            default :
                cout<<"Wrong MCParticleSelection"<<endl;
                gSystem->Exit(1);
        }
        
        if( isSelected < 0 ) continue;
        if( seen[isSelected] ) { continue; }
        else{  seen[isSelected] = 1; }
        
        
        TLorentzVector l = TLorentzVector( event[isSelected].px(), event[isSelected].py(), event[isSelected].pz(), event[isSelected].e() );
        AliJBaseTrack track( l );
        track.SetID(counter);
        track.SetMCIndex( isSelected );
        track.SetParticleType(kJHadron);
        track.SetTrackEff(1.);
        track.SetCharge( event[isSelected].charge()*3) ;
        if( fabs(fEtaBoost) > 1e-4){
            track.Boost(boost);
        }
        if( fabs(track.Eta())>5 ) continue;
        new((*inputList)[counter++]) AliJBaseTrack( track );
        if( track.Pt() > leadingPt ) leadingPt = track.Pt();
    }
    //if( leadingPt < 5 ) { inputList->Clear(); }
    return inputList->GetEntriesFast();
}
