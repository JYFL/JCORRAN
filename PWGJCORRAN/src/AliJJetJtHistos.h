//
//  AliJJetJtHistos.h
//  
//  Jaro Ruuskanen, June-July 2014
//
//

#ifndef ____ALIJJETJTHISTOS__
#define ____ALIJJETJTHISTOS__

#include <iostream>
#include "TH1D.h"
#include "TMath.h"
#include "AliJCard.h"

class AliJJetJtHistos {
	
public:

	//constructor
    //AliJJetJtHistos(int NJetBins, double JetPtBorders[], int NConLimits, double ConPtLowLimits[]);
    //bk
     AliJJetJtHistos( AliJCard * card );
    //AliJJetJtHistos(int NJetBins, double JetPtBorders[], int NConLimits, double ConPtLowLimits[],int NConPtBins, double ConPtBorders[]);
    void CreateJetJtHistos();
    void WriteHistograms();


   


    //bkend

    //histograms
    //pt-histograms
    TH1D *fhChargedPt, *fhChargedPtWeight;
    TH1D *fhChargedPtEsko, *fhChargedPtWeightEsko;
    TH1D *fhChargedPtJacek, *fhChargedPtJacekNoWeight;
    TH1D *fhChargedPtFiete, *fhChargedPtFieteNoWeight;
    TH1D *fhChargedPtEven, *fhChargedPtEvenWeight;
    TH1D *fhAllPt, *fhPhotonPt, *fhionPt, *fhKaonPt, *fhProtonPt;
    TH1D *fhJetPt, *fhJetPtWeight;
    TH1D *fhJetBgPt, *fhJetBgPtWeight;
    TH1D *fhSubtractedJetPt, *fhSubtractedJetPtWeight;
    
    //rho-histogram
    TH1D *fhRho;
    
    //r-histogram
    TH1D *fhRadius;
    
    //area-histogram
    TH1D *fhArea;
    
    //z- and jt-histograms
    TH1D *fhZ;
    TH1D *fhBgZ;
    TH1D *fhJt;
    TH1D *fhBgJt;
	TH1D *fhQ;
    TH1D *fhZBG[10], *fhJtBG[10];
    
    //eta-histograms
    //TH1D *fhChargedEta, *fhPhotonEta, *fhProtonEta, *fhPionEta, *fhKaonEta, *fhJetEta;
    
   	// counter -histogram
	TH1D *fhCounter[10];
	TH1D *fhKCounter[10];
	
	//histograms with jet-pt bins and constituent-pt bins
	//--------------------------------
	//jet R-histogram
    TH1D *fhRadiusBin[10];
    
    //max pt of constituents
    TH1D *fhConMaxPtBin[10];
    
    //number of constituents
    TH1D *fhNumOfConBin[10];
    
    //z, jt and jet-pt
	TH1D *fhJetPtBin[10];
	TH1D *fhJetBgPtBin[10];
	TH1D *fhSubtractedJetPtBin[10];
    
	TH1D *fhZBin[10];
	TH1D *fhBgZBin[10];
	TH1D *fhJtBin[10];
	TH1D *fhBgJtBin[10];
	TH1D *fhJtWeightBin[10];
	TH1D *fhBgJtWeightBin[10];
	TH1D *fhLogJtWeightBin[10];
	TH1D *fhBgLogJtWeightBin[10];
	TH1D *fhPtAssocJET[10];
	TH1D *fhJtBinBin[10][10];
	TH1D *fhPtAssocUE[10][10];
	TH1D *fhJtWeightBinBin[10][10];
	TH1D *fhLogJtWeightBinBin[10][10];
	
	//bk
	TH1D *fhJtWithPtCutWeightBinBin[10][10];
	TH1D *fhBgJtWithPtCutWeightBinBin[10][10];
	TH1D *fhLogJtWithPtCutWeightBinBin[10][10];
	TH1D *fhBgLogJtWithPtCutWeightBinBin[10][10];
	TH1D *fhLogJtWithPtCutWeightBinBinWithNewAxis[10][10];
	//bkend
	

/*
	TH1D *fhZBinBG[10][10];
	TH1D *fhJtBinBG[10][10];
	TH1D *fhJtWeightBinBG[10][10];
	TH1D *fhLogJtWeightBinBG[10][10];
	TH1D *fhJtBinBinBG[10][10][10];
	TH1D *fhJtWeightBinBinBG[10][10][10];
	TH1D *fhLogJtWeightBinBinBG[10][10][10];

	//bk
	TH1D *fhJtWithPtCutWeightBinBinBG[10][10][10];
	TH1D *fhLogJtWithPtCutWeightBinBinBG[10][10][10];
	TH1D *fhLogJtWithPtCutWeightBinBinBGWithNewAxis[10][10];
	//bkend
	
	TH1D *fAliJJetJtHistosJT[10][10][10][10];
	TH1D *fAliJJetJtHistosJTBG[10][10][10][10][10];
	*/
	//testing

	//TH1D *fhEtaDiffR[10][10];
	//TH1D *fhPhiDiffR[10][10];
	//TH1D *fhRapDiffR[10][10];

private:
    AliJCard *fCard;
};

#endif /* defined(____AliJJetJtHistos__) */
