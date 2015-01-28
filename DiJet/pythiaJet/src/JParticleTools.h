//===========================================================
// JParticleTools.h
// D.J Kim
//===========================================================

#ifndef JPARTICLETOOLS_H
#define JPARTICLETOOLS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "AliJBaseCard.h"
#include "AliJJetFinder.h"
#include "AliJBaseTrack.h"
#include "AliJConst.h"
#include "set"
#include "map"
#include "vector"

using namespace std;


class JParticleTools {

	public:

		JParticleTools ();
		JParticleTools (AliJBaseCard *incard, AliJJetFinder *triggJet);
		~JParticleTools(); 

		void Prepare(TClonesArray *inputList);
		bool FindLParticle();
		int JetSubstractedAssocList();
		void ReconstructJets();
		void FillJetHistos(int cBin);
		void Clear();

        AliJJetFinder * GetJetFinder(){ return triggJet; }

		void Print(){
			cout<<endl;
			cout<<"IndexLP = " << indexLP << endl;
			cout<<endl;
		}

		void PrintJets();
		std::vector<int> IndexArrayOfInputParticles;
		std::vector<int> IndexArrayOfAssocParticles;
		TObjArray PointerArrayOfAssocParticles;
		TObjArray PointerArrayOfUEParticles;

		// initialize again in the event loop
		TClonesArray *fAllList;

		int indexLP;
		bool hasLP; 
		AliJBaseTrack *LeadingParticle;

		//Jets
		AliJJetFinder 		*triggJet; // don't delete here and should be decleared before the event loop
		TLorentzVector  *fRecoJetWLP;
		TLorentzVector  *fRecoLeadingJet;
		std::vector<int> GetJetConstituents(){ return fJetConstituents; }
		std::vector<int> GetLeadingJetConstituents(){ return fLeadingJetConstituents; }
		std::vector<int> fJetConstituents;
		std::vector<int> fLeadingJetConstituents;

	private:
		// initialize once in constructor
		AliJBaseCard *card;
		double TrackEtaRange, TriggerFiducialEtaCut;
		double maxEtaRange;
		double maxTriggEtaRange;
		double triggFiducCut;
		TRandom3 *random;
};

#endif

