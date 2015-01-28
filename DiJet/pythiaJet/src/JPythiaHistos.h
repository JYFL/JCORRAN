//===========================================================
// JPythiaHistos.h
//===========================================================

#ifndef JPYTHIAHISTOS_H
#define JPYTHIAHISTOS_H

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

#define kPtMax 20

using namespace std;

/*
   inline ostream &operator << (ostream &out_file, const TLorentzVector &Vec)
   {
   out_file<<"Px="<<Vec.Px()<<" Py="<<Vec.Py()<<" Pz="<<Vec.Pz()<<" E="<<Vec.E()<<" M="<<Vec.M()<<endl;
   out_file<<"Theta="<<Vec.Theta()<<" Phi="<<Vec.Phi()<<" p="<<Vec.Rho()<<endl;
   return(out_file);
   }  
   */

class JPythiaHistos {

    public:
        JPythiaHistos(AliJCard* cardP); //constructor
        virtual ~JPythiaHistos(){delete fhistoList;}    //destructor
        JPythiaHistos(const JPythiaHistos& obj);
        JPythiaHistos& operator=(const JPythiaHistos& obj);

        void scaleNotEquidistantHisto(TH1D *hid, double sc=1){
            for(int i=1;i<= hid->GetNbinsX();i++){
                hid->SetBinContent(i,hid->GetBinContent(i)*sc/hid->GetBinWidth(i));
                hid->SetBinError(i,hid->GetBinError(i)*sc/hid->GetBinWidth(i));
            }   
        }

        // create histograms 
        void CreatePythiaHistos();

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

        TH1D *hZ[kPtDim][kPtDim];
    
        // Sami added these
        TH1D * fhIncPtParton23;     // inclusive pT -distribution of parton 23
        TH1D * fhIncInvPtParton23;  // inclusive invariant pT -distribution of parton 23, filled with weight 1/pT23
        TH1D * fhIncEtaParton23;    // inclusive eta -distribution of parton 23
    
        //===================================================
        // parton 71 Histogram
        //===================================================
        TH1D * fhNParton71 ; // comment me
        TH1D * fhNStringGroup; // comment me
        TH1D * fhNStringGroupFrom[2]; // comment me
        TH1D * fhNTracksInStringGroupFrom[2]; // comment me
        TH1D * fhRapidity71From[2]; // comment me
        TH1D * fhPt71From[2]; // comment me

        TH1D * hPyKT[2][kPtMax][kPtMax];
        TH1D * hPyZt[2][kPtMax][kPtMax];
        TH1D * hPyZa[2][kPtMax][kPtMax];
        TH1D * hPyXhHat[2][kPtMax][kPtMax];
        TH1D * hPyJetTriggPt[2][kPtMax][kPtMax];
        TH1D * hPyJetAssocPt[2][kPtMax][kPtMax];
        TH1D * hPyJetPout[2][kPtMax][kPtMax];
        TH1D * hKtMarta;
        TH1D * hJetPtSpectra;
        TH2D * hKtMult;

    protected:
        double fmaxEtaRange;                       // evident
        double fmaxTriggEtaRange;                  // should be the same as above. Use for GeoAccCorr
        double ftriggFiducCut;                     // fiducial cut for the trigger part in eta. Not in use I think (Jan) 

        TList *fhistoList; // comment me
        bool   fUseDirectory;
        TDirectory * fTopDirectory;

};

#endif
