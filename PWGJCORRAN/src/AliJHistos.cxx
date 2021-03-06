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

#include "AliJHistos.h"
#include  "AliJCard.h"
#include  "AliJBaseTrack.h"
#include  "AliJPhoton.h"
#include  "AliJTrack.h"
#include <TGrid.h>
#include <TPRegexp.h>

//______________________________________________________________________________
AliJHistos::AliJHistos(AliJCard* cardP) :
  fCard(cardP),
  fHMG(NULL),
  fCentBin(),
  fVtxBin(),
  fPTtBin(),
  fPTaBin(),
  fXEBin(),
  fKLongBin(),
  fRGapBin(),
  fEtaGapBin(),
  fPhiGapBin(),
  fMassBin(),
  fTypBin(),
  fTypBin3(),
  fPairPtBin(),
  fhMixStat(),
  fTestHist(),
  fhPtNear(),
  fhPtFar(),
  fhPhi(),
  fhDphiAssoc(),
  fhDphiAssocXEbin(),
  fhDphiAssoc2DIAA(),
  fhDphiAssoc2D(),
  fhDphiDetaKlong(),
  fhDphiDetaKlongR(),
  fhDphiDetaXe(),
  fhDphiDetaXeR(),
  fhDphiDetaPta(),
  fhDphiDetaPtaR(),
  fhBgAssocKlong(),
  fhBgAssocKlongR(),
  fhBgAssocXe(),
  fhBgAssocXeR(),
  fhBgAssocPta(),
  fhBgAssocPtaR(),
  fhInvariantMassXe(),
  fhInvariantMassKlong(),
  fhInvariantMassPta(),
  fhInvariantMassXeLikeSign(),
  fhInvariantMassKlongLikeSign(),
  fhInvariantMassPtaLikeSign(),
  fhInvariantMassXeUnlikeSign(),
  fhInvariantMassKlongUnlikeSign(),
  fhInvariantMassPtaUnlikeSign(),
  fhDphiAssocIsolTrigg(),
  fhMeanPtAssoc(),
  fhMeanZtAssoc(),
  fhPtAssocUE(),
  fhPtaEtaGapN(),
  fhPtaRGapN(),
  fhPtAssocUEIsolTrigg(),
  fhPtAssocN(),
  fhPtAssocF(),
  fhCosThetaStar(),
  fhCMSrap(),
  fpCMSrap(),
  fhInvMass(),
  fhPairPtMass(),
  fhPairDPhi(),
  fhPairDpT(),
  fhPairPtDphi(),
  fhPairPt(),
  fhDEtaNear(),
  fhDEtaNearM(),
  fhDEtaNearXEbin(),
  fhDEtaNearMXEbin(),
  fhDRNearPt(),
  fhDRFarPt(),
  fhDRNearPtMoon(),
  fhDRFarPtMoon(),
  fhDRNearPtMoonM(),
  fhDRFarPtMoonM(),
  fhDEtaFar(),
  fhIphiTrigg(),
  fhIetaTrigg(),
  fhIphiAssoc(),
  fhIetaAssoc(),
  fhFixPtBin(),
  fhTriggPtBin(),
  fhTriggPtBinIsolTrigg(),
  fhTriggMult(),
  fhAssocPtBin(),
  fhxEN(),
  fhxEF(),
  fhxEFIsolTrigg(),
  fhPoutF(),
  fhxEPtBin(),
  fhJT(),
  fhJTBg(),
  fhJTBgR(),
  fhJTLikeSign(),
  fhJTBgLikeSign(),
  fhJTBgRLikeSign(),
  fhJTUnlikeSign(),
  fhJTBgUnlikeSign(),
  fhJTBgRUnlikeSign(),
  fhJTKlong(),
  fhJTKlongBg(),
  fhJTKlongBgR(),
  fhJTKlongLikeSign(),
  fhJTKlongBgLikeSign(),
  fhJTKlongBgRLikeSign(),
  fhJTKlongUnlikeSign(),
  fhJTKlongBgUnlikeSign(),
  fhJTKlongBgRUnlikeSign(),
  fhJTPta(),
  fhJTPtaBg(),
  fhJTPtaBgR(),
  fhJTPtaLikeSign(),
  fhJTPtaBgLikeSign(),
  fhJTPtaBgRLikeSign(),
  fhJTPtaUnlikeSign(),
  fhJTPtaBgUnlikeSign(),
  fhJTPtaBgRUnlikeSign(),
  fHmgInclusive(NULL),
  fhIetaTriggFromFile(),
  fhIetaAssocFromFile(),
  fhIphiTriggFromFile(),
  fhIphiAssocFromFile(),
  fhDphiAssocMixFromFile(),
  fhLPpt(),
  fhLPpairPt(),
  fhChargedPt(),
  fhChargedPtNoCorr(),
  fhChargedPtJacek(),
  fhChargedPtJacekEta(),
  fhChargedPtFiete(),
  fhVdelta2(),
  fhVdelta3(),
  fhVN(),
  fhTrackingEfficiency(),
  fpV2(),
  fpV3(),
  fpVdeltaNorm(),
  fhChargedEta(),
  fhLPeta(),
  fhAssocMult(),
  fhChargedMult(),
  fhChargedMultCut(),
  fhChargedMultCent(),
  fhXt(),
  fhXtWeighted(),
  fhXtWeightedHT(),
  fhPtForXt(),
  fhConeActivity(),
  fhConeActivityIsolated(),
  fhPerpConeActivity(),
  fhPerpConeActivityIsolated(),
  fhV0AMult(),
  fhZVertRaw(),
  fhZVertRawErr(),
  fhZVert(),
  fhCentr(),
  fhiCentr(),
  fhEventPerRun(),
  fhVertexZTriggVtx(),
  fhIsolatedLPpt(),
  fhBkgActivity(),
  fhDphiLPJet(),
  fhDEtaLPJet(),
  fhDPtLPJet(),
  fhLPJetPTt(),
  fhLPJetPt(),
  fhLPJetEtaPTt(),
  fhLPJetRapidityPTt(),
  fhLPJetMassPTt(),
  fhLeadingJetWLPPTt(),
  fhJetPt(),
  fhLeadingJetPt(),
  fhLeadingJetWLPPt(),
  fhJetAssymPTt(),
  fhJetMassPTt(),
  fhJetUEPt(),
  fhJetDphi(),
  fhJetDeta(),
  fhJetMultPt(),
  fhJetRho(),
  fhJetRhoSigma(),
  fhJetPartMult(),
  fhRecoDiJetM(),
  fhRecoDiJetdPhi(),
  fhRecoDiJetkT(),
  fhNParton71(),
  fhNStringGroup(),
  fhNStringGroupFrom(),
  fhNTracksInStringGroupFrom(),
  fhRapidity71From(),
  fhPt71From(),
  fhTrackSelection(),
  fNJacek(0),
  fPttJacek(0),
  fNEta(0),
  fEta(0),
  fNJanFiete(0),
  fJanFiete(0),
  fhEvents(),
  fhEventTrigger(),
  fmaxEtaRange(0),
  fmaxTriggEtaRange(0),
  ftriggFiducCut(0),
  fnUE(0),
  fnUEfar(0),
  fLowRange(0),
  fHighRange(0)
{   // constructor

    fmaxEtaRange = fCard->Get("EtaRange");
    ftriggFiducCut =  fCard->Get("TriggerFiducialEtaCut"); //FK// Fiduc cut 
    fmaxTriggEtaRange =  fmaxEtaRange - ftriggFiducCut; //FK// Trigger range

    fHMG = new AliJHistManager( "HistManager","AliJHistos");
    //for (int hiklong = 0; hiklong < fCard->GetNoOfBins(kLongType); hiklong++)
    //kRGapType kEtaGapType
    fCentBin   .Set("Cent",   "C", "Cend:%2.0f-%2.0f%%" ).SetBin( fCard->GetVector("CentBinBorders"));
    fVtxBin    .Set("Vtx",    "V", "Vtx:%2.0f-%2.0f" ).SetBin(fCard->GetVector("zVertBins"));
    fPTtBin    .Set("PTt",    "T", "%.2f<p_{Tt}<%.2f").SetBin(fCard->GetVector("TriggPtBorders"));
    fPTaBin    .Set("PTa",    "A", "%.2f<p_{Ta}<%.2f").SetBin(fCard->GetVector("AssocPtBorders"));
    fXEBin     .Set("XE",     "X", "%.1f<x_{E}<%.1f" ).SetBin(fCard->GetVector("xEBorders"));
    fKLongBin  .Set("KLong", "L",  "%.1f<k_{#parallel}<%.1f").SetBin(fCard->GetVector("KlongBorders"));
    fRGapBin   .Set("RGap",  "R",  "%.1f<R_{gap}<%.1f").SetBin(fCard->GetVector("RGapThresholds"));
    fEtaGapBin .Set("EtaGap", "E", "%.1f<#eta_{gap}<%.1f").SetBin(fCard->GetVector("EtaGapThresholds"));
    fPhiGapBin .Set("PhiGap", "P", "%.1f<#phi_{gap}<%.1f" ).SetBin(fCard->GetVector("EtaGapThresholds"));
    fMassBin   .Set("Mass",   "M", "%.1f<M_{jj}<%.1f").SetBin(fCard->GetVector("PairInvariantMassBins"));
    fTypBin    .Set("Type",   "D", "", AliJBin::kSingle ).SetBin( "0 1" );
    fTypBin3    .Set("Type3",   "D", "", AliJBin::kSingle ).SetBin( "0 1 2 3" );
    //card->IsLessThanUpperPairPtCut(-ipairpt)
    fPairPtBin .Set("PairPt", "", AliJBin::kSingle ).SetBin( fCard->GetN("UpperPairPtCut") );

    const int nJacek =  73 ;
    double pttJacek[nJacek+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
      1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,
      10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 60, 70, 80, 90, 100};
  
    //const int nJacek =  59 ;
    //double pttJacek[nJacek] = { 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.5, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.9, 0.95, 1.,
    //    1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3., 3.2, 3.4, 3.6, 3.8, 4., 4.5, 5., 5.5, 6., 6.5, 7., 8., 9., 10,
    //    11., 12., 13., 14., 15., 16., 18., 20, 22, 24, 26., 28., 30.};
  
    fNJacek = nJacek;  // Number of bins
    fPttJacek = new double[fNJacek+1];
    for(int i = 0; i <= fNJacek; i++){
      fPttJacek[i] = pttJacek[i];
    }
  
    const int nEta = 3;
    double eta[nEta+1] = {-0.8,-0.2,0.3,0.8};// eta bins
  
    fNEta = nEta;  // Number of bins
    fEta = new double[fNEta+1];
    for(int i = 0; i <= fNEta; i++){
      fEta[i] = eta[i];
    }
  
    const int nJanFiete=200;
    double janFiete[nJanFiete+1]={0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5,
      5.75, 6, 6.25, 6.5, 6.75, 7, 7.25, 7.5, 7.75, 8, 8.25, 8.5, 8.75, 9, 9.25, 9.5, 9.75, 10, 10.25, 10.5, 10.75, 11, 11.25, 11.5, 11.75,
      12, 12.25, 12.5, 12.75, 13, 13.25, 13.5, 13.75, 14, 14.25, 14.5, 14.75, 15, 15.25, 15.5, 15.75, 16, 16.25, 16.5, 16.75, 17, 17.25, 17.5,
      17.75, 18, 18.25, 18.5, 18.75, 19, 19.25, 19.5, 19.75, 20, 20.25, 20.5, 20.75, 21, 21.25, 21.5, 21.75, 22, 22.25, 22.5, 22.75, 23, 23.25,
      23.5, 23.75, 24, 24.25, 24.5, 24.75, 25, 25.25, 25.5, 25.75, 26, 26.25, 26.5, 26.75, 27, 27.25, 27.5, 27.75, 28, 28.25, 28.5, 28.75, 29,
      29.25, 29.5, 29.75, 30, 30.25, 30.5, 30.75, 31, 31.25, 31.5, 31.75, 32, 32.25, 32.5, 32.75, 33, 33.25, 33.5, 33.75, 34, 34.25, 34.5, 34.75,
      35, 35.25, 35.5, 35.75, 36, 36.25, 36.5, 36.75, 37, 37.25, 37.5, 37.75, 38, 38.25, 38.5, 38.75, 39, 39.25, 39.5, 39.75, 40, 40.25, 40.5,
      40.75, 41, 41.25, 41.5, 41.75, 42, 42.25, 42.5, 42.75, 43, 43.25, 43.5, 43.75, 44, 44.25, 44.5, 44.75, 45, 45.25, 45.5, 45.75, 46, 46.25,
      46.5, 46.75, 47, 47.25, 47.5, 47.75, 48, 48.25, 48.5, 48.75, 49, 49.25, 49.5, 49.75, 50};

    fNJanFiete = nJanFiete;  // Number of bins
    fJanFiete = new double[fNJanFiete+1];
    for(int i = 0; i <= fNJanFiete; i++){
      fJanFiete[i] = janFiete[i];
    }
  
    //fhtyp[1] = "Real";
    //fhtyp[2] = "Mixed";
    //fhtyp[3] = "Rap. Gap";
}

//______________________________________________________________________________
AliJHistos::AliJHistos(const AliJHistos& obj) : 
  fCard(obj.fCard),
  fHMG(obj.fHMG),
  fCentBin(obj.fCentBin),
  fVtxBin(obj.fVtxBin),
  fPTtBin(obj.fPTtBin),
  fPTaBin(obj.fPTaBin),
  fXEBin(obj.fXEBin),
  fKLongBin(obj.fKLongBin),
  fRGapBin(obj.fRGapBin),
  fEtaGapBin(obj.fEtaGapBin),
  fPhiGapBin(obj.fPhiGapBin),
  fMassBin(obj.fMassBin),
  fTypBin(obj.fTypBin),
  fTypBin3(obj.fTypBin3),
  fPairPtBin(obj.fPairPtBin),
  fhMixStat(obj.fhMixStat),
  fTestHist(obj.fTestHist),
  fhPtNear(obj.fhPtNear),
  fhPtFar(obj.fhPtFar),
  fhPhi(obj.fhPhi),
  fhDphiAssoc(obj.fhDphiAssoc),
  fhDphiAssocXEbin(obj.fhDphiAssocXEbin),
  fhDphiAssoc2DIAA(obj.fhDphiAssoc2DIAA),
  fhDphiAssoc2D(obj.fhDphiAssoc2D),
  fhDphiDetaKlong(obj.fhDphiDetaKlong),
  fhDphiDetaKlongR(obj.fhDphiDetaKlongR),
  fhDphiDetaXe(obj.fhDphiDetaXe),
  fhDphiDetaXeR(obj.fhDphiDetaXeR),
  fhDphiDetaPta(obj.fhDphiDetaPta),
  fhDphiDetaPtaR(obj.fhDphiDetaPtaR),
  fhBgAssocKlong(obj.fhBgAssocKlong),
  fhBgAssocKlongR(obj.fhBgAssocKlongR),
  fhBgAssocXe(obj.fhBgAssocXe),
  fhBgAssocXeR(obj.fhBgAssocXeR),
  fhBgAssocPta(obj.fhBgAssocPta),
  fhBgAssocPtaR(obj.fhBgAssocPtaR),
  fhInvariantMassXe(obj.fhInvariantMassXe),
  fhInvariantMassKlong(obj.fhInvariantMassKlong),
  fhInvariantMassPta(obj.fhInvariantMassPta),
  fhInvariantMassXeLikeSign(obj.fhInvariantMassXeLikeSign),
  fhInvariantMassKlongLikeSign(obj.fhInvariantMassKlongLikeSign),
  fhInvariantMassPtaLikeSign(obj.fhInvariantMassPtaLikeSign),
  fhInvariantMassXeUnlikeSign(obj.fhInvariantMassXeUnlikeSign),
  fhInvariantMassKlongUnlikeSign(obj.fhInvariantMassKlongUnlikeSign),
  fhInvariantMassPtaUnlikeSign(obj.fhInvariantMassPtaUnlikeSign),
  fhDphiAssocIsolTrigg(obj.fhDphiAssocIsolTrigg),
  fhMeanPtAssoc(obj.fhMeanPtAssoc),
  fhMeanZtAssoc(obj.fhMeanZtAssoc),
  fhPtAssocUE(obj.fhPtAssocUE),
  fhPtaEtaGapN(obj.fhPtaEtaGapN),
  fhPtaRGapN(obj.fhPtaRGapN),
  fhPtAssocUEIsolTrigg(obj.fhPtAssocUEIsolTrigg),
  fhPtAssocN(obj.fhPtAssocN),
  fhPtAssocF(obj.fhPtAssocF),
  fhCosThetaStar(obj.fhCosThetaStar),
  fhCMSrap(obj.fhCMSrap),
  fpCMSrap(obj.fpCMSrap),
  fhInvMass(obj.fhInvMass),
  fhPairPtMass(obj.fhPairPtMass),
  fhPairDPhi(obj.fhPairDPhi),
  fhPairDpT(obj.fhPairDpT),
  fhPairPtDphi(obj.fhPairDPhi),
  fhPairPt(obj.fhPairPt),
  fhDEtaNear(obj.fhDEtaNear),
  fhDEtaNearM(obj.fhDEtaNearM),
  fhDEtaNearXEbin(obj.fhDEtaNearXEbin),
  fhDEtaNearMXEbin(obj.fhDEtaNearMXEbin),
  fhDRNearPt(obj.fhDRNearPt),
  fhDRFarPt(obj.fhDRFarPt),
  fhDRNearPtMoon(obj.fhDRNearPtMoon),
  fhDRFarPtMoon(obj.fhDRFarPtMoon),
  fhDRNearPtMoonM(obj.fhDRNearPtMoonM),
  fhDRFarPtMoonM(obj.fhDRFarPtMoonM),
  fhDEtaFar(obj.fhDEtaFar),
  fhIphiTrigg(obj.fhIphiTrigg),
  fhIetaTrigg(obj.fhIetaTrigg),
  fhIphiAssoc(obj.fhIphiAssoc),
  fhIetaAssoc(obj.fhIetaAssoc),
  fhFixPtBin(obj.fhFixPtBin),
  fhTriggPtBin(obj.fhTriggPtBin),
  fhTriggPtBinIsolTrigg(obj.fhTriggPtBinIsolTrigg),
  fhTriggMult(obj.fhTriggMult),
  fhAssocPtBin(obj.fhAssocPtBin),
  fhxEN(obj.fhxEN),
  fhxEF(obj.fhxEF),
  fhxEFIsolTrigg(obj.fhxEFIsolTrigg),
  fhPoutF(obj.fhPoutF),
  fhxEPtBin(obj.fhxEPtBin),
  fhJT(obj.fhJT),
  fhJTBg(obj.fhJTBg),
  fhJTBgR(obj.fhJTBgR),
  fhJTLikeSign(obj.fhJTLikeSign),
  fhJTBgLikeSign(obj.fhJTBgLikeSign),
  fhJTBgRLikeSign(obj.fhJTBgRLikeSign),
  fhJTUnlikeSign(obj.fhJTUnlikeSign),
  fhJTBgUnlikeSign(obj.fhJTBgUnlikeSign),
  fhJTBgRUnlikeSign(obj.fhJTBgRUnlikeSign),
  fhJTKlong(obj.fhJTKlong),
  fhJTKlongBg(obj.fhJTKlongBg),
  fhJTKlongBgR(obj.fhJTKlongBgR),
  fhJTKlongLikeSign(obj.fhJTKlongLikeSign),
  fhJTKlongBgLikeSign(obj.fhJTKlongBgLikeSign),
  fhJTKlongBgRLikeSign(obj.fhJTKlongBgRLikeSign),
  fhJTKlongUnlikeSign(obj.fhJTKlongUnlikeSign),
  fhJTKlongBgUnlikeSign(obj.fhJTKlongBgUnlikeSign),
  fhJTKlongBgRUnlikeSign(obj.fhJTKlongBgRUnlikeSign),
  fhJTPta(obj.fhJTPta),
  fhJTPtaBg(obj.fhJTPtaBg),
  fhJTPtaBgR(obj.fhJTPtaBgR),
  fhJTPtaLikeSign(obj.fhJTPtaLikeSign),
  fhJTPtaBgLikeSign(obj.fhJTPtaBgLikeSign),
  fhJTPtaBgRLikeSign(obj.fhJTPtaBgRLikeSign),
  fhJTPtaUnlikeSign(obj.fhJTPtaUnlikeSign),
  fhJTPtaBgUnlikeSign(obj.fhJTPtaBgUnlikeSign),
  fhJTPtaBgRUnlikeSign(obj.fhJTPtaBgRUnlikeSign),
  fHmgInclusive(obj.fHmgInclusive),
  fhIetaTriggFromFile(),
  fhIetaAssocFromFile(),
  fhIphiTriggFromFile(),
  fhIphiAssocFromFile(),
  fhDphiAssocMixFromFile(),
  fhLPpt(obj.fhLPpt),
  fhLPpairPt(obj.fhLPpairPt),
  fhChargedPt(obj.fhChargedPt),
  fhChargedPtNoCorr(obj.fhChargedPtNoCorr),
  fhChargedPtJacek(obj.fhChargedPtJacek),
  fhChargedPtJacekEta(obj.fhChargedPtJacekEta),
  fhChargedPtFiete(obj.fhChargedPtFiete),
  fhVdelta2(obj.fhVdelta2),
  fhVdelta3(obj.fhVdelta3),
  fhVN(obj.fhVN),
  fhTrackingEfficiency(obj.fhTrackingEfficiency),
  fpV2(obj.fpV2),
  fpV3(obj.fpV3),
  fpVdeltaNorm(obj.fpVdeltaNorm),
  fhChargedEta(obj.fhChargedEta),
  fhLPeta(obj.fhLPeta),
  fhAssocMult(obj.fhAssocMult),
  fhChargedMult(obj.fhChargedMult),
  fhChargedMultCut(obj.fhChargedMultCut),
  fhChargedMultCent(obj.fhChargedMultCent),
  fhXt(obj.fhXt),
  fhXtWeighted(obj.fhXtWeighted),
  fhXtWeightedHT(obj.fhXtWeightedHT),
  fhPtForXt(obj.fhPtForXt),
  fhConeActivity(obj.fhConeActivity),
  fhConeActivityIsolated(obj.fhConeActivityIsolated),
  fhPerpConeActivity(obj.fhPerpConeActivity),
  fhPerpConeActivityIsolated(obj.fhPerpConeActivityIsolated),
  fhV0AMult(obj.fhV0AMult),
  fhZVertRaw(obj.fhZVertRaw),
  fhZVertRawErr(obj.fhZVertRawErr),
  fhZVert(obj.fhZVert),
  fhCentr(obj.fhCentr),
  fhiCentr(obj.fhiCentr),
  fhEventPerRun(obj.fhEventPerRun),
  fhVertexZTriggVtx(obj.fhVertexZTriggVtx),
  fhIsolatedLPpt(obj.fhIsolatedLPpt),
  fhBkgActivity(obj.fhBkgActivity),
  fhDphiLPJet(obj.fhDphiLPJet),
  fhDEtaLPJet(obj.fhDEtaLPJet),
  fhDPtLPJet(obj.fhDPtLPJet),
  fhLPJetPTt(obj.fhLPJetPTt),
  fhLPJetPt(obj.fhLPJetPt),
  fhLPJetEtaPTt(obj.fhLPJetEtaPTt),
  fhLPJetRapidityPTt(obj.fhLPJetRapidityPTt),
  fhLPJetMassPTt(obj.fhLPJetMassPTt),
  fhLeadingJetWLPPTt(obj.fhLeadingJetWLPPTt),
  fhJetPt(obj.fhJetPt),
  fhLeadingJetPt(obj.fhLeadingJetPt),
  fhLeadingJetWLPPt(obj.fhLeadingJetWLPPt),
  fhJetAssymPTt(obj.fhJetAssymPTt),
  fhJetMassPTt(obj.fhJetMassPTt),
  fhJetUEPt(obj.fhJetUEPt),
  fhJetDphi(obj.fhJetDphi),
  fhJetDeta(obj.fhJetDeta),
  fhJetMultPt(obj.fhJetMultPt),
  fhJetRho(obj.fhJetRho),
  fhJetRhoSigma(obj.fhJetRhoSigma),
  fhJetPartMult(obj.fhJetPartMult),
  fhRecoDiJetM(obj.fhRecoDiJetM),
  fhRecoDiJetdPhi(obj.fhRecoDiJetdPhi),
  fhRecoDiJetkT(obj.fhRecoDiJetkT),
  fhNParton71(obj.fhNParton71),
  fhNStringGroup(obj.fhNStringGroup),
  fhNStringGroupFrom(obj.fhNStringGroupFrom),
  fhNTracksInStringGroupFrom(obj.fhNTracksInStringGroupFrom),
  fhRapidity71From(obj.fhRapidity71From),
  fhPt71From(obj.fhPt71From),
  fhTrackSelection(obj.fhTrackSelection),
  fNJacek(obj.fNJacek),
  fPttJacek(obj.fPttJacek),
  fNEta(obj.fNEta),
  fEta(obj.fEta),
  fNJanFiete(obj.fNJanFiete),
  fJanFiete(obj.fJanFiete),
  fhEvents(obj.fhEvents),
  fhEventTrigger(obj.fhEventTrigger),
  fmaxEtaRange(obj.fmaxEtaRange),
  fmaxTriggEtaRange(obj.fmaxTriggEtaRange),
  ftriggFiducCut(obj.ftriggFiducCut),
  fnUE(obj.fnUE),
  fnUEfar(obj.fnUEfar),
  fLowRange(obj.fLowRange),
  fHighRange(obj.fHighRange)
{
    // copy constructor
    JUNUSED(obj);
}

//______________________________________________________________________________
AliJHistos& AliJHistos::operator=(const AliJHistos& obj){
    // copy constructor
    JUNUSED(obj);
    return *this;
}

AliJHistos::~AliJHistos() {
	delete fHMG;
	delete fHmgInclusive;
    delete []fJanFiete;
    delete []fPttJacek;
    delete []fEta;
}

//______________________________________________________________________________
void AliJHistos::CreateAzimuthCorrHistos()
{
  // Comment needed here!
  fHMG->cd();
  
    int    bins = 240; // 240 is divisible by 2,3,4,612*24=280    -1/3 and  0.5 and 5/3  are bin edges 
    //double fLowRange = -1.0/3, fHighRange= 5.0/3;
    fLowRange = -9./20.;   //lower range for dphi histos
    fHighRange= fLowRange+2;       //upper range for dphi histos;
    double ptbw=10/100.0;  //see hPt histo below, let's make 10 bins per 1GeV/c

    fnUE=20;
    double uEa = fCard->GetBinBorder(kAssocType, 0), uEb = fCard->GetBinBorder(kAssocType, fCard->GetNoOfBins(kAssocType));
    double logUEbw = (log(uEb)-log(uEa))/fnUE;
    for(int ij=0;ij<=fnUE;ij++) fUEBinsx[ij]=uEa*exp(ij*logUEbw);

    fnUEfar=10;
    logUEbw = (log(uEb)-log(uEa))/fnUEfar;
    for(int ij=0;ij<=fnUE;ij++) fUEBinsxFar[ij]=uEa*exp(ij*logUEbw);


    if(fCard->GetNoOfBins(kCentrType) > kMaxNoCentrBin ){
        cout<<"ERROR: No of Centrality bins exceed max dim in AliJHistos.cxx "<<endl;
        exit(0);
    }

    //==================================
    //  trigger pt fhistos 
    //==================================
    //TODO tmp
    double pTt1 = fPTtBin.GetMin();
    double pTt2 = fPTtBin.GetMax();
    double pTa1 = fPTaBin.GetMin();
    double pTa2 = fPTaBin.GetMax();

    fhTriggPtBinIsolTrigg
        << TH1D( "hTriggPtBinIsolTrigg", "", (int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2)
        <<  fTypBin << fCentBin << fPTtBin  
        << "END";
    fhTriggMult
        << TH1D( "hTriggMult", "",  100, -0.5, 99.5) 
        <<  fCentBin << fPTtBin  << "END";
    fhIphiTrigg
        << TH1D( "fhIphiTrigg", "",  bins, -kJPi-0.1, kJPi+0.1) 
        <<  fCentBin << fPTtBin  << "END";
    fhIetaTrigg
        << TH1D( "hIetaTrigg", "",  80, -fmaxEtaRange, fmaxEtaRange) 
        <<  fCentBin << fPTtBin  << "END";// inclusive eta
    fhTriggPtBin
        << TH1D( "hTriggPtBin", "", (int)TMath::Ceil((pTt2-pTt1)/ptbw),pTt1, pTt2) 
        <<  fCentBin << fVtxBin << fPTtBin  << "END";

    //=====================================
    //  associated pt fhistos with etaGaps
    //=====================================

    fhDphiAssoc
        << TH1D( "hDphiAssoc", "",  bins, fLowRange, fHighRange) 
        <<  fTypBin << fCentBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";

    fhDEtaNear
        << TH1D( "hDEtaNear", "",  320, -2*fmaxEtaRange, 2*fmaxEtaRange) 
        <<  fCentBin << fVtxBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";
    fhDEtaNearM
        << TH1D( "hDEtaNearM", "",  320, -2*fmaxEtaRange, 2*fmaxEtaRange) 
        <<  fCentBin << fVtxBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";
    //=====================================
    //  associated pt fhistos with etaGaps XE bins
    //=====================================
    // xe bins
    fhDphiAssocXEbin
        << TH1D( "hDphiAssocXEbin", "",  bins, fLowRange, fHighRange) 
        <<  fTypBin << fCentBin << fEtaGapBin << fPTtBin << fXEBin  << "END";
    fTestHist
        << TH1D( "testHist","", 5 , 0 ,5 )
        <<fTypBin << fCentBin << fEtaGapBin << fPTtBin << fXEBin << "END";
    for( int ityp=0;ityp<fTypBin.Size();ityp++ ){
        for( int ic=0;ic<fCentBin.Size();ic++){
            for( int ie=0;ie<fEtaGapBin.Size();ie++ ){
                for( int it=0;it<fPTtBin.Size();it++ ){
                    for( int ix=0;ix<fXEBin.Size();ix++ ){
                        fTestHist[ityp][ic][ie][it][ix]->SetBinContent( 1, double(ityp) );
                        fTestHist[ityp][ic][ie][it][ix]->SetBinContent( 2, double(ic) );
                        fTestHist[ityp][ic][ie][it][ix]->SetBinContent( 3, double(ie) );
                        fTestHist[ityp][ic][ie][it][ix]->SetBinContent( 4, double(it) );
                        fTestHist[ityp][ic][ie][it][ix]->SetBinContent( 5, double(ix) );
                    }
                }
            }
        }
    }
    fhDEtaNearXEbin
        << TH1D( "hDEtaNearXEbin", "",  320, -2*fmaxEtaRange, 2*fmaxEtaRange) 
        <<  fCentBin << fVtxBin << fEtaGapBin << fPTtBin << fXEBin  << "END";
    fhDEtaNearMXEbin
        << TH1D( "hDEtaNearMXEbin", "",  320, -2*fmaxEtaRange, 2*fmaxEtaRange) 
        <<  fCentBin << fVtxBin << fEtaGapBin << fPTtBin << fXEBin  << "END";
    //=======================================
    //  associated fpt fhistos without etaGaps
    //=======================================
    fhDphiAssocIsolTrigg
        << TH1D( "hDphiAssocIsolTrigg", "",  bins, fLowRange, fHighRange) 
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";//FK//
    fhMeanPtAssoc
        << TProfile( "hMeanPtAssoc", "", bins, fLowRange, fHighRange) 
        <<  fCentBin << fPTtBin << fPTaBin  << "END";
    fhMeanZtAssoc
        << TProfile( "hMeanZtAssoc", "", bins, fLowRange, fHighRange) 
        <<  fCentBin << fPTtBin << fPTaBin  << "END";
    fhAssocPtBin
        << TH1D( "hAssocPtBin", "", (int)TMath::Ceil((pTa2-pTa1)/ptbw), pTa1, pTa2) 
        <<  fCentBin << fPTtBin << fPTaBin  << "END";
    fhIphiAssoc
        << TH1D( "fhIphiAssoc", "",  bins, -kJPi-0.1, kJPi+0.1) 
        <<  fCentBin << fPTaBin  << "END";
    fhIetaAssoc
        << TH1D( "hIetaAssoc", "",  80, -fmaxEtaRange, fmaxEtaRange) 
        <<  fCentBin << fPTaBin  << "END";
    fhDEtaFar
        << TH1D( "hDEtaFar", "",  120, -2*fmaxEtaRange, 2*fmaxEtaRange) 
        <<  fTypBin << fCentBin << fPTtBin  << "END";

    //==========================
    //UE fhistos
    //==========================
    fhPtAssocUE
        << TH1D( "hPtAssocUE", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fEtaGapBin << fPTtBin  << "END";
    fhPtaEtaGapN
        << TH1D( "hPtaEtaGapN", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fEtaGapBin << fPTtBin  << "END";
    fhPtaRGapN
        << TH1D( "hPtaRGapN", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fRGapBin << fPTtBin  << "END";
    fhPtAssocUEIsolTrigg
        << TH1D( "hPtAssocUEIsolTrigg", "", fnUE, fUEBinsx) 
        <<  fPTtBin  << "END";//FK//
    fhPtAssocN
        << TH1D( "hPtAssocN", "",  fnUE, fUEBinsx) 
        <<  fPTtBin  << "END";
    fhPtAssocF
        << TH1D( "hPtAssocF", "",  fnUE, fUEBinsx) 
        <<  fPTtBin  << "END";
  
    //======================
    // invariant mass histograms
    //======================

    fhInvariantMassXe 
        << TH1D("hInvariantMassXe","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fXEBin  << "END";

    fhInvariantMassKlong 
        << TH1D("hInvariantMassKlong","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fKLongBin  << "END";

    fhInvariantMassPta 
        << TH1D("hInvariantMassPta","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";

    // Like sign pairs for invariant mass histograms
    
    fhInvariantMassXeLikeSign 
        << TH1D("hInvariantMassXeLikeSign","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fXEBin  << "END";

    fhInvariantMassKlongLikeSign 
        << TH1D("hInvariantMassKlongLikeSign","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fKLongBin  << "END";

    fhInvariantMassPtaLikeSign
        << TH1D("hInvariantMassPtaLikeSign","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";

    // Unlike sign pairs for invariant mass histograms
    
    fhInvariantMassXeUnlikeSign 
        << TH1D("hInvariantMassXeUnlikeSign","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fXEBin  << "END";

    fhInvariantMassKlongUnlikeSign 
        << TH1D("hInvariantMassKlongUnlikeSign","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fKLongBin  << "END";

    fhInvariantMassPtaUnlikeSign
        << TH1D("hInvariantMassPtaUnlikeSign","",1500,0,3)
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";

    //=======================
    //jT fhistos
    //=======================
  
    int nJT = 100;
    double jtLow = 0.05, jtHigh = 20;
  
    double logBinsJt[101];
    double logJt = (log(jtHigh)-log(jtLow))/nJT;
    for(int ij=0;ij<=nJT;ij++) logBinsJt[ij]=jtLow*exp(ij*logJt);
  
    fhPoutF
        << TH1D( "hPoutF", "",  300, 0, 15) 
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";//FK//
    
    // Histograms in xlong bins
    
    fhJT
        << TH1D( "hJT", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fXEBin  << "END";

    fhJTBg
        << TH1D( "hJTBg", "",  nJT, logBinsJt)
        <<  fCentBin << fEtaGapBin << fPTtBin << fXEBin  << "END";

    fhJTBgR
        << TH1D( "hJTBgR", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fXEBin  << "END";

    fhJTLikeSign
        << TH1D( "hJTLikeSign", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fXEBin  << "END";

    fhJTBgLikeSign
        << TH1D( "hJTBgLikeSign", "",  nJT, logBinsJt)
        <<  fCentBin << fEtaGapBin << fPTtBin << fXEBin  << "END";

    fhJTBgRLikeSign
        << TH1D( "hJTBgRLikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fXEBin  << "END";

    fhJTUnlikeSign
        << TH1D( "hJTUnlikeSign", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fXEBin  << "END";

    fhJTBgUnlikeSign
        << TH1D( "hJTBgUnlikeSign", "",  nJT, logBinsJt)
        <<  fCentBin << fEtaGapBin << fPTtBin << fXEBin  << "END";

    fhJTBgRUnlikeSign
        << TH1D( "hJTBgRUnlikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fXEBin  << "END";

    fhBgAssocXe
        << TH1D( "hBgAssocXe", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fXEBin  << "END";

    fhBgAssocXeR
        << TH1D( "hBgAssocXeR", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fRGapBin << fPTtBin << fXEBin  << "END";

    fhDphiDetaXe
        << TH2D( "hDphiDetaXe", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, -kJPi/2, kJPi/2) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fXEBin  << "END";

    fhDphiDetaXeR
        << TH2D( "hDphiDetaXeR", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, -kJPi/2, kJPi/2) 
        <<  fCentBin << fRGapBin << fPTtBin << fXEBin  << "END";

    // Histograms in klong bins

    fhJTKlong
        << TH1D( "hJTKlong", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fKLongBin  << "END";
    
    fhJTKlongBg
        << TH1D( "hJTKlongBg", "",  nJT, logBinsJt) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fKLongBin  << "END";

    fhJTKlongBgR
        << TH1D( "hJTKlongBgR", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fKLongBin  << "END";

    fhJTKlongLikeSign
        << TH1D( "hJTKlongLikeSign", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fKLongBin  << "END";
    
    fhJTKlongBgLikeSign
        << TH1D( "hJTKlongBgLikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fKLongBin  << "END";

    fhJTKlongBgRLikeSign
        << TH1D( "hJTKlongBgRLikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fKLongBin  << "END";

    fhJTKlongUnlikeSign
        << TH1D( "hJTKlongUnlikeSign", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fKLongBin  << "END";
    
    fhJTKlongBgUnlikeSign
        << TH1D( "hJTKlongBgUnlikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fKLongBin  << "END";

    fhJTKlongBgRUnlikeSign
        << TH1D( "hJTKlongBgRUnlikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fKLongBin  << "END";

    fhBgAssocKlong
        << TH1D( "hBgAssocKlong", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fKLongBin  << "END";

    fhBgAssocKlongR
        << TH1D( "hBgAssocKlongR", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fRGapBin << fPTtBin << fKLongBin  << "END";

    fhDphiDetaKlong
        << TH2D( "hDphiDetaKlong", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, -kJPi/2, kJPi/2) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fKLongBin  << "END";

    fhDphiDetaKlongR
        << TH2D( "hDphiDetaKlongR", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, -kJPi/2, kJPi/2) 
        <<  fCentBin << fRGapBin << fPTtBin << fKLongBin  << "END";

    // Histograms in pta bins

    fhJTPta
        << TH1D( "hJTPta", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaBg
        << TH1D( "hJTPtaBg", "",  nJT, logBinsJt) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaBgR
        << TH1D( "hJTPtaBgR", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaLikeSign
        << TH1D( "hJTPtaLikeSign", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaBgLikeSign
        << TH1D( "hJTPtaBgLikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaBgRLikeSign
        << TH1D( "hJTPtaBgRLikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaUnlikeSign
        << TH1D( "hJTPtaUnlikeSign", "",  nJT, logBinsJt) 
        <<  fTypBin << fCentBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaBgUnlikeSign
        << TH1D( "hJTPtaBgUnlikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";

    fhJTPtaBgRUnlikeSign
        << TH1D( "hJTPtaBgRUnlikeSign", "",  nJT, logBinsJt) 
        <<  fCentBin << fRGapBin << fPTtBin << fPTaBin  << "END";

    fhBgAssocPta
        << TH1D( "hBgAssocPta", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";

    fhBgAssocPtaR
        << TH1D( "hBgAssocPtaR", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fRGapBin << fPTtBin << fPTaBin  << "END";

    fhDphiDetaPta
        << TH2D( "hDphiDetaPta", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, -kJPi/2, kJPi/2) 
        <<  fCentBin << fEtaGapBin << fPTtBin << fPTaBin  << "END";

    fhDphiDetaPtaR
        << TH2D( "hDphiDetaPtaR", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, -kJPi/2, kJPi/2) 
        <<  fCentBin << fRGapBin << fPTtBin << fPTaBin  << "END";
}


void AliJHistos::CreateIAAMoons()
{
    fHMG->cd();
    //--- IAA signal ---
    fhDRNearPt
        << TH1D( "hDRNearPt", "",  fnUE, fUEBinsx) 
        <<  fTypBin << fCentBin << fVtxBin << fRGapBin << fPTtBin  << "END";
    fhDRFarPt
        << TH1D( "hDRFarPt", "",  fnUEfar, fUEBinsxFar) 
        <<  fTypBin << fCentBin << fVtxBin << fRGapBin << fPTtBin  << "END";

    // --- Moons ---
    fhDRNearPtMoon
        << TH1D( "hDRNearPtMoon", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fVtxBin << fRGapBin << fPhiGapBin << fPTtBin  << "END";
    fhDRNearPtMoonM
        << TH1D( "hDRNearPtMoonM", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fVtxBin << fRGapBin << fPhiGapBin << fPTtBin  << "END";

    fhDRFarPtMoon
        << TH1D( "hDRFarPtMoon", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fVtxBin << fRGapBin << fPhiGapBin << fPTtBin  << "END";
    fhDRFarPtMoonM
        << TH1D( "hDRFarPtMoonM", "",  fnUE, fUEBinsx) 
        <<  fCentBin << fVtxBin << fRGapBin << fPhiGapBin << fPTtBin  << "END";

    //==========================
    // 2D fhistos 
    //==========================
    fhDphiAssoc2DIAA
        << TH2D( "hDphiAssoc2DIAA", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, fLowRange, fHighRange) 
        <<  fTypBin << fCentBin << fVtxBin << fPTtBin << fPTaBin  << "END";
    fhDphiAssoc2D
        << TH2D( "hDphiAssoc2D", "",  100, -2*fmaxEtaRange, 2*fmaxEtaRange, 100, fLowRange, fHighRange) 
        <<  fRGapBin << fPhiGapBin  << "END";
}

void AliJHistos::CreateXEHistos(){
    //==================================
    //  xe slopes
    //==================================
    fHMG->cd();
    double xel=0.0, xeh=1.2;
    int nbxE = int((xeh-xel)/0.04);

    fhxEPtBin
        << TH1D( "hxEPtBin", "",  nbxE, xel, xeh) 
        <<  fTypBin3 << fPTtBin << fPTaBin  << "END";
    fhxEF 
        << TH1D( "hxEF", "", nbxE, xel, xeh)
        << fTypBin << fPTtBin <<"END";
    fhxEFIsolTrigg
        << TH1D( "hxEFIsoTrigg", "", nbxE, xel, xeh)
        << fTypBin << fPTtBin << "END";
    fhxEN 
        << TH1D("hxEN", "", nbxE, xel, xeh)
        << fTypBin << fPTtBin << "END";

}

void AliJHistos::CreatePairPtCosThetaStar(){
    // pairs
    fHMG->cd();
    int    bins = 288; // 12*24    -1/3 and  0.5 and 5/3  are bin edges 
    double lowRange = -1./3, highRange= 5./3;
    //=================
    //pairPT
    //=================
    fhPairPtDphi
        << TH1D( "hPairPtDphi", "",  bins, lowRange, highRange) 
        <<  fTypBin << fPTtBin << fPTaBin  << "END";
    fhPairPt
        << TH1D( "hPairPt", "",  210, 0, 70) 
        <<  fTypBin << fPTtBin << fPTaBin  << "END";
    //==================================
    //  CosThetaStar fhistos 
    //==================================
    //cout<<"ippt="<<fPairPtBin<<" "<<fCard->IsLessThanUpperPairPtCut(-fPairPtBin)<<endl;
    fhCosThetaStar
        << TH1D( "hCosThetaStar", "",  100, 0, 1) 
        <<  fTypBin << fPairPtBin << fMassBin  << "END";
    fhCMSrap
        << TH2D( "hCMSrap", "",  100, 0, 1, 50, -1, 1) 
        <<  fPairPtBin << fMassBin  << "END";
    fhInvMass
        << TH1D( "hInvMass", "",  200, 0, 40) 
        <<  fPairPtBin << "END";

    fhPairPtMass
        << TH1D( "hPairPtMass", "",  250, 0, 50) 
        <<  fMassBin  << "END";
    fhPairDPhi
        << TH1D( "hPairDPhi", "",   bins, lowRange, highRange) 
        <<  fMassBin  << "END";
    fhPairDpT
        << TH1D( "hPairDpT", "",  150, 0-25./150./2., 25-25./150./2.) 
        <<  fMassBin  << "END";
    fpCMSrap
        << TProfile("pCMSrap","no pair pT cut",100,-1,1) 
        << "END";
}

//______________________________________________________________________________
void AliJHistos::CreatePtCorrHistos(){
    // pt corr histos
    fHMG->cd();
    int ptbins=30;
    double lpt=0,upt=8;
    fhPtNear
        << TH1D( "hPtNear", "",  ptbins, lpt, upt) 
        <<  fTypBin << 3 << fCentBin  << "END";
    fhPtFar
        << TH1D( "hPtFar", "",  ptbins, lpt, upt) 
        <<  fTypBin << 3 << fCentBin  << "END";
}

//______________________________________________________________________________
void AliJHistos::CreateRunByRunHistos(int runID, int runcounter) const {
  // Todo
  fHMG->cd();
  JUNUSED(runID);
  JUNUSED(runcounter);
} //TODO


//______________________________________________________________________________
void AliJHistos::CreateEventTrackHistos(){
  // comment needed
  fHMG->cd();
    int nBINS=150;
    double logBinsX[nBINS+1], limL=0.1, limH=100;
    double logBW = (log(limH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);

    fhLPpt       <<  TH1D("hLPpt","LP pt", nBINS, logBinsX ) << "END";
    fhLPpairPt   << TH1D("hLPpairPt","LP pair pt", nBINS, logBinsX )<< "END";
    fhChargedEta << TH1D("hChargedEta","All eta",100,-1.0,1.0)<< "END";
    fhLPeta      << TH1D("hLPeta","LP eta",100,-1.0,1.0)<< "END";
    fhIsolatedLPpt << TH1D("hIsolatedLPpt","Isolated LP pt", nBINS, logBinsX )<< "END";
    fhChargedPtFiete << TH1D("hChargedPtFiete", "Jan Fiete bins", fNJanFiete, fJanFiete )<< "END";


    fhAssocMult << TH1D("hAssocMult","Assoc  mlt",100,-0.5,99.5)<< "END";

    fhChargedMult 
        << TH1D("hChargedMult","", 300, 0., 3500.)
        << fCentBin << "END"; 
    fhChargedMultCut 
        << TH1D("hChargedMultCut","",  300, 0., 3500.)
        << fCentBin << "END"; 
    fhZVert 
        << TH1D("hZVert", "", 100, -30., 30.)
        << fCentBin << "END";
    fhChargedPt 
        << TH1D("hChargedPt","", nBINS, logBinsX )
        << fCentBin << "END";
    fhChargedPtNoCorr 
        << TH1D("hChargedPtNoCorr","", nBINS, logBinsX )
        << fCentBin << "END";
    fhChargedPtJacek 
        << TH1D("hChargedPtJacek","", fNJacek, fPttJacek )
        << fCentBin << "END";
    fhChargedPtJacekEta
        << TH1D("hChargedPtJacekEta","", fNJacek, fPttJacek )
        << fCentBin << 3 << "END";

    fhVdelta2 << TH1D("hVdelta2","", 100, -0.2, 5)
        << fCentBin << "END";
    fhVdelta3 << TH1D("hVdelta3","", 100, -0.2, 5)
        << fCentBin << "END";
    fhVN << TH1D("hVN","", 100, -kJPi, kJPi)
        << fCentBin << "END";
    fhTrackingEfficiency << TProfile("hTrackingEff","",fNJacek, fPttJacek)
        << fCentBin << "END";
    fhV0AMult << TH1D("hV0Mult","", 2000,0,2000 )
        << fCentBin << "END";
    fpV2 << TProfile("pV2", "v2 with cent", 50, 0, 90) << "END";
    fpV3 << TProfile("pV3", "v3 with cent", 50, 0, 90) << "END";
    fpVdeltaNorm << TProfile("pVdeltaNorm", "mult with cent", 50, 0, 90) << "END";
    fhChargedMultCent << TH2D("hChargedMultCent ", "log(fmult) vs cent", 100, 0, 90, 100, 1, 10) << "END";
    fhZVertRaw << TH1D("hZVertRaw","vertex 0", 120, -30., 30.) << "END";
    fhZVertRawErr << TH1D("hZVertRawErr","vertex 0 Err", 100, 0, 0.1) << "END";
    fhCentr << TH1D("hCentr","centrality", 101, -0.5, 100.5) << "END";
    fhiCentr << TH1D("hiCentr","centrality",10, -0.5, 9.5) << "END";
    fhEventPerRun << TH1D("hEventPerRun","log(eve)/run",200, 0, 30.0) << "END";
    fhBkgActivity 
        << TH1D("hBkgActivity", "", 200, 0, 20) 
        << fPTtBin <<"END";

    //------------------ for Abs Norm FK --------------------------------
    double   binsVertexMult[] = {0,1,2,3,4,5,10000};
    int   nbinsVertexMult  = sizeof(binsVertexMult)/sizeof(double)-1;
    //double binsVertexZ[]    = {-10,-5,-2,0,2,5,10};
    double binsVertexZ[]    = {-10,-6,-3,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,3,6,10};
    int   nbinsVertexZ   = sizeof(binsVertexZ)/sizeof(double)-1;
    fhVertexZTriggVtx << TH2D("hVertexZTriggVtx","Vertex counts", nbinsVertexMult, binsVertexMult, nbinsVertexZ, binsVertexZ) << "END";

    // event counter
    fhEvents        
        << TH1D("hEvents","events passing cuts", 100, -0.5, 100-0.5 ) << "END";
    fhEventTrigger  
        << TH1D("hEventTrigger","Trigger count", 50, -0.5, 50.-.5 )
        << "END";
    fhTrackSelection
        << TH1D("hTrackSelection","checking bit convention", 100, -0.5, 100-0.5) << "END";
    // TODO fhEvents->SetXTitle( "0 - all, 1 - SDD selected, 2 - has vertex, 3 - good vertex, 4 - trigger + MB, 5 - trigger, 6 - BX, 7 - centrality" );
}

void AliJHistos::CreateJetHistos(){
    fHMG->cd();
    // jet histos
    int nBINS=200;
    double logBinsX[nBINS+1], limL=0.1, limH=200;
    double logBW = (log(limH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);

    fhLPJetPt
        << TH1D( "hLPJetPt", "",   nBINS, logBinsX) 
        <<  fCentBin  << "END";
    fhJetPt
        << TH1D( "hJetPt", "",   nBINS, logBinsX) 
        <<  fCentBin  << "END";
    fhLeadingJetPt
        << TH1D( "hLeadingJetPt", "",   nBINS, logBinsX) 
        <<  fCentBin  << "END";
    fhLeadingJetWLPPt
        << TH1D( "hLeadingJetWLPPt", "",   nBINS, logBinsX) 
        <<  fCentBin  << "END";
    fhLPJetPTt
        << TH1D( "hLPJetPTt", "",   nBINS, logBinsX) 
        <<  fCentBin << fPTtBin  << "END";
    fhLeadingJetWLPPTt
        << TH1D( "hLeadingJetWLPPTt", "",   nBINS, logBinsX) 
        <<  fCentBin << fPTtBin  << "END";
    fhLPJetEtaPTt
        << TH1D( "hLPJetEtaPTt", "",  100, -1, 1) 
        <<  fCentBin << fPTtBin  << "END";
    fhLPJetRapidityPTt
        << TH1D( "hLPJetRapidityPTt", "",  100, -1, 1) 
        <<  fCentBin << fPTtBin  << "END";
    fhLPJetMassPTt
        << TH1D( "hLPJetMassPTt", "",   nBINS, logBinsX) 
        <<  fCentBin << fPTtBin  << "END";
    fhDphiLPJet
        << TH1D( "hDphiLPJet", "",  160, -kJPi/2., kJPi/2.) 
        <<  fCentBin << fPTtBin  << "END";
    fhDEtaLPJet
        << TH1D( "hDEtaLPJet", "",   240, -1.2, 1.2) 
        <<  fCentBin << fPTtBin  << "END";
    // LP jet vs Leading Jet
    fhDPtLPJet
        << TH1D( "hDPtLPJet", "",  100, 0, 10.) 
        <<  fCentBin << fPTtBin  << "END";
    //==================================
    fhJetDphi << TH1D( "hJetDphi", "#Delta#phi", 100,-TMath::Pi(), TMath::Pi())
        << fTypBin << fPTtBin <<"END";
    fhJetDeta << TH1D("hJetDeta","#Delta#eta ",100,-1, 1)
        << fTypBin << fPTtBin <<"END";
    fhJetMultPt << TH1D("hJetMultPt","Jet Multiplicity ",100,0-0.5, 100-0.5)
        << fPTtBin <<"END";
    fhJetRho << TH1D("hJetRho","#rho",200,0, 50)
        << fPTtBin <<"END";
    fhJetRhoSigma << TH1D("hJetRhoSigma","#sigma",200,0, 50)
        << fPTtBin <<"END";
    // How many particles in a jet
    fhJetPartMult << TH1D("hJetPartMult","Number of consituents",100, 0-0.5, 100-0.5)
        << fPTtBin <<"END";
    fhRecoDiJetM
        << TH1D("hRecoDiJetM", "Invariant Mass", 201,-0.5, 200 ) 
        << "END";
    fhJetUEPt
        << TH1D("hJetUEPt","UE particles p_{T} by Jet substract",nBINS,logBinsX) 
        << "END";
    // pytfPTaBin
    fhNParton71
        << TH1D("hNParton71", "hNParton71", 100, -0.5, 100-0.5) 
        <<"END";
    fhNStringGroup
        << TH1D("hNStringGroup", "hNStringGroup", 100, -0.5, 100-0.5) 
        <<"END";
    fhNStringGroupFrom << TH1D("hNStringGroupFrom", "hNStringGroupFrom", 100, -0.5, 100-0.5) << 2 <<"END";
    fhNTracksInStringGroupFrom << TH1D("hNTracksInStringGroupFrom", "hNTracksInStringGroupFrom", 100, -0.5, 100-0.5) << 2 << "END";
    fhRapidity71From << TH1D("hRapidity71From", "hRapidity71From", 100, -8, 8 ) << 2 << "END";
    fhPt71From
        << TH1D("hPt71From", "hPt71From", 1000, 0, 100) 
        << 2 << "END";

}

//______________________________________________________________________________
void AliJHistos::CreateXtHistos() {
    // TODO comment
    //
    fHMG->cd();
    // Esko
    TH1::SetDefaultSumw2(kTRUE);
    cout << "GetDefaultSumw2() = " << TH1::GetDefaultSumw2() << endl;

    // xT binning
    int nBinsXt=200;
    double logBinsXt[nBinsXt+1];
    double xTLimL = 1e-5, xTLimH = 1.0, xTlogBW = (log(xTLimH)-log(xTLimL))/nBinsXt;
    for(int ij=0;ij<=nBinsXt;ij++) logBinsXt[ij]=xTLimL*exp(ij*xTlogBW);


    // pT binning
    int nBinsPt=200;
    double logBinsPt[nBinsPt+1];
    double pTLimL=0.1, pTLimH=200 , pTlogBW = (log(pTLimH)-log(pTLimL))/nBinsPt;
    for(int ij=0;ij<=nBinsPt;ij++) logBinsPt[ij]=pTLimL*exp(ij*pTlogBW);

    fhConeActivity << TProfile("hActivity", "Mean activity inside cone", nBinsPt, logBinsPt );
    fhPerpConeActivity << TProfile("hPerpActivity", "Mean activity inside perpendicular cone", nBinsPt, logBinsPt );
    fhConeActivityIsolated << TProfile("hActivityIsolated", "Mean pion activity inside cone isolated", nBinsPt, logBinsPt );

    fhPerpConeActivityIsolated << TProfile("hPerpActivityIsolated", "Mean activity inside perpendicular cone", nBinsPt, logBinsPt );
    fhPtForXt 
        << TH1D("hPtForXt","", fNJacek, fPttJacek )
        << 3 << fCentBin <<"END";

    fhXt 
        << TH1D("hXt", "Charged xT", nBinsXt, logBinsXt )
        << 3 << fCentBin <<"END";

    fhXtWeighted 
        << TH1D("hXtWeighted", "Charged xT", nBinsXt, logBinsXt )
        << 3 << fCentBin <<"END";

    fhXtWeightedHT 
        << TH1D("hXtWeightedHT", "", nBinsXt, logBinsXt )
        << 3 << fCentBin <<"END";
}

//______________________________________________________________________________
//void AliJHistos::ReadInclusiveHistos(TFile *inFile){
void AliJHistos::ReadInclusiveHistos(const char *inclusFileName){
	// read inclusive histos
	fHMG->cd();

	TPMERegexp sep("::");
	int ncol = sep.Split( inclusFileName );
    TString filename = sep[0];

	if (TString(inclusFileName).BeginsWith("alien:"))  TGrid::Connect("alien:");
	TFile *inclusFile = TFile::Open(filename);
    TDirectory * dir =  (TDirectory*) inclusFile;
    if( ncol > 1 ) dir = (TDirectory*)( inclusFile->Get(sep[1]));
    if( !dir ) {
		cout << " ReadInclusiveHistos wrong file name or dirname !!!!" << endl;
        return;
	}

     cout<<inclusFileName<<"\t"<<filename<<"\t";
     if( ncol > 1 ) cout<<sep[1];
	 cout<<endl;
	 dir->Print();

	 dir->cd();

	 fHmgInclusive = new AliJHistManager("hst");
	 fHmgInclusive->LoadConfig();

	 fhIetaTriggFromFile = fHmgInclusive->GetTH1D("hIetaTrigg");
	 fhIetaTriggFromFile.Print();
	 fhIetaTriggFromFile[0][0]->Print();

	 fhIphiTriggFromFile = fHmgInclusive->GetTH1D("fhIphiTrigg"); // TODO
	 fhIphiTriggFromFile.Print();
	 fhIetaAssocFromFile = fHmgInclusive->GetTH1D("hIetaAssoc");
	 fhIetaAssocFromFile.Print();
	 fhIphiAssocFromFile = fHmgInclusive->GetTH1D("fhIphiAssoc");
	 fhIphiAssocFromFile.Print();

	 int numCent = fCard->GetNoOfBins(kCentrType);
	 int numPtt  = fCard->GetNoOfBins(kTriggType);
	 int numPta  = fCard->GetNoOfBins(kAssocType);
	 int numEtaGaps = fCard->GetNoOfBins(kEtaGapType);
	 int numZvtx = fCard->GetNoOfBins(kZVertType);

	 //------------ R e a d   mixed  D a t a ------------    
	 const int zFirstBin = 0 ;
	 const int etaGapFirstBin = 0 ;
	 AliJTH1D hDEtaNearTmp = fHmgInclusive->GetTH1D("hDEtaNear");
	 for (int hic = 0;hic < numCent; hic++) {
		 for (int hit = 0; hit < numPtt;hit++){
			 for (int hia = 0; hia < numPta; hia++){
				 fhDEtaNearMixFromFile[hic][hit][hia]= hDEtaNearTmp[hic][zFirstBin][etaGapFirstBin][hit][hia]; 
				 for (int iEtaGap=0; iEtaGap < numEtaGaps; iEtaGap++){//fdphi slices 
					 for (int hiz = 0; hiz < numZvtx; hiz++) {
						 if( iEtaGap==etaGapFirstBin && hiz==zFirstBin ) continue;
						 TH1D *hid = hDEtaNearTmp[hic][hiz][iEtaGap][hit][hia];
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


	 //for (int hic = 0;hic < fCard->GetNoOfBins(kCentrType);hic++) {
	 //   for (int hit = 0; hit < fCard->GetNoOfBins(kTriggType);hit++){
	 //      for (int hia = 0; hia < fCard->GetNoOfBins(kAssocType); hia++){
	 //         hDphiAssocMixSpectraFile[hic][hit][hia]= (TH1D*) inclusFile->Get(Form("xhDphiAssoc%02dC%02dE00T%02dA%02d",1, hic, hit, hia));//FK//mix2
	 //      }
	 //   }
	 //}
}
