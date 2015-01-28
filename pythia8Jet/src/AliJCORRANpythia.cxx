// jcorran main class
// used in local and grid execution
// blah
// blah
// blah

#include "AliJCORRANpythia.h"

#include "AliJTrackCounter.h"
#include "TClonesArray.h"

#include "AliJCard.h"
#include "AliJHistos.h"
#include "AliJCorrelations.h"
#include "AliJEventPool.h"

#include  "AliJTrack.h"
#include  "AliJConst.h"



#include "AliJEfficiency.h"
#include <iostream>


ClassImp(AliJCORRANpythia)

AliJCORRANpythia::AliJCORRANpythia() :
    TObject(),
    fExecLocal(kTRUE),
    fFirstEvent(kTRUE),
    fcard(0),
    finputFile(0), 
    fevt(0),
    fhistos(0), 
    fcorrelations(0), 
    fassocPool(0),  
    fchargedHadronList(0), 
	ftriggList(0),  
	fassocList(0), 
	fpairList(0),
	fpairCounterList(0),
	finputList(0), 
	fdmg(0), 
	fnumberEvents(0), 
	fieout(0), 
	fEventCounter(0), 
	fcent(0), 
	fncBin(0), 
	fnPttBin(0), 
	fbTriggCorrel(0), 
	fbLPCorrel(0), 
	fbLPpairCorrel(0), 
	fTrackEtaRange(0), 
	flowerPtAssocBoarder(0), 
	fEfficiency(0),
	fpythiaInfo(0),
	fPythia(0)
{
	// constructor
}

AliJCORRANpythia::AliJCORRANpythia(Bool_t execLocal) :
	TObject(),
	fExecLocal(execLocal),
	fFirstEvent(kTRUE),
	fcard(0),
	finputFile(0), 
	fevt(0),
	fhistos(0), 
	fcorrelations(0), 
	fassocPool(0),  
	fchargedHadronList(0), 
	ftriggList(0),  
	fassocList(0), 
	fpairList(0),
	fpairCounterList(0),
	finputList(0), 
	fdmg(0), 
	fnumberEvents(0), 
	fieout(0), 
	fEventCounter(0), 
	fcent(0), 
	fncBin(0), 
	fnPttBin(0), 
	fbTriggCorrel(0), 
	fbLPCorrel(0), 
	fbLPpairCorrel(0), 
	fTrackEtaRange(0), 
	flowerPtAssocBoarder(0), 
	fEfficiency(0),
	fpythiaInfo(0),
	fPythia(0),
	fsigma1GenRes(0)
{
	// constructor
}

AliJCORRANpythia::~AliJCORRANpythia(){
	// destructor
}

AliJCORRANpythia::AliJCORRANpythia(const AliJCORRANpythia& obj){
	// copy constructor
}

AliJCORRANpythia& AliJCORRANpythia::operator=(const AliJCORRANpythia& obj){
	// copy constructor
	return *this;
}


void AliJCORRANpythia::Initialize() const{

}

void AliJCORRANpythia::UserCreateOutputObjects(){
	// local init
    //---------------------
    //Pythia initialization
    //---------------------
    fPythia = new Pythia;   // Generator.

	if(fcard->Get("DoResonancePythia")==1) { 
		// A class to generate the fictitious resonance initial state.
		fsigma1GenRes = new Sigma1GenRes();
		// Hand pointer to Pythia.
		fPythia->setSigmaPtr( fsigma1GenRes );
	}

	// Read in commands from external file.
	fPythia->readFile(fPythiaConfig.Data());
	fPythia->settings.listChanged();

	// Extract settings to be used in the main program.
	int    fnumberEvents  = fPythia->mode("Main:numberOfEvents");
	SetNumberEvents( fnumberEvents );

	fPythia->readString(Form("Random:seed=%02d",frandom_seed));
	// Initialize. Beam parameters set in .cmnd file.
	fPythia->init();

	fSQRTS =    fPythia->info.eCM() ;

	cout << "jcorran user create output objects ----------------" << endl;


	fIsolationR = fcard->Get("IsolationR");

	fhistos = new AliJHistos( fcard );          
	fhistos->CreateEventTrackHistos();
	fhistos->CreateAzimuthCorrHistos();
	fhistos->CreateXEHistos();
	fhistos->CreateXtHistos();
	fhistos->CreateIAAMoons();

	//TODO: inclusive fhistos shipping along
	char *inclusFileName = NULL;

	fcorrelations = new AliJCorrelations( fcard, fhistos);
	cout<<endl<< " -----" <<endl; 

	//==================================

	//cout<<kParticleTypeStrName[kPhoton]<<" "<<kParticleTypeStrName[fjtrigg]<<endl;
	// EventPool for Mixing
	fassocPool   = new AliJEventPool( fcard, fhistos, fcorrelations, fjassoc);

	fchargedHadronList  = new TClonesArray(kParticleProtoType[kJHadron],1500);      
	ftriggList  = new TClonesArray(kParticleProtoType[fjtrigg],1500);      
	fassocList  = new TClonesArray(kParticleProtoType[fjassoc],1500);      
	finputList = NULL;
	fpairList     = new TClonesArray(kParticleProtoType[fjtrigg],1500);
	fpairCounterList  = new TClonesArray("AliJTrackCounter",1500);

	fdmg = new JPythiaDataManager(fcard, fhistos);

	//==== Read the Data files =====
	if( fExecLocal ){
		fieout = fnumberEvents/20;
		if (fieout<1) fieout=1;
		fhistos->fhEventPerRun->Fill(fnumberEvents>0?log(fnumberEvents):1);
	}

	//-------------------------------------------------------------------
	fEventCounter=0;

	fcent = -1;

	fbTriggCorrel  = fcard->Get("CorrelationType")==0;
	fbLPCorrel     = fcard->Get("CorrelationType")==1;
	fbLPpairCorrel = fcard->Get("CorrelationType")==2;
	fTrackEtaRange = fcard->Get("EtaRange");
	flowerPtAssocBoarder = fcard->GetBinBorder(kAssocType, 0);

	fFirstEvent = kTRUE;
	fevt = -1;

	cout << "end of jcorran user create output objects ----------------" << endl;

}

void AliJCORRANpythia::UserExec(){
	// event loop
	// Generate events. Quit if failure.
	Event	 event      = fPythia->event;
	fdmg->SetEvent( event );

	if(!fPythia->next()){
		cout << " Event generation aborted.\n";
	}
	fevt++;

	if(fevt % fieout == 0) cout << fevt << "\t" << int(float(fevt)/fnumberEvents*100) << "%" << endl ;

	fcent = 1; //ntracks;
	//cout<<"evt="<<fevt <<" c="<<  fcent <<endl;
	int cBin        = 0;// fcard->GetBin(kCentrType, fcent);
	if(cBin<0) return;
	double zVert = 0.;
	int zBin        = 0;//fcard->GetBin(kZVertType, zVert); //should be alway >0; checked in fdmg->IsGoodEvent()

	fEventCounter++;

	//------------------------------------------------------------------
	// Triggers and associated
	//----------------------ooooo---------------------------------------
	if(fjtrigg==kJHadron || fjassoc==kJHadron){
		fchargedHadronList->Clear();
		fdmg->RegisterList(fchargedHadronList);
		// apply efficiencies

		for( int i = 0; i < fchargedHadronList->GetEntries(); i++ ){

			AliJBaseTrack *triggTr = (AliJBaseTrack*)fchargedHadronList->At(i);
			double ptt = triggTr->Pt();

			double effCorr = 1.;///fEfficiency->GetCorrection(ptt, fHadronSelectionCut, fcent);  // here you generate warning if ptt>30
			fhistos->fhTrackingEfficiency[cBin]->Fill( ptt, 1./effCorr );
			triggTr->SetTrackEff( 1./effCorr );
		}
	}

	//---- assign input list ---- 
	if(fjtrigg==kJHadron) finputList = fchargedHadronList;
	int noAllTriggTracks = finputList->GetEntries();
	int noAllChargedTracks = fchargedHadronList->GetEntries();
	fhistos->fhChargedMult[cBin]->Fill(noAllChargedTracks);
	fhistos->fhChargedMultCent->Fill(fcent, noAllChargedTracks>0 ? log(noAllChargedTracks) : 0);

	fhistos->fhCentr->Fill(fcent);
	fhistos->fhiCentr->Fill(cBin);

	//----- Generate trigg list and find LP             --
	//----- Fiducial cut should be used in AliJCorrelations--
	//----------------------------------------------------
	AliJTrackCounter *lPtrackCounter = new AliJTrackCounter(), *LPpairCounter = new AliJTrackCounter();
	AliJBaseTrack *lPTr = NULL;
	int noTriggs=0;
	ftriggList->Clear();
	for(int itrack=0; itrack<noAllTriggTracks; itrack++){
		AliJBaseTrack *triggTr = (AliJBaseTrack*)finputList->At(itrack);
		triggTr->SetTriggBin( fcard->GetBin(kTriggType, triggTr->Pt()) );

		double ptt = triggTr->Pt();
		double etat = triggTr->Eta();
		//TODO iCut == 0;

		double effCorr = 1.0/triggTr->GetTrackEff();

		if( ptt>flowerPtAssocBoarder ){
			//FK//double effCorr = 1./fcard->TrackEfficiency(ptt, fcent);  // here you generate warning if ptt>30
			//double effCorr = 1./fcard->TrackEfficiency(ptt, etat, cBin);  // here you generate warning if ptt>30
			fhistos->fhChargedPt[cBin]->Fill(ptt, effCorr );
			fhistos->fhChargedPtNoCorr[cBin]->Fill( ptt );
			fhistos->fhChargedEta->Fill(triggTr->Eta(), effCorr);
			//fhistos->fhChargedPtJacek[cBin]->Fill(ptt, effCorr );
			fhistos->fhChargedPtJacek[cBin]->Fill(ptt, ptt>0 ? 1./ptt*effCorr : 0); //One CANNOT do 1/ptt here!! First unfold.
			fhistos->fhChargedPtFiete->Fill(ptt, effCorr );
		}

		if( !triggTr->IsInTriggerBin() ) continue;
		//FK//triggTr->SetTrackEff( fcard->TrackEfficiency(triggTr->Pt(), fcent) );
		int iptt = triggTr->GetTriggBin();
		fhistos->fhIphiTrigg[cBin][iptt]->Fill( triggTr->Phi(), effCorr);
		fhistos->fhIetaTrigg[cBin][iptt]->Fill( triggTr->Eta(), effCorr);

		if( ptt > lPtrackCounter->Pt() ) {
			lPtrackCounter->Store(noTriggs, ptt, iptt);
			lPTr = triggTr;
		}
		//cout <<"1 ptt="<< triggTr->Pt() <<" bin="<< triggTr->GetTriggBin() << " st=" << triggTr->GetStatus() << " trigg eff=" << triggTr->GetTrackEff() << endl; 
		new ((*ftriggList)[noTriggs++]) AliJBaseTrack(*triggTr);
		fhistos->fhTriggMult[cBin][iptt]->Fill(noAllTriggTracks);
	}

	//----------------------------------------------------
	//----- Find sum of two leading particles ------------
	//----------------------------------------------------
	if(fbLPpairCorrel){
		int noPairs=0;
		fpairList->Clear();
		fpairCounterList->Clear();
		for(int ii=0;ii<noTriggs-1;ii++){
			AliJBaseTrack *triggTr1 = (AliJBaseTrack*)ftriggList->At(ii);
			for(int jj=ii+1;jj<noTriggs;jj++){
				AliJBaseTrack *triggTr2 = (AliJBaseTrack*)ftriggList->At(jj);
				TLorentzVector lVPair = triggTr1->GetLorentzVector()+triggTr2->GetLorentzVector();
				double fdphi = DeltaPhi(triggTr1->Phi(), triggTr2->Phi());
				new ((*fpairList)[noPairs]) AliJBaseTrack(lVPair);
				new ((*fpairCounterList)[noPairs++]) 
					AliJTrackCounter( triggTr1->GetID(), triggTr2->GetID(), ii, jj,  fdphi );
			}
		}

		// ----- Find the Leading Pair --------------------------
		AliJBaseTrack *pairTr = NULL;
		for(int ii=0;ii<noPairs;ii++){
			pairTr = (AliJBaseTrack*)fpairList->At(ii);
			AliJTrackCounter   *pairCounter = (AliJTrackCounter*)fpairCounterList->At(ii);
			//cout<<pairTr->Pt()<<endl;    pairCounter->Print();
			if(pairTr->Pt() > LPpairCounter->Pt() && fabs(pairCounter->GetPairDPhi())<1.0) {
				int iptt  = fcard->GetBin(kTriggType, pairTr->Pt());
				LPpairCounter = pairCounter;
				LPpairCounter->Store(ii, pairTr->Pt(), iptt); 
			}
		}
		if(LPpairCounter->Exists()){
			pairTr->SetParticleType(kJHadron);
			//double effCorr = 1./fcard->TrackEfficiency(lPtrackCounter->GetLPpt());
			fhistos->fhLPpairPt->Fill(pairTr->Pt());
		}
	}

	//--------------------------------------------------
	//---   Generate assoc list and pool             ---
	//--------------------------------------------------
	fassocList->Clear();
	int noAssocs=0;
	double  sumPtAroundLP = 0;
	if(fjassoc==kJHadron) finputList = fchargedHadronList;
	int noAllAssocTracks = finputList->GetEntries();

	for(int itrack=0;itrack<noAllAssocTracks; itrack++){

		AliJBaseTrack *assocTr = (AliJBaseTrack*)finputList->At(itrack);
		double pta = assocTr->Pt();
		double etaa = assocTr->Eta();

		assocTr->SetAssocBin( fcard->GetBin(kAssocType, assocTr->Pt()) );

		if(assocTr->IsInAssocBin()){ 

			int ipta  = assocTr->GetAssocBin();
			//assocTr->SetTrackEff( fcard->TrackEfficiency(pta, fcent) );
			// TODO ???? AGAIN?
			//assocTr->SetTrackEff( fcard->TrackEfficiency(pta, etaa, cBin) );
			double effCorr = 1.0/assocTr->GetTrackEff();
			fhistos->fhIphiAssoc[cBin][ipta]->Fill( assocTr->Phi(), effCorr);
			fhistos->fhIetaAssoc[cBin][ipta]->Fill( assocTr->Eta(), effCorr);
			new ((*fassocList)[noAssocs++]) AliJBaseTrack(*assocTr);
		}
		// determine an activity in the cone around trigger 
		if(lPtrackCounter->Exists() && (lPTr->GetID()!=assocTr->GetID()) && (0.5 < assocTr->Pt())){
			double dPhi = DeltaPhi( assocTr->Phi(), lPTr->Phi() );
			double dEta = assocTr->Eta() - lPTr->Eta();
			if(fIsolationR > sqrt(dPhi*dPhi+dEta*dEta))  sumPtAroundLP += assocTr->Pt();//FK//
		}
	}


	FillXtHistos(finputList, lPtrackCounter);

	//-----------------------------------------------
	// Set the isolation flag
	//-----------------------------------------------
	if( lPtrackCounter->Exists() ){
		//double effCorr = 1./fcard->TrackEfficiency(lPtrackCounter->Pt(), fcent);
		//TODO ??? AGAIN AGAIN???
		double effCorr = 1.;///fEfficiency->GetCorrection(lPtrackCounter->Pt(), fHadronSelectionCut, fcent );
		fhistos->fhLPpt->Fill(lPtrackCounter->Pt(), effCorr);
		fhistos->fhLPeta->Fill(lPTr->Eta(), effCorr);
		fhistos->fhBkgActivity[lPtrackCounter->GetPtBin()]->Fill(sumPtAroundLP/lPtrackCounter->Pt());

		if( sumPtAroundLP/lPtrackCounter->Pt()< fcard->GetCutOnBkgActivity() &&     ///relative activity
				( fabs(lPTr->Eta()) < (fTrackEtaRange - fIsolationR) )   ){ //fiducial cut 

			fhistos->fhIsolatedLPpt->Fill(lPtrackCounter->Pt(), effCorr );

			AliJBaseTrack* lpTrigger = (AliJBaseTrack*) ftriggList->At(lPtrackCounter->GetIndex());
			lpTrigger->SetIsIsolated(1);
		}
	}

	fhistos->fhAssocMult->Fill(noAssocs);
	//cout<"Triggs = "<<<noTriggs<<" Assoc = "<<noAssocs<<" all = "<<noAllTracks<<endl;

	if(noAssocs>0 ) fassocPool->AcceptList(fassocList, fcent, zVert, noAllChargedTracks, fevt);

	//------------------------------------------------------------------
	// Do the Correlation 
	//----------------------ooooo---------------------------------------
	int noTriggTracs=-1;
	noTriggTracs = fbTriggCorrel ? noTriggs : 1;
	if(fbLPCorrel && !lPtrackCounter->Exists()) return;
	if(fbLPpairCorrel && !LPpairCounter->Exists()) return;
	AliJBaseTrack *triggTr = NULL;

	for(int ii=0;ii<noTriggTracs;ii++){ // trigger loop 
		if (fbTriggCorrel)  triggTr = (AliJBaseTrack*)ftriggList->At(ii);
		if (fbLPCorrel)     triggTr = (AliJBaseTrack*)ftriggList->At(lPtrackCounter->GetIndex());
		if (fbLPpairCorrel) triggTr = (AliJBaseTrack*)fpairList->At(LPpairCounter->GetIndex());
		//for(int ii=0;ii<noIsolPizero;ii++){ // trigger loop }
		//AliJBaseTrack *triggTr = (AliJBaseTrack*)isolPizeroList->At(ii);
		double ptt = triggTr->Pt();
		int iptt   = triggTr->GetTriggBin(); 
		if(iptt<0) {
			cout<<"Not registered trigger ! I better stop here." <<endl; 
			exit(-1);
		}
		double effCorr = 1.0/triggTr->GetTrackEff();
		fhistos->fhTriggPtBin[cBin][zBin][iptt]->Fill(ptt, effCorr);//inclusive

		if(triggTr->GetIsIsolated()>0) fhistos->fhTriggPtBinIsolTrigg[kReal][cBin][iptt]->Fill(ptt, effCorr);

		for(int jj=0;jj<noAssocs;jj++){ // assoc loop
			AliJBaseTrack  *assocTr = (AliJBaseTrack*)fassocList->At(jj);
			//assocTr->PrintOut("assoc track");
			if(fbLPpairCorrel && 
					(assocTr->GetID()==LPpairCounter->GetPairTrackID(0) || 
					 assocTr->GetID()==LPpairCounter->GetPairTrackID(1)) ) continue;
			//-------------------------------------------------------------
			fcorrelations->FillAzimuthHistos(kReal, cBin, zBin, triggTr, assocTr);
			//-------------------------------------------------------------
		} // end assoc loop
	} // end trigg loop
	// == mix trigg with assoc
	if (fbLPpairCorrel) 
		fassocPool->Mix(fpairList,  kAzimuthFill, fcent, zVert, noAllChargedTracks, fevt);
	else
		fassocPool->Mix(ftriggList, kAzimuthFill, fcent, zVert, noAllChargedTracks, fevt);

	//--------------------------------------------------------------
	// End of the Correlation
	//--------------------------------------------------------------



}

void AliJCORRANpythia::Terminate() {
	// termination

	fPythia->statistics();
	long nTried = fPythia->info.nTried();
	long nSelected = fPythia->info.nSelected();
	long nAccepted = fPythia->info.nAccepted();
	double sigmaGen = fPythia->info.sigmaGen();
	double sigmaErr = fPythia->info.sigmaErr();
	TString pythiaStat = Form("eCM = %d, nTried = %lu, nSelected = %lu, nAccepted = %lu, sigmaGen = %.3f, sigmaErr = %.3f",fSQRTS,nTried,nSelected,nAccepted,sigmaGen,sigmaErr);
	TNamed *fpythiaStat = new TNamed("pythiaStat",pythiaStat.Data());
	fpythiaStat->Write();
	// setting eCM, IFR, MPI, KT....
	fPythia->settings.listAll();
	fPythia->settings.writeFile(fPythiaSettingAll.Data(), true);  
	SavePythiaSetting();

	delete fpythiaStat;
	delete fpythiaInfo;

	delete fPythia;

	if (fsigma1GenRes) delete fsigma1GenRes;

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
	//fEfficiency->Write();
}

particleType  AliJCORRANpythia::GetParticleType(char *inchar){
	// part type
	for(int i=0;i<kNumberOfParticleTypes;i++) {
		//cout<<"<"<<inchar<<"> <"<<particleTypeStr[i]<<"> "<<strcmp(inchar,particleTypeStr[i])<<" "<<(particleType)i<<endl;
		if(strcmp(inchar,kParticleTypeStrName[i])==0) return (particleType)i;
	}
	std::cout<<"Particle type <"<<inchar<<"> not recognized"<<std::endl;
	exit(1);
}

double AliJCORRANpythia::DeltaPhi(double phi1, double phi2) {
	// dphi
	double res =  atan2(sin(phi1-phi2), cos(phi1-phi2));
	//return res>-kJPi/3.0 ? res : kJTwoPi+res ; 
	return res>-kJPi*9./20. ? res : kJTwoPi+res ; 
}

void AliJCORRANpythia::ScaleNotEquidistantHisto(TH1D *hid, const double sc=1){
	// histo scaler
	for(int i=1;i<= hid->GetNbinsX();i++){
		hid->SetBinContent(i,hid->GetBinContent(i)*sc/hid->GetBinWidth(i));
		hid->SetBinError(i,hid->GetBinError(i)*sc/hid->GetBinWidth(i));
	}   
}

void AliJCORRANpythia::SavePythiaSetting(){
	// read card
	ifstream infile;
	TString ms;
	string line;

	infile.open(fPythiaSettingAll.Data(),ios::in);
	if (infile.is_open())
	{
		while ( infile.good() )
		{
			getline (infile,line);
			cout << line << endl;
			ms += line + "\n";
		}
		infile.close();
	}
	else cout << "Unable to open file"; 
	fpythiaInfo = new TNamed("pythiaInfo",ms.Data());
	fpythiaInfo->Write();

	return;
}

// Fill xT histograms (Esko)
void AliJCORRANpythia::FillXtHistos(TClonesArray *finputList, AliJTrackCounter *lPtrackCounter){

	enum xTtype { kInclusive=0, kIsolated=1, kIsolatedLP=2} ;
	double LowerPtCut = 0.2;
	double IsolationR = 0.4;
	double fCutOnBkgActivity = 0.10;
	int cBin  = fcard->GetBin(kCentrType, fcent);
	double sqrts = fSQRTS;
	int triggCounter = 0, assocCounter = 0;
	int noAllTriggTracks = finputList->GetEntries();
	//cout << "Entering Esko's analysis loop" << endl;
	//cout << "Sqrts = " << sqrts << endl;
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
			if( LowerPtCut < pta ){
				double dPhi = DeltaPhi( assocTr->Phi(), triggTr->Phi() );
				double dEta = assocTr->Eta() - triggTr->Eta();
				if(IsolationR > sqrt(dPhi*dPhi+dEta*dEta))  sumPtAroundTrigger += assocTr->Pt();//FK//
			}
			//cout << "  Assoc number " << assocCounter++ << endl;
		}
		// If pT sum is below the limit, fill to the isolated histos
		if( sumPtAroundTrigger/ptt < fCutOnBkgActivity &&     ///relative activity
				( fabs(triggTr->Eta()) < (fTrackEtaRange - IsolationR) )   ){ //fiducial cut
			// pT and xT
			// kInclusive kIsolated, kLPIsolated
			fhistos->fhPtForXt[kIsolated][cBin]->Fill(ptt,effCorr*1.0/ptt);
			fhistos->fhXt[kIsolated][cBin]->Fill(xtt, effCorr );
			fhistos->fhXtWeighted[kIsolated][cBin]->Fill(xtt,effCorr*1.0/xtt);
		}
		//cout<<"pT sum around trigger = " << sumPtAroundTrigger << endl;
	}

} // end FillXtHistos
