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

#include "AliJHistos.h"

void scaleNotEquidistantHisto(TH1D *hid, double sc);
particleType  GetParticleType(char *);

int main(int argc, char **argv) {

    //==== Read arguments ===== 
    TROOT root("nanoDST","nanoDST analysis");
    if ( argc<6 ) {
        cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout<<"+  "<<argv[0]<<" [inputFilelist] [outputFile] [inCard.input] [trigger] [assoc] <inclusiveSpectra>"<<endl;
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
    if(argc>=7 ) inclusFileName = argv[6];

    TFile *fout = new TFile(outFile,"RECREATE","Final analysis");//, *inclusFile = NULL;

    cout <<"=================================================="<<endl;
    cout <<"* inp. file list \t"<<inputFile<<endl;
    cout <<"* out file name \t"<<outFile<<endl;
    cout <<"* card name \t"<<cardName<<endl;
    cout <<"* Trigg Particle = "<< argv[4] <<" x "<< "Assoc = "<< argv[5] << endl;
    if(argc<7){
        cout<<"* K_perp bacground from flat" <<endl;
    } else {
        cout<<"* K_perp bacground from the " << inclusFileName << endl;
    }
    cout <<"=================================================="<<endl;

    // === Create all what you need ====
    AliJCard *card = new AliJCard(cardName);
    //card->ReadLine( "CentBinBorders=0,10,100;sumTriggerAndAssoc=1;testVal=4,5,6;TriggPtBorders=1,2,3;Jet:PtBins=0,10" );
    card->ReCompile();
    card->PrintOut();


    fout->mkdir( "test0" )->cd();
    AliJHistos * h0 = new AliJHistos(card);
    h0->CreateEventTrackHistos();
    h0->CreateJetHistos();

    fout->mkdir( "test1" )->cd();
    AliJHistos * h1 = new AliJHistos(card);
    h1->CreateEventTrackHistos();
    h1->CreateJetHistos();
    
    for( int i=0;i<100;i++ ) h0->fhLPJetMassPTt[0][0]->Fill( 30 );
    for( int i=0;i<200;i++ ) h1->fhLPJetMassPTt[0][0]->Fill( 30 );
    for( int i=0;i<100;i++ ) h0->fhLPJetMassPTt[0][1]->Fill( 30 );
    for( int i=0;i<100;i++ ) h1->fhLPJetMassPTt[0][2]->Fill( 30 );

    fout->Write();
    cout<<"DEBUG 8"<<endl;



}
