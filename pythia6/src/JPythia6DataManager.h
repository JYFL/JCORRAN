//===========================================================
//===========================================================

#ifndef JPYTHIA6DATAMANAGER_H
#define JPYTHIA6DATAMANAGER_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "TPythia6.h"
#include "TParticlePDG.h"
#include "TMCParticle.h"
#include "TParticle.h"

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

class JPythia6DataManager {

  public:


    TRandom3 *randomGen; 

    JPythia6DataManager (AliJCard *incard, AliJHistos *inhistos): 
      card(incard), histos(inhistos){
        randomGen = new TRandom3(0);

        TrackEtaRange = card->Get("EtaRange");
		TriggerFiducialEtaCut  = card->Get("TriggerFiducialEtaCut");
		fParticles = new TClonesArray("TParticle", 1000);
	  }


	Int_t RegisterList(TClonesArray *inputList){
		int counter = 0;
		fParticles->Clear();
		event.ImportParticles(fParticles);
		for( int i=0;i<fParticles->GetEntriesFast();i++ ){
			TParticle   *fParticle = (TParticle *)fParticles->At(i);
			if( (fParticle->GetStatusCode() != 1) || (TMath::Abs(fParticle->GetPDG()->Charge())==0) ) continue; // final and charged
			if( !card->IsInEtaRange(fParticle->Eta())) continue; // eta cut
			TLorentzVector l = TLorentzVector( fParticle->Px(), fParticle->Py(), fParticle->Pz(), fParticle->Energy() );
			AliJBaseTrack track( l );
			track.SetID(counter);
			track.SetParticleType(kJHadron);
			track.SetTrackEff(1.);
			track.SetCharge( Char_t(fParticle->GetPDG()->Charge()) );  // charge of particle
			//AliJBaseTrack track( event[i].px(), event[i].py(), event[i].pz(), event[i].e(), counter, kHadron, 1 );
			new((*inputList)[counter++]) AliJBaseTrack( track );
		}
		return inputList->GetEntriesFast();
	}


	void Print(){
		cout<<endl;
	}

	TPythia6 event;
	TClonesArray *fParticles;
	AliJCard *card;
	AliJHistos *histos;

	int pTtBin, pTaBin;
	double TrackEtaRange, TriggerFiducialEtaCut;

};

#endif

