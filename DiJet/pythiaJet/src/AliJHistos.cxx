#include  "AliJHistos.h"

#include  "AliJCard.h"
#include  "AliJBaseTrack.h"
#include  "AliJPhoton.h"
#include  "AliJTrack.h"
#include  "AliJPiZero.h"

const int nCJACEK =  74 ;
double pttJacek[nCJACEK] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 
	1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9, 
	10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 60, 70, 80, 90, 100};
//const int nCJACEK =  60 ;
//double pttJacek[nCJACEK] = { 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.5, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.9, 0.95, 1.,
//    1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3., 3.2, 3.4, 3.6, 3.8, 4., 4.5, 5., 5.5, 6., 6.5, 7., 8., 9., 10,
//    11., 12., 13., 14., 15., 16., 18., 20, 22, 24, 26., 28., 30.};

const int nEta = 3;
double eta[nEta+1] = {-0.8,-0.2,0.3,0.8};// eta bins

const int nFIETE=201;
double janFiete[nFIETE]={0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5, 
	5.75, 6, 6.25, 6.5, 6.75, 7, 7.25, 7.5, 7.75, 8, 8.25, 8.5, 8.75, 9, 9.25, 9.5, 9.75, 10, 10.25, 10.5, 10.75, 11, 11.25, 11.5, 11.75, 
	12, 12.25, 12.5, 12.75, 13, 13.25, 13.5, 13.75, 14, 14.25, 14.5, 14.75, 15, 15.25, 15.5, 15.75, 16, 16.25, 16.5, 16.75, 17, 17.25, 17.5, 
	17.75, 18, 18.25, 18.5, 18.75, 19, 19.25, 19.5, 19.75, 20, 20.25, 20.5, 20.75, 21, 21.25, 21.5, 21.75, 22, 22.25, 22.5, 22.75, 23, 23.25, 
	23.5, 23.75, 24, 24.25, 24.5, 24.75, 25, 25.25, 25.5, 25.75, 26, 26.25, 26.5, 26.75, 27, 27.25, 27.5, 27.75, 28, 28.25, 28.5, 28.75, 29, 
	29.25, 29.5, 29.75, 30, 30.25, 30.5, 30.75, 31, 31.25, 31.5, 31.75, 32, 32.25, 32.5, 32.75, 33, 33.25, 33.5, 33.75, 34, 34.25, 34.5, 34.75,
	35, 35.25, 35.5, 35.75, 36, 36.25, 36.5, 36.75, 37, 37.25, 37.5, 37.75, 38, 38.25, 38.5, 38.75, 39, 39.25, 39.5, 39.75, 40, 40.25, 40.5, 
	40.75, 41, 41.25, 41.5, 41.75, 42, 42.25, 42.5, 42.75, 43, 43.25, 43.5, 43.75, 44, 44.25, 44.5, 44.75, 45, 45.25, 45.5, 45.75, 46, 46.25, 
	46.5, 46.75, 47, 47.25, 47.5, 47.75, 48, 48.25, 48.5, 48.75, 49, 49.25, 49.5, 49.75, 50};

//______________________________________________________________________________
AliJHistos::AliJHistos(AliJCard* cardP):
	fUseDirectory(true)
{   // constructor

	fcard=cardP;
	fmaxEtaRange = fcard->Get("EtaRange");
	ftriggFiducCut =  fcard->Get("TriggerFiducialEtaCut"); //FK// Fiduc cut 
	fmaxTriggEtaRange =  fmaxEtaRange - ftriggFiducCut; //FK// Trigger range

	fhistoList = new TList();
	fTopDirectory = gDirectory;

	//fhtyp[1] = "Real";
	//fhtyp[2] = "Mixed";
	//fhtyp[3] = "Rap. Gap";
}

//______________________________________________________________________________
AliJHistos::AliJHistos(const AliJHistos& obj){
	// copy constructor
}

//______________________________________________________________________________
AliJHistos& AliJHistos::operator=(const AliJHistos& obj){
	// copy constructor
	return *this;
}

//______________________________________________________________________________
void AliJHistos::CreateAzimuthCorrHistos()
{
	int    bins = 240; // 240 is divisible by 2,3,4,612*24=280    -1/3 and  0.5 and 5/3  are bin edges 
	//double lr = -1.0/3, ur= 5.0/3;
	lr = -9./20.;   //lower range for dphi histos
	ur= lr+2;       //upper range for dphi histos;
	double ptbw=10/100.0;  //see hPt histo below, let's make 10 bins per 1GeV/c

	nUE=20;
	double uEa = fcard->GetBinBorder(kAssocType, 0), UEb = fcard->GetBinBorder(kAssocType, fcard->GetNoOfBins(kAssocType));
	double logUEbw = (log(UEb)-log(uEa))/nUE;
	for(int ij=0;ij<=nUE;ij++) uEBinsx[ij]=uEa*exp(ij*logUEbw);

	nUEfar=10;
	logUEbw = (log(UEb)-log(uEa))/nUEfar;
	for(int ij=0;ij<=nUE;ij++) uEBinsxFar[ij]=uEa*exp(ij*logUEbw);


	if(fcard->GetNoOfBins(kCentrType) > kMaxNoCentrBin ){
		cout<<"ERROR: No of Centrality bins exceed max dim in AliJHistos.cxx "<<endl;
		exit(0);
	}

	//==================================
	//  trigger pt fhistos 
	//==================================
	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);

		for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++)
		{
			float pTt1 = fcard->GetBinBorder(kTriggType, hit);
			float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
			sprintf(fhtit, "C: %2.0f-%2.0f%% pTt: %3.1f-%3.1f", b1, b2, pTt1, pTt2);

			for(int ityp=0; ityp<2; ityp++){
				sprintf(fhname, "hTriggPtBinIsolTrigg%02d%02d%02d", ityp, hic, hit); //FK// Trigger isolated hadron
				fhTriggPtBinIsolTrigg[ityp][hic][hit] = new TH1D(fhname, fhtit,(int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2);//FK//
				fhTriggPtBinIsolTrigg[ityp][hic][hit]->Sumw2();//FK//
				fhistoList->Add(fhTriggPtBinIsolTrigg[ityp][hic][hit]);

			}

			sprintf(fhname, "hTriggMult%02d%02d", hic, hit);
			fhTriggMult[hic][hit] = new TH1D(fhname, fhtit, 100, -0.5, 99.5);
			fhTriggMult[hic][hit]->Sumw2();
			fhistoList->Add(fhTriggMult[hic][hit]);

			sprintf(fhname, "hIphiTrigg%02d%02d", hic,  hit);
			fhIphiTrigg[hic][hit] = new TH1D(fhname, fhtit, bins, -kJPi-0.1, kJPi+0.1);
			fhIphiTrigg[hic][hit]->Sumw2();//FK
			fhistoList->Add(fhIphiTrigg[hic][hit]);

			sprintf(fhname, "hIetaTrigg%02d%02d", hic,  hit);
			fhIetaTrigg[hic][hit] = new TH1D(fhname, fhtit, 80, -fmaxEtaRange, fmaxEtaRange);// inclusive eta
			fhIetaTrigg[hic][hit]->Sumw2();//FK
			fhistoList->Add(fhIetaTrigg[hic][hit]);

		}
	}

	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
		for (int hiz = 0; hiz < fcard->GetNoOfBins(kZVertType); hiz++) {
			float z1 = fcard->GetBinBorder(kZVertType, hiz);
			float z2 = fcard->GetBinBorder(kZVertType, hiz + 1);
			for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
				float pTt1 = fcard->GetBinBorder(kTriggType, hit);
				float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
				sprintf(fhtit, "C: %2.0f-%2.0f%% Z:%1.2f-%2.1f pTt: %3.1f-%3.1f", b1, b2, z1, z2, pTt1, pTt2);

				sprintf(fhname, "hTriggPtBin%02d%02d%02d", hic, hiz, hit);
				fhTriggPtBin[hic][hiz][hit] = new TH1D(fhname, fhtit,(int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2);
				fhTriggPtBin[hic][hiz][hit]->Sumw2();
				fhistoList->Add(fhTriggPtBin[hic][hiz][hit]);

			}
		}
	}

	//=====================================
	//  associated pt fhistos with etaGaps
	//=====================================
	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
		for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
			float pTt1 = fcard->GetBinBorder(kTriggType, hit);
			float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
			for(int iEtaGap=0; iEtaGap < fcard->GetNoOfBins(kEtaGapType); iEtaGap++){ 
				float etaGap1 = fcard->GetBinBorder(kEtaGapType, iEtaGap );
				float etaGap2 = fcard->GetBinBorder(kEtaGapType, iEtaGap + 1 );
				for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
					float pTa1 = fcard->GetBinBorder(kAssocType, hia);
					float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);

					for(int ityp=0; ityp<2; ityp++){
						MakeDirectory("hDphiAssoc");
						sprintf(fhtit, "C: %2.0f-%2.0f%% %1.1f<#Delta#eta<%1.1f  pTt: %2.1f-%2.1f pta: %2.1f-%2.1f",
								b1, b2, etaGap1, etaGap2, pTt1, pTt2, pTa1,pTa2);
						sprintf(fhname, "hDphiAssoc%02dC%02dE%02dT%02dA%02d", ityp, hic, iEtaGap, hit, hia);
						fhDphiAssoc[ityp][hic][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, bins, lr, ur);
						fhDphiAssoc[ityp][hic][iEtaGap][hit][hia]->Sumw2();
						fhistoList->Add(fhDphiAssoc[ityp][hic][iEtaGap][hit][hia]);
						JumpToDefalutDirectory();

						for (int hiz = 0; hiz < fcard->GetNoOfBins(kZVertType); hiz++) {
							float z1 = fcard->GetBinBorder(kZVertType, hiz);
							float z2 = fcard->GetBinBorder(kZVertType, hiz + 1);
							MakeDirectory("hDEtaNear");
							sprintf(fhtit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f %1.1f<#Delta#phi<%1.1f  pTt: %2.1f-%2.1f pta: %2.1f-%2.1f",
									b1, b2, z1, z2, etaGap1, etaGap2, pTt1, pTt2, pTa1,pTa2);
							sprintf(fhname, "hDEtaNear%02d%02d%02d%02d%02d%02d", ityp, hic, hiz, iEtaGap, hit, hia);
							//hDEtaNear[ityp][hic][hiz][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, 320, -2*fmaxEtaRange, 2*fmaxEtaRange);
							//hDEtaNear[ityp][hic][hiz][iEtaGap][hit][hia]->Sumw2();
							//fhistoList->Add(hDEtaNear[ityp][hic][hiz][iEtaGap][hit][hia]);
							if( ityp == 0 ){
								fhDEtaNear[hic][hiz][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, 320, -2*fmaxEtaRange, 2*fmaxEtaRange);
								fhDEtaNear[hic][hiz][iEtaGap][hit][hia]->Sumw2();
								fhistoList->Add(fhDEtaNear[hic][hiz][iEtaGap][hit][hia]);
							}
							else{
								fhDEtaNearM[hic][hiz][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, 320, -2*fmaxEtaRange, 2*fmaxEtaRange);
								fhDEtaNearM[hic][hiz][iEtaGap][hit][hia]->Sumw2();
								fhistoList->Add(fhDEtaNearM[hic][hiz][iEtaGap][hit][hia]);
							}
							JumpToDefalutDirectory();
						}
					}

				}
			}
		}
	}

	//=====================================
	//  associated pt fhistos with etaGaps XE bins
	//=====================================
	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
		for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
			float pTt1 = fcard->GetBinBorder(kTriggType, hit);
			float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
			for(int iEtaGap=0; iEtaGap < fcard->GetNoOfBins(kEtaGapType); iEtaGap++){ 
				float etaGap1 = fcard->GetBinBorder(kEtaGapType, iEtaGap );
				float etaGap2 = fcard->GetBinBorder(kEtaGapType, iEtaGap + 1 );
				// xe bins
				for (int hixe = 0; hixe < fcard->GetNoOfBins(kXeType); hixe++){
					float xe1 = fcard->GetBinBorder(kXeType, hixe);
					float xe2 = fcard->GetBinBorder(kXeType, hixe + 1);
					for(int ityp=0; ityp<2; ityp++){
						MakeDirectory("hDphiAssocXEbin");
						sprintf(fhtit, "C: %2.0f-%2.0f%% %1.1f<#Delta#eta<%1.1f  pTt: %2.1f-%2.1f x_{E}: %2.1f-%2.1f",
								b1, b2, etaGap1, etaGap2, pTt1, pTt2, xe1, xe2);
						sprintf(fhname, "hDphiAssocXEbin%02dC%02dE%02dT%02dA%02d", ityp, hic, iEtaGap, hit, hixe);
						fhDphiAssocXEbin[ityp][hic][iEtaGap][hit][hixe] = new TH1D(fhname, fhtit, bins, lr, ur);
						fhDphiAssocXEbin[ityp][hic][iEtaGap][hit][hixe]->Sumw2();
						fhistoList->Add(fhDphiAssocXEbin[ityp][hic][iEtaGap][hit][hixe]);
						JumpToDefalutDirectory();

						for (int hiz = 0; hiz < fcard->GetNoOfBins(kZVertType); hiz++) {
							float z1 = fcard->GetBinBorder(kZVertType, hiz);
							float z2 = fcard->GetBinBorder(kZVertType, hiz + 1);
							MakeDirectory("hDEtaNearXEbin");
							sprintf(fhtit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f %1.1f<#Delta#phi<%1.1f  pTt: %2.1f-%2.1f x_{E}: %2.1f-%2.1f",
									b1, b2, z1, z2, etaGap1, etaGap2, pTt1, pTt2, xe1, xe2);
							sprintf(fhname, "hDEtaNearXEbin%02d%02d%02d%02d%02d%02d", ityp, hic, hiz, iEtaGap, hit, hixe);
							//hDEtaNear[ityp][hic][hiz][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, 320, -2*fmaxEtaRange, 2*fmaxEtaRange);
							//hDEtaNear[ityp][hic][hiz][iEtaGap][hit][hia]->Sumw2();
							//fhistoList->Add(hDEtaNear[ityp][hic][hiz][iEtaGap][hit][hia]);
							if( ityp == 0 ){
								fhDEtaNearXEbin[hic][hiz][iEtaGap][hit][hixe] = new TH1D(fhname, fhtit, 320, -2*fmaxEtaRange, 2*fmaxEtaRange);
								fhDEtaNearXEbin[hic][hiz][iEtaGap][hit][hixe]->Sumw2();
								fhistoList->Add(fhDEtaNearXEbin[hic][hiz][iEtaGap][hit][hixe]);
							}
							else{
								fhDEtaNearMXEbin[hic][hiz][iEtaGap][hit][hixe] = new TH1D(fhname, fhtit, 320, -2*fmaxEtaRange, 2*fmaxEtaRange);
								fhDEtaNearMXEbin[hic][hiz][iEtaGap][hit][hixe]->Sumw2();
								fhistoList->Add(fhDEtaNearMXEbin[hic][hiz][iEtaGap][hit][hixe]);
							}
							JumpToDefalutDirectory();
						}
					}

				}
			}
		}
	}

	//=======================================
	//  associated fpt fhistos without etaGaps
	//=======================================
	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
		for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
			float pTa1 = fcard->GetBinBorder(kAssocType, hia);
			float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);
			for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
				float pTt1 = fcard->GetBinBorder(kTriggType, hit);
				float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
				for(int ityp=0; ityp<2; ityp++){
					sprintf(fhtit, "C: %2.0f-%2.0f%% pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", b1, b2, pTt1, pTt2, pTa1,pTa2);
					MakeDirectory("hDphiAssocIsolTrigg");
					sprintf(fhname, "hDphiAssocIsolTrigg%02d%02d%02d%02d", ityp, hic, hit, hia); //FK// trigger isolated particle
					fhDphiAssocIsolTrigg[ityp][hic][hit][hia] = new TH1D(fhname, fhtit, bins, lr, ur);//FK//
					fhDphiAssocIsolTrigg[ityp][hic][hit][hia]->Sumw2(); 
					fhistoList->Add(fhDphiAssocIsolTrigg[ityp][hic][hit][hia]);
					JumpToDefalutDirectory();
				}
				sprintf(fhtit, "C: %2.0f-%2.0f%% pTt: %2.1f-%2.1f pta: %2.1f-%2.1f",
						b1, b2, pTt1, pTt2, pTa1,pTa2);

				MakeDirectory("hMeanPtAssoc");
				sprintf(fhname, "hMeanPtAssoc%02d%02d%02d", hic, hit, hia);
				fhMeanPtAssoc[hic][hit][hia] = new TProfile(fhname, fhtit,bins, lr, ur);
				fhistoList->Add(fhMeanPtAssoc[hic][hit][hia]);
				JumpToDefalutDirectory();

				MakeDirectory("hMeanZtAssoc");
				sprintf(fhname, "hMeanZtAssoc%02d%02d%02d", hic, hit, hia);
				fhMeanZtAssoc[hic][hit][hia] = new TProfile(fhname, fhtit,bins, lr, ur);
				fhistoList->Add(fhMeanZtAssoc[hic][hit][hia]);
				JumpToDefalutDirectory();

				MakeDirectory("hAssocPtBin");
				sprintf(fhname, "hAssocPtBin%02d%02d%02d", hic, hit, hia);
				fhAssocPtBin[hic][hit][hia]  = new TH1D(fhname, fhtit,(int)TMath::Ceil((pTa2-pTa1)/ptbw), pTa1, pTa2);
				fhAssocPtBin[hic][hit][hia]->Sumw2();
				fhistoList->Add(fhAssocPtBin[hic][hit][hia]);
				JumpToDefalutDirectory();

			}
			sprintf(fhtit, "C: %2.0f-%2.0f%% pta: %2.1f-%2.1f",
					b1, b2, pTa1,pTa2);

			sprintf(fhname, "hIphiAssoc%02d%02d", hic,  hia);
			fhIphiAssoc[hic][hia] = new TH1D(fhname, fhtit, bins, -kJPi-0.1, kJPi+0.1);
			fhIphiAssoc[hic][hia]->Sumw2();//FK
			fhistoList->Add(fhIphiAssoc[hic][hia]);

			sprintf(fhname, "hIetaAssoc%02d%02d", hic,  hia);
			fhIetaAssoc[hic][hia] = new TH1D(fhname, fhtit, 80, -fmaxEtaRange, fmaxEtaRange);
			fhIetaAssoc[hic][hia]->Sumw2();//FK
			fhistoList->Add(fhIetaAssoc[hic][hia]);
		}
	}

	float pTa1 = fcard->GetBinBorder(kAssocType, 0);
	float pTa2 = fcard->GetBinBorder(kAssocType, fcard->GetNoOfBins(kAssocType) );
	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
		for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
			float pTt1 = fcard->GetBinBorder(kTriggType, hit);
			float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
			for(int ityp=0; ityp<2; ityp++){
				sprintf(fhtit, "C: %2.0f-%2.0f%% pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", b1, b2, pTt1, pTt2, pTa1,pTa2);
				sprintf(fhname, "hDEtaFar%02d%02d%02d", ityp, hic, hit );
				MakeDirectory("hDEtaFar");
				fhDEtaFar[ityp][hic][hit] = new TH1D(fhname, fhtit, 120, -2*fmaxEtaRange, 2*fmaxEtaRange);
				JumpToDefalutDirectory();
				fhDEtaFar[ityp][hic][hit]->Sumw2();
				fhistoList->Add(fhDEtaFar[ityp][hic][hit]);
			}
		}
	}

	//==========================
	//UE fhistos
	//==========================
	for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
		float pTt1 = fcard->GetBinBorder(kTriggType, hit);
		float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);

		for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) { //FK//
			float b1 = fcard->GetBinBorder(kCentrType, hic);
			float b2 = fcard->GetBinBorder(kCentrType, hic + 1);

			for(int iEtaGap=0; iEtaGap < fcard->GetNoOfBins(kEtaGapType); iEtaGap++){ 
				float etaGapThr = fcard->GetBinBorder(kEtaGapType, iEtaGap );  //FK//
				sprintf(fhtit, "C: %2.0f-%2.0f%% #Delta#eta>%1.1f  pTt: %2.1f-%2.1f", b1, b2, etaGapThr, pTt1, pTt2);

				sprintf(fhname, "hPtAssocUE%02d%02d%02d",hic, iEtaGap, hit);
				MakeDirectory("hPtAssocUE");
				fhPtAssocUE[hic][iEtaGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
				JumpToDefalutDirectory();
				//fhPtAssocUE[hic][iEtaGap][hit] = new TH1D(fhname, fhtit,nUE, uEa, UEb);//FK//
				fhPtAssocUE[hic][iEtaGap][hit]->Sumw2();
				fhistoList->Add(fhPtAssocUE[hic][iEtaGap][hit]);


				sprintf(fhname, "hPtaEtaGapN%02d%02d%02d",hic, iEtaGap, hit);
				MakeDirectory("hPtaEtaGapN");
				fhPtaEtaGapN[hic][iEtaGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
				JumpToDefalutDirectory();
				fhPtaEtaGapN[hic][iEtaGap][hit]->Sumw2();
				fhistoList->Add(fhPtaEtaGapN[hic][iEtaGap][hit]);

				sprintf(fhname, "hPtaRGapN%02d%02d%02d",hic, iEtaGap, hit);
				MakeDirectory("hPtaRGapN");
				fhPtaRGapN[hic][iEtaGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
				JumpToDefalutDirectory();
				fhPtaRGapN[hic][iEtaGap][hit]->Sumw2();
				fhistoList->Add(fhPtaRGapN[hic][iEtaGap][hit]);
			}
		}

		sprintf(fhtit, "pTt: %3.1f-%3.1f", pTt1, pTt2);

		sprintf(fhname, "hPtAssocUEIsolTrigg%02d", hit); //FK// Trigger isolated hadron
		fhPtAssocUEIsolTrigg[hit] = new TH1D(fhname, fhtit,nUE, uEBinsx);//FK//
		//fhPtAssocUEIsolTrigg[hit] = new TH1D(fhname, fhtit,nUE, uEa, UEb);//FK//
		fhPtAssocUEIsolTrigg[hit]->Sumw2(); //FK//
		fhistoList->Add(fhPtAssocUEIsolTrigg[hit]);

		sprintf(fhname, "hPtAssocN%02d", hit);
		fhPtAssocN[hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
		//fhPtAssocN[hit] = new TH1D(fhname, fhtit, nUE, uEa, UEb);//FK//
		fhPtAssocN[hit]->Sumw2();
		fhistoList->Add(fhPtAssocN[hit]);

		sprintf(fhname, "hPtAssocF%02d", hit);
		fhPtAssocF[hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
		//fhPtAssocF[hit] = new TH1D(fhname, fhtit, nUE, uEa, UEb);//FK// 
		fhPtAssocF[hit]->Sumw2();
		fhistoList->Add(fhPtAssocF[hit]);
	}

	//=======================
	//pout fhistos
	//=======================
	/*
	   const int NPB=100;
	   double poutBinsx[101], pouta=0.05, poutb=exp(3);
	   double logbw = (log(poutb)-log(pouta))/NPB;
	   for(int ij=0;ij<=NPB;ij++) poutBinsx[ij]=pouta*exp(ij*logbw);
	   */

	for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
		float b1 = fcard->GetBinBorder(kCentrType, hic);
		float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
		for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
			float pTt1 = fcard->GetBinBorder(kTriggType, hit);
            float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
            sprintf(fhtit, "C: %2.0f-%2.0f%% pTt: %3.1f-%3.1f",
                    b1, b2, pTt1, pTt2 );
            sprintf(fhname, "hZKlong%02d%02d",  hic, hit);
            fhZKlong[hic][hit] = new TH1D( fhname, fhtit, 200, 0, 2 );
            for(int ityp=0; ityp<2; ityp++){
                for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
                    float pTa1 = fcard->GetBinBorder(kAssocType, hia);
                    float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);
                    sprintf(fhtit, "C: %2.0f-%2.0f%% pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", 
                            b1, b2, pTt1, pTt2, pTa1, pTa2);

                    sprintf(fhname, "hPoutF%02d%02d%02d%02d", ityp, hic, hit, hia);
                    //fhPoutF[ityp][hic][hit][hia] = new TH1D(fhname, fhtit, NPB, poutBinsx);
                    MakeDirectory("hPoutF");
                    fhPoutF[ityp][hic][hit][hia] = new TH1D(fhname, fhtit, 300, 0, 15);//FK//
                    JumpToDefalutDirectory();
                    fhPoutF[ityp][hic][hit][hia]->Sumw2();
                    fhistoList->Add(fhPoutF[ityp][hic][hit][hia]);

                }//hia
            }//ityp
            for (int hiklong = 0; hiklong < fcard->GetNoOfBins(kLongType); hiklong++){
                float klong1 = fcard->GetBinBorder(kLongType, hiklong);
                float klong2 = fcard->GetBinBorder(kLongType, hiklong + 1);
                for(int iEtaGap=0; iEtaGap < fcard->GetNoOfBins(kEtaGapType); iEtaGap++){ 
                    //let's not compute mixed frandom background - takes a lot of time.
                    float etaGapThr = fcard->GetBinBorder(kEtaGapType, iEtaGap );
                    sprintf(fhtit, "C %2.0f-%2.0f%% #Delta#eta>%1.1f  pTt: %2.1f-%2.1f k_{#parallel}: %2.1f-%2.1f",
                            b1, b2, etaGapThr, pTt1, pTt2, klong1, klong2);

                    sprintf(fhname, "hJTKlongBgC%02dE%02dT%02dA%02d", hic, iEtaGap, hit, hiklong);
                    MakeDirectory("hJTKlongBg");
                    fhJTKlongBg[hic][iEtaGap][hit][hiklong] = new TH1D(fhname, fhtit, 600, 0, 10);
                    JumpToDefalutDirectory();
                    fhJTKlongBg[hic][iEtaGap][hit][hiklong]->Sumw2();
                    fhistoList->Add(fhJTKlongBg[hic][iEtaGap][hit][hiklong]);

                    sprintf(fhname, "hJTKlongBgRC%02dR%02dT%02dA%02d", hic, iEtaGap, hit, hiklong);
                    fhJTKlongBgR[hic][iEtaGap][hit][hiklong] = new TH1D(fhname, fhtit, 600, 0, 10); 
                    fhJTKlongBgR[hic][iEtaGap][hit][hiklong]->Sumw2();
                    fhistoList->Add(fhJTKlongBgR[hic][iEtaGap][hit][hiklong]);
                } //eta

                sprintf(fhtit, "C: %2.0f-%2.0f%%  pTt: %2.1f-%2.1f k_{#parallel}: %2.1f-%2.1f",
                        b1, b2, pTt1, pTt2, klong1, klong2);
                for(int ityp=0; ityp<2; ityp++){
                    sprintf(fhname, "hJTKlong%02dC%02dT%02dA%02d", ityp, hic, hit, hiklong);
                    MakeDirectory("hJTKlong");
                    fhJTKlong[ityp][hic][hit][hiklong] = new TH1D(fhname, fhtit, 600, 0, 10);
                    JumpToDefalutDirectory();
                    fhJTKlong[ityp][hic][hit][hiklong]->Sumw2();
                    fhistoList->Add(fhJTKlong[ityp][hic][hit][hiklong]);
                }
            }//klong 
            // xe bins
            for (int hixe = 0; hixe < fcard->GetNoOfBins(kXeType); hixe++){
                float xe1 = fcard->GetBinBorder(kXeType, hixe);
                float xe2 = fcard->GetBinBorder(kXeType, hixe + 1);
                for(int iEtaGap=0; iEtaGap < fcard->GetNoOfBins(kEtaGapType); iEtaGap++){ 
                    //let's not compute mixed frandom background - takes a lot of time.
                    float etaGapThr = fcard->GetBinBorder(kEtaGapType, iEtaGap );
                    sprintf(fhtit, "C %2.0f-%2.0f%% #Delta#eta>%1.1f  pTt: %2.1f-%2.1f x_{E}: %2.1f-%2.1f",
                            b1, b2, etaGapThr, pTt1, pTt2, xe1, xe2);

                    sprintf(fhname, "hJTBgC%02dE%02dT%02dA%02d", hic, iEtaGap, hit, hixe);
                    MakeDirectory("hJTBg");
                    fhJTBg[hic][iEtaGap][hit][hixe] = new TH1D(fhname, fhtit, 600, 0, 10);
                    JumpToDefalutDirectory();
                    fhJTBg[hic][iEtaGap][hit][hixe]->Sumw2();
                    fhistoList->Add(fhJTBg[hic][iEtaGap][hit][hixe]);

                    sprintf(fhname, "hJTBgRC%02dR%02dT%02dA%02d", hic, iEtaGap, hit, hixe);
                    fhJTBgR[hic][iEtaGap][hit][hixe] = new TH1D(fhname, fhtit, 600, 0, 10); 
                    fhJTBgR[hic][iEtaGap][hit][hixe]->Sumw2();
                    fhistoList->Add(fhJTBgR[hic][iEtaGap][hit][hixe]);
                } //eta

                sprintf(fhtit, "C: %2.0f-%2.0f%%  pTt: %2.1f-%2.1f x_{E}: %2.1f-%2.1f",
                        b1, b2, pTt1, pTt2, xe1, xe2);
                for(int ityp=0; ityp<2; ityp++){
                    sprintf(fhname, "hJT%02dC%02dT%02dA%02d", ityp, hic, hit, hixe);
                    MakeDirectory("hJT");
                    fhJT[ityp][hic][hit][hixe] = new TH1D(fhname, fhtit, 600, 0, 10);
                    JumpToDefalutDirectory();
                    fhJT[ityp][hic][hit][hixe]->Sumw2();
                    fhistoList->Add(fhJT[ityp][hic][hit][hixe]);
                }
            }//xe bins
            // pta bins
            for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
                float pta1 = fcard->GetBinBorder(kAssocType, hia);
                float pta2 = fcard->GetBinBorder(kAssocType, hia + 1);
                for(int iEtaGap=0; iEtaGap < fcard->GetNoOfBins(kEtaGapType); iEtaGap++){ 
                    //let's not compute miptad frandom background - takes a lot of time.
                    float etaGapThr = fcard->GetBinBorder(kEtaGapType, iEtaGap );
                    sprintf(fhtit, "C %2.0f-%2.0f%% #Delta#eta>%1.1f  pTt: %2.1f-%2.1f pTa: %2.1f-%2.1f",
                            b1, b2, etaGapThr, pTt1, pTt2, pta1, pta2);

                    sprintf(fhname, "hJTPtaBgC%02dE%02dT%02dA%02d", hic, iEtaGap, hit, hia);
                    MakeDirectory("hJTPtaBg");
                    fhJTPtaBg[hic][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, 600, 0, 10);
                    JumpToDefalutDirectory();
                    fhJTPtaBg[hic][iEtaGap][hit][hia]->Sumw2();
                    fhistoList->Add(fhJTPtaBg[hic][iEtaGap][hit][hia]);

                    sprintf(fhname, "hJTPtaBgRC%02dR%02dT%02dA%02d", hic, iEtaGap, hit, hia);
                    fhJTPtaBgR[hic][iEtaGap][hit][hia] = new TH1D(fhname, fhtit, 600, 0, 10); 
                    fhJTPtaBgR[hic][iEtaGap][hit][hia]->Sumw2();
                    fhistoList->Add(fhJTPtaBgR[hic][iEtaGap][hit][hia]);
                } //eta

                sprintf(fhtit, "C: %2.0f-%2.0f%%  pTt: %2.1f-%2.1f x_{E}: %2.1f-%2.1f",
                        b1, b2, pTt1, pTt2, pta1, pta2);
                for(int ityp=0; ityp<2; ityp++){
                    sprintf(fhname, "hJTPta%02dC%02dT%02dA%02d", ityp, hic, hit, hia);
                    MakeDirectory("hJTPta");
                    fhJTPta[ityp][hic][hit][hia] = new TH1D(fhname, fhtit, 100, -2.4, 4);
                    JumpToDefalutDirectory();
                    fhJTPta[ityp][hic][hit][hia]->Sumw2();
                    fhistoList->Add(fhJTPta[ityp][hic][hit][hia]);
                }
            }//pta bins
        }//hit
    }//cent

}


void AliJHistos::CreateIAAMoons()
{
    //--- IAA signal ---
    for(int ityp=0; ityp<2; ityp++){
        for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
            float b1 = fcard->GetBinBorder(kCentrType, hic);
            float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
            for (int hiz = 0; hiz < fcard->GetNoOfBins(kZVertType); hiz++) {
                float z1 = fcard->GetBinBorder(kZVertType, hiz);
                float z2 = fcard->GetBinBorder(kZVertType, hiz + 1);
                for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
                    float pTt1 = fcard->GetBinBorder(kTriggType, hit);
                    float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
                    for(int iRGap=0; iRGap <= fcard->Get("EtaGapSignalBin"); iRGap++){ 
                        float rGap1 = fcard->GetBinBorder(kEtaGapType, iRGap );
                        float rGap2 = fcard->GetBinBorder(kEtaGapType, iRGap + 1 );
                        sprintf(fhtit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f pTt: %2.1f-%2.1f pta: %2.1f-%2.1f %2.1f<R<%2.1f",
                                b1, b2, z1, z2, pTt1, pTt2, fcard->GetBinBorder(kAssocType,0), fcard->GetBinBorder(kAssocType, fcard->GetNoOfBins(kAssocType)),
                                rGap1, rGap2);

                        sprintf(fhname, "hDRNearPt%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, hit);
                        fhDRNearPt[ityp][hic][hiz][iRGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                        fhDRNearPt[ityp][hic][hiz][iRGap][hit]->Sumw2();
                        fhistoList->Add(fhDRNearPt[ityp][hic][hiz][iRGap][hit]);

                        sprintf(fhname, "hDRFarPt%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, hit);
                        fhDRFarPt[ityp][hic][hiz][iRGap][hit] = new TH1D(fhname, fhtit, nUEfar, uEBinsxFar);
                        fhDRFarPt[ityp][hic][hiz][iRGap][hit]->Sumw2();
                        fhistoList->Add(fhDRFarPt[ityp][hic][hiz][iRGap][hit]);
                    }
                }
            }
        }
    }

    // --- Moons ---
    for(int ityp=0; ityp<2; ityp++){
        for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
            float b1 = fcard->GetBinBorder(kCentrType, hic);
            float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
            for (int hiz = 0; hiz < fcard->GetNoOfBins(kZVertType); hiz++) {
                float z1 = fcard->GetBinBorder(kZVertType, hiz);
                float z2 = fcard->GetBinBorder(kZVertType, hiz + 1);
                for(int iPhiGap=0; iPhiGap < fcard->GetNoOfBins(kEtaGapType); iPhiGap++){ 
                    float phiGap1 = fcard->GetBinBorder(kEtaGapType, iPhiGap );
                    float phiGap2 = fcard->GetBinBorder(kEtaGapType, iPhiGap + 1 );
                    for(int iRGap=iPhiGap; iRGap < fcard->GetNoOfBins(kEtaGapType); iRGap++){ //not from 0
                        float rGap1 = fcard->GetBinBorder(kEtaGapType, iRGap );
                        float rGap2 = fcard->GetBinBorder(kEtaGapType, iRGap + 1 );
                        for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
                            float pTt1 = fcard->GetBinBorder(kTriggType, hit);
                            float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
                            sprintf(fhtit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f pTt: %2.1f-%2.1f pta: %2.1f-%2.1f %2.1f<R<%2.1f %2.1f<#Delta#phi<%2.1f",
                                    b1, b2, z1, z2, pTt1, pTt2, fcard->GetBinBorder(kAssocType,0), fcard->GetBinBorder(kAssocType, fcard->GetNoOfBins(kAssocType)),
                                    rGap1, rGap2, phiGap1, phiGap2);

                            MakeDirectory("hDRNearPtMoon");
                            sprintf(fhname, "hDRNearPtMoon%02d%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, iPhiGap, hit);
                            //hDRNearPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                            //hDRNearPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                            //fhistoList->Add(hDRNearPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit]);
                            if( ityp == 0 ){
                                fhDRNearPtMoon[hic][hiz][iRGap][iPhiGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                                fhDRNearPtMoon[hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                                fhistoList->Add(fhDRNearPtMoon[hic][hiz][iRGap][iPhiGap][hit]);
                            }
                            else{
                                fhDRNearPtMoonM[hic][hiz][iRGap][iPhiGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                                fhDRNearPtMoonM[hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                                fhistoList->Add(fhDRNearPtMoonM[hic][hiz][iRGap][iPhiGap][hit]);
                            }
                            JumpToDefalutDirectory();

                            MakeDirectory("hDRFarPtMoon");
                            sprintf(fhname, "hDRFarPtMoon%02d%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, iPhiGap, hit);
                            //hDRFarPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                            //hDRFarPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                            //fhistoList->Add(hDRFarPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit]);
                            if( ityp == 0 ){
                                fhDRFarPtMoon[hic][hiz][iRGap][iPhiGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                                fhDRFarPtMoon[hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                                fhistoList->Add(fhDRFarPtMoon[hic][hiz][iRGap][iPhiGap][hit]);
                            }
                            else{
                                fhDRFarPtMoonM[hic][hiz][iRGap][iPhiGap][hit] = new TH1D(fhname, fhtit, nUE, uEBinsx);
                                fhDRFarPtMoonM[hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                                fhistoList->Add(fhDRFarPtMoonM[hic][hiz][iRGap][iPhiGap][hit]);
                            }
                            JumpToDefalutDirectory();
                        }
                    }
                }
            }
        }
    }

    //==========================
    // 2D fhistos 
    //==========================
    for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
        float b1 = fcard->GetBinBorder(kCentrType, hic);
        float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
        for (int hiz = 0; hiz < fcard->GetNoOfBins(kZVertType); hiz++) {
            float z1 = fcard->GetBinBorder(kZVertType, hiz);
            float z2 = fcard->GetBinBorder(kZVertType, hiz + 1);
            for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
                float pTa1 = fcard->GetBinBorder(kAssocType, hia);
                float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);
                for(int hit=0; hit < fcard->GetNoOfBins(kTriggType); hit++){
                    float pTt1 = fcard->GetBinBorder(kTriggType, hit);
                    float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
                    for(int ityp=0; ityp<2; ityp++){
                        sprintf(fhtit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", b1, b2, z1, z2, pTt1, pTt2, pTa1,pTa2);
                        MakeDirectory("hDphiAssoc2DIAA");
                        sprintf(fhname, "hDphiAssoc2DIAA%02d%02d%02d%02d%02d", ityp, hic, hiz, hit, hia);
                        fhDphiAssoc2DIAA[ityp][hic][hiz][hit][hia] = new TH2D(fhname, fhtit, 100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, lr, ur);
                        fhDphiAssoc2DIAA[ityp][hic][hiz][hit][hia]->Sumw2();
                        fhistoList->Add(fhDphiAssoc2DIAA[ityp][hic][hiz][hit][hia]);
                        JumpToDefalutDirectory();
                    }
                }
            }
        }
    }
    for(int iPhiGap=0; iPhiGap < fcard->GetNoOfBins(kEtaGapType); iPhiGap++){ 
        float phiGap1 = fcard->GetBinBorder(kEtaGapType, iPhiGap );
        float phiGap2 = fcard->GetBinBorder(kEtaGapType, iPhiGap + 1 );
        for(int iRGap=0; iRGap < fcard->GetNoOfBins(kEtaGapType); iRGap++){ 
            float rGap1 = fcard->GetBinBorder(kEtaGapType, iRGap );
            float rGap2 = fcard->GetBinBorder(kEtaGapType, iRGap + 1 );

            sprintf(fhtit, "%2.1f<R<%2.1f %2.1f<#phi<%2.1f", rGap1, rGap2, phiGap1, phiGap2);
            sprintf(fhname, "hDphiAssoc2D%02d%02d", iRGap, iPhiGap);
            fhDphiAssoc2D[iRGap][iPhiGap] = new TH2D(fhname, fhtit, 100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, lr, ur);
            fhistoList->Add(fhDphiAssoc2D[iRGap][iPhiGap]);
        }
    }

}




//void AliJHistos::CreateXEHistos(particleType assocPart)
void AliJHistos::CreateXEHistos(){
    //==================================
    //  xe slopes
    //==================================
    for (int hip = 0; hip < fcard->GetNoOfBins(kTriggType);hip++){
        //double xel=fcard->GetBinBorder(kAssocType,0)/fcard->GetBinBorder(kTriggType, hip+1);
        //double xeh=fcard->GetBinBorder(kAssocType,1)/fcard->GetBinBorder(kTriggType, hip);
        //xeh = xeh>2 ? 2 : xeh;
        double xel=0.0, xeh=1.2;
        int nbxE = int((xeh-xel)/0.04);

        sprintf(fhtit, "F %3.1f<pTrigg<%3.1f ", fcard->GetBinBorder(kTriggType, hip), fcard->GetBinBorder(kTriggType, hip+1)); 
        for(int ityp=0;ityp<2;ityp++){
            sprintf(fhname, "hxEF%02d%02d", ityp, hip); 
            fhxEF[ityp][hip] = new TH1D(fhname, fhtit, nbxE, xel, xeh); fhxEF[ityp][hip]->Sumw2(); 
            fhistoList->Add(fhxEF[ityp][hip]);
            sprintf(fhname, "hxEFIsolTrigg%02d%02d", ityp, hip); //FK//
            fhxEFIsolTrigg[ityp][hip] = new TH1D(fhname, fhtit, nbxE, xel, xeh); fhxEFIsolTrigg[ityp][hip]->Sumw2();//FK// 
            fhistoList->Add(fhxEFIsolTrigg[ityp][hip]);
        }
        sprintf(fhtit, "N %3.1f<pTrigg<%3.1f ", fcard->GetBinBorder(kTriggType, hip), fcard->GetBinBorder(kTriggType, hip+1)); 
        for(int ityp=0;ityp<2;ityp++){
            sprintf(fhname, "hxEN%02d%02d", ityp, hip); 
            fhxEN[ityp][hip] = new TH1D(fhname, fhtit, nbxE, xel, xeh); fhxEN[ityp][hip]->Sumw2(); 
            fhistoList->Add(fhxEN[ityp][hip]);
        }
        for(int ityp=0;ityp<3;ityp++){
            for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
                float pTa1 = fcard->GetBinBorder(kAssocType, hia);
                float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);
                MakeDirectory("hxEPtBin");
                sprintf(fhname, "hxEPtBin%02d%02d%02d",ityp, hip, hia);
                sprintf(fhtit, "type=%d %3.1f<pTt<%3.1f, %3.1f<pTa<%3.1f", ityp, fcard->GetBinBorder(kTriggType, hip), fcard->GetBinBorder(kTriggType, hip+1), pTa1, pTa2); 
                fhxEPtBin[ityp][hip][hia]  = new TH1D(fhname, fhtit, nbxE, xel, xeh);
                fhxEPtBin[ityp][hip][hia]->Sumw2();
                fhistoList->Add(fhxEPtBin[ityp][hip][hia]);
                JumpToDefalutDirectory();
            }
        }
    }
}


void AliJHistos::CreatePairPtCosThetaStar(){
    // pairs
    int    bins = 288; // 12*24    -1/3 and  0.5 and 5/3  are bin edges 
    double lr = -1./3, ur= 5./3;
    //=================
    //pairPT
    //=================
    for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
        float pTt1 = fcard->GetBinBorder(kTriggType, hit);
        float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
        for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
            float pTa1 = fcard->GetBinBorder(kAssocType, hia);
            float pTa2 = fcard->GetBinBorder(kAssocType, hia + 1);
            sprintf(fhtit, "pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", pTt1, pTt2, pTa1,pTa2);
            for(int ityp=0; ityp<2; ityp++){
                MakeDirectory("hPairPtDphi");
                sprintf(fhname, "hPairPtDphi%02d%02d%02d", ityp, hit, hia);
                fhPairPtDphi[ityp][hit][hia] = new TH1D(fhname, fhtit, bins, lr, ur);
                JumpToDefalutDirectory();
                fhPairPtDphi[ityp][hit][hia]->Sumw2();
                fhistoList->Add(fhPairPtDphi[ityp][hit][hia]);
                MakeDirectory("hPairPt");
                sprintf(fhname, "hPairPt%02d%02d%02d", ityp, hit, hia);
                fhPairPt[ityp][hit][hia] = new TH1D(fhname, fhtit, 210, 0, 70);
                JumpToDefalutDirectory();
                fhPairPt[ityp][hit][hia]->Sumw2();
                fhistoList->Add(fhPairPt[ityp][hit][hia]);
            }
        }
    }
    //==================================
    //  CosThetaStar fhistos 
    //==================================
    for(int ipairpt=0; ipairpt<fcard->IsLessThanUpperPairPtCut(-999); ipairpt++){
        for (int him = 0; him < fcard->GetNoOfBins(kMassType); him++){
            float bM1 = fcard->GetBinBorder(kMassType, him);
            float bM2 = fcard->GetBinBorder(kMassType, him + 1);
            for(int ityp=0; ityp<2; ityp++){
                //cout<<"ippt="<<ipairpt<<" "<<fcard->IsLessThanUpperPairPtCut(-ipairpt)<<endl;
                sprintf(fhname, "hCosThetaStar%02d%02d%02d", ityp, ipairpt, him);
                sprintf(fhtit, "Inv Mass: %3.0f -%3.0f pair_pT<%d", bM1, bM2, fcard->IsLessThanUpperPairPtCut(-ipairpt));
                MakeDirectory("hCosThetaStar");
                fhCosThetaStar[ityp][ipairpt][him] = new TH1D(fhname, fhtit, 100, 0, 1);
                fhCosThetaStar[ityp][ipairpt][him]->Sumw2();
                JumpToDefalutDirectory();
                fhistoList->Add(fhCosThetaStar[ityp][ipairpt][him]);
            }
            sprintf(fhname, "hCMSrap%02d%02d", ipairpt, him);
            sprintf(fhtit, "Mass: %3.0f -%3.0f pair_pT<%d", bM1, bM2, fcard->IsLessThanUpperPairPtCut(-ipairpt));
            fhCMSrap[ipairpt][him] = new TH2D(fhname, fhtit, 100, 0, 1, 50, -1, 1);
            fhistoList->Add(fhCMSrap[ipairpt][him]);
        }
        sprintf(fhname, "hInvMass%02d", ipairpt);
        sprintf(fhtit, "pair_pT<%d", fcard->IsLessThanUpperPairPtCut(-ipairpt));
        fhInvMass[ipairpt] = new TH1D(fhname, fhtit, 200, 0, 40);
        fhInvMass[ipairpt]->Sumw2();
        fhistoList->Add(fhInvMass[ipairpt]);
    }

    for (int him = 0; him < fcard->GetNoOfBins(kMassType); him++){
        float bM1 = fcard->GetBinBorder(kMassType, him);
        float bM2 = fcard->GetBinBorder(kMassType, him + 1);
        sprintf(fhname, "hPairPtMass%02d", him);
        sprintf(fhtit, "Mass %3.0f -%3.0f", bM1, bM2 );
        fhPairPtMass[him] = new TH1D(fhname, fhtit, 250, 0, 50);
        fhPairPtMass[him]->Sumw2();
        fhistoList->Add(fhPairPtMass[him]);
        sprintf(fhname, "hPairDPhi%02d", him);
        sprintf(fhtit, "Mass %3.0f -%3.0f", bM1, bM2 );
        fhPairDPhi[him] = new TH1D(fhname, fhtit,  bins, lr, ur);
        fhPairDPhi[him]->Sumw2();
        fhistoList->Add(fhPairDPhi[him]);
        sprintf(fhname, "hPairDpT%02d", him);
        sprintf(fhtit, "Mass %3.0f -%3.0f", bM1, bM2 );
        fhPairDpT[him] = new TH1D(fhname, fhtit, 150, 0-25./150./2., 25-25./150./2.);
        fhPairDpT[him]->Sumw2();
        fhistoList->Add(fhPairDpT[him]);
    }
    fpCMSrap = new TProfile("pCMSrap","no pair pT cut",100,-1,1);
}



//______________________________________________________________________________
void AliJHistos::CreatePtCorrHistos(){
    // pt corr histos
    int ptbins=30;
    double lpt=0,upt=8;
    for(int ityp=0; ityp<2; ityp++)
        for(int ich=0; ich<2; ich++)
            for(int hic=0;hic<fcard->GetNoOfBins(kCentrType);hic++){
                float b1 = fcard->GetBinBorder(kCentrType, hic);
                float b2 = fcard->GetBinBorder(kCentrType, hic+1);
                sprintf(fhname,"hPtNear%02d%02d%02d",ityp,ich,hic); sprintf(fhtit,"Near C: %2.0f-%2.0f ",b1,b2);
                fhPtNear[ityp][ich][hic] = new TH1D(fhname, fhtit, ptbins, lpt, upt);
                fhPtNear[ityp][ich][hic]->Sumw2();
                fhistoList->Add(fhPtNear[ityp][ich][hic]);
                sprintf(fhname,"hPtFar%02d%02d%02d",ityp,ich,hic); sprintf(fhtit,"Far C: %2.0f-%2.0f ",b1,b2);
                fhPtFar[ityp][ich][hic] = new TH1D(fhname, fhtit, ptbins, lpt, upt);
                fhPtFar[ityp][ich][hic]->Sumw2();
                fhistoList->Add(fhPtFar[ityp][ich][hic]);
            }
}

//______________________________________________________________________________

void AliJHistos::CreateRunByRunHistos(int runID, int runcounter){
    // This is PHENIX specific method

    if(fcard->GetNoOfBins(kCentrType) > kMaxNoCentrBin ){
        cout<<"ERROR: No of Centrality bins exceed max dim in THistos.cc "<<endl;
        exit(0);
    }

    // EmCal TOF:
    sprintf(fhname,"TofPbSc%02d",runcounter);
    sprintf(fhtit,"PbScRun%d",runID);
    fTofPbSc[runcounter] = new TH1D(fhname, fhtit, 100, -5., 10.);
    fTofPbSc[runcounter]->Sumw2();
    fhistoList->Add(fTofPbSc[runcounter]);
    sprintf(fhname,"TofPbGl%02d",runcounter);
    sprintf(fhtit,"PbGlRun%d",runID);
    fTofPbGl[runcounter] = new TH1D(fhname, fhtit, 100, -5., 10.);
    fTofPbGl[runcounter]->Sumw2();
    fhistoList->Add(fTofPbGl[runcounter]);

}

//______________________________________________________________________________
//void AliJHistos::ReadInclusiveHistos(TFile *inFile){
void AliJHistos::ReadInclusiveHistos(const char *inclusFileName){
    // read inclusive histos
    TFile *inclusFile = new TFile(inclusFileName, "READ");

    //inFile->ls();
    //cout<<((TH1D*) inclusFile->Get(Form("hIetaTrigg%02d%02d", 0, 0)))->GetEntries() <<endl; 
    //abort();

    for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
        for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
            fhIetaTriggFromFile[hic][hit] = (TH1D*) inclusFile->Get(Form("hIetaTrigg%02d%02d", hic, hit));//FK//mix2 
            fhistoList->Add(fhIetaTriggFromFile[hic][hit]);
            fhIphiTriggFromFile[hic][hit] = (TH1D*) inclusFile->Get(Form("hIphiTrigg%02d%02d", hic, hit));//FK//mix2 
            fhistoList->Add(fhIphiTriggFromFile[hic][hit]);
            cout<<"c=" << hic <<" tr="<< hit <<" entries="<< fhIetaTriggFromFile[hic][hit]->GetEntries() << endl; 
        }
    }

    for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
        for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
            fhIetaAssocFromFile[hic][hia] = (TH1D*) inclusFile->Get(Form("hIetaAssoc%02d%02d", hic, hia));//FK//mix2
            fhistoList->Add(fhIetaAssocFromFile[hic][hia]);
            fhIphiAssocFromFile[hic][hia] = (TH1D*) inclusFile->Get(Form("hIphiAssoc%02d%02d", hic, hia));//FK//mix2
            fhistoList->Add(fhIphiAssocFromFile[hic][hia]);
            cout<<"c=" << hic <<" as="<< hia <<" entries="<< fhIetaAssocFromFile[hic][hia]->GetEntries() <<endl; 
        }
    }

    int numCent = fcard->GetNoOfBins(kCentrType);
    int numPtt  = fcard->GetNoOfBins(kTriggType);
    int numPta  = fcard->GetNoOfBins(kAssocType);
    int numEtaGaps = fcard->GetNoOfBins(kEtaGapType);
    int numZvtx = fcard->GetNoOfBins(kZVertType);

    //------------ R e a d   mixed  D a t a ------------    
    const int zFirstBin = 0 ;
    const int etaGapFirstBin = 0 ;
    for (int hic = 0;hic < numCent; hic++) {
        for (int hit = 0; hit < numPtt;hit++){
            for (int hia = 0; hia < numPta; hia++){
                fhDEtaNearMixFromFile[hic][hit][hia]=  (TH1D*) inclusFile->Get(Form("hDEtaNear/hDEtaNear%02d%02d%02d%02d%02d%02d", 1, hic, zFirstBin, etaGapFirstBin, hit, hia));
                for (int iEtaGap=0; iEtaGap < numEtaGaps; iEtaGap++){//fdphi slices 
                    for (int hiz = 0; hiz < numZvtx; hiz++) {
                        if( iEtaGap==etaGapFirstBin && hiz==zFirstBin ) continue;
                        TH1D *hid = (TH1D*) inclusFile->Get(Form("hDEtaNear/hDEtaNear%02d%02d%02d%02d%02d%02d", 1, hic, hiz, iEtaGap, hit, hia));
                        fhDEtaNearMixFromFile[hic][hit][hia]->Add(hid);
                    }
                }
                //normalize to traingle
                double counts  = fhDEtaNearMixFromFile[hic][hit][hia]->Integral();
                double bw      = fhDEtaNearMixFromFile[hic][hit][hia]->GetBinWidth(1);
                int rebin = 4;
                if(counts<5000) rebin=8;
                if(counts<3000) rebin=10;
                if(counts<1000) rebin=16;
                fhDEtaNearMixFromFile[hic][hit][hia]->Rebin(rebin);
                if(counts>0)  fhDEtaNearMixFromFile[hic][hit][hia]->Scale(2.*fmaxEtaRange/counts/bw/rebin);//triangle  f(0)=1, f(1.6)=0
                //if(counts>0)  fhDEtaNearMixFromFile[hic][hit][hia]->Scale(2.*fmaxEtaRange/counts/bw);
                //cout<<"c=" << hic <<" as="<< hia <<" entries="<< fhDEtaNearMixFromFile[hic][hit][hia]->GetEntries() <<endl; 

            }   
        }   
    }   


    //for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
    //   for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
    //      for (int hia = 0; hia < fcard->GetNoOfBins(kAssocType); hia++){
    //         hDphiAssocMixSpectraFile[hic][hit][hia]= (TH1D*) inclusFile->Get(Form("xhDphiAssoc%02dC%02dE00T%02dA%02d",1, hic, hit, hia));//FK//mix2
    //      }
    //   }
    //}
}

//______________________________________________________________________________
void AliJHistos::CreateEventTrackHistos(){
    // Histograms ============================================================
    int nBINS=150;
    double logBinsX[nBINS+1], limL=0.1, LimH=100;
    double logBW = (log(LimH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);

    fhLPpt      = new TH1D("hLPpt","LP pt", nBINS, logBinsX );           fhLPpt->Sumw2(); 
    fhistoList->Add(fhLPpt);
    fhLPpairPt  = new TH1D("hLPpairPt","LP pair pt", nBINS, logBinsX );  fhLPpairPt->Sumw2(); 
    fhistoList->Add(fhLPpairPt);
    fhChargedEta= new TH1D("hChargedEta","All eta",100,-1.0,1.0);        fhChargedEta->Sumw2(); 
    fhistoList->Add(fhChargedEta);
    fhLPeta     = new TH1D("hLPeta","LP eta",100,-1.0,1.0);              fhLPeta->Sumw2(); 
    fhistoList->Add(fhLPeta);
    fhIsolatedLPpt      = new TH1D("hIsolatedLPpt","Isolated LP pt", nBINS, logBinsX ); fhIsolatedLPpt->Sumw2();
    fhistoList->Add(fhIsolatedLPpt);

    fhChargedPtFiete = new TH1D("hChargedPtFiete", "Jan Fiete bins", nFIETE-1, janFiete );
    fhChargedPtFiete->Sumw2();
    fhistoList->Add(fhChargedPtFiete);


    fhAssocMult = new TH1D("hAssocMult","Assoc  mlt",100,-0.5,99.5);
    fhistoList->Add(fhAssocMult);

    for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
        float b1 = fcard->GetBinBorder(kCentrType, hic);
        float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
        fhChargedMult[hic] = new TH1D(Form("hChargedMult%d",hic),Form("C: %2.0f-%2.0f%%",b1,b2), 300, 0., 3500.); 
        fhChargedMult[hic]->StatOverflows();
        fhistoList->Add(fhChargedMult[hic]);

        fhChargedMultCut[hic] = new TH1D(Form("hChargedMultCut%d",hic),Form("C: %2.0f-%2.0f%%",b1,b2), 300, 0., 3500.); 
        fhChargedMultCut[hic]->StatOverflows();
        fhistoList->Add(fhChargedMultCut[hic]);
        fhZVert[hic] = new TH1D(Form("hZVert%d",hic),Form("C: %2.0f-%2.0f%%",b1,b2), 100, -30., 30.); 
        fhistoList->Add(fhZVert[hic]);

        fhChargedPt[hic] = new TH1D(Form("hChargedPt%d",hic), Form("C: %2.0f-%2.0f%%",b1,b2), nBINS, logBinsX );
        fhChargedPt[hic]->Sumw2();
        fhistoList->Add(fhChargedPt[hic]);

        fhChargedPtNoCorr[hic] = new TH1D(Form("hChargedPtNoCorr%d",hic), Form("C: %2.0f-%2.0f%%",b1,b2), nBINS, logBinsX );
        fhChargedPtNoCorr[hic]->Sumw2();
        fhistoList->Add(fhChargedPtNoCorr[hic]);

        fhChargedPtJacek[hic] = new TH1D(Form("hChargedPtJacek%d",hic), Form("C: %2.0f-%2.0f%%",b1,b2), nCJACEK-1, pttJacek );
        fhChargedPtJacek[hic]->Sumw2();
        fhistoList->Add(fhChargedPtJacek[hic]);
        for(int ieta=0;ieta<3;ieta++) {
            fhChargedPtJacekEta[hic][ieta] = new TH1D(Form("hChargedPtJacekC%02dE%02d",hic,ieta), Form("C: %2.0f-%2.0f%% %.1f<#eta<%.1f",b1,b2,eta[ieta],eta[ieta+1]), nCJACEK-1, pttJacek );
            fhChargedPtJacekEta[hic][ieta]->Sumw2();
            fhistoList->Add(fhChargedPtJacekEta[hic][ieta]);
        }

        fhVdelta2[hic] = new TH1D(Form("hVdelta2_%d",hic), Form("C: %2.0f-%2.0f%%",b1,b2), 100, -0.2, 5);
        fhVdelta2[hic]->Sumw2();
        fhVdelta2[hic]->StatOverflows();
        fhistoList->Add(fhVdelta2[hic]);

        fhVdelta3[hic] = new TH1D(Form("hVdelta3_%d",hic), Form("C: %2.0f-%2.0f%%",b1,b2), 100, -0.2, 5);
        fhVdelta3[hic]->Sumw2();
        fhVdelta3[hic]->StatOverflows();
        fhistoList->Add(fhVdelta3[hic]);

        fhVN[hic] = new TH1D(Form("hVN_%d",hic), Form("C: %2.0f-%2.0f%%",b1,b2), 100, -kJPi, kJPi);
        fhVN[hic]->Sumw2();
        fhistoList->Add(fhVN[hic]);

        fhTrackingEfficiency[hic] = new TProfile(Form("hTrackingEff_%02d",hic), Form("C: %2.0f-%2.0f%%",b1,b2),nCJACEK-1, pttJacek);
        fhistoList->Add(fhTrackingEfficiency[hic]);

        fhV0AMult[hic] = new TH1D(Form("hV0Mult_%02d",hic), Form("C : %2.0f-%2.0f%%", b1, b2), 2000,0,2000 );
        fhV0AMult[hic]->Sumw2();
        fhistoList->Add(fhV0AMult[hic]);
    }
    fpV2 = new TProfile("pV2", "v2 with cent", 50, 0, 90);
    fhistoList->Add(fpV2);
    fpV3 = new TProfile("pV3", "v3 with cent", 50, 0, 90);
    fhistoList->Add(fpV3);
    fpVdeltaNorm = new TProfile("pVdeltaNorm", "mult with cent", 50, 0, 90);
    fhistoList->Add(fpVdeltaNorm);

    fhChargedMultCent = new TH2D("hChargedMultCent ", "log(fmult) vs cent", 100, 0, 90, 100, 1, 10);
    fhistoList->Add(fhChargedMultCent);

    fhZVertRaw  = new TH1D("hZVertRaw","vertex 0", 120, -30., 30.);
    fhistoList->Add(fhZVertRaw);
    fhZVertRawErr  = new TH1D("hZVertRawErr","vertex 0 Err", 100, 0, 0.1);
    fhistoList->Add(fhZVertRawErr);
    fhCentr          = new TH1D("hCentr","centrality", 101, -0.5, 100.5);
    fhistoList->Add(fhCentr);
    fhiCentr         = new TH1D("hiCentr","centrality",10, -0.5, 9.5);
    fhistoList->Add(fhiCentr);
    fhEventPerRun = new TH1D("hEventPerRun","log(eve)/run",200, 0, 30.0);
    fhistoList->Add(fhEventPerRun);

    char name[100], fhtit[100];
    for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++){
        float pTt1 = fcard->GetBinBorder(kTriggType, hit);
        float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);
        sprintf(fhtit, "pTt: %3.1f-%3.1f", pTt1, pTt2);
        sprintf(name,"hBkgActivity%d",hit);
        fhBkgActivity[hit]=new TH1D(name, fhtit, 200, 0, 20);
        fhistoList->Add(fhBkgActivity[hit]);
    }

    //------------------ for Abs Norm FK --------------------------------
    double   binsVertexMult[] = {0,1,2,3,4,5,10000};
    int   nbinsVertexMult  = sizeof(binsVertexMult)/sizeof(double)-1;
    //double binsVertexZ[]    = {-10,-5,-2,0,2,5,10};
    double binsVertexZ[]    = {-10,-6,-3,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,3,6,10};
    int   nbinsVertexZ   = sizeof(binsVertexZ)/sizeof(double)-1;
    fhVertexZTriggVtx = new TH2D("hVertexZTriggVtx","Vertex counts", nbinsVertexMult, binsVertexMult, nbinsVertexZ, binsVertexZ);
    fhistoList->Add(fhVertexZTriggVtx);

    // event counter
    fhEvents = new TH1D("hEvents","events passing cuts", 8, -0.5, 7.5 );
    fhEvents->SetXTitle( "0 - all, 1 - SDD selected, 2 - has vertex, 3 - good vertex, 4 - trigger + MB, 5 - trigger, 6 - BX, 7 - centrality" );
    fhistoList->Add( fhEvents );
}

//______________________________________________________________________________
void AliJHistos::CreateXtHistos() {

    MakeDirectory("xT");
    // Esko
    TH1::SetDefaultSumw2(kTRUE);
    cout << "GetDefaultSumw2() = " << TH1::GetDefaultSumw2() << endl;

    // xT binning
    int NBINSxT=200;
    double LogBinsXt[NBINSxT+1];
    double xTLimL = 1e-5, xTLimH = 1.0, xTlogBW = (log(xTLimH)-log(xTLimL))/NBINSxT;
    for(int ij=0;ij<=NBINSxT;ij++) LogBinsXt[ij]=xTLimL*exp(ij*xTlogBW);


    // pT binning
    int NBINSpT=200;
    double LogBinsPt[NBINSpT+1];
    double pTLimL=0.1, pTLimH=200 , pTlogBW = (log(pTLimH)-log(pTLimL))/NBINSpT;
    for(int ij=0;ij<=NBINSpT;ij++) LogBinsPt[ij]=pTLimL*exp(ij*pTlogBW);

    sprintf(fhtit, "Mean activity inside cone");
    sprintf(fhname, "hActivity");
    hConeActivity = new TProfile(fhname, fhtit, NBINSpT, LogBinsPt );
    fhistoList->Add(hConeActivity);

    sprintf(fhtit, "Mean activity inside perpendicular cone");
    sprintf(fhname, "hPerpActivity");
    hPerpConeActivity = new TProfile(fhname, fhtit, NBINSpT, LogBinsPt );
    fhistoList->Add(hPerpConeActivity);

    sprintf(fhtit, "Mean pion activity inside cone isolated");
    sprintf(fhname, "hActivityIsolated");
    hConeActivityIsolated = new TProfile(fhname, fhtit, NBINSpT, LogBinsPt );
    fhistoList->Add(hConeActivityIsolated);

    sprintf(fhtit, "Mean activity inside perpendicular cone");
    sprintf(fhname, "hPerpActivityIsolated");
    hPerpConeActivityIsolated = new TProfile(fhname, fhtit, NBINSpT, LogBinsPt );
    fhistoList->Add(hPerpConeActivityIsolated);


    // name & title & bins for histos
    for(int iTriggerType = 0 ; iTriggerType < 3 ; iTriggerType++) {
        for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
            float b1 = fcard->GetBinBorder(kCentrType, hic);
            float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
            fhPtForXt[iTriggerType][hic] = new TH1D(Form("hPtForXt%02d%02d",iTriggerType,hic), Form("C: %2.0f-%2.0f%%",b1,b2), nCJACEK-1, pttJacek );
            fhPtForXt[iTriggerType][hic]->Sumw2();
            fhistoList->Add(fhPtForXt[iTriggerType][hic]);

            sprintf(fhtit, "Charged xT Type%02d C: %2.0f-%2.0f%%", iTriggerType, b1, b2);
            sprintf(fhname, "hXt%02d%02d", iTriggerType,hic);

            fhXt[iTriggerType][hic] = new TH1D(fhname, fhtit, NBINSxT, LogBinsXt );
            fhXt[iTriggerType][hic]->Sumw2();

            sprintf(fhtit, "Charged xT Type%02d C: %2.0f-%2.0f%%", iTriggerType,b1,b2);
            sprintf(fhname, "hXtWeighted%02d%02d", iTriggerType,hic);

            fhXtWeighted[iTriggerType][hic] = new TH1D(fhname, fhtit, NBINSxT, LogBinsXt );
            fhXtWeighted[iTriggerType][hic]->Sumw2();

            sprintf(fhname, "hXtWeightedHT%02d%02d", iTriggerType,hic);

            fhXtWeightedHT[iTriggerType][hic] = new TH1D(fhname, fhtit, NBINSxT, LogBinsXt );
            fhXtWeightedHT[iTriggerType][hic]->Sumw2();

            fhistoList->Add( fhXt[iTriggerType][hic] );
            fhistoList->Add( fhXtWeighted[iTriggerType][hic] );
            fhistoList->Add( fhXtWeightedHT[iTriggerType][hic] );
        }
    }
    JumpToDefalutDirectory();

}


void AliJHistos::CreateJetHistos(){
    // jet histos
    int nBINS=200;
    double logBinsX[nBINS+1], limL=0.1, LimH=200;
    double logBW = (log(LimH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);

    for (int hic = 0;hic < fcard->GetNoOfBins(kCentrType);hic++) {
        float b1 = fcard->GetBinBorder(kCentrType, hic);
        float b2 = fcard->GetBinBorder(kCentrType, hic + 1);
        sprintf(fhtit, "cent: %3.0f -%3.0f", b1, b2);
        sprintf(fhname, "hLPJetPt%02d", hic);
        fhLPJetPt[hic] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
        fhLPJetPt[hic]->Sumw2();
        fhistoList->Add(fhLPJetPt[hic]);

        sprintf(fhname, "hJetPt%02d", hic);
        fhJetPt[hic] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
        fhJetPt[hic]->Sumw2();
        fhistoList->Add(fhJetPt[hic]);

        sprintf(fhname, "hLeadingJetPt%02d", hic);
        fhLeadingJetPt[hic] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
        fhLeadingJetPt[hic]->Sumw2();
        fhistoList->Add(fhLeadingJetPt[hic]);

        sprintf(fhname, "hLeadingJetWLPPt%02d", hic);
        fhLeadingJetWLPPt[hic] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
        fhLeadingJetWLPPt[hic]->Sumw2();
        fhistoList->Add(fhLeadingJetWLPPt[hic]);
        for (int hit = 0; hit < fcard->GetNoOfBins(kTriggType);hit++)
        {
            float pTt1 = fcard->GetBinBorder(kTriggType, hit);
            float pTt2 = fcard->GetBinBorder(kTriggType, hit + 1);

            sprintf(fhtit, "cent: %3.0f -%3.0f pTt: %3.1f-%3.1f", b1, b2, pTt1, pTt2);
            sprintf(fhname, "hLPJetPTt%02d%02d", hic, hit);
            fhLPJetPTt[hic][hit] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
            fhLPJetPTt[hic][hit]->Sumw2();
            fhistoList->Add(fhLPJetPTt[hic][hit]);

            sprintf(fhname, "hLeadingJetWLPPTt%02d%02d", hic, hit);
            fhLeadingJetWLPPTt[hic][hit] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
            fhLeadingJetWLPPTt[hic][hit]->Sumw2();
            fhistoList->Add(fhLeadingJetWLPPTt[hic][hit]);

            sprintf(fhname, "hLPJetEtaPTt%02d%02d", hic, hit);
            fhLPJetEtaPTt[hic][hit] = new TH1D(fhname, fhtit, 100, -1, 1);
            fhLPJetEtaPTt[hic][hit]->Sumw2();
            fhistoList->Add(fhLPJetEtaPTt[hic][hit]);

            sprintf(fhname, "hLPJetRapidityPTt%02d%02d", hic, hit);
            fhLPJetRapidityPTt[hic][hit] = new TH1D(fhname, fhtit, 100, -1, 1);
            fhLPJetRapidityPTt[hic][hit]->Sumw2();
            fhistoList->Add(fhLPJetRapidityPTt[hic][hit]);

            sprintf(fhname, "hLPJetMassPTt%02d%02d", hic, hit);
            fhLPJetMassPTt[hic][hit] = new TH1D(fhname, fhtit,  nBINS, logBinsX);
            fhLPJetMassPTt[hic][hit]->Sumw2();
            fhistoList->Add(fhLPJetMassPTt[hic][hit]);

            sprintf(fhname, "hDphiLPJet%02d%02d", hic, hit);
            fhDphiLPJet[hic][hit] = new TH1D(fhname, fhtit, 160, -kJPi/2., kJPi/2.);
            fhDphiLPJet[hic][hit]->Sumw2();
            fhistoList->Add(fhDphiLPJet[hic][hit]);

            sprintf(fhname, "hDEtaLPJet%02d%02d", hic, hit);
            fhDEtaLPJet[hic][hit] = new TH1D(fhname, fhtit,  240, -1.2, 1.2);
            fhDEtaLPJet[hic][hit]->Sumw2();
            fhistoList->Add(fhDEtaLPJet[hic][hit]);
            // LP jet vs Leading Jet
            sprintf(fhname, "hDPtJet%02d%02d", hic, hit);
            fhDPtLPJet[hic][hit] = new TH1D(fhname, fhtit, 100, 0, 10.);
            fhDPtLPJet[hic][hit]->Sumw2();
            fhistoList->Add(fhDPtLPJet[hic][hit]);

        }
    }

    //==================================
    for (int hip = 0; hip < fcard->GetNoOfBins(kTriggType) ;hip++){
        float pTt1 = fcard->GetBinBorder(kTriggType, hip);
        float pTt2 = fcard->GetBinBorder(kTriggType, hip + 1);

        TString iName = Form("%02d", hip );
        TString iTitle = Form(" , jetPt > %3.0f", pTt1);
        TString iTitle2 = Form(" , %3.0f < p_{T,jet} < %3.0f", pTt1, pTt2);

        fhJetMultPt[hip] = new TH1D("hJetMultPt"+iName,"Jet Multiplicity "+iTitle,100,0, 100);
        fhJetMultPt[hip]->Sumw2();
        fhistoList->Add(fhJetMultPt[hip]);

        fhJetRho[hip] = new TH1D("hJetRho"+iName,"#rho"+iTitle2,200,0, 50);
        fhJetRho[hip]->Sumw2();
        fhistoList->Add(fhJetRho[hip]);

        fhJetRhoSigma[hip] = new TH1D("hJetRhoSigma"+iName,"#sigma"+iTitle2,200,0, 50);
        fhJetRhoSigma[hip]->Sumw2(); 
        fhistoList->Add(fhJetRhoSigma[hip]);

        // How many particles in a jet
        fhJetPartMult[hip] = new TH1D("hJetPartMult"+iName,"Number of consituents"+iTitle2,101, -0.5, 100.5);
        fhJetPartMult[hip]->Sumw2();
        fhistoList->Add(fhJetPartMult[hip]);
    }
    fhJetUEPt = new TH1D("hJetUEPt","UE particles p_{T} by Jet substract",nBINS,logBinsX);
    fhistoList->Add(fhJetUEPt);

    // pythia
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

}





















