//Simple histogram class for AliJJetJt Analysis
//author: B.K Kim,  D.J. Kim


#include "AliJJetJtHistos.h"


AliJJetJtHistos::AliJJetJtHistos(AliJCard *card): fhChargedPt(0)
    ,fhChargedPtWeight(0)
    ,fhChargedPtEsko(0)
    ,fhChargedPtWeightEsko(0)
    ,fhChargedPtJacek(0)
    ,fhChargedPtJacekNoWeight(0)
    ,fhChargedPtFiete(0)
    ,fhChargedPtFieteNoWeight(0)
    ,fhChargedPtEven(0)
    ,fhChargedPtEvenWeight(0)
    ,fhAllPt(0)
    ,fhPhotonPt(0)
    ,fhionPt(0)
    ,fhKaonPt(0)
    ,fhProtonPt(0)
    ,fhJetPt(0)
    ,fhJetPtWeight(0)
    ,fhJetBgPt(0)
    ,fhJetBgPtWeight(0)
    ,fhSubtractedJetPt(0)
    ,fhSubtractedJetPtWeight(0)
    ,fhRho(0)
    ,fhRadius(0)
    ,fhArea(0)
    ,fhZ(0)
    ,fhBgZ(0)
    ,fhJt(0)
    ,fhBgJt(0)
    ,fhQ(0)
    ,fCard(card)
    
{
    for (int i=0; i<10; i++){
        fhCounter[i] = NULL;
        fhKCounter[i] = NULL;
        fhRadiusBin[i]=NULL;
        fhConMaxPtBin[i]=NULL;
        fhNumOfConBin[i]=NULL;
        fhJetPtBin[i]=NULL;
        fhJetBgPtBin[i]=NULL;
        fhSubtractedJetPtBin[i]=NULL;
        fhZBin[i]=NULL;
        fhBgZBin[i]=NULL;
        fhJtBin[i]=NULL;
        fhBgJtBin[i]=NULL;
        fhJtWeightBin[i]=NULL;
        fhBgJtWeightBin[i]=NULL;
        fhLogJtWeightBin[i]=NULL;
        fhBgLogJtWeightBin[i]=NULL;
        fhPtAssocJET[i]=NULL;

        
        for (int j=0; j<10; j++){
            fhJtBinBin[i][j]=NULL;
            fhPtAssocUE[i][j]=NULL;
            fhJtWeightBinBin [i][j]=NULL;
            fhLogJtWeightBinBin [i][j]=NULL;
            fhJtWithPtCutWeightBinBin [i][j]=NULL;
            fhBgJtWithPtCutWeightBinBin [i][j]=NULL;
            fhLogJtWithPtCutWeightBinBin [i][j]=NULL;
            fhBgLogJtWithPtCutWeightBinBin [i][j]=NULL;
            fhLogJtWithPtCutWeightBinBinWithNewAxis [i][j]=NULL;
        }

    } 
} 

void AliJJetJtHistos::CreateJetJtHistos()
{
//bkend

	int NR = 6;

    //const bool fullJet = fCard->Get("FullJet") ;
    const int NJetTriggPtBins = fCard->GetN("JetTriggPtBorders") - 1 ;
    double JetTriggPtBorders[NJetTriggPtBins+1];
    for (int counter = 0; counter < fCard->GetN("JetTriggPtBorders"); counter++ ){
        JetTriggPtBorders[counter] = fCard->Get("JetTriggPtBorders",counter);
    }

    // get jet bin widths
    double JetTriggPtBinWidth[NJetTriggPtBins];
    for (int counter = 0; counter < NJetTriggPtBins; counter++){
        JetTriggPtBinWidth[counter] = fCard->Get("JetTriggPtBorders",counter+1) - fCard->Get("JetTriggPtBorders",counter);
    }

    //const int NJetConstPtLowLimits = 3;
    const int NJetConstPtLowLimits = fCard->GetN("JetConstPtLowLimits");
    double JetConstPtLowLimits[NJetConstPtLowLimits];
    for (int counter = 0; counter < fCard->GetN("JetConstPtLowLimits"); counter++ ){
        JetConstPtLowLimits[counter] = fCard->Get("JetConstPtLowLimits",counter);
    }

    const int NJetAssocPtBins = fCard->GetN("JetAssocPtBorders") - 1;
    double JetAssocPtBorders[NJetAssocPtBins+1];
    for (int counter = 0; counter < fCard->GetN("JetAssocPtBorders"); counter++ ){
        JetAssocPtBorders[counter] = fCard->Get("JetAssocPtBorders",counter);
    }


   const int NJetBins = NJetTriggPtBins; 
   const int NConLimits = NJetConstPtLowLimits;
   const int NConPtBins = NJetAssocPtBins;

	TH1D::AddDirectory(kFALSE);

	if( NJetBins > 10 || NConLimits > 10 || NConPtBins > 10 ){
		std::cout << "ERROR: too small arrays for histrograms! Fix the header file. " << std::endl;
		exit(0);
	}

	fhQ = new TH1D("hQ", "Parton's p_{T} spectrum;p_{T,q} [GeV/c];N", 100, 0, 30);
	fhQ->Sumw2();

	fhRadius = new TH1D("hRadius", "", 200, 0, 1);
	fhRadius->Sumw2();

	fhArea = new TH1D("hArea", "", 600, 0, TMath::Pi());
	fhArea->Sumw2();

	// pT -histograms
	const double minpT =  0.0e0;
	const double maxpT = 150.0e0;
	const double pTbinWidth = 0.2e0;
	const int NpTbins = int( (maxpT - minpT) / pTbinWidth ) + 1;

	int NBINS=150;
	double LogBinsX[NBINS+1], LimL=0.1, LimH=150;
	double logBW = (log(LimH)-log(LimL))/NBINS;
	for(int ij=0;ij<=NBINS;ij++) LogBinsX[ij]=LimL*exp(ij*logBW);

	fhChargedPt = new TH1D("hChargedPt","JCORRAN pT distribution;p_{T} [GeV/c];N", NBINS, LogBinsX);
	fhChargedPt->Sumw2();

	fhChargedPtWeight = new TH1D("hChargedPtWeight","JCORRAN pT distribution - with weight;p_{T} [GeV/c];N/{j_T}", NBINS, LogBinsX);
	fhChargedPtWeight->Sumw2();

	fhJetPt = new TH1D("hJetPt", "hJetPt;p_{T} [GeV/c];N", NBINS, LogBinsX);
	fhJetPt->Sumw2();

	fhJetPtWeight = new TH1D("hJetPtWeight", "hJetPtWeight;p_{T} [GeV/c];N/j_{T}", NBINS, LogBinsX);
	fhJetPtWeight->Sumw2();

	fhJetBgPt = new TH1D("hJetBgPt", "hJetBgPt;p_{T} [GeV/c];N", NBINS, LogBinsX);
	fhJetBgPt->Sumw2();

	fhJetBgPtWeight = new TH1D("hJetBgPtWeight", "hJetBgPtWeight;p_{T} [GeV/c];N/j_{T}", NBINS, LogBinsX);
	fhJetBgPtWeight->Sumw2();
	fhSubtractedJetPt = new TH1D("hSubtractedJetPt", "hSubtractedJetPt;p_{T} [GeV/c];N", NBINS, LogBinsX);
	fhSubtractedJetPt->Sumw2();

	fhSubtractedJetPtWeight = new TH1D("hSubtractedJetPtWeight", "hSubtractedJetPtWeight;p_{T} [GeV/c];N/{j_T}", NBINS, LogBinsX);
	fhSubtractedJetPtWeight->Sumw2();

	int NBINSEsko=150;
	double LogBinsXEsko[NBINS+1], LimLEsko=0.1, LimHEsko=150;
	double logBWEsko = (log(LimHEsko)-log(LimLEsko))/NBINSEsko;
	for(int ij=0;ij<=NBINSEsko;ij++) LogBinsXEsko[ij]=LimLEsko*exp(ij*logBWEsko);

	fhChargedPtEsko = new TH1D("hChargedPtEsko","JCORRAN pT distribution - Esko binning;p_{T} [GeV/c];N", NBINSEsko, LogBinsXEsko);
	fhChargedPtEsko->Sumw2();

	fhChargedPtWeightEsko = new TH1D("hChargedPtWeightEsko","JCORRAN pT distribution - with weight and Esko binning;p_{T} [GeV/c];N/{j_T}", NBINSEsko, LogBinsXEsko);
	fhChargedPtWeightEsko->Sumw2();

	const int NCJACEK =  74 ;
	double pttJacek[NCJACEK] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
		1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 60, 70, 80, 90, 100};

	fhChargedPtJacek = new TH1D("hChargedPtJacek","Jacek bins;p_{T} [GeV/c];N/{j_T}", NCJACEK-1, pttJacek);
	fhChargedPtJacek->Sumw2();

	fhChargedPtJacekNoWeight = new TH1D("hChargedPtJacekNoWeight","Jacek bins - NoWeight;p_{T} [GeV/c];N", NCJACEK-1, pttJacek);
	fhChargedPtJacekNoWeight->Sumw2();

	const int NFIETE=201;
	double JanFiete[NFIETE]={0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5,
		5.75, 6, 6.25, 6.5, 6.75, 7, 7.25, 7.5, 7.75, 8, 8.25, 8.5, 8.75, 9, 9.25, 9.5, 9.75, 10, 10.25, 10.5, 10.75, 11, 11.25, 11.5, 11.75,
		12, 12.25, 12.5, 12.75, 13, 13.25, 13.5, 13.75, 14, 14.25, 14.5, 14.75, 15, 15.25, 15.5, 15.75, 16, 16.25, 16.5, 16.75, 17, 17.25, 17.5, 
		17.75, 18, 18.25, 18.5, 18.75, 19, 19.25, 19.5, 19.75, 20, 20.25, 20.5, 20.75, 21, 21.25, 21.5, 21.75, 22, 22.25, 22.5, 22.75, 23, 23.25,
		23.5, 23.75, 24, 24.25, 24.5, 24.75, 25, 25.25, 25.5, 25.75, 26, 26.25, 26.5, 26.75, 27, 27.25, 27.5, 27.75, 28, 28.25, 28.5, 28.75, 29, 
		29.25, 29.5, 29.75, 30, 30.25, 30.5, 30.75, 31, 31.25, 31.5, 31.75, 32, 32.25, 32.5, 32.75, 33, 33.25, 33.5, 33.75, 34, 34.25, 34.5, 34.75,
		35, 35.25, 35.5, 35.75, 36, 36.25, 36.5, 36.75, 37, 37.25, 37.5, 37.75, 38, 38.25, 38.5, 38.75, 39, 39.25, 39.5, 39.75, 40, 40.25, 40.5, 
		40.75, 41, 41.25, 41.5, 41.75, 42, 42.25, 42.5, 42.75, 43, 43.25, 43.5, 43.75, 44, 44.25, 44.5, 44.75, 45, 45.25, 45.5, 45.75, 46, 46.25,
		46.5, 46.75, 47, 47.25, 47.5, 47.75, 48, 48.25, 48.5, 48.75, 49, 49.25, 49.5, 49.75, 50};
	fhChargedPtFiete = new TH1D("hChargedPtFiete", "Jan Fiete bins;p_{T} [GeV/c];N/{j_T}", NFIETE-1, JanFiete );
	fhChargedPtFiete->Sumw2();

	fhChargedPtFieteNoWeight = new TH1D("hChargedPtFieteNoWeight", "Jan Fiete bins - NoWeight;p_{T} [GeV/c];N", NFIETE-1, JanFiete );
	fhChargedPtFieteNoWeight->Sumw2();

	fhChargedPtEven  = new TH1D("hChargedPtEven","Even binning pT ;p_{T} [GeV/c];N",NpTbins, minpT, maxpT);
	fhChargedPtEven->Sumw2();  // include statistical errors using command "Sumw2"

	fhChargedPtEvenWeight = new TH1D("hChargedPtEvenWeight","Even binning pT - with weight;p_{T} [GeV/c];N/{j_T}",NpTbins, minpT, maxpT);
	fhChargedPtEvenWeight->Sumw2();  // include statistical errors using command "Sumw2"

	fhAllPt  = new TH1D("hAllPt","hAllPt;p_{T} [GeV/c];N",NpTbins, minpT, maxpT);
	fhAllPt->Sumw2();  // include statistical errors using command "Sumw2"

	fhPhotonPt  = new TH1D("hPhotonPt","hPhotonPt;p_{T} [GeV/c];N",NpTbins, minpT, maxpT);
	fhPhotonPt->Sumw2();  // include statistical errors using command "Sumw2"

	//hPionPt = new TH1D("hPionPt", "hPionPt;p_{T} [GeV/c];N", NpTbins, minpT, maxpT);
	//hPionPt->Sumw2();

	//hKaonPt = new TH1D("hKaonPt", "hKaonPt;p_{T} [GeV/c];N", NpTbins, minpT, maxpT);
	//hKaonPt->Sumw2();

	//hProtonPt = new TH1D("hProtonPt", "hProtonPt;p_{T} [GeV/c];N", NpTbins, minpT, maxpT);
	//hProtonPt->Sumw2();

	//rho-histogram
	fhRho = new TH1D("hRho", "hRho;#rho [GeV/c];N", 200, 0, 50);
	fhRho->Sumw2();

	// eta -histograms
	//const double etaRange = 0.8;
	//const double etaBinWidth = 0.025e0;
	//const int NetaBins = int( 2.2*etaRange / etaBinWidth ) + 1;

	//bkfhChargedEta = new TH1D("hChargedEta","hChargedEta;#eta;N",NetaBins, -1.1*etaRange, 1.1*etaRange);
	//bkbkfhChargedEta->Sumw2();

	//bkbkfhJetEta = new TH1D("hJetEta", "hJetEta;#eta;N", NetaBins, -1.1*etaRange, 1.1*etaRange);
	//bkbkfhJetEta->Sumw2();

	//bkbkfhPhotonEta = new TH1D("hPhotonEta","hPhotonEta;#eta;N",NetaBins, -1.1*etaRange, 1.1*etaRange);
	//bkbkfhPhotonEta->Sumw2();

	//bkbkfhProtonEta = new TH1D("hProtonEta","hProtonEta;#eta;N",NetaBins, -1.1*etaRange, 1.1*etaRange);
	//bkbkfhProtonEta->Sumw2();

	//bkbkfhPionEta = new TH1D("hPionEta","hPionEta;#eta;N",NetaBins, -1.1*etaRange, 1.1*etaRange);
	//bbkbkfhPionEta->Sumw2();

	//bkbkfhKaonEta = new TH1D("hKaonEta","hKaonEta;#eta;N",NetaBins, -1.1*etaRange, 1.1*etaRange);
	//bkbkfhKaonEta->Sumw2();

	// counter -histogram
	for (int i=0; i<NR; i++){
	fhCounter[i] = new TH1D(Form("hCounter_%d",i),Form("hCounter_%d",i),6,0,6);
	fhKCounter[i] = new TH1D(Form("hKCounter_%d",i), Form("hKCounter_%d",i), 17, 0, 17);
	}

	// z and jt histograms
	int NBINSZ=150;
	double LogBinsZ[NBINSZ+1], LimLZ=0.001, LimHZ=1.1;
	double logBWZ = (TMath::Log(LimHZ)-TMath::Log(LimLZ))/NBINSZ;
	for(int ij=0;ij<=NBINSZ;ij++) LogBinsZ[ij]=LimLZ*exp(ij*logBWZ);// TODO: change bins

	fhZ = new TH1D("hZ", "hZ;Z;N", NBINSZ, LogBinsZ);
	fhZ->Sumw2();
	fhBgZ = new TH1D("hBgZ", "hBgZ;Z;N", NBINSZ, LogBinsZ);
	fhBgZ->Sumw2();
	for (int i=0; i<NR; i++){
		fhZBG[i] = new TH1D(Form("hZ_%d",i), Form("hZBG;Z;N_%d",i), NBINSZ, LogBinsZ);
		fhZBG[i]->Sumw2();
	}

	int NBINSJt=150;
	double LogBinsJt[NBINSJt+1], LimLJt=0.01, LimHJt=10; 
	double logBWJt = (TMath::Log(LimHJt)-TMath::Log(LimLJt))/NBINSJt;
	for(int ij=0;ij<=NBINSJt;ij++) LogBinsJt[ij]=LimLJt*exp(ij*logBWJt);// TODO: change bins
	int NBINSJtW=150;
	double LimLJtW=TMath::Log(0.01), LimHJtW=TMath::Log(10); 

	fhJt = new TH1D("hJt", "hJt;j_{T} [GeV/c];N", NBINSJt, LogBinsJt);
	fhJt->Sumw2();
	fhBgJt = new TH1D("hBgJt", "hJtBg;j_{T} [GeV/c];N", NBINSJt, LogBinsJt);
	fhBgJt->Sumw2();
	for (int i=0; i<NR; i++){
	fhJtBG[i] = new TH1D(Form("hJtBG_%d",i), Form("hJtBG;j_{T} [GeV/c];N_%d",i), NBINSJt, LogBinsJt);
	fhJtBG[i]->Sumw2();
	}

	int fnUE=200;
	double uEa = 0.00001, uEb = 50;
	double fUEBinsx[fnUE+1];
	double logUEbw = (log(uEb)-log(uEa))/fnUE;
	for(int ij=0;ij<=fnUE;ij++) fUEBinsx[ij]=uEa*exp(ij*logUEbw);


	// JetPtBins
	char  fhname[40], fhtit[60];
	double ptbw = 10/100.0; // not final

	for( int hit = 0; hit < NJetBins; hit++ ){

		double pTt1 = JetTriggPtBorders[hit];
		double pTt2 = JetTriggPtBorders[hit+1];
		sprintf(fhtit, "p_{T,Jet}: %3.1f-%3.1f GeV", pTt1, pTt2);

		//R
		sprintf(fhname, "hRadiusBin%02d", hit);
		fhRadiusBin[hit] = new TH1D(fhname, fhtit, 100, 0, 1);
		fhRadiusBin[hit]->SetXTitle("R");
		fhRadiusBin[hit]->SetYTitle("N");
		fhRadiusBin[hit]->Sumw2();

		//max pt of constituents
		sprintf(fhname, "hConMaxPtBin%02d", hit);
		fhConMaxPtBin[hit] = new TH1D(fhname, fhtit, 2*pTt2, 0, pTt2);
		fhConMaxPtBin[hit]->SetXTitle("j_{T} [GeV/c]");
		fhConMaxPtBin[hit]->SetYTitle("N");
		fhConMaxPtBin[hit]->Sumw2();

		//number of constituents
		sprintf(fhname, "hNumOfConBin%02d", hit);
		fhNumOfConBin[hit] = new TH1D(fhname, fhtit, 50, 0, 50);
		fhNumOfConBin[hit]->SetXTitle("number of constituents");
		fhNumOfConBin[hit]->SetYTitle("N");
		fhNumOfConBin[hit]->Sumw2();

		//z
		sprintf(fhname, "hZBin%02d", hit);
		fhZBin[hit] = new TH1D(fhname, fhtit, NBINSZ, LogBinsZ);
		fhZBin[hit]->SetXTitle("Z");
		fhZBin[hit]->SetYTitle("N");
		fhZBin[hit]->Sumw2();

		sprintf(fhname, "hBgZBin%02d", hit);
		fhBgZBin[hit] = new TH1D(fhname, fhtit, NBINSZ, LogBinsZ);
		fhBgZBin[hit]->SetXTitle("Z");
		fhBgZBin[hit]->SetYTitle("N");
		fhBgZBin[hit]->Sumw2();
/*
		for (int i=0; i<NR; i++){
			sprintf(fhname, "hZBinBG%02d_%d", hit,i);
			fhZBinBG[hit][i] = new TH1D(fhname, fhtit, NBINSZ, LogBinsZ);
			fhZBinBG[hit][i]->SetXTitle("Z");
			fhZBinBG[hit][i]->SetYTitle("N");
			fhZBinBG[hit][i]->Sumw2();
		}
*/
		//jt
		sprintf(fhname, "hJtBin%02d", hit);
		fhJtBin[hit] = new TH1D(fhname, fhtit, NBINSJt, LogBinsJt);
		fhJtBin[hit]->SetXTitle("j_{T} [GeV/c]");
		fhJtBin[hit]->SetYTitle("N");
		fhJtBin[hit]->Sumw2();

		sprintf(fhname, "hBgJtBin%02d", hit);
		fhBgJtBin[hit] = new TH1D(fhname, fhtit, NBINSJt, LogBinsJt);
		fhBgJtBin[hit]->SetXTitle("j_{T} [GeV/c]");
		fhBgJtBin[hit]->SetYTitle("N");
		fhBgJtBin[hit]->Sumw2();
    /*
		for (int i=0; i<NR; i++){
			sprintf(fhname, "hJtBinBG%02d_%d", hit,i);
			fhJtBinBG[hit][i] = new TH1D(fhname, fhtit, NBINSJt, LogBinsJt);
			fhJtBinBG[hit][i]->SetXTitle("j_{T} [GeV/c]");
			fhJtBinBG[hit][i]->SetYTitle("N");
			fhJtBinBG[hit][i]->Sumw2();
		}
*/
		sprintf(fhname, "hJtWeightBin%02d", hit);
		fhJtWeightBin[hit] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
		fhJtWeightBin[hit]->SetXTitle("j_{T} [GeV/c]");
		fhJtWeightBin[hit]->SetYTitle("N/j_{T}");
		fhJtWeightBin[hit]->Sumw2();
		sprintf(fhname, "hBgJtWeightBin%02d", hit);
		fhBgJtWeightBin[hit] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
		fhBgJtWeightBin[hit]->SetXTitle("j_{T} [GeV/c]");
		fhBgJtWeightBin[hit]->SetYTitle("N/j_{T}");
		fhBgJtWeightBin[hit]->Sumw2();
/*
        for (int i=0; i<NR; i++){
            sprintf(fhname, "hJtWeightBinBG%02d_%d", hit,i);
            fhJtWeightBinBG[hit][i] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
            fhJtWeightBinBG[hit][i]->SetXTitle("j_{T} [GeV/c]");
            fhJtWeightBinBG[hit][i]->SetYTitle("N/j_{T}");
            fhJtWeightBinBG[hit][i]->Sumw2();
        }*/

		sprintf(fhname, "hLogJtWeightBin%02d", hit);
		fhLogJtWeightBin[hit] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
		fhLogJtWeightBin[hit]->SetXTitle("ln(j_{T} [GeV/c] )");
		fhLogJtWeightBin[hit]->SetYTitle("N/j_{T}");
		fhLogJtWeightBin[hit]->Sumw2();
		
		sprintf(fhname, "hBgLogJtWeightBin%02d", hit);
		fhBgLogJtWeightBin[hit] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
		fhBgLogJtWeightBin[hit]->SetXTitle("ln(j_{T} [GeV/c] )");
		fhBgLogJtWeightBin[hit]->SetYTitle("N/j_{T}");
		fhBgLogJtWeightBin[hit]->Sumw2();
	/*	
        for (int i=0; i<NR; i++){
            sprintf(fhname, "hLogJtWeightBinBG%02d_%d", hit,i);
            fhLogJtWeightBinBG[hit][i] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
            fhLogJtWeightBinBG[hit][i]->SetXTitle("ln(j_{T} [GeV/c] )");
            fhLogJtWeightBinBG[hit][i]->SetYTitle("N/j_{T}");
            fhLogJtWeightBinBG[hit][i]->Sumw2();
        }
*/
		//jet pt
		sprintf(fhname, "hJetPtBin%02d", hit);
		fhJetPtBin[hit] = new TH1D(fhname, fhtit,(int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2);
		fhJetPtBin[hit]->SetXTitle("p_{T} [GeV/c]");
		fhJetPtBin[hit]->SetYTitle("N");
		fhJetPtBin[hit]->Sumw2();

		sprintf(fhname, "hJetBgPtBin%02d", hit);
		fhJetBgPtBin[hit] = new TH1D(fhname, fhtit,(int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2);
		fhJetBgPtBin[hit]->SetXTitle("p_{T} [GeV/c]");
		fhJetBgPtBin[hit]->SetYTitle("N");
		fhJetBgPtBin[hit]->Sumw2();
		//subtracted jet pt
		sprintf(fhname, "hSubtractedJetPtBin%02d", hit);
		fhSubtractedJetPtBin[hit] = new TH1D(fhname, fhtit,(int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2);
		fhSubtractedJetPtBin[hit]->SetXTitle("j_{T} [GeV/c]");
		fhSubtractedJetPtBin[hit]->SetYTitle("N");
		fhSubtractedJetPtBin[hit]->Sumw2();

		sprintf(fhname, "hPtAssocJET%02d", hit);
		sprintf(fhtit, "Associated Pt of Jets%02d", hit);
		fhPtAssocJET[hit] = new TH1D(fhname, fhtit,fnUE,fUEBinsx);
		fhPtAssocJET[hit] -> Sumw2();
		//Difference of eta phi and Y between jets having different R size

    /*
		for (int i=0; i<NR; i++){
			sprintf(fhname, "hEtaDiffR%02d_%d", hit, i);
			fhEtaDiffR[hit][i] = new TH1D(fhname, fhtit,100, -0.8, 0.8);
			fhEtaDiffR[hit][i] ->Sumw2();


			sprintf(fhname, "hPhiDiffR%02d_%d", hit, i);
			fhPhiDiffR[hit][i] = new TH1D(fhname, fhtit,100, -1*TMath::Pi()/8, TMath::Pi()/8);
			fhPhiDiffR[hit][i] ->Sumw2();

			sprintf(fhname, "hRapDiffR%02d_%d", hit, i);
			fhRapDiffR[hit][i] = new TH1D(fhname, fhtit,100, -2, 2);
			fhRapDiffR[hit][i] ->Sumw2();


			sprintf(fhname, "hPtAssocUE%02d_%d", hit,i);
			sprintf(fhtit, "Associated Pt of underlying events%02d_%d", hit,i);
			fhPtAssocUE[hit][i] = new TH1D(fhname, fhtit,fnUE,fUEBinsx );
			fhPtAssocUE[hit][i] -> Sumw2();

		}
*/
		

		for (int hia = 0; hia < NConLimits; hia++) {
			sprintf(fhtit, "max{p_{T, constituent}} > %2.0f GeV , p_{T,Jet}: %3.1f-%3.1f GeV", JetConstPtLowLimits[hia], pTt1, pTt2);

			//jt
			sprintf(fhname, "hJtBinBin%02d%02d", hit, hia);
			fhJtBinBin[hit][hia] = new TH1D(fhname, fhtit, NBINSJt, LogBinsJt);
			fhJtBinBin[hit][hia]->SetXTitle("j_{T} [GeV/c]");
			fhJtBinBin[hit][hia]->SetYTitle("N");
			fhJtBinBin[hit][hia]->Sumw2();


        /*	
            for (int i=0; i<NR; i++){
                sprintf(fhname, "hJtBinBinBG%02d%02d_%d", hit, hia,i);
                fhJtBinBinBG[hit][hia][i] = new TH1D(fhname, fhtit, NBINSJt, LogBinsJt);
                fhJtBinBinBG[hit][hia][i]->SetXTitle("j_{T} [GeV/c]");
                fhJtBinBinBG[hit][hia][i]->SetYTitle("N");
                fhJtBinBinBG[hit][hia][i]->Sumw2();
            }*/

			sprintf(fhname, "hJtWeightBinBin%02d%02d", hit, hia);
			fhJtWeightBinBin[hit][hia] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
			fhJtWeightBinBin[hit][hia]->SetXTitle("j_{T} [GeV/c]");
			fhJtWeightBinBin[hit][hia]->SetYTitle("N/j_{T}");
			fhJtWeightBinBin[hit][hia]->Sumw2();
			
			/*
			for (int i=0; i<NR; i++){
			sprintf(fhname, "hJtWeightBinBinBG%02d%02d_%d", hit, hia,i);
			fhJtWeightBinBinBG[hit][hia][i] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
			fhJtWeightBinBinBG[hit][hia][i]->SetXTitle("j_{T} [GeV/c]");
			fhJtWeightBinBinBG[hit][hia][i]->SetYTitle("N/j_{T}");
			fhJtWeightBinBinBG[hit][hia][i]->Sumw2();
			}*/

			sprintf(fhname, "hLogJtWeightBinBin%02d%02d", hit, hia);
			fhLogJtWeightBinBin[hit][hia] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
			fhLogJtWeightBinBin[hit][hia]->SetXTitle("ln(j_{T} [GeV/c] )");
			fhLogJtWeightBinBin[hit][hia]->SetYTitle("N/j_{T}");
			fhLogJtWeightBinBin[hit][hia]->Sumw2();

/*
			
			for (int i=0; i<NR; i++){
			sprintf(fhname, "hLogJtWeightBinBinBG%02d%02d_%d", hit, hia,i);
			fhLogJtWeightBinBinBG[hit][hia][i] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
			fhLogJtWeightBinBinBG[hit][hia][i]->SetXTitle("ln(j_{T} [GeV/c] )");
			fhLogJtWeightBinBinBG[hit][hia][i]->SetYTitle("N/j_{T}");
			fhLogJtWeightBinBinBG[hit][hia][i]->Sumw2();
			}

			fAliJJetJtHistosJT[0][0][hit][hia] = new TH1D( Form( "fhJT%02dC%02dT%02dA%02d", 0, 0, hit, hia ), 
					Form( "j_{T} jet %d-%d assoc %.1f-", (Int_t)pTt1, (Int_t)pTt2, 
						JetConstPtLowLimits[hia]), 500, -2.3, 4 );
			fAliJJetJtHistosJT[0][0][hit][hia]->SetXTitle( "ln( j_{T} [GeV/c] )" );
			fAliJJetJtHistosJT[0][0][hit][hia]->SetYTitle( "N" );
			fAliJJetJtHistosJT[0][0][hit][hia]->Sumw2();
*/

/*
			for (int i=0; i<NR; i++){
			fAliJJetJtHistosJTBG[0][0][hit][hia][i] = new TH1D( Form( "fhJT%02dC%02dT%02dA%02dBG_%d", 0, 0, hit, hia,i ), 
					Form( "j_{T} jet %d-%d assoc %.1f-", (Int_t)pTt1, (Int_t)pTt2, 
						JetConstPtLowLimits[hia]), 500, -2.3, 4 );
			fAliJJetJtHistosJTBG[0][0][hit][hia][i]->SetXTitle( "ln( j_{T} [GeV/c] )" );
			fAliJJetJtHistosJTBG[0][0][hit][hia][i]->SetYTitle( "N" );
			fAliJJetJtHistosJTBG[0][0][hit][hia][i]->Sumw2();
			}*/
		}

		//bk
		for (int hiptbin = 0; hiptbin < NConPtBins; hiptbin++) {
			
			sprintf(fhtit, " %2.0f GeV  < p_{T, constituent} < %2.0f GeV , p_{T,Jet}: %3.1f-%3.1f GeV", JetAssocPtBorders[hiptbin],
					JetAssocPtBorders[hiptbin+1], pTt1, pTt2);
			sprintf(fhname, "hJtWithPtCutWeightBinBin%02d%02d", hit, hiptbin);
			fhJtWithPtCutWeightBinBin[hit][hiptbin] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
			fhJtWithPtCutWeightBinBin[hit][hiptbin]->SetXTitle("j_{T} [GeV/c]");
			fhJtWithPtCutWeightBinBin[hit][hiptbin]->SetYTitle("N/j_{T}");
			fhJtWithPtCutWeightBinBin[hit][hiptbin]->Sumw2();

			sprintf(fhname, "hBgJtWithPtCutWeightBinBin%02d%02d", hit, hiptbin);
			fhBgJtWithPtCutWeightBinBin[hit][hiptbin] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
			fhBgJtWithPtCutWeightBinBin[hit][hiptbin]->SetXTitle("j_{T} [GeV/c]");
			fhBgJtWithPtCutWeightBinBin[hit][hiptbin]->SetYTitle("N/j_{T}");
			fhBgJtWithPtCutWeightBinBin[hit][hiptbin]->Sumw2();
/*
            for (int i=0; i<NR; i++){
                sprintf(fhname, "hJtWithPtCutWeightBinBinBG%02d%02d_%d", hit, hiptbin,i);
                fhJtWithPtCutWeightBinBinBG[hit][hiptbin][i] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
                fhJtWithPtCutWeightBinBinBG[hit][hiptbin][i]->SetXTitle("j_{T} [GeV/c]");
                fhJtWithPtCutWeightBinBinBG[hit][hiptbin][i]->SetYTitle("N/j_{T}");
                fhJtWithPtCutWeightBinBinBG[hit][hiptbin][i]->Sumw2();
            }
*/
			sprintf(fhname, "hLogJtWithPtCutWeightBinBin%02d%02d", hit, hiptbin );
                        fhLogJtWithPtCutWeightBinBin[hit][hiptbin] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
                        fhLogJtWithPtCutWeightBinBin[hit][hiptbin]->SetXTitle("ln(j_{T} [GeV/c] )");
                        fhLogJtWithPtCutWeightBinBin[hit][hiptbin]->SetYTitle("N/j_{T}");
                        fhLogJtWithPtCutWeightBinBin[hit][hiptbin]->Sumw2();
			
			sprintf(fhname, "hBgLogJtWithPtCutWeightBinBin%02d%02d", hit, hiptbin );
                        fhBgLogJtWithPtCutWeightBinBin[hit][hiptbin] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
                        fhBgLogJtWithPtCutWeightBinBin[hit][hiptbin]->SetXTitle("ln(j_{T} [GeV/c] )");
                        fhBgLogJtWithPtCutWeightBinBin[hit][hiptbin]->SetYTitle("N/j_{T}");
                        fhBgLogJtWithPtCutWeightBinBin[hit][hiptbin]->Sumw2();
			sprintf(fhname, "hLogJtWithPtCutWeightBinBinWithNewAxis%02d%02d", hit, hiptbin );
                        fhLogJtWithPtCutWeightBinBinWithNewAxis[hit][hiptbin] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
                        fhLogJtWithPtCutWeightBinBinWithNewAxis[hit][hiptbin]->SetXTitle("ln(j_{T} [GeV/c] )");
                        fhLogJtWithPtCutWeightBinBinWithNewAxis[hit][hiptbin]->SetYTitle("N/j_{T}");
                        fhLogJtWithPtCutWeightBinBinWithNewAxis[hit][hiptbin]->Sumw2();

            /*

			sprintf(fhname, "hLogJtWithPtCutWeightBinBinBGWithNewAxis%02d%02d", hit, hiptbin );
                        fhLogJtWithPtCutWeightBinBinBGWithNewAxis[hit][hiptbin] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
                        fhLogJtWithPtCutWeightBinBinBGWithNewAxis[hit][hiptbin]->SetXTitle("ln(j_{T} [GeV/c] )");
                        fhLogJtWithPtCutWeightBinBinBGWithNewAxis[hit][hiptbin]->SetYTitle("N/j_{T}");
                        fhLogJtWithPtCutWeightBinBinBGWithNewAxis[hit][hiptbin]->Sumw2();
			for (int i=0; i<NR; i++){
                        sprintf(fhname, "hLogJtWithPtCutWeightBinBinBG%02d%02d_%d", hit, hiptbin,i);
                        fhLogJtWithPtCutWeightBinBinBG[hit][hiptbin][i] = new TH1D(fhname, fhtit, NBINSJtW, LimLJtW, LimHJtW);
                        fhLogJtWithPtCutWeightBinBinBG[hit][hiptbin][i]->SetXTitle("ln(j_{T} [GeV/c] )");
                        fhLogJtWithPtCutWeightBinBinBG[hit][hiptbin][i]->SetYTitle("N/j_{T}");
                        fhLogJtWithPtCutWeightBinBinBG[hit][hiptbin][i]->Sumw2();
			}*/
		
		}
		//bkend

	}



}

void AliJJetJtHistos::WriteHistograms(){



    //TDirectory * cwd = gDirectory;
    //nwd = gDirectory->mkdir("JJetJt");
    //if( nwd ) nwd->cd();
    // fill jetPt histos
    fhJetPt->Write();
    fhJetPtWeight->Write();
    fhJetBgPt->Write();
    fhJetBgPtWeight->Write();
    fhZ->Write();
    fhBgZ->Write();
    fhJt->Write();
    fhBgJt->Write();
    cout<<"Wrutubg= histograms"<<endl;
    const int NJetTriggPtBins = fCard->GetN("JetTriggPtBorders") - 1 ;
    const int NJetAssocPtBins = fCard->GetN("JetAssocPtBorders") - 1;
    const int NJetConstPtLowLimits = fCard->GetN("JetConstPtLowLimits"); 
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhJetPtBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhJetBgPtBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhZBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhBgZBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhJtBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhBgJtBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhJtWeightBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhBgJtWeightBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhLogJtWeightBin[iBin]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)    fhBgLogJtWeightBin[iBin]->Write();
    
    TDirectory *cwd = gDirectory;
    TDirectory *nwd =  gDirectory->mkdir("JtBinBin");
    nwd->cd();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
        for (int ipTa = 0; ipTa < NJetAssocPtBins; ipTa++) 
            fhJtWithPtCutWeightBinBin[iBin][ipTa]->Write();
 
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
        for (int ipTa = 0; ipTa < NJetAssocPtBins; ipTa++) 
            fhBgJtWithPtCutWeightBinBin[iBin][ipTa]->Write();

    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
        for (int ipTa = 0; ipTa < NJetAssocPtBins; ipTa++) 
            fhLogJtWithPtCutWeightBinBin[iBin][ipTa]->Write();
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
        for (int ipTa = 0; ipTa < NJetAssocPtBins; ipTa++) 
            fhBgLogJtWithPtCutWeightBinBin[iBin][ipTa]->Write();
    cwd->cd();
    
    
    nwd =  gDirectory->mkdir("JtLimitBinBin");
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
         for (int ilimit = 0; ilimit < NJetConstPtLowLimits; ilimit++) 
            fhJtBinBin[iBin][ilimit]->Write();
    
    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
         for (int ilimit = 0; ilimit < NJetConstPtLowLimits; ilimit++) 
            fhJtWeightBinBin[iBin][ilimit]->Write();

    for (int iBin = 0; iBin < NJetTriggPtBins; iBin++)
         for (int ilimit = 0; ilimit < NJetConstPtLowLimits; ilimit++) 
            fhLogJtWeightBinBin[iBin][ilimit]->Write();

    cwd->cd();




}
