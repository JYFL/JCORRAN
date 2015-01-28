
#include "AliJHistosDiJetAnalysis.h"

AliJHistosDiJetAnalysis::AliJHistosDiJetAnalysis(AliJBaseCard * card):
        fReferenceBin(),
        fJetType(),
        fJetSelectionBin(),
        fDiJetSelectionBin(),
        fJetPtBin(),

        fhDiJetDRtoRef(),
        fhDiJetDPttoRef(),
        fhDiJetPtAsymRef(),
        fhDiJetInvMToRef(),
        fhDiJetPtPairToRef(),
        fhDiJetPt(),
        fhDiJetLeadingParticlePt(),
        fhDiJetNConst(),
        fhDiJetDPhi(),
        fhDiJetPtPair(),
        fhDiJetInvM(),
        fhDiJetDPt(),
        fhDiJetPtAssym(),

        fhJet2Pt(),
        fhJet2Phi(),
        fhJet2Eta(),
        fhJet2Charge(),
        fhJet2NConst(),
        fCard(card)
{;}

void AliJHistosDiJetAnalysis::CreateHistos(){
    fhRecoDiJetM       = new TH1D("hRecoDiJetM", "Invariant Mass", 1001*5,-1, 1000 );
    int nPtTriggBin = 7;
    double pi0 = -TMath::Pi();
    double pi1 = TMath::Pi();
    int npi = 1000;
    int neta = 100;
    double eta0 = -2;
    double eta1 = 2;
    int nptpair = 1000;
    double ptpair0=0;
    double ptpair1=100;

    TVector *jetPtBins = fCard->GetVector("Jet:PtBins");
    int NJetPtBin = jetPtBins->GetNrows()-1;
    TVector *PtPairBins = fCard->GetVector("Jet:PtPairBins");
    //double ptPairBin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40, 50, 100, 150 };
    int NPtPairBin = PtPairBins->GetNrows()-1;


    int nBINS=150;
    double logBinsX[nBINS+1], limL=0.1, LimH=100;
    double logBW = (log(LimH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);


    int nBINS2=300;
    double logBinsX2[nBINS2+2], limL2=0.1, LimH2=1000;
    double logBW2 = (log(LimH2)-log(limL2))/nBINS2;
    for(int ij=0;ij<=nBINS2;ij++) logBinsX2[ij+1]=limL2*exp(ij*logBW2);
    logBinsX2[0]=0;
    TDirectory * owd = gDirectory;
    TDirectory * nwd = NULL;
    TString hname;

    fHMG = new AliJHistManager( "HistManager");
    fReferenceBin   .Set("Reference", "R", "Ref%.0f", AliJBin::kSingle  ).SetBin("0");
    fJetType        .Set("JetType", "T", "Type%.0f", AliJBin::kSingle ).SetBin(6);
    fJetSelectionBin   .Set("JetSelection", "J", "JetSelection%0.f", AliJBin::kSingle  ).SetBin("0 1");
    fDiJetSelectionBin   .Set("DiJetSelection", "D", "DiJetSelection%0.f", AliJBin::kSingle  ).SetBin(4);
    fJetPtBin       .Set("JetPtBin", "P", "%.1f<p_{Tjet}<%.1f" ).SetBin( jetPtBins );


    fhDiJetDRtoRef 
        << TH1D("hDiJetDRtoRef","",100,-10,10)
        <<fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetDPttoRef 
        << TH1D("hDiJetDPttoRef","", nBINS2, logBinsX2)
        <<fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetPtAsymRef 
        << TH1D("hDiJetPtAsymRef","",200,-1,1)
        <<fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetInvMToRef 
        << TH2D("hDiJetInvMToRef","",nBINS2, logBinsX2, nBINS2, logBinsX2)
        <<fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetPtPairToRef 
        << TH2D("hDiJetPtPairToRef","", nBINS2, logBinsX2, nBINS2, logBinsX2)
        <<fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetPt 
        << TH1D("hDiJetPt","", nBINS2, logBinsX2)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetLeadingParticlePt 
        << TH1D("hDiJetLeadingParAticlePt","", nBINS2, logBinsX2)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetNConst 
        << TH1D("hDiJetNConst","",100,0,100)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetDPhi
        << TH1D("hDiJetDPhi","",100, -TMath::Pi(), TMath::Pi())
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetPtPair 
        << TH1D("hDiJetPtPair","", nBINS2, logBinsX2)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetInvM 
        << TH1D("hDiJetInvM","", nBINS2, logBinsX2)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetDPt 
        << TH1D("hDiJetDPt","", nBINS2, logBinsX2)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";
    fhDiJetPtAssym
        << TH1D("hDiJetPtAssym","",200,-1,1)
        << 3 << fReferenceBin<<fJetType<< fJetSelectionBin <<fDiJetSelectionBin << fJetPtBin << "END";

    fhJet2Pt
        << TH1D("hJet2Pt","", nBINS2, logBinsX2)
        << fJetType<< fJetSelectionBin<<"END";
    fhJet2Phi
        << TH1D("hJet2Phi","", 100, -TMath::Pi(), TMath::Pi())
        << fJetType<< fJetSelectionBin<<"END";
    fhJet2Eta
        << TH1D("hJet2Eta","", 100, -6, 6)
        << fJetType<< fJetSelectionBin<<"END";
    fhJet2Charge
        << TH1D("hJet2Charge","", 2, 0, 2)
        << fJetType<< fJetSelectionBin<<"END";
    fhJet2NConst
        << TH1D("hJet2NConst","", 100, 0, 100)
        << fJetType<< fJetSelectionBin<<"END";
};
