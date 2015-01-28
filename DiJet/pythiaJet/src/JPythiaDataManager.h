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
#include <TSystem.h>

#include "AliJCard.h"
#include "JPythiaHistos.h"
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
      enum { kJCharged, kJChargedAndNeutral, kJChargedAndPhoton, kJParton71 };
    
    JPythiaDataManager (AliJCard *incard, JPythiaHistos *inpyHistos):
      card(incard), pyHistos(inpyHistos){
        TrackEtaRange = card->Get("EtaRange");
        TriggerFiducialEtaCut  = card->Get("TriggerFiducialEtaCut");
        fEtaBoost = card->Get("EtaBoost");
        fTrackSelection = card->Get("MCParticleSelection");
      }

    
    void SetTrackSelection(int i ){ fTrackSelection=i; }
	bool SetEvent( Event &inevent) { 
		event = inevent;
		return event.size();
	}
    
    Int_t RegisterList(TClonesArray *inputList);
    
    void Print(){ cout<<endl; }

    Event event;
    AliJCard *card;
    JPythiaHistos *pyHistos;

    double TrackEtaRange, TriggerFiducialEtaCut;
    double fEtaBoost;
    int fTrackSelection;

};

#endif

