//===========================================================
//===========================================================

#ifndef JHERWIGDATAMANAGER_H
#define JHERWIGDATAMANAGER_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>

#include <ThePEG/Handlers/AnalysisHandler.h>
#include <ThePEG/EventRecord/Particle.h>
#include <ThePEG/EventRecord/Event.h>
#include <ThePEG/EventRecord/StandardSelectors.h>

using namespace ThePEG;
//using namespace Herwig;

#include <TLorentzVector.h>
#include <TRandom3.h>
#include <TClonesArray.h>

#include "AliJCard.h"
#include "AliJHistos.h"
#include "AliJBaseTrack.h"
#include "AliJConst.h"

using namespace std;
typedef std::vector<int> INTS;

using namespace std;

class JHerwigDataManager {

	public:


		TRandom3 *randomGen; 

		JHerwigDataManager (AliJCard *incard, AliJHistos *inhistos): 
			card(incard), histos(inhistos){
				randomGen = new TRandom3(0);

				TrackEtaRange = card->Get("EtaRange");
				TriggerFiducialEtaCut  = card->Get("TriggerFiducialEtaCut");
			}


		Int_t RegisterList(EventPtr event, TClonesArray *inputList){
			int counter = 0;
			inputList->Clear();
			tPVector fs=event->getFinalState();
			for(tPVector::iterator it = fs.begin();it != fs.end(); it++) {
				if(!ChargedSelector::Check(**it)) continue;
				if( !card->IsInEtaRange((*it)->eta())) continue; // eta cut
				TLorentzVector l;
				l.SetPxPyPzE((*it)->momentum().x()/GeV,(*it)->momentum().y()/GeV,(*it)->momentum().z()/GeV,(*it)->momentum().e()/GeV);
				AliJBaseTrack track( l );
				track.SetLabel(counter);
				track.SetParticleType(kHadron);
				track.SetTrackEff(1.);
				track.SetCharge( Char_t((*it)->data().charge()) );  // charge of particle
				new((*inputList)[counter++]) AliJBaseTrack( track );
			}
			return inputList->GetEntriesFast();
		}


		void Print(){
			cout<<endl;
		}

		bool isParentID(int IDA, ThePEG::tPPtr B);

		AliJCard *card;
		AliJHistos *histos;

		int pTtBin, pTaBin;
		double TrackEtaRange, TriggerFiducialEtaCut;

};

#endif

