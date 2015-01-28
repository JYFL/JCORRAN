//===========================================================
//===========================================================

#ifndef JPYTHIADATAMANAGER_H
#define JPYTHIADATAMANAGER_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <Pythia.h>

#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TClonesArray.h"

#include <AliJCard.h>
#include "AliJHistos.h"
#include "AliJBaseTrack.h"
#include "AliJConst.h"
#include "set"
#include "map"
#include "vector"

using namespace std;
typedef std::vector<int> INTS;

using namespace std;
using namespace Pythia8; 

class JPythiaDataManager {

  public:


    TRandom3 *randomGen; 

    JPythiaDataManager (AliJCard *incard, AliJHistos *inhistos): 
      card(incard), histos(inhistos){
        randomGen = new TRandom3(0);

        SearchParentMethod  = 2;    //0=no, 1=closest parton, 2=shower
        TrackEtaRange = card->Get("EtaRange");
        TriggerFiducialEtaCut  = card->Get("TriggerFiducialEtaCut");
      }


    //bool FindLParticle(AliJBaseTrack &track, int SearchParentMethod);
    bool FindMotherPartons(int particleIndex, TLorentzVector particle, int SearchParentMethod);
    bool FindParton71();
    bool ScanTree( int it=0, int depth=0 );
	bool SetEvent( Event &inevent) { 
		event = inevent;
		return event.size();
	}
    bool IsFrom23(int it){
      while( !(event[it].mother1()==0 || event[it].statusAbs()==23 || it < 2) ) it = event[it].mother1();
      return event[it].statusAbs()==23;
    }

	Int_t RegisterList(TClonesArray *inputList){
		int counter = 0;
		for(int i = 0; i < event.size(); i++){
			if( !event[i].isFinal() || !event[i].isCharged() || !event[i].isHadron() ) continue;
			//if( event[i].statusAbs()!=71  ) continue;
			//if( !IsFrom23(i) ) continue;
			if( !card->IsInEtaRange(event[i].eta())) continue;
			TLorentzVector l = TLorentzVector( event[i].px(), event[i].py(), event[i].pz(), event[i].e() );
			AliJBaseTrack track( l );
			track.SetID(counter);
			track.SetParticleType(kJHadron);
			track.SetTrackEff(1.);
      track.SetCharge( Char_t(event[i].chargeType()) );  // charge of particle
			//AliJBaseTrack track( event[i].px(), event[i].py(), event[i].pz(), event[i].e(), counter, kHadron, 1 );
			new((*inputList)[counter++]) AliJBaseTrack( track );
		}
		return inputList->GetEntriesFast();
	}


	void Print(){
		cout<<endl;
		cout<<"IndexLP = " << indexLP << endl;
		cout<< "mother1 = " << mother1 << " mother2 = " <<  mother2-1 << endl;
		for(int im=mother1; im<mother2; im++)cout << im <<"\t px="<< event[im].px() <<"\t py=" <<  event[im].py() << endl;  
	}

	TLorentzVector parton71;
	int SearchParentMethod;

	Event event;
	AliJCard *card;
	AliJHistos *histos;

	int pTtBin, pTaBin;
	int indexLP, indexParton71, indexParton23;
	int mother1, mother2;

	double TrackEtaRange, TriggerFiducialEtaCut;

	// For Scan
	vector<int> fScanStatus;    
	bool fScanInheritFrom23;

	vector<int> fScanParton71s;   // All parton71
	map<int, vector<int> > fScanStringGroup; // [stringid][parton71]
	set<int> fScanStringGroupFrom23;        // set of stringid from 
	vector<int> fScanStringGroupFrom23vector;

};

#endif

