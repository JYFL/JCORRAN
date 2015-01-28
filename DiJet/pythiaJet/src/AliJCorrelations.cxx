#include <TObjArray.h>
#include "AliJCorrelations.h"

#include "AliJCorrelations.h"
#include "AliJHistos.h"
#include "AliJBaseTrack.h"
#include "AliJCard.h"
#include "AliJPiZero.h"
#include "TProfile.h"
#include "AliJHistos.h"

#include "JParticleTools.h"
#include "JPythiaHistos.h"


//==========================================================================
// don't count the trigger here ! You'll miss the not associated triggers
// but you  will miss them in the main code too, unles you loop over those
// fevent with no cgl (set the masMix to 1)
//==========================================================================
// blah

AliJCorrelations::AliJCorrelations( AliJCard *cardIn, AliJHistos *histosIn):
    fPTool(0x0)
{
    // constructor
    fcard   = cardIn;
    fhistos = histosIn;
    fcoPiZero  = new AliJPiZero();   //local instant copied to pi0 TClonesArray
    fcoPiZero->ResetToZero();

    float xbin[20]={0};
    for(int i=0;i<=fcard->GetNoOfBins(kCentrType);i++) xbin[i]=fcard->GetBinBorder(kCentrType,i);
    fhMixStat = new TProfile("hMixStat","Mult vs cent", fcard->GetNoOfBins(kCentrType), xbin );
    fnReal=0;
    fnMix=0;
    fsumTriggerAndAssoc = int( fcard->Get("sumTriggerAndAssoc") );
    fmaxEtaRange    = fcard->Get("EtaRange");
    fRSignalBin = int(fcard->Get("EtaGapSignalBin"));

    fsamplingMethod = 0; //flat by default
    fDPhiUERegion[0]    = fcard->Get("DPhiUERegion",0);
    fDPhiUERegion[1]    = fcard->Get("DPhiUERegion",1);
    cout << fmaxEtaRange <<" fDPhiUERegion[0]="<< fDPhiUERegion[0] <<" fDPhiUERegion[1]="<< fDPhiUERegion[1] <<endl;
    fIsHeavyIon = 0;// TODO AliJRunTable::GetInstance().IsHeavyIon();
    fawayPhiGap = fcard->GetBinBorder(kEtaGapType, 2);

    for(int iRGap=0; iRGap < fcard->GetNoOfBins(kEtaGapType); iRGap++)
        fRGap[iRGap]   = fcard->GetBinBorder( kEtaGapType, iRGap);

    //dPhiRange = fhistos->GetDphiRange();
    frandom = new TRandom3(); //FK// frandom generator for kperp flow UE
    frandom->SetSeed(0); //FK//

    // load fast fhistos
    fPi0PtRange[0] = fcard->Get("pi0PtRange",0);
    fPi0PtRange[1] = fcard->Get("pi0PtRange",1);
    fPi0MassRange[0] = fcard->Get("pi0MassRange",0);
    fPi0MassRange[1] = fcard->Get("pi0MassRange",1);
    fPi0AsymmetryRange[0] = fcard->Get("pi0AsymRange",0);
    fPi0AsymmetryRange[1] = fcard->Get("pi0AsymRange",1);
    fPi0UseSM = fcard->Get("pi0UseSM");
    fPi0ConvDelta = fcard->Get("pi0ConvDelta");

    if( fcard->Get("pi0DoSMCut",0) == 1 )
        fPi0DoSMCut = kTRUE;
    else
        fPi0DoSMCut = kFALSE;
}

AliJCorrelations::AliJCorrelations(const AliJCorrelations& obj){
    // copy constructor
}
AliJCorrelations& AliJCorrelations::operator=(const AliJCorrelations& obj){
    return *this;
    // copy constructor
}


void AliJCorrelations::FillHisto(corrFillType cFTyp, fillType fTyp,    int cBin, int zBin, AliJBaseTrack *ftk1, AliJBaseTrack *ftk2){
    // histo filler
    if( cFTyp == kAzimuthFill )
        FillAzimuthHistos( fTyp, cBin, zBin, ftk1, ftk2);

}

//=============================================================================================
void AliJCorrelations::FillAzimuthHistos(fillType fTyp, int CentBin, int ZBin, AliJBaseTrack *ftk1, AliJBaseTrack *ftk2)
    //=============================================================================================
{
    // histo filler
    bool twoTracks = false;
    if(ftk1->GetParticleType()==kJHadron && ftk2->GetParticleType()==kJHadron) twoTracks =true;

    //double-counting check
    if(fTyp == kReal && twoTracks && ftk1->GetID()==ftk2->GetID()) return;

    //==== JET
    AliJBaseTrack * jet1 = fPTool->GetJetFinder()->GetMotherJet( ftk1 );
    AliJBaseTrack * jet2 = fPTool->GetJetFinder()->GetMotherJet( ftk2 );

    //----------------------------------------------------------------
    double ptt = ftk1->Pt();
    double pta = ftk2->Pt();


    double trackPairEff = 1./( ftk1->GetTrackEff() * ftk2->GetTrackEff() ); 

    bool bIsolatedTrigger =  ftk1->GetIsIsolated()>0  ? true : false; //FK// trigger particle is isolated hadron

    //phit= ftk1->Phi();                                 //for RP
    int pTtBin       = ftk1->GetTriggBin();
    int pTaBin       = ftk2->GetAssocBin();
    double tphi      = ftk1->Phi();
    double aphi      = ftk2->Phi();
    double fdphi      = DeltaPhi(tphi, aphi);  //radians
    double dphipipi = atan2(sin(tphi-aphi), cos(tphi-aphi));
    double dEta      = ftk1->Eta() - ftk2->Eta();
    double fdEta     = fabs(dEta); 
    //double dEtaFar   = ftk1->Eta() + ftk2->Eta();

    double xe        = -pta*cos(tphi-aphi)/ptt;
    double pout      = fabs(pta*sin(tphi-aphi));
    bool   bReal     = fTyp==kReal;
    bool   bNear     = cos(tphi-aphi) > 0 ? true : false;

    int etaGapBin = fcard->GetBin( kEtaGapType, fabs(dEta));

    int phiNearGapBin = fcard->GetBin( kEtaGapType, fabs(dphipipi) );
    int rNearGapBin   = fcard->GetBin( kEtaGapType, fabs(ftk1->DeltaR(*ftk2)));
    //here the angle must be 0-2pi and not (-pi,pi)
    int phiFarGapBin = fcard->GetBin( kEtaGapType, fabs(fdphi-kJPi) );
    int rFarGapBin   = fcard->GetBin( kEtaGapType, sqrt(pow(fdphi-kJPi,2)+dEta*dEta) );

    //if( rGapBin != rNearGapBin ) cout<<"dR vs rNearGapBin = "<<rGapBin<<"\t"<<rNearGapBin<<endl;
    //if( rGapBin != rFarGapBin ) cout<<"dR vs rFarGapBin = "<<rGapBin<<"\t"<<rFarGapBin<<endl;
    //----------------------------------------------------------------

    //acceptance correction  triangle  or mixed fevent
    //double geoAccCorr = 1; 
    double geoAccCorr = ( fsamplingMethod == 0 ) ? GetGeoAccCorrFlat(dEta) : GetGeoAccCorrIncl(dEta, CentBin, pTtBin, pTaBin);

    if(pTtBin<0 || pTaBin<0 || etaGapBin<0 ){
        cout<<"Error in FillAzimuthHistos: some pT or eta out of bin. pTtBin="<<pTtBin<<" pTaBin="<<pTaBin <<" etaGapBin="<< etaGapBin << endl;
        ftk1->Print();
        ftk2->Print();
        exit(-1);
        //return;
    }
    if(fdphi==0) cout <<" fdphi=0; ptt="<<  ptt<<"   pta="<<pta<<"  TID="<<ftk1->GetID()<<"  AID="<<ftk2->GetID() <<" tphi="<< tphi <<" aphi="<< aphi << endl;


    // ===================================================================
    // =======  Fill Histograms                              =============
    // ===================================================================

    // ----- fdphi Correlation Functioni ---------------------------------------
    //for(int iEtaGap=0; iEtaGap<=etaGapBin; iEtaGap++) 
    // When hists are filled for thresholds they are not properly normalized and need to be subtracted
    // This induced improper errors - subtraction of not-independent entries


    //=======================================
    //======      PYTHIA             ========
    //=======================================
    /*  Removed by BS 
    if( jet1 && jet2 && fTyp == kReal){

        fhistos->fhDphiAssoc[fTyp][CentBin][etaGapBin][pTtBin][pTaBin]->Fill( fdphi/kJPi , geoAccCorr * trackPairEff);

        TLorentzVector netPair = (*jet1)+(*jet2);
        double kT = netPair.Pt()*sqrt(2/TMath::Pi());            //should be x-checked
        fPyHisto->hPyKT[0][pTtBin][pTaBin]->Fill(kT, kT>0 ? 1/kT : 1);
        //calculate zt as pTtrigger/pTjet1
        //calculate za as pTassoc/pTjet2
        double zt = ftk1->Pt() / jet1->Pt();
        fPyHisto->hPyZt[0][pTtBin][pTaBin]->Fill(zt); //0<z<2 100 bins
        double za = ftk2->Pt() / jet2->Pt();
        fPyHisto->hPyZa[0][pTtBin][pTaBin]->Fill(za); //0<z<2 100 bins
        double xhHat = jet2->Pt()/jet1->Pt();
        fPyHisto->hPyXhHat[0][pTtBin][pTaBin]->Fill(xhHat);
        //please add these....
        fPyHisto->hPyJetTriggPt[0][pTtBin][pTaBin]->Fill(jet1->Pt()); //jet1 is the trigger jet right?
        fPyHisto->hPyJetAssocPt[0][pTtBin][pTaBin]->Fill(jet2->Pt()); //jet2 is the assoc jet right?
        double JetPout      = fabs(jet2->Pt()*sin(jet1->Phi()-jet2->Phi())); // 
        fPyHisto->hPyJetPout[0][pTtBin][pTaBin]->Fill(JetPout);

        TObjArray & dijets = *(fPTool->GetJetFinder()->GetDiJetList());
        if( 0 && dijets.GetEntries() > 1
                && jet1 == dijets[0] && jet2 == dijets[1] )
        {
            fPyHisto->hPyKT[1][pTtBin][pTaBin]->Fill(kT, kT>0 ? 1/kT : 1);
            fPyHisto->hPyZt[1][pTtBin][pTaBin]->Fill(zt); //0<z<2 100 bins
            fPyHisto->hPyZa[1][pTtBin][pTaBin]->Fill(za); //0<z<2 100 bins
            fPyHisto->hPyXhHat[1][pTtBin][pTaBin]->Fill(xhHat);
            fPyHisto->hPyJetTriggPt[1][pTtBin][pTaBin]->Fill(jet1->Pt()); //jet1 is the trigger jet right?
            fPyHisto->hPyJetAssocPt[1][pTtBin][pTaBin]->Fill(jet2->Pt()); //jet2 is the assoc jet right?
            fPyHisto->hPyJetPout[1][pTtBin][pTaBin]->Fill(JetPout);

            }
            }else{
            */
    {
        fhistos->fhDphiAssoc[1][CentBin][etaGapBin][pTtBin][pTaBin]->Fill( fdphi/kJPi , geoAccCorr * trackPairEff);
    }

    //=======================================
    if(bIsolatedTrigger) fhistos->fhDphiAssocIsolTrigg[fTyp][CentBin][pTtBin][pTaBin]->Fill( fdphi/kJPi , geoAccCorr * trackPairEff); //FK//
    //-------------------------------------------------------------------------

    // ----- Eta Width -----
    //xxx
    //     if( bNear ) //one could check the phiGapBin, but in the pi/2 <1.6 and thus phiGap is always>-1
    //         fhistos->hDEtaNear[fTyp][CentBin][ZBin][phiNearGapBin][pTtBin][pTaBin]->Fill( dEta , geoAccCorr * trackPairEff );
    if( bNear ){ //one could check the phiGapBin, but in the pi/2 <1.6 and thus phiGap is always>-1
        if( fTyp == 0 )
            fhistos->fhDEtaNear[CentBin][ZBin][phiNearGapBin][pTtBin][pTaBin]->Fill( dEta , geoAccCorr * trackPairEff );
        else
            fhistos->fhDEtaNearM[CentBin][ZBin][phiNearGapBin][pTtBin][pTaBin]->Fill( dEta , geoAccCorr * trackPairEff );
    }
    else  if(phiFarGapBin<=3) 
        fhistos->fhDEtaFar[fTyp][CentBin][pTtBin]->Fill( dEta, geoAccCorr * trackPairEff ); 

    // ----- IAA fhistos ------
    fhistos->fhDphiAssoc2DIAA[fTyp][CentBin][ZBin][pTtBin][pTaBin]->Fill( dEta, fdphi/kJPi, trackPairEff);

    if(rNearGapBin>=0){
        if(rNearGapBin <= fRSignalBin) fhistos->fhDRNearPt[fTyp][CentBin][ZBin][rNearGapBin][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );
        // - moon -
        if(rNearGapBin>0){
            for( int irs = 0; irs <= fRSignalBin;irs++ ){
                if( rNearGapBin < irs ) continue;
                if( phiNearGapBin > irs ) continue;
                for ( int ir1=0;ir1<= rNearGapBin;ir1++ ){
                    if( irs > ir1 ) continue;
                    if( ir1 > rNearGapBin ) continue;
                    double rGapS= fRGap[irs+1];
                    double rGap1= fRGap[ir1+1];
                    if( rGap1 < TMath::Abs(dphipipi) ) continue;
                    if( rGapS < TMath::Abs(dphipipi) ) continue;
                    double dEtaMin = sqrt(rGap1*rGap1-dphipipi*dphipipi);
                    double dEtaMax = dEtaMin + sqrt(rGapS*rGapS-dphipipi*dphipipi);
                    double eta = TMath::Abs( fdEta );
                    //double eta = fdEta ;
                    if( eta > dEtaMin && eta < dEtaMax ){
                        if( fTyp == 0 )
                            fhistos->fhDRNearPtMoon[CentBin][ZBin][ir1][irs][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );
                        else
                            fhistos->fhDRNearPtMoonM[CentBin][ZBin][ir1][irs][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );
                        if(bReal)       fhistos->fhDphiAssoc2D[ir1][irs]->Fill( dEta, fdphi/kJPi, geoAccCorr * trackPairEff );
                    }
                }
            }
        }
    }

    if(rFarGapBin>=0){
        if(rFarGapBin <= fRSignalBin) fhistos->fhDRFarPt[fTyp][CentBin][ZBin][rFarGapBin][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );
        // - moon -
        if(rFarGapBin>0){
            for( int irs = 0; irs <= fRSignalBin;irs++ ){
                if( rFarGapBin < irs ) continue;
                if( phiFarGapBin > irs ) continue;
                for ( int ir1=0;ir1<= rFarGapBin;ir1++ ){
                    if( irs > ir1 ) continue;
                    if( ir1 > rFarGapBin ) continue;
                    double rGapS= fRGap[irs+1];
                    double rGap1= fRGap[ir1+1];
                    if( rGap1 < TMath::Abs(fdphi-kJPi) ) continue;
                    if( rGapS < TMath::Abs(fdphi-kJPi) ) continue;
                    double dEtaMin = sqrt(rGap1*rGap1- (fdphi-kJPi)*(fdphi-kJPi) );
                    double dEtaMax = dEtaMin + sqrt(rGapS*rGapS-(fdphi-kJPi)*(fdphi-kJPi) );
                    double eta = TMath::Abs( fdEta );
                    //double eta = fdEta ;
                    if( eta > dEtaMin && eta < dEtaMax ){
                        //if( eta > dEtaMin && eta < dEtaMax && dEta <0 ){
                        // xxx
                        //                        fhistos->hDRFarPtMoon[fTyp][CentBin][ZBin][ir1][irs][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );
                        if( fTyp == 0 )
                            fhistos->fhDRFarPtMoon[CentBin][ZBin][ir1][irs][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );
                        else
                            fhistos->fhDRFarPtMoonM[CentBin][ZBin][ir1][irs][pTtBin]->Fill( pta, geoAccCorr * trackPairEff );

                        if(bReal)       fhistos->fhDphiAssoc2D[ir1][irs]->Fill( dEta, fdphi/kJPi, geoAccCorr * trackPairEff );
                    }
                    }
                }
            }
        }


        //---------------------------------
        //if(pairMass>10 && fcard->IsLessThanUpperPairPtCut(pairPt) ){
        //    cout<<pairMass<<" e1="<<ftk1->Eta()-CMSrap<<" e2="<<ftk2->Eta()-CMSrap<<" cms="<<CMSrap;
        //    cout<<" ppt="<<pairPt<<" tphi="<<tphi<<" aphi="<<aphi<<" t1="<<ptt<<" t2="<<pta<<endl;
        //    double thetaStar = fabs(2*atan(exp(-ftk1->Eta()-CMSrap)));
        //    cout<<thetaStar<<" "<<fabs(cos(thetaStar))<<endl;
        //}

        //---------------------------------
        if(bNear) {
            fhistos->fhxEN [fTyp][pTtBin]->Fill(-xe, geoAccCorr * trackPairEff);

            TLorentzVector vTrigger = ftk1->GetLorentzVector(), vAssoc = ftk2->GetLorentzVector();    //wrt trigger particle
            TLorentzVector vThrust(vTrigger);    //wrt trigger particle
            //FK// if(fsumTriggerAndAssoc) vThrust += vAssoc;                                              //wrt sum of trigger and assoc

            double klong = vThrust.Vect().Dot(vAssoc.Vect())/vThrust.P();
            int iKlongBin = fcard->GetBin(kLongType, klong );

            fhistos->fhZKlong[CentBin][pTtBin]->Fill( klong / ptt );

            if(iKlongBin>=0){
                double kperp  =  vAssoc.Perp(vThrust.Vect()); 
                double weight = kperp>1e-3 ? geoAccCorr * trackPairEff/kperp : 0;
                fhistos->fhJTKlong[fTyp][CentBin][pTtBin][iKlongBin]->Fill(kperp, weight );
                //cout <<" kperp="<< kperp <<" geo="<< geoAccCorr <<" eff="<< trackPairEff <<" w="<< geoAccCorr * trackPairEff/kperp <<endl; 
            }
            int iXeBin = fcard->GetBin(kXeType, vThrust.Vect().Dot(vAssoc.Vect())/pow(vThrust.P(),2) );
            if(iXeBin>=0){
                double kperp  =  vAssoc.Perp(vThrust.Vect()); 
                double weight = kperp>1e-3 ? geoAccCorr * trackPairEff/kperp : 0;
                fhistos->fhJT[fTyp][CentBin][pTtBin][iXeBin]->Fill(kperp, weight );
                //cout <<" kperp="<< kperp <<" geo="<< geoAccCorr <<" eff="<< trackPairEff <<" w="<< geoAccCorr * trackPairEff/kperp <<endl; 
            }
            double kperp  =  vAssoc.Perp(vThrust.Vect()); 
            double weight = kperp>1e-3 ? geoAccCorr * trackPairEff : 0;
            fhistos->fhJTPta[fTyp][CentBin][pTtBin][pTaBin]->Fill(TMath::Log(kperp),weight);


            TLorentzVector vAssocRndm;
            TLorentzVector vThrustRndm;    

            //-------------------------------------
            // Rapidity Gap background
            // ------------------------------------
            double etaTriggRndm, etaAssocRndm;
            double dphiAssocRndm;
            // iTrial = 20;
            //dR = Sqrt( dEta*dEta + dPhi*dPhi );
            if(fTyp==kReal && ( etaGapBin >=0 || rNearGapBin >=0 ) ){
                for(int iEtaGap=0; iEtaGap<=etaGapBin; iEtaGap++) fhistos->fhPtaEtaGapN[CentBin][iEtaGap][pTtBin]->Fill(pta, trackPairEff); 
                for(int iRGap=0; iRGap<=rNearGapBin; iRGap++) fhistos->fhPtaRGapN[CentBin][iRGap][pTtBin]->Fill(pta, trackPairEff); 
                for(int itrial=0; itrial<20; itrial++){  //For each high eta gap track generate ten others

                    if(fsamplingMethod == 0){
                        etaTriggRndm  = frandom->Uniform(-fmaxEtaRange, fmaxEtaRange);
                        etaAssocRndm  = frandom->Uniform(-fmaxEtaRange, fmaxEtaRange);
                    } else {
                        etaTriggRndm  = fhistos->fhIetaTriggFromFile[CentBin][pTtBin]->GetRandom();
                        etaAssocRndm  = fhistos->fhIetaAssocFromFile[CentBin][pTaBin]->GetRandom();
                    }
                    if( rNearGapBin >=0 ) { 
                        if( fsamplingMethod == 0) {
                            dphiAssocRndm = kJPi * frandom->Uniform(-0.5, 0.5);
                        } else {
                            dphiAssocRndm = fhistos->fhIphiAssocFromFile[CentBin][pTaBin]->GetRandom();
                        }
                        vAssocRndm.SetPtEtaPhiM(pta, etaAssocRndm, tphi + dphiAssocRndm, 0); //set randomized assoc  TLorentz vector
                    } else {
                        vAssocRndm.SetPtEtaPhiM(pta, etaAssocRndm, aphi, 0); //set randomized assoc  TLorentz vector
                    }

                    vThrustRndm.SetPtEtaPhiM(vTrigger.Pt(),etaTriggRndm, tphi, 0);           

                    double dEtaRndm = etaTriggRndm - etaAssocRndm;
                    double geoAccCorrRndm = (fsamplingMethod == 0 || phiNearGapBin<0 ) ? 
                        GetGeoAccCorrFlat(dEtaRndm) : GetGeoAccCorrIncl(dEtaRndm, CentBin, pTtBin, pTaBin);

                    int iKlongBin = fcard->GetBin(kLongType, vThrustRndm.Vect().Dot(vAssocRndm.Vect())/vThrustRndm.P());
                    if(iKlongBin>=0){
                        double kperp    =  vAssocRndm.Perp(vThrustRndm.Vect()); 
                        if(kperp>1e-3) { //here we used and BgFidCut
                            for(int iEtaGap=0; iEtaGap<=etaGapBin; iEtaGap++)
                                fhistos->fhJTKlongBg[CentBin][iEtaGap][pTtBin][iKlongBin]->Fill(kperp, geoAccCorrRndm * trackPairEff/kperp );
                            for(int iRGap=0; iRGap<=rNearGapBin; iRGap++)
                                fhistos->fhJTKlongBgR[CentBin][iRGap][pTtBin][iKlongBin]->Fill(kperp, geoAccCorrRndm * trackPairEff/kperp );
                        }
                    }
                    int iXeBin = fcard->GetBin(kXeType, vThrustRndm.Vect().Dot(vAssocRndm.Vect())/pow(vThrustRndm.P(),2) );
                    if(iXeBin>=0){
                        double kperp    =  vAssocRndm.Perp(vThrustRndm.Vect()); 
                        if(kperp>1e-3) { //here we used and BgFidCut
                            for(int iEtaGap=0; iEtaGap<=etaGapBin; iEtaGap++)
                                fhistos->fhJTBg[CentBin][iEtaGap][pTtBin][iXeBin]->Fill(kperp, geoAccCorrRndm * trackPairEff/kperp );
                            for(int iRGap=0; iRGap<=rNearGapBin; iRGap++)
                                fhistos->fhJTBgR[CentBin][iRGap][pTtBin][iXeBin]->Fill(kperp, geoAccCorrRndm * trackPairEff/kperp );
                        }
                    }
                }  //trials
            }// Eta Gap Random


        } else {
            fhistos->fhPoutF[fTyp][CentBin][pTtBin][pTaBin]->Fill(pout, geoAccCorr * trackPairEff);
            fhistos->fhxEF [fTyp][pTtBin]->Fill(xe, geoAccCorr * trackPairEff);
            if(bIsolatedTrigger) fhistos->fhxEFIsolTrigg[fTyp][pTtBin]->Fill(xe, geoAccCorr * trackPairEff);
        }

        //TVector3 V3trigg = ftk1->GetVector3();
        //TVector3 V3assoc = ftk2->GetVector3();
        //double pairMass = sqrt(2*V3trigg.Mag()*V3assoc.Mag()*(1-cos(V3trigg.Angle(V3assoc))));
        //int imass = fcard->GetBin(kMassType, pairMass);
        //TVector3 V3pairPt = V3trigg + V3assoc;
        //double pairPt = V3pairPt.Perp();
        //double wPairPt = pairPt>0 ? 1/pairPt : 0;

        ////================================
        //// Pair pT and cos(Theta^star) ===
        ////================================
        //if(!bNear) {
        //    fhistos->fhPairPt[fTyp][pTtBin][pTaBin]->Fill( pairPt, trackPairEff);
        //    double pairDphi = V3pairPt.DeltaPhi(V3trigg);
        //    if(pairDphi<-kJPi/3.0) pairDphi += kJTwoPi;
        //    fhistos->fhPairPtDphi[fTyp][pTtBin][pTaBin]->Fill( pairDphi/kJPi );
        //}

        //if ( fTyp == kReal ) {
        //    //cout<<" ptt="<< ptt <<" pta="<< pta <<" fdphi="<< fdphi <<" pairPt="<< pairPt << endl;

        //    int ipairPt = fcard->IsLessThanUpperPairPtCut(pairPt);
        //    //cout<<"ppt="<<pairPt<<" mass="<<pairMass<<" ip="<<ipairPt<<" im="<<imass<<endl;
        //    if(imass>=0) {
        //        fhistos->fhPairPtMass[imass]->Fill( pairPt, trackPairEff*wPairPt );
        //        fhistos->fhPairDPhi[imass]->Fill( fdphi/kJPi, trackPairEff );
        //        fhistos->fhPairDpT[imass]->Fill( ptt-pta, trackPairEff );
        //    }
        //    if(imass>=0 && ipairPt>=0 ){
        //        double CMSrap = (ftk1->Eta()+ftk2->Eta())/2.;
        //        double cosThetaStar = fabs(cos(2*atan(exp(-ftk1->Eta()+CMSrap))));
        //        fhistos->fhCosThetaStar[fTyp][ipairPt][imass]->Fill( cosThetaStar, trackPairEff );

        //        //frandom boost, Let's try just trigger phi as a cms boost.
        //        cosThetaStar = fabs(cos(2*atan(exp(-ftk1->Eta()+tphi))));
        //        fhistos->fhCosThetaStar[kMixed][ipairPt][imass]->Fill( cosThetaStar, trackPairEff );

        //        fhistos->fhInvMass[ipairPt]->Fill(pairMass, trackPairEff);
        //        fhistos->fhCMSrap[ipairPt][imass]->Fill( cosThetaStar, CMSrap*trackPairEff );
        //        fhistos->fpCMSrap->Fill( cosThetaStar, fabs(CMSrap)*trackPairEff );
        //    }
        //}


        if ( fTyp == kReal ) {
            //must be here, not in main, to avoid counting triggers
            fhistos->fhAssocPtBin[CentBin][pTtBin][pTaBin]->Fill(pta ); //I think It should not be waighted by Eff

            //++++++++++++++++++++++++++++++++++++++++++++++++++
            // in order to get mean pTa in the jet peak one has
            // to fill fhMeanPtAssoc in |DeltaEta|<0.4
            // +++++++++++++++++++++++++++++++++++++++++++++++++
            if(etaGapBin>=0 && etaGapBin<2){
                fhistos->fhMeanPtAssoc[CentBin][pTtBin][pTaBin]->Fill( fdphi/kJPi , pta );
                fhistos->fhMeanZtAssoc[CentBin][pTtBin][pTaBin]->Fill( fdphi/kJPi , pta/ptt);
            }

            //UE distribution
            if(fabs(dphipipi/kJPi)>fDPhiUERegion[0] && fabs(dphipipi/kJPi)<fDPhiUERegion[1]){
                for(int iEtaGap=0; iEtaGap<=etaGapBin; iEtaGap++)  //FK// UE Pta spectrum for different eta gaps
                    fhistos->fhPtAssocUE[CentBin][iEtaGap][pTtBin]->Fill(pta, trackPairEff); 
                if(bIsolatedTrigger){ //FK// trigger is isolated hadron
                    fhistos->fhPtAssocUEIsolTrigg[pTtBin]->Fill(pta, trackPairEff); //FK// 
                }
            }
            if(fabs(fdphi/kJPi)<0.15)             fhistos->fhPtAssocN[pTtBin]->Fill(pta, trackPairEff);
            if(fabs(fdphi/kJPi-1)<0.15)           fhistos->fhPtAssocF[pTtBin]->Fill(pta, trackPairEff);

            fnReal++;  
        } else { // only mix
            fnMix++;
        }
    }


    double AliJCorrelations::GetGeoAccCorrFlat(double deltaEta){
        //FK// calculate acceptance correction on pseudorapidity triangle 
        double absDEta = fabs(deltaEta);

        double denominator = 1 - absDEta/(2*fmaxEtaRange);
        //double denominator = 1 - (absDEta - ftriggFiducCut)/(2*fmaxEtaRange-2*ftriggFiducCut);//When Fid>0 max_Deta je mensi nez 2*EtaMax
        //double denominator = 1 - (absDEta - ftriggFiducCut)/(2*fmaxEtaRange-ftriggFiducCut);
        if(denominator > 1e-6)
            return 1.0/denominator;
        else
            return 0;
    }

    double AliJCorrelations::GetGeoAccCorrIncl(double deltaEta, int  CentBin, int pTtBin, int  pTaBin){
        // histo filler

        if(fhistos->fhDEtaNearMixFromFile[CentBin][pTtBin][pTaBin]->GetEntries()<1000) return GetGeoAccCorrFlat(deltaEta);

        int bin =  fhistos->fhDEtaNearMixFromFile[CentBin][pTtBin][pTaBin]->FindBin(deltaEta);
        double denominator  =  fhistos->fhDEtaNearMixFromFile[CentBin][pTtBin][pTaBin]->GetBinContent(bin);
        if(denominator > 1e-6)
            return 1.0/denominator;
        else
            return 0;

    }

    double AliJCorrelations::DeltaPhi(double phi1, double phi2) {
        // dphi
        double res =  atan2(sin(phi1-phi2), cos(phi1-phi2));
        //double res = phi1 - phi2;
        //return res>-kJPi/3.0 ? res : kJTwoPi+res ; 
        return res>-kJPi*9./20. ? res : kJTwoPi+res ; 
    }

