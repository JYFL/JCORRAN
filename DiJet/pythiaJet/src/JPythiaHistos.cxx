#include  "JPythiaHistos.h"

#include  "AliJCard.h"
#include  "AliJBaseTrack.h"
#include <TGraph.h>

const int nCJACEK2 =  76 ;
double pttJacek2[nCJACEK2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,1.0,
    1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 60, 70, 80, 90, 100, 150, 200, 300
};

//______________________________________________________________________________
JPythiaHistos::JPythiaHistos(AliJCard* cardP):
    fUseDirectory(true)
{
    // constructor

    fcard=cardP;
    fmaxEtaRange = fcard->Get("EtaRange");
    //TODO ftriggFiducCut =  fcard->Get("TriggerFiducialEtaCut"); //FK// Fiduc cut 
    fmaxTriggEtaRange =  fmaxEtaRange - ftriggFiducCut; //FK// Trigger range

    fhistoList = new TList();
    fTopDirectory = gDirectory;

}

//______________________________________________________________________________
JPythiaHistos::JPythiaHistos(const JPythiaHistos& obj){
    // copy constructor
}

//______________________________________________________________________________
JPythiaHistos& JPythiaHistos::operator=(const JPythiaHistos& obj){
    // copy constructor
    return *this;
}

void JPythiaHistos::CreatePythiaHistos() {
    int bins = 200;
    double lr=0.;
    double ur=1.5;
    //==================================
    for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
        float pTt1 = fcard->GetBinBorder(kTriggType, hit);
        float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
        for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
            float pTa1 = fcard->GetBinBorder(kAssocType, hia);
            float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);
            char htit[100], hname[100];
            sprintf(htit, "pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", pTt1, pTt2, pTa1,pTa2);

            sprintf(hname, "hZ%02d%02d", hit, hia);
            hZ[hit][hia] = new TH1D(hname, htit, bins, lr, ur);
            hZ[hit][hia]->Sumw2();
        }
    }

    fhNParton71 = new TH1D("hNParton71", "hNParton71", 100, -0.5, 100-0.5);
    fhistoList->Add(fhNParton71);
    fhNStringGroup = new TH1D("hNStringGroup", "hNStringGroup", 100, -0.5, 100-0.5);
    fhistoList->Add(fhNStringGroup);
    for( int i=0;i<2;i++){
        TString idx = Form("%02d", i);
        fhNStringGroupFrom[i] = new TH1D("hNStringGroupFrom"+idx, "hNStringGroupFrom"+idx, 100, -0.5, 100-0.5);
        fhistoList->Add(fhNStringGroupFrom[i]);
        fhNTracksInStringGroupFrom[i] = new TH1D("hNTracksInStringGroupFrom"+idx, "hNTracksInStringGroupFrom"+idx, 100, -0.5, 100-0.5);
        fhistoList->Add(fhNTracksInStringGroupFrom[i]);
        fhRapidity71From[i] = new TH1D("hRapidity71From"+idx, "hRapidity71From"+idx, 100, -8, 8 );
        fhistoList->Add(fhRapidity71From[i]);
        fhPt71From[i] = new TH1D("hPt71From"+idx, "hPt71From"+idx, 1000, 0, 100);
        fhistoList->Add(fhPt71From[i]);
    }

    int nTriggBin = fcard->GetNoOfBins( kTriggType );
    int nAssocBin = fcard->GetNoOfBins( kAssocType );

    int nBINS=300;
    double logBinsX[nBINS+1], limL=0.1, LimH=500;
    double logBW = (log(LimH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);
    
    fhIncPtParton23 = new TH1D("hIncPtParton23","Inclusive pT distribution of parton 23", nBINS, logBinsX); fhIncPtParton23->Sumw2();
    fhIncInvPtParton23 = new TH1D("hIncInvPtParton23","Inclusive invariant pT distribution of parton 23", nBINS, logBinsX); fhIncInvPtParton23->Sumw2();
    fhIncEtaParton23 = new TH1D("hIncEtaParton23", "Inclusive eta distribution of parton 23", 501, -10.0, 10.0); fhIncEtaParton23->Sumw2();
    

    for ( int it=0;it<nTriggBin;it++ ){
        for( int ia=0;ia<nAssocBin;ia++ ){
            float pTt1 = fcard->GetBinBorder(kTriggType, it);
            float pTt2 = fcard->GetBinBorder(kTriggType, it + 1);
            float pTa1 = fcard->GetBinBorder(kAssocType, ia);
            float pTa2 = fcard->GetBinBorder(kAssocType, ia + 1);
            TString titleA = Form("pTt: %3.1f-%3.1f pTa:%3.1f-%3.1f", pTt1, pTt2, pTa1, pTa2 ); 
            for( int itype=0;itype<2;itype++ ){
                TString idx = Form("%02d%02d%02d",itype,it,ia);
                hPyKT[itype][it][ia] = new TH1D("hPyKT"+idx, "Pythia Kt "+titleA, 100,0, 100 );
                hPyKT[itype][it][ia]->StatOverflows();
                hPyZt[itype][it][ia] = new TH1D("hPyZt"+idx, "Pythia Zt "+titleA, 100,0,2 );
                hPyZt[itype][it][ia]->StatOverflows();
                hPyZa[itype][it][ia] = new TH1D("hPyZa"+idx, "Pythia Za "+titleA, 100,0,2 );
                hPyZa[itype][it][ia]->StatOverflows();
                hPyXhHat[itype][it][ia] = new TH1D("hPyXhHat"+idx, "Pythia XhHat "+titleA, 100,0, 4 );
                hPyXhHat[itype][it][ia]->StatOverflows();

                hPyJetTriggPt[itype][it][ia]= new TH1D("hPyJetTriggPt"+idx, titleA, nBINS, logBinsX );
                hPyJetTriggPt[itype][it][ia]->StatOverflows();
                hPyJetAssocPt[itype][it][ia]= new TH1D("hPyJetAssocPt"+idx, titleA, nBINS, logBinsX );
                hPyJetAssocPt[itype][it][ia]->StatOverflows();
                hPyJetPout[itype][it][ia]= new TH1D("hPyJetPout"+idx, titleA, 300, 0, 100 );
                hPyJetPout[itype][it][ia]->StatOverflows();

            }
        }
    }
    /* 
       int nBINSJ=20;
       double logBinsJX[nBINSJ+10];
       limL=0.1; LimH=20;
       logBW = (log(LimH)-log(limL))/nBINSJ;
       for(int ij=0;ij<=nBINSJ;ij++) logBinsJX[ij]=limL*exp(ij*logBW);
       */
    //int nBINSJ = 4;
    //double logBinsJX[5] = { 20, 40, 60, 80, 110 };

    TVector *jetPtBins = fcard->GetVector("Jet:PtBins");
    int nBINSJ = jetPtBins->GetNrows()-1;
    Double_t logBinsJX[100];
    for( int i=0;i<=nBINSJ;i++ ) { logBinsJX[i] = (*jetPtBins)[i+1];
        cout<< i<<"\t"<<logBinsJX[i]<<endl; 
    }


    hJetPtSpectra = new TH1D("hJetPtSpectra", "Jet Pt Spectra", nCJACEK2, pttJacek2 );
}
