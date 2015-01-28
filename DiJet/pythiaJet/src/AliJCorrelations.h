//===========================================================
// AliJCorrelations.h
//   Created  Thu Apr 17 12:40:29 EEST 2008  by classmaker
//   Jan Rak
//===========================================================

#ifndef ALIJCORRELATIONS_H
#define ALIJCORRELATIONS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <TRandom3.h>  //FK//
#include <TObjArray.h>

#include  "AliJConst.h"

using namespace std;

class AliJCorrelations;
class AliJHistos;
class AliJBaseTrack;
class AliJCard;
class AliJPiZero;
class TProfile;
class JParticleTools;
class JPythiaHistos;

class AliJCorrelations {

    public:
        AliJCorrelations( AliJCard *cardIn, AliJHistos *histosIn);

        virtual ~AliJCorrelations(){;}    //destructor
        AliJCorrelations(const AliJCorrelations& obj);
        AliJCorrelations& operator=(const AliJCorrelations& obj);
  
        void PrintOut(){cout<<"Real correl = "<<fnReal<<"  mixed = "<<fnMix<<endl;}
        TProfile *GetMixStat(){ return fhMixStat; }

        void FillHisto(corrFillType cFTyp, fillType fTyp,    int cBin, int zBin, AliJBaseTrack *ftk1, AliJBaseTrack *ftk2);
        void FillAzimuthHistos (fillType fTyp,    int cBin, int zBin, AliJBaseTrack *ftk1, AliJBaseTrack *ftk2);

        AliJPiZero* GetPiZero(){return fcoPiZero;}

        double GetGeoAccCorrFlat(double deltaEta);
        double GetGeoAccCorrIncl(double deltaEta, int  CentBin, int pTtBin, int  pTaBin);

        void SetSampligInclusive(){fsamplingMethod = 1;}

        bool   IsInPi0PtRangeFast(float fpt ) const { return fPi0PtRange[0] < fpt && fpt < fPi0PtRange[1];}
        bool   IsInPi0MassRangeFast(float mass) const { return fPi0MassRange[0] < mass && mass < fPi0MassRange[1];}
        bool   IsInPi0AsymmetryRangeFast(double asymm) const { return fPi0AsymmetryRange[0] < asymm && asymm < fPi0AsymmetryRange[1];}

        double DeltaPhi(double phi1, double phi2);

        //double DeltaPhi(double phi1, double phi2) {
        //    double res =  atan2(sin(phi1-phi2), cos(phi1-phi2));
        //    //return res>-kJPi/3.0 ? res : kJTwoPi+res ; 
        //    return res > dPhiRange ? res : kJTwoPi+res ; 
        //}

        void SetParticleTools(JParticleTools * ptool ){ fPTool = ptool; }
        JParticleTools * GetParticleTools(){ return fPTool; }

        void SetPyHistos( JPythiaHistos * h){ fPyHisto = h; }

    protected:

        AliJCard*   fcard; // card
        AliJHistos* fhistos;  // histos
        JPythiaHistos * fPyHisto;
        int fnReal; // comment me
        int fnMix; // comment me
        int fsumTriggerAndAssoc; // comment me
        int fsamplingMethod; // comment me
        int fIsHeavyIon; // comment me
        double fawayPhiGap; // comment me
        double fDPhiUERegion[2]; // comment me
        double fRGap[30]; // comment me
        double fmaxEtaRange; // comment me
        Bool_t fPi0DoSMCut; // comment me
        float fPi0PtRange[2]; // comment me
        float fPi0MassRange[2]; // comment me
        float fPi0AsymmetryRange[2]; // comment me
        Int_t fPi0UseSM; // comment me
        Int_t fPi0ConvDelta; // comment me

        int fRSignalBin; // comment me
        //double dPhiRange;
        AliJPiZero* fcoPiZero; //local instant copied to pi0 TClonesArray

        TProfile *fhMixStat; // comment me

        JParticleTools * fPTool;

        TRandom3 *frandom; // comment me
};

#endif






















