// $Id: jtkt_phenix.C,v 1.29 2008/02/18 10:09:32 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file jtkt_phenix.h
  \brief
  \author J. Rak, D.J.Kim, B.S Chang (University of Jyvaskyla)
  \email: djkim@cc.jyu.fi
  \version $Revision: 1.29 $
  \date $Date: 2008/02/18 10:09:32 $
  */
////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <TROOT.h>
#include <TSystem.h>
#include <TStopwatch.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TF1.h>
#include <TProfile.h>
#include <TFile.h>
#include <TChain.h>
#include <TObject.h>
#include <TTree.h>
#include <TRandom.h>
#include <TVector3.h>
#include <TLorentzVector.h>

#include "AliJCorrelations.h"
#include "AliJEventPool.h"
#include "AliJCORRAN.h"
#include "AliJCard.h"

#include "AliJRunTable.h"
#include "AliJTrackCut.h"
#include "AliJEfficiency.h"

void scaleNotEquidistantHisto(TH1D *hid, double sc);
particleType  GetParticleType(char *);

int main(int argc, char **argv) {

    //==== Read arguments ===== 
    TROOT root("nanoDST","nanoDST analysis");
    if ( argc<4 ) {
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout<<"+  "<<argv[0]<<" [inputFilelist] [outputFile] [inCard.input]"<<endl;
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout << endl << endl;
        exit(1);
    }

    char *outFile, *inputFile, *cardName;
    inputFile = argv[1];
    outFile   = argv[2];
    cardName  = argv[3];

    TFile *fout = new TFile(outFile,"RECREATE","Efficiency Scan");//, *inclusFile = NULL;
    TDirectory *fEffHistDir = gDirectory->mkdir("EffHist");

    cout <<"=================================================="<<endl;
    cout <<"* inp. file list \t"<<inputFile<<endl;
    cout <<"* out file name \t"<<outFile<<endl;
    cout <<"* card name \t"<<cardName<<endl;
    cout <<"=================================================="<<endl;

    // === Create all what you need ====
    AliJCard *card = new AliJCard(cardName);
    card->PrintOut();




    AliJDataManager * fdmg = new AliJDataManager(fcard, fhistos, fcorrelations, fExecLocal);
    fdmg->SetExecLocal( fExecLocal );

    if( fExecLocal ){
        fdmg->ChainInputStream(finputFile);
        // TODO: run header is not supposed to be here
        frunHeader = fdmg-> GetRunHeader();

        // for grid running, numberEvents is filled by the encapsulating
        // grid task, which has access to the input handlers and can
        // extract event numbers out of it
        fnumberEvents = fdmg->GetNEvents();
        fieout = fnumberEvents/20;

        if (fieout<1) fieout=1;
        cout<<"RunID = "<<frunHeader->GetRunNumber()<< " Looping over "<<fnumberEvents<<" events"<<endl;

        //==== RunTable
        fRunTable = & AliJRunTable::GetSpecialInstance();
        fRunTable->SetRunNumber( frunHeader->GetRunNumber() );

        fhistos->fhEventPerRun->Fill(fnumberEvents>0?log(fnumberEvents):1);
    }
    // TODO

    //==== Create Output  
    fEffHistDir->cd();
    fEfficiencyScanner->UserCreateOutputObjects();

    Int_t numberEvents = fJCORRAN->GetNumberEvents();

    gROOT->cd();
    AliJEfficiencyScanner * fEfficiencyScanner = new AliJEfficiencyScanner();
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Loop over all events
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TStopwatch timer;
    timer.Start();
    for(Int_t evt=0; evt<numberEvents; evt++) {
        frunHeader = fdmg->GetRunHeader();
        fEfficiencyScanner->SetJRunHeader( fdmg->GetAliJRunHeader() );//TODO
        fEfficiencyScanner->SetEventHeader( fdmg->GetEventHeader() );
        fEfficiencyScanner->SetJMCTrackList( fdmg->GetMCTrackList()    );
        fEfficiencyScanner->SetJTrackList( fdmg->GetTrackList()    );

        fEfficiencyScanner->UserExec();
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // End of Event Loop
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    //Int_t EventCounter = fJCORRAN->GetEventCounter();

    fout->cd();
    fEfficiencyScanner->Terminate();
    //write Trigg ID and Assoc ID
    fout->Write(); 
    fout->Close();
    timer.Print();

    cout << "Total Number of Event scanned from input = " << numberEvents << endl;
    //cout << "Total Number of Event used for analysis  = " << EventCounter << endl;
    cout<<"All files properly closed. Good Bye!"<<endl;
}

