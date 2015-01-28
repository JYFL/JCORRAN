#ifndef ALIJCORRANPYTHIA6_H
#define ALIJCORRANPYTHIA6_H

#include <JPythia6DataManager.h>
#include <AliJConst.h>
#include <TPythia6.h>

// jcorran main class
// used in local and grid execution
// blah

class AliJCard;
class AliJHistos;
class AliJCorrelations;
class AliJEventPool;
class AliJHistos;
class AliJEfficiency;
class JPythia6DataManager;
class AliJTrackCounter;

class TClonesArray;
class TF1;

class AliJCORRANpythia6 : public TObject
{
	public:
		AliJCORRANpythia6();
		AliJCORRANpythia6(Bool_t execLocal);
		virtual ~AliJCORRANpythia6();
		AliJCORRANpythia6(const AliJCORRANpythia6& obj);
		AliJCORRANpythia6& operator=(const AliJCORRANpythia6& obj);

		void Initialize() const;
		void UserCreateOutputObjects();
		void UserExec();
		void Terminate();

		Int_t GetNumberEvents() const { return fnumberEvents; }
		AliJHistos *GetHistos() { return fhistos; }
		AliJCorrelations *GetCorrelations() { return fcorrelations; }
		AliJEventPool *GetAssocPool() { return fassocPool; }
		Int_t GetEventCounter() const { return fEventCounter; }
		AliJCard *GetCard() { return fcard; }
		void FillXtHistos(TClonesArray*, AliJTrackCounter *lPtrackCounter); // Esko

		void SetCard( AliJCard *c ) { fcard = c; }
		void SetTrigger( char* p ) { fjtrigg = GetParticleType(p); }
		void SetAssoc( char* p ) { fjassoc = GetParticleType(p); }
		void SetInputFile( char *f ) { finputFile = f; }
		void SetNumberEvents( Int_t n ) { fnumberEvents = n; }
		void SetRandomSeed( Int_t n ) { frandom_seed = n; }
		void SetPythiaConfig( TString config) { fPythiaConfig = config; }
		void SetPythiaSettingAll( TString settingAll) { fPythiaSettingAll = settingAll; }
		void SetBeamCMEnergy( Float_t eCM ) { fSQRTS = eCM; }

		double DeltaPhi(double phi1, double phi2);
		particleType  GetParticleType(char *inchar);
		void ScaleNotEquidistantHisto(TH1D *hid, const double sc);
		void SavePythiaSetting();



	private:

		Bool_t fExecLocal; // exec mode
		Bool_t fFirstEvent; //!

		particleType fjtrigg; // assoc
		particleType fjassoc; // trigger

		AliJCard *fcard; // card
		char * finputFile; //!

		Int_t fevt; // event number
		AliJHistos *fhistos; //!
		AliJCorrelations *fcorrelations; //!
		AliJEventPool *fassocPool; //! 
		TClonesArray *fchargedHadronList; //!
		TClonesArray *ftriggList; //! 
		TClonesArray *fassocList; //!
		TClonesArray *fpairList; //!
		TClonesArray *fpairCounterList; //!
		TClonesArray *finputList; //!

		JPythia6DataManager *fdmg;

		int fnumberEvents; //!
		int fieout; //!
		int fEventCounter; //!
		double fcent; //!
		Int_t fncBin; //!
		Int_t fnPttBin; //!
		bool fbTriggCorrel; //!
		bool fbLPCorrel; //!
		bool fbLPpairCorrel; //!
		double fTrackEtaRange; //!
		double flowerPtAssocBoarder; //!

		AliJEfficiency *fEfficiency;
		TNamed *fpythiaInfo;
		TPythia6 *fPythia;
		Int_t frandom_seed;
		TString fPythiaConfig;
		TString fPythiaSettingAll;
		float fIsolationR;
		Float_t fSQRTS;

		ClassDef(AliJCORRANpythia6, 1); // EMCAL for jcorran

};

#endif
























