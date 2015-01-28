/**************************************************************************
 * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// Comment describing what this class does needed!

#ifndef ALIJHISTOSDIJETANALYSIS_H
#define ALIJHISTOSDIJETANALYSIS_H
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include "AliJBaseCard.h"
#include "AliJConst.h"




class AliJHistosDiJetAnalysis{
    public:
        AliJHistosDiJetAnalysis(AliJBaseCard * card):fCard(card){;}

        AliJCard * fCard;       // card
        AliJHistManager * fHMG; // Histogram manager
        AliJBin fJetPtBin;
        AliJBin fJetPtPairBin;
        AliJBin fInvMBin;
        AliJBin fJetTypeBin;
        AliJBin fJetSelectionBin;
        AliJBin fDiJetSelectionBin;

        // Jets
        AliJTH1D fhJetPt;
        AliJTH1D fhJetE;
        //DiJets
        AliJTH1D fhDiJetPtPair;
        AliJTH1D fhDiJetInvM;
        AliJTH1D fhDiJetKtA;
        AliJTH1D fhDiJetDeltaR;
        AliJTH1D fhDiJetDeltaPhi;
        AliJTH1D fhDiJetPtAsymm;

        // PYTHIA
        AliJTH1D fPythiaJetSum;
        AliJTH1D fPythiaJetPtPair;
        AliJTH1D fhPtHat;

        void CreateHistos(){
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

            /*
               for(int ityp=0;ityp<30;ityp++) {
               TString iTStr = Form("%02d", ityp );
               for( int hip=0;hip<nPtTriggBin;hip++ ){
               TString iPtStr = Form("%02d", hip );
               TString iName = iTStr+iPtStr;
               TString iTitle= iTStr+iPtStr;
               fhJetDphi[ityp][hip] = new TH1D("fhJetDphi"+iName, "fhJetDphi"+iTitle,npi, pi0,pi1);
               fhJetDphi[ityp][hip]->Sumw2();
               fhJetDeta[ityp][hip] = new TH1D("fhJetDeta"+iName, "fhJetDeta"+iTitle,neta, eta0,eta1);
               fhJetDeta[ityp][hip]->Sumw2();

               fhJetAssymDiPt[ityp][hip] = new TH1D("hJetAssymDiPt"+iName,"Jet Assym between Leading, SubLeading jets "+iTitle,100,0, 1);
               fhJetAssymDiPt[ityp][hip]->Sumw2();


               fhJetDPt[ityp][hip] = new TH1D("hJetDPt"+iName,"#Delta p_{T}"+iTitle,1000,-50, 50);
               fhJetDPt[ityp][hip]->Sumw2();


               }
               hDiJetTriggPt[ityp] = new TH1D("hDiJetTriggPt"+iTStr, "hDiJetTriggPt"+iTStr,nBINS, logBinsX);hDiJetTriggPt[ityp]->Sumw2();
               hDiJetAssocPt[ityp] = new TH1D("hDiJetAssocPt"+iTStr, "hDiJetAssocPt"+iTStr,nBINS, logBinsX);hDiJetAssocPt[ityp]->Sumw2();
               hDiJetPt[ityp] = new TH1D("hDiJetPt"+iTStr, "hDiJetPt"+iTStr,nBINS, logBinsX);hDiJetPt[ityp]->Sumw2();
               hDiJetHadronPt[ityp] = new TH1D("hDiJetHadronPt"+iTStr, "hDiJetHadronPt"+iTStr,nBINS, logBinsX);hDiJetHadronPt[ityp]->Sumw2();
               for( int i2=0;i2<2;i2++ ){
               TString iName = iTStr+Form("%02d",i2);
               hDiJetTrackESum[ityp][i2] = new TH1D("hDiJetTrackESum"+iName, "hDiJetTrackESum"+iName,nBINS, logBinsX);hDiJetTrackESum[ityp][i2]->Sumw2();
               }
               hDiJetTrackESumRatio[ityp] = new TH1D("hDiJetTrackESumRatio"+iTStr, "hDiJetTrackESumRatio"+iTStr,100, 0, 1);hDiJetTrackESumRatio[ityp]->Sumw2();
               hDiJetptRatio[ityp] = new TH1D("hDiJetptRatio"+iTStr, "hDiJetptRatio"+iTStr,100, 0, 1);hDiJetptRatio[ityp]->Sumw2();
               hDiJetDeltPhi[ityp] = new TH2D("hDiJetDeltPhi"+iTStr,"",npi,pi0,pi1,nptpair,ptpair0,ptpair1);hDiJetDeltPhi[ityp]->Sumw2();
               for( int ipt=0;ipt<NJetPtBin;ipt++ ){
               TString nameIPT = iTStr+Form("%02d",ipt);
               for( int iptpair=0;iptpair<NPtPairBin;iptpair++ ){
               TString nameIPTPAIR = nameIPT + Form("%02d",iptpair);
               hDiJetDeltaPhi[ityp][ipt][iptpair] = new TH1D("hDiJetDeltaPhi"+nameIPTPAIR, "hDiJetDeltaPhi"+nameIPTPAIR, npi, pi0, pi1 );hDiJetDeltaPhi[ityp][ipt][iptpair]->Sumw2();
            //Add( hDiJetDeltaPhi, new TH1D("hDiJetDeltaPhi",,,,), 30, NJetPtBin, NPtPairBin );
            }
            }
            for( int i2=0;i2<3;i2++ ){
            TString iName = iTStr+Form("%02d",i2);
            hDiJetPtPair[ityp][i2] = new TH2D("hDiJetPtPair"+iName,"",nptpair,ptpair0,ptpair1,nBINS,logBinsX);hDiJetPtPair[ityp][i2]->Sumw2();
            hDiJetPtPairRandom[ityp][i2] = new TH2D("hDiJetPtPairRandom"+iName,"",nptpair,ptpair0,ptpair1,nBINS,logBinsX);hDiJetPtPairRandom[ityp][i2]->Sumw2();
            }
            }

            // ATLAS Paper figures
            const int NmultType = 4; 
            TString htitle[NmultType] = {"#geq 2jets","#geq 3 jets","#geq 4 jets","#geq 5 jets"};
            for(int ityp=0;ityp<4;ityp++) {
            TString hname = Form("hLeadingDiJetDeltaPhiNjetsAtlas%02d",ityp);
            hLeadingDiJetDeltaPhiNjetsAtlas[ityp] = new TH1D(hname,htitle[ityp]+"p_{T}>100GeV, p_{T,max}> 110GeV", npi, pi0, pi1 );hLeadingDiJetDeltaPhiNjetsAtlas[ityp]->Sumw2();
            for( int ipt=0;ipt<NJetPtBin;ipt++ ){
            TString hname = Form("hLeadingDiJetDeltaPhiNjets%02d%02d",ityp,ipt);
            htitle[ityp] += Form("%.1f< p_{T,jet} < %.1f",(*jetPtBins)[ipt+1],(*jetPtBins)[ipt+2]);
            hLeadingDiJetDeltaPhiNjets[ityp][ipt] = new TH1D(hname,htitle[ityp], npi, pi0, pi1 );hLeadingDiJetDeltaPhiNjets[ityp][ipt]->Sumw2();
            }
            }
            */

            int nBINS2=300;
            double logBinsX2[nBINS2+2], limL2=0.1, LimH2=1000;
            double logBW2 = (log(LimH2)-log(limL2))/nBINS2;
            for(int ij=0;ij<=nBINS2;ij++) logBinsX2[ij+1]=limL2*exp(ij*logBW2);
            logBinsX2[0]=0;
            int nBINS3=500;
            double logBinsX3[nBINS3+3], limL3=0.1, LimH3=10000;
            double logBW3 = (log(LimH3)-log(limL3))/nBINS3;
            for(int ij=0;ij<=nBINS3;ij++) logBinsX3[ij+1]=limL3*exp(ij*logBW3);
            logBinsX3[0]=0;
            TDirectory * owd = gDirectory;
            TDirectory * nwd = NULL;
            TString hname;
            TString strJetType[] = { "Incomming", "OutGoing" , "Full R=0.4 Anti-kt", "Full R=0.8 Anti-kt" , "Charge R=0.4 Anti-kt", "Charge R=0.8 Anti-kt"};
            TString strJetSel[] = { "-5<#eta<5", "-0.4<#eta<0.4" };
            TString strDiJetSel[] = { "Di-jet 2 higher p_{T}", "Di-jet AliceAcc", "Di-jet AliceAcc2"}; 
            TString strDiJetCase[] = { "in both", "only in outgoing", "only in Reconstructed"}; 
            for( int ijtyp=2;ijtyp<kJNJetTyp;ijtyp++ ){
                for( int ijs=0;ijs<kJNJetSelecton;ijs++ ){
                    for( int ipt=0;ipt<NJetPtBin+2;ipt++ ){
                        TString index = Form("%02dJ%02dP%02d", ijtyp, ijs, ipt );
                        TString title = " "+strJetType[ijtyp]+" "+strJetSel[ijs]+" "+Form("%.1f<E<%.1f", (*jetPtBins)[ipt+1],(*jetPtBins)[ipt+2]);
                        hname="hPartonJetDistance";JumpToDir(owd,hname);
                        fhPartonJetDistance[ijtyp][ijs][ipt] = new TH2D(hname+index, "Jet Distance"+title,nBINS2, logBinsX2, 100, 0, 10) ;
                        hname="hPartonJetDistanceOut";JumpToDir(owd,hname);
                        fhPartonJetDistanceOut[ijtyp][ijs][ipt] = new TH2D(hname+index, "Jet Distance"+title,nBINS2, logBinsX2, 100, 0, 10) ;
                    }
                    for( int ipro=0;ipro<5;ipro++ ){
                        TString index = Form("%02dJ%02dP%02d", ijtyp, ijs, ipro );
                        TString title = " "+strJetType[ijtyp]+" "+strJetSel[ijs]+" "+Form("%.1f<prob<%.1f", ipro*0.2 , ipro*0.2+0.2);
                        hname="hPartonJetDistance2";JumpToDir(owd,hname);
                        fhPartonJetDistance2[ijtyp][ijs][ipro] = new TH2D(hname+index,"Jet Distance"+title, 200, 0, 2, 100, 0, 10 );
                        hname="hPartonJetERatio";JumpToDir(owd,hname);
                        fhPartonJetERatio[ijtyp][ijs][ipro] = new TH1D(hname+index, "Ratio of E"+title, 200, 0, 2 );
                        hname="hPartonJetDeltaR";JumpToDir(owd,hname);
                        fhPartonJetDeltaR[ijtyp][ijs][ipro] = new TH1D( hname+index, "#DeltaR"+title, 100, 0, 10 );
                        hname="hPartonJetDeltaPhi";JumpToDir(owd,hname);
                        fhPartonJetDeltaPhi[ijtyp][ijs][ipro] = new TH1D( hname+index, "#Delta#phi"+title, 100, -3.2,3.2 );
                    }
                }
            }
            for( int iref=0;iref<kJNRef;iref++ ){
                for( int ijtyp=0;ijtyp<kJNJetTyp;ijtyp++ ){
                    for( int ijs=0;ijs<kJNJetSelecton;ijs++ ){
                        for( int idjs=0;idjs<kJNDiJetSelection;idjs++ ){
                            for( int ipt=0;ipt<NJetPtBin+2;ipt++ ){
                                TString title = " "+strJetType[ijtyp]+" "+strJetSel[ijs]+" "+strDiJetSel[idjs]+" " + Form("%.1f<p_{Tjet,ref}<%.1f", (*jetPtBins)[ipt+1],(*jetPtBins)[ipt+2]);
                                TString index = Form("R%02dT%02dJ%02dD%02dP%02d", iref, ijtyp, ijs, idjs, ipt );
                                hname="hDiJetDRtoRef";JumpToDir(owd,hname);
                                fhDiJetDRtoRef[iref][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetDRtoRef"+index,"#DeltaR to Ref"+title,100,-10,10);
                                hname="hDiJetDPhitoRef";JumpToDir(owd,hname);
                                fhDiJetDPhitoRef[iref][ijtyp][ijs][idjs][ipt] = new TH1D(hname+index,"#Delta#phi to Ref"+title,100,-10,10);
                                hname="hDiJetDEtatoRef";JumpToDir(owd,hname);
                                fhDiJetDEtatoRef[iref][ijtyp][ijs][idjs][ipt] = new TH1D(hname+index,"#Delta#eta to Ref"+title,100,-10,10);
                                hname="hDiJetDPttoRef";JumpToDir(owd,hname);
                                fhDiJetDPttoRef[iref][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetDPttoRef"+index,"#DeltaP_{T} to Ref"+title, nBINS2, logBinsX2);
                                hname="hDiJetPtAsymRef";JumpToDir(owd,hname);
                                fhDiJetPtAsymRef[iref][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetPtAsymRef"+index,"P_{T} Asym to Ref"+title,200,-1,1);
                                hname="hDiJetInvMToRef";JumpToDir(owd,hname);
                                fhDiJetInvMToRef[iref][ijtyp][ijs][idjs][ipt] = new TH2D("hDiJetInvMToRef"+index,"Invariant Mass to Ref"+title,nBINS3, logBinsX3, nBINS3, logBinsX3);
                                hname="hDiJetPtPairToRef";JumpToDir(owd,hname);
                                fhDiJetPtPairToRef[iref][ijtyp][ijs][idjs][ipt] = new TH2D("hDiJetPtPairToRef"+index,"p_{T}pair to Ref"+title, nBINS2, logBinsX2, nBINS2, logBinsX2);
                                hname="hDiJetEnergyToRef";JumpToDir(owd,hname);
                                fhDiJetEnergyToRef[iref][ijtyp][ijs][idjs][ipt] = new TH2D("hDiJetEnergyToRef"+index,"Energy to Ref"+title, nBINS2, logBinsX2, nBINS2, logBinsX2);
                                JumpToDir(owd);
                                for( int is=0;is<3;is++ ){
                                    TString title = " "+strJetType[ijtyp]+" "+strJetSel[ijs]+" "+strDiJetSel[idjs]+" " + Form("%.1f<p_{Tjet,ref}<%.1f", (*jetPtBins)[ipt+1],(*jetPtBins)[ipt+2]) + strDiJetCase[is];
                                    TString index2 = Form("R%02dT%02dJ%02dD%02dS%02dP%02d", iref, ijtyp, ijs, idjs, is, ipt );
                                    hname="hDiJetPt";JumpToDir(owd,hname);
                                    fhDiJetPt[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetPt"+index2,"di-Jet Pt"+title, nBINS2, logBinsX2);
                                    hname="hDiJetLeadingParticlePt";JumpToDir(owd,hname);
                                    fhDiJetLeadingParticlePt[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D(hname+index2,"leading particle pt"+title, nBINS2, logBinsX2);
                                    hname="hDiJetNConst";JumpToDir(owd,hname);
                                    fhDiJetNConst[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetNConst"+index2,"Number of constituents"+title,100,0,100);
                                    hname="hDiJetDPhi";JumpToDir(owd,hname);
                                    fhDiJetDPhi[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetDPhi"+index2,"di-jet #Deltaphi"+title,100, -TMath::Pi(), TMath::Pi());
                                    hname="hDiJetPtPair";JumpToDir(owd,hname);
                                    fhDiJetPtPair[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetPtPair"+index2,"p_{T,pair}"+title, nBINS2, logBinsX2);
                                    hname="hDiJetInvM";JumpToDir(owd,hname);
                                    fhDiJetInvM[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetInvM"+index2,"Invariant Mass"+title, nBINS3, logBinsX3);
                                    hname="hDiJetDPt";JumpToDir(owd,hname);
                                    fhDiJetDPt[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetDPt"+index2,"#Deltap_{T}"+title, nBINS2, logBinsX2);
                                    hname="hDiJetPtAssym";JumpToDir(owd,hname);
                                    fhDiJetPtAssym[iref][is][ijtyp][ijs][idjs][ipt] = new TH1D("hDiJetPtAssym"+index2,"p_{T} Asym"+title,200,-1,1);
                                }

                            }
                        }
                    }
                }
            }
            hname="QA";JumpToDir(owd,hname);

            fhParticlePt = new TH1D("hParticlePt", "Pt of particle", nBINS2, logBinsX2 );
            fhPtHat = new TH1D("hPtHat", "pthat of event", nBINS2, logBinsX2 );
            for( int ijtyp=0;ijtyp<kJNJetTyp;ijtyp++ ){
                for( int ijs=0;ijs<kJNJetSelecton;ijs++ ){
                    TString index = Form("T%02dJ%02d", ijtyp, ijs );
                    TString title = " "+strJetType[ijtyp]+" "+strJetSel[ijs];
                    fhJet2Pt[ijtyp][ijs]=new TH1D("hJet2Pt"+index,"p_{T}"+title, nBINS2, logBinsX2);
                    fhJet2Phi[ijtyp][ijs]=new TH1D("hJet2Phi"+index,"#phi"+title, 100, -TMath::Pi(), TMath::Pi());
                    fhJet2Eta[ijtyp][ijs]=new TH1D("hJet2Eta"+index,"#eta"+title, 100, -6, 6);
                    fhJet2Charge[ijtyp][ijs]=new TH1D("hJet2Charge"+index,"Charge"+title, 2, 0, 2);
                    fhJet2NConst[ijtyp][ijs]=new TH1D("hJet2NConst"+index,"Number of constituents"+title, 100, 0, 100);
                    fhJet2Energy[ijtyp][ijs]=new TH1D("hJet2Energy"+index,"Number of constituents"+title, 100, 0, 100);
                }
            }
            owd->cd();

            for( int ijtyp=0;ijtyp<kJNJetTyp;ijtyp++ ){
                for( int ijs=0;ijs<kJNJetSelecton;ijs++ ){
                    for( int idjs=0;idjs<kJNDiJetSelection;idjs++ ){
                        for( int iptbin=0;iptbin<2;iptbin++ ){
                            for( int ipt=0;ipt<NJetPtBin+2;ipt++ ){
                                //if( ijtyp == 0 && js!=0 && idjs!=0 ) continue;
                                //if( ijtyp == 1 && idjs!=0 ) return;
                                TString title = " "+strJetType[ijtyp]+" "+strJetSel[ijs]+" "+strDiJetSel[idjs]+" " + Form("%.1f<p_{Tjet,ref}<%.1f", (*jetPtBins)[ipt+1],(*jetPtBins)[ipt+2]);
                                TString index = Form("T%02dJ%02dD%02dB%02dP%02d", ijtyp, ijs, idjs, iptbin, ipt );

                                hname="hDiJetPtPairOrg";JumpToDir(owd,hname);
                                fhDiJetPtPairOrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D(hname+index,"p_{T,pair}"+title, nBINS2, logBinsX2);

                                hname="hDiJetDPhiOrg";JumpToDir(owd,hname);
                                fhDiJetDPhiOrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D("hDiJetDPhiOrg"+index,"di-jet #Deltaphi"+title,100, -TMath::Pi(), TMath::Pi());

                                hname="hDiJetDEtaOrg";JumpToDir(owd,hname);
                                fhDiJetDEtaOrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D(hname+index,"#Delta#eta to Ref"+title,100,-10,10);

                                hname="hDiJetDROrg";JumpToDir(owd,hname);
                                fhDiJetDROrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D("hDiJetDROrg"+index,"#DeltaR to Ref"+title,100,-10,10);

                                hname="hDiJetPtAssymOrg";JumpToDir(owd,hname);
                                fhDiJetPtAssymOrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D("hDiJetPtAssymOrg"+index,"p_{T} Asym"+title,200,-1,1);

                                hname="hDiJetEAssymOrg";JumpToDir(owd,hname);
                                fhDiJetEAssymOrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D("hDiJetEAssymOrg"+index,"E Asym"+title,200,-1,1);

                                hname="hDiJetKtAOrg";JumpToDir(owd,hname);
                                fhDiJetKtAOrg[ijtyp][ijs][idjs][iptbin][ipt] = new TH1D(hname+index,"k_{T} "+title , nBINS2, logBinsX2);
                            }
                        }

                    }
                }
            }
            JumpToDir(owd,"pythiaDiJetSum");
            for( int it=0;it<2;it++ ){
                for( int ipt=0;ipt<NJetPtBin+2;ipt++ ){
                    TString title = Form("%.1f<p_{Tjet,ref}<%.1f", (*jetPtBins)[ipt+1],(*jetPtBins)[ipt+2]);
                    TString index = Form("T%02dP%02d", it, ipt );

                    fPythiaJetPtPair[it][ipt] = new TH1D("hPythiaJetPtPair"+index,"p_{T,pair}"+title, nBINS2, logBinsX2);
                }
                fPythiaJetSum[it] = new TH1D( Form("hPythiaJetSumT%02d",it), "Sum of Pythia 4 jets", nBINS2, logBinsX );
            }
            JumpToDir(owd);
            fhJetPartonProb = new TH1D("hJetPrtonProb","probability of jet parton", 100,0,1);
            fhJetPartonProb0 = new TH1D("hJetPrtonProb0","probability of jet parton", 100,0,1);
            fhJetPartonProb1 = new TH1D("hJetPrtonProb1","probability of jet parton", 100,0,1);
        }
};


#endif


