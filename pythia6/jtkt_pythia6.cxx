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
#include "AliJCORRANpythia6.h"
#include "AliJCard.h"

#include "AliJRunTable.h"
#include "AliJTrackCut.h"
#include "AliJEfficiency.h"

void scaleNotEquidistantHisto(TH1D *hid, double sc);
particleType  GetParticleType(char *);

int main(int argc, char **argv) {

    //==== Read arguments ===== 
    TROOT root("nanoDST","nanoDST analysis");
    if ( argc<6 ) {
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout<<"+  "<<argv[0]<<" [inputFilelist] [outputFile] [inCard.input] [trigger] [assoc] [pythiaConfig file] <inclusiveSpectra>"<<endl;
        cout<<"+  InclusiveSpectra for k_perp bck simulation. If not given, sampling from flat #Delta#eta"<<endl;
        cout<<"+  InclusiveSpectra is just the output file with the same binning"<<endl;
        cout<<"+  trigger/assoc: "; for(int i=0;i<kNumberOfParticleTypes;i++)
            cout<<kParticleTypeStrName[i]<<" "; cout<<"+"<<endl<<"+"<<endl;
        cout<<"+  1) for pizer0 mass distribs trigger must be pizero"<<endl;
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout << endl << endl;
        exit(1);
    }

    char *outFile, *inputFile, *cardName, *inclusFileName=NULL, *jtrigg, *jassoc;
    inputFile = argv[1];
    outFile   = argv[2];
    cardName  = argv[3];
    jtrigg = argv[4];
    jassoc = argv[5];
    Float_t eCM = atof(argv[6]);
    Int_t nEvents = atoi(argv[7]);
    Int_t random_seed = atoi(argv[8]);

    TFile *fout = new TFile(outFile,"RECREATE","Final analysis");//, *inclusFile = NULL;

    cout <<"=================================================="<<endl;
    cout <<"* inp. file list \t"<<inputFile<<endl;
    cout <<"* out file name \t"<<outFile<<endl;
    cout <<"* card name \t"<<cardName<<endl;
    cout <<"* Trigg Particle = "<< argv[4] <<" x "<< "Assoc = "<< argv[5] << endl;
	cout <<"* Beam eCM = "<< eCM << endl;
	cout <<"* random_seed = "<< random_seed << endl;
    cout <<"=================================================="<<endl;

    // === Create all what you need ====
    AliJCard *card = new AliJCard(cardName);
    card->PrintOut();
    
    AliJCORRANpythia6 *fJCORRAN;
    fJCORRAN = new AliJCORRANpythia6( kTRUE );
    
    fJCORRAN->SetCard( card );
    fJCORRAN->SetTrigger( jtrigg );
    fJCORRAN->SetAssoc( jassoc );
    fJCORRAN->SetInputFile( inputFile );
    fJCORRAN->SetRandomSeed( random_seed );
    fJCORRAN->SetNumberEvents(nEvents);
    fJCORRAN->SetBeamCMEnergy(eCM);
    
    fJCORRAN->UserCreateOutputObjects();
    
    Int_t numberEvents = fJCORRAN->GetNumberEvents();
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Loop over all events
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TStopwatch timer;
    timer.Start();
    for(Int_t evt=0; evt<numberEvents; evt++) {
      
       fJCORRAN->UserExec();
     }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // End of Event Loop
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    
    Int_t EventCounter = fJCORRAN->GetEventCounter();
    
    fout->cd();
    fJCORRAN->Terminate();
    //write Trigg ID and Assoc ID
    TNamed *triggid = new TNamed("triggid",kParticleTypeStrName[fJCORRAN->GetParticleType(jtrigg)]);
    TNamed *associd = new TNamed("associd",kParticleTypeStrName[fJCORRAN->GetParticleType(jassoc)]);
    triggid->Write();
    associd->Write();
    card->WriteCard(fout);
    fout->Write(); 
    fout->Close();
    timer.Print();

    cout << "Total Number of Event scanned from input = " << numberEvents << endl;
    cout << "Total Number of Event used for analysis  = " << EventCounter << endl;
    cout<<"All files properly closed. Good Bye!"<<endl;
}

