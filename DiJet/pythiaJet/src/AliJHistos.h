//===========================================================
// AliJHistos.h
//  
//   J
//===========================================================

#ifndef ALIJHISTOS_H
#define ALIJHISTOS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TList.h>
#include <TLorentzVector.h>

#include  "AliJConst.h"

class AliJCard;
class AliJBaseTrack;
class AliJPhoton;
class AliJTrack;
class AliJPiZero;

using namespace std;

inline ostream &operator << (ostream &out_file, const TLorentzVector &Vec)
{
    out_file<<"Px="<<Vec.Px()<<" Py="<<Vec.Py()<<" Pz="<<Vec.Pz()<<" E="<<Vec.E()<<" M="<<Vec.M()<<endl;
    out_file<<"Theta="<<Vec.Theta()<<" Phi="<<Vec.Phi()<<" p="<<Vec.Rho()<<endl;
    return(out_file);
}  

class AliJHistos {

    public:
        AliJHistos(AliJCard* cardP); //constructor
        virtual ~AliJHistos(){delete fhistoList;}    //destructor
      AliJHistos(const AliJHistos& obj);
      AliJHistos& operator=(const AliJHistos& obj);

        // create histograms 
        void CreatePi0MassHistos();
        void CreateAzimuthCorrHistos();
        void CreateIAAMoons();
        void CreateXEHistos();
        void CreateXtHistos();

        void CreateEventTrackHistos();
        void CreateJetHistos();

        void CreatePairPtCosThetaStar();
        void CreatePtCorrHistos();
        void CreateRunByRunHistos(int runID, int runcounter);

        void ReadInclusiveHistos(const char *inclusFileName);
        
        TList *GetHistoList() { return fhistoList; }

        void UseDirectory(bool b) { fUseDirectory=b; }
        bool UseDrectory(){ return fUseDirectory; }

        TDirectory * MakeDirectory(TString name){
            JumpToDefalutDirectory();
            TDirectory * dir = gDirectory->GetDirectory(name);
            if( !dir ) dir = gDirectory->mkdir(name);
            dir->cd();
            return dir;
        }
        TDirectory * JumpToDefalutDirectory(){
            fTopDirectory->cd();
            return gDirectory;
        }

    public:
        AliJCard  *fcard; // comment me
        char  fhname[40], fhtit[40]; // comment me
        TString fhtyp[3]; // comment me

        TProfile *fhMixStat; // comment me

        //==Pi0 Invariant Mass Spectra===========================================
        TH1D *fhPi0PtSgn[kMaxNoCentrBin],      *fhPi0PtBgn[kMaxNoCentrBin]; // comment me
        TH1D *fhPi0PtMix[kMaxNoCentrBin]; // comment me
        TH1D *fhKMass[2];   //K0 short
        TH2D *fhPi0Mass[2]; //pi0 
        TH1D *fhPi0TriggMass[2][kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhPairTheta[kMaxNoCentrBin][kPtDim]; // comment me

        //==Pt stat. fcorrelations ===============================================
        TH1D *fhPtNear[2][2][kMaxNoCentrBin]; // comment me
        TH1D *fhPtFar [2][2][kMaxNoCentrBin]; // comment me

        TH1D *fZKlong[kPtDim];

        //assorted
        TH1D *fhPhi           [kMaxNoCentrBin]; // comment me
        TH1D *fhDphiAssoc  [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; // comment me
        TH1D *fhDphiAssocXEbin  [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; // comment me
        //2D
        TH2D *fhDphiAssoc2DIAA[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; // comment me
        TH2D *fhDphiAssoc2D[kPtDim][kPtDim]; // comment me

        TH1D *fhDphiAssocIsolTrigg [2][kMaxNoCentrBin][kPtDim][kPtDim]; // comment me
        TProfile *fhMeanPtAssoc [kMaxNoCentrBin][kPtDim][kPtDim]; // comment me
        TProfile *fhMeanZtAssoc [kMaxNoCentrBin][kPtDim][kPtDim]; // comment me
        TH1D *fhPtAssocUE[kMaxNoCentrBin][kPtDim][kPtDim], *fhPtAssocN[kPtDim], *fhPtAssocF[kPtDim]; //FK// Eta Gap dependent UE
        TH1D *fhPtaEtaGapN[kMaxNoCentrBin][kPtDim][kPtDim]; //FK// pta spectrum with Eta Gap 
        TH1D *fhPtaRGapN[kMaxNoCentrBin][kPtDim][kPtDim]; //FK// pta spectrum with Eta Gap 
        TH1D *fhPtAssocUEIsolTrigg[kPtDim]; //FK// trigger isolated hadron

        //cosThetaStar
        TH1D *fhCosThetaStar[2][kMaxNoCentrBin][kPtDim]; // comment me
        TH2D *fhCMSrap[kMaxNoCentrBin][kPtDim];           // comment me
        TProfile *fpCMSrap; // comment me
        TH1D *fhInvMass[kPtDim], *fhPairPtMass[kPtDim], *fhPairDPhi[kPtDim], *fhPairDpT[kPtDim]; // comment me
        TH1D *fhPairPtDphi[3][kPtDim][kPtDim], *fhPairPt[3][kPtDim][kPtDim]; // comment me

// xxx
//         TH1D *hDEtaNear    [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim];
         TH1D *fhDEtaNear    [kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me
         TH1D *fhDEtaNearM    [kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me
         TH1D *fhDEtaNearXEbin    [kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me
         TH1D *fhDEtaNearMXEbin    [kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me

        TH1D *fhDRNearPt[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; // comment me
        TH1D *fhDRFarPt [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; // comment me
// xxx
//         TH1D *hDRNearPtMoon[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim];
//         TH1D *hDRFarPtMoon [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim];
         TH1D *fhDRNearPtMoon[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me
         TH1D *fhDRFarPtMoon [kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me
         TH1D *fhDRNearPtMoonM[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me
         TH1D *fhDRFarPtMoonM [kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim]; // comment me

        //TH1D *hDEtaUE      [2][kMaxNoCentrBin][kPtDim][kPtDim];
        TH1D *fhDEtaFar     [2][kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhIphiTrigg      [kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhIetaTrigg      [kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhIphiAssoc      [kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhIetaAssoc      [kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhFixPtBin       [kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhTriggPtBin     [kMaxNoCentrBin][kPtDim][kPtDim]; // comment me
        TH1D *fhTriggPtBinIsolTrigg  [2][kMaxNoCentrBin][kPtDim]; //FK// trigger isolated hadron
        TH1D *fhTriggMult      [kMaxNoCentrBin][kPtDim]; // comment me
        TH1D *fhAssocPtBin     [kMaxNoCentrBin][kPtDim][kPtDim]; // comment me
        //TH1D *hInvMassLike[2][kMaxNoCentrBin][kPtDim], *hInvMassUnLike[2][kMaxNoCentrBin][kPtDim];

        //==================================================
        // xe slopes - done manually for the pp purpose only
        // you have to determine 7 trigger bins
        //==================================================
        TH1D     *fhxEN[2][kPtDim], *fhxEF[2][kPtDim],  *fhxEFIsolTrigg[2][kPtDim]; // comment me
        TH1D     *fhPoutF[2][kMaxNoCentrBin][kPtDim][kPtDim]; // comment me
		TH1D	 *fhxEPtBin[3][kPtDim][kPtDim]; // all=0,near=1,far=2
		// xe bins
        TH1D     *fhJT[2][kMaxNoCentrBin][kPtDim][kPtDim];  // comment me
        TH1D     *fhJTBg[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];  // comment me
        TH1D     *fhJTBgR[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];  // comment me
		// klong bins
        TH1D     *fhJTKlong[2][kMaxNoCentrBin][kPtDim][kPtDim];  // comment me
        TH1D     *fhJTKlongBg[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];  // comment me
        TH1D     *fhJTKlongBgR[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];  // comment me
		// pta bins
        TH1D     *fhJTPta[2][kMaxNoCentrBin][kPtDim][kPtDim];  // comment me
        TH1D     *fhJTPtaBg[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];  // comment me
        TH1D     *fhJTPtaBgR[kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];  // comment me
        TH1D     *fhZKlong[kMaxNoCentrBin][kPtDim];

        //FK//mix2    inclusve spectra
        TH1D *fhIetaTriggFromFile  [kMaxNoCentrBin][kPtDim];//FK//mix2 
        TH1D *fhIetaAssocFromFile  [kMaxNoCentrBin][kPtDim];//FK//mix2 
        TH1D *fhIphiTriggFromFile  [kMaxNoCentrBin][kPtDim];//FK//mix2 
        TH1D *fhIphiAssocFromFile  [kMaxNoCentrBin][kPtDim];//FK//mix2 
        TH1D *fhDphiAssocMixFromFile  [kMaxNoCentrBin][kPtDim][kPtDim];//FK//mix2

        TH1D *fhDEtaNearMixFromFile[kMaxNoCentrBin][kPtDim][kPtDim]; // comment me

        //===================================================
        // Event/Track histograms
        //===================================================
        TH1D *fhLPpt; // comment me
        TH1D *fhLPpairPt; // comment me
        TH1D *fhChargedPt[kMaxNoCentrBin],  *fhChargedPtNoCorr[kMaxNoCentrBin]; // comment me
        TH1D *fhChargedPtJacek[kMaxNoCentrBin]; // comment me
        TH1D *fhChargedPtJacekEta[kMaxNoCentrBin][3]; // 3 bins in eta
        TH1D *fhChargedPtFiete; // comment me
        TH1D *fhVdelta2[kMaxNoCentrBin], *fhVdelta3[kMaxNoCentrBin], *fhVN[kMaxNoCentrBin]; // comment me
        TProfile *fhTrackingEfficiency[kMaxNoCentrBin];
        TProfile *fpV2, *fpV3, *fpVdeltaNorm; // comment me
        TH1D *fhChargedEta; // comment me
        TH1D *fhLPeta; // comment me
        TH1D *fhAssocMult; // comment me
        TH1D *fhChargedMult[kMaxNoCentrBin], *fhChargedMultCut[kMaxNoCentrBin]; // comment me
        TH2D *fhChargedMultCent; // comment me

		// XT histogrmas
		TH1D     *fhXt[kPtDim][kMaxNoCentrBin];
		TH1D     *fhXtWeighted[kPtDim][kMaxNoCentrBin];
		TH1D     *fhXtWeightedHT[kPtDim][kMaxNoCentrBin]; // HT pions
        TH1D 	 *fhPtForXt[kPtDim][kMaxNoCentrBin]; // comment me
        TProfile *hConeActivity;          // pT sum in cone, to be compared to the ALICE UE results
        TProfile *hConeActivityIsolated;  // activity for isolated triggers
        TProfile *hPerpConeActivity;      // pT sum in cone perpendicular to the leading particle
        TProfile *hPerpConeActivityIsolated;  // same as above but for isolated leading particle

		TH1D * fhV0AMult[kMaxNoCentrBin];

		TH1D *fhZVertRaw; // comment me
		TH1D *fhZVertRawErr; // comment me
		TH1D *fhZVert[kMaxNoCentrBin]; // comment me
		TH1D *fhCentr; // comment me
		TH1D *fhiCentr; // comment me
		TH1D *fhEventPerRun; // comment me
		TH2D* fhVertexZTriggVtx; // comment me

		TH1D *fhIsolatedLPpt; // comment me
		TH1D *fhBkgActivity[15]; // comment me

		// D.J
		//===================================================
		// Jet with LP Histograms
		//===================================================
		TH1D *fhDphiLPJet  [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhDEtaLPJet  [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhDPtLPJet  [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhLPJetPTt  [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhLPJetPt   [kMaxNoCentrBin]; // comment me
		TH1D *fhLPJetEtaPTt  [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhLPJetRapidityPTt  [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhLPJetMassPTt  [kMaxNoCentrBin][kPtDim]; // comment me

		TH1D *fhLeadingJetWLPPTt  [kMaxNoCentrBin][kPtDim]; // comment me

		TH1D *fhJetPt   [kMaxNoCentrBin]; // comment me
		TH1D *fhLeadingJetPt   [kMaxNoCentrBin]; // comment me
		TH1D *fhLeadingJetWLPPt   [kMaxNoCentrBin]; // comment me

		TH1D *fhJetAssymPTt   [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhJetMassPTt    [kMaxNoCentrBin][kPtDim]; // comment me
		TH1D *fhJetUEPt; // comment me

		//===================================================
		// Jet Histograms
		//===================================================
		TH1D * fhJetMultPt[kPtDim]; // comment me

		//==================================================
		// Background study
		//==================================================
		TH1D * fhJetRho[kPtDim]; // comment me
		TH1D * fhJetRhoSigma[kPtDim]; // comment me

		TH1D * fhJetPartMult[kPtDim]; // comment me

		TH1D *fhRecoDiJetdPhi[kPtDim]; // comment me
		TH1D *fhRecoDiJetkT[kPtDim]; // comment me

		//===================================================
		// parton 71 Histogram
		//===================================================
		TH1D * fhNParton71 ; // comment me
		TH1D * fhNStringGroup; // comment me
		TH1D * fhNStringGroupFrom[2]; // comment me
		TH1D * fhNTracksInStringGroupFrom[2]; // comment me
		TH1D * fhRapidity71From[2]; // comment me
		TH1D * fhPt71From[2]; // comment me

		//===================================================
		// PHENIX histograms
		//===================================================
		//==Run-by-Run calib ================================
		TH1D *fTofPbSc[kMaxNoRuns]; TH1D *fTofPbGl[kMaxNoRuns]; // comment me
		//==Pt and FlipSlide Spectra=============================================
		TH1D *fhCglPt3PC[kMaxNoCentrBin],    *fhCglPtFlip3PC[kMaxNoCentrBin]; // comment me
		TH1D *fhCglPt32PC[kMaxNoCentrBin],   *fhCglPtFlip32PC[kMaxNoCentrBin]; // comment me

		//===================================================
		// Jiri pi0 fhistos
		//===================================================
		TH1D *fhAsymm[kMaxNoCentrBin][4]; // comment me
		TH1D *fhConvDelta[kMaxNoCentrBin][4]; // comment me
		TH1D *fhZDelta[kMaxNoCentrBin][4]; // comment me
		TH2D *fhConvZDelta[kMaxNoCentrBin][4]; // comment me

		// additional histos
		TH1D *fhEvents;

	protected:
		double fmaxEtaRange;                       // evident
		double fmaxTriggEtaRange;                  // should be the same as above. Use for GeoAccCorr
		double ftriggFiducCut;                     // fiducial cut for the trigger part in eta. Not in use I think (Jan) 
		int nUE, nUEfar;                           // logarithmic binning for some pT and UE histos
		double uEBinsx[101], uEBinsxFar[101];
		double lr, ur;                             // lower and upper range for dphi histos

		TList *fhistoList; // comment me
		bool   fUseDirectory;
		TDirectory * fTopDirectory;

};

#endif






















