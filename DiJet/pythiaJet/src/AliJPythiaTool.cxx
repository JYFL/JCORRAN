#include "AliJPythiaTool.h"
#include "AliJJet.h"
#include "TFile.h"
double Eta( TLorentzVector & v){ 
    return v.Pt() > 1e-5 ? v.Eta() : v.Pz()>0 ? 1e11 : -1e11;
}
void Print( TLorentzVector v){
    cout<<Form("(px,py,pz,E,m) = ( %10.4f,%10.4f,%10.4f,%10.4f,%10.4f )",v.Px(), v.Py(), v.Pz(), v.E(), v.M()  );
    cout<<Form("  (pT,Eta,Phi) = ( %10.4f,%7.4f,%7.4f )",v.Pt(), Eta(v), v.Phi())<<endl;
}
void Print( Pythia8::Particle & p){
    Print( TLorentzVector(p.px(),p.py(),p.pz(),p.e()) );
}

AliJPythiaTool::AliJPythiaTool( Pythia8::Event & event, int pymode):
    fEvent(&event),
    fParticle("AliJBaseTrack",1000 ),
    fPartonGroup("AliJJet",100 ),
    fPartonOrgGroup("AliJBaseTrack",100 ),
    fPythiaMode(pymode)

{
    switch( fPythiaMode ){
        case kJNormalPythia :
            fIdOf23[0] = 5;fIdOf23[1] = 6;
            break;
        case kJResornancePythia :
            fIdOf23[0] = 6;fIdOf23[1] = 7;
            break;
        default :
            UnExpectedError( 124, "None Exist Pythia Mode", pymode,0 );
    }
    CreateHistos();
}

TObjArray * AliJPythiaTool::GetDiJet(int type){
    //AliJJet ( px, py, pz. e. id. ptype, charge)
    //
    return NULL;
}


void AliJPythiaTool::ScanPartons(){
    //==== INIT 
    fSeenScanFinalParton.clear();
    fSeenScanFinalParton.resize( fEvent->size(), 0) ;
    fPartonGroup.Clear();
    fPartonGroupMap.clear();
    fParticle.Clear();
    Pythia8::Event & event = (*fEvent);
    fEventStatus = 0;

    //==== Particle Loop
    for( int i=0;i<event.size();i++ ){
        if( fEventStatus > 0 ) return;
        Pythia8::Particle & p = (*fEvent)[i];
        new( fParticle[i] ) AliJBaseTrack(p.px(),p.py(),p.pz(),p.e(), i , 0, int(p.charge()*3));
        AliJBaseTrack * par = (AliJBaseTrack*) fParticle[i];
        par->SetStatus( p.statusAbs() );
    }

    //ScanFinalParton( -1, -1, fIdOf23[0], 0, fIdOf23[0], 0 );
    //ScanFinalParton( -1, -1, fIdOf23[1], 0, fIdOf23[1], 0 );
    ScanFinalParton( NULL, 1 );
    ScanFinalParton( NULL, 2 );
    //cout<<"DEBUG D1 "<<fPartonGroupMap.size()<<endl;
    int idx0=1;int idx1=-1;
    for( INTSMAP2::iterator it0=fPartonGroupMap.begin();it0!=fPartonGroupMap.end();it0++ ){
      int index0 = 0;
      int ig0 = it0->first;
      if( ig0 == 1 ) index0 = 0;
      else if( ig0 == 5 ) index0 = 1;
      else{ index0 = ++idx0; }
      INTSMAP & im = it0->second;
      //cout<<"DEBUG E1 index/sizeof jet "<<index0 <<"\t"<<im.size()<<endl;
      idx1=-1;
      for( INTSMAP::iterator it1=im.begin();it1!=im.end();it1++ ){
          idx1++;
          if( idx1 > 1 ) {
              //cout<<"DEBUG E2 : three jet!! - "<<idx1<<endl;
              continue;
          }
          int ig1 = it1->first;
          INTS & vec = it1->second;
          new( fPartonGroup[2*index0+idx1] ) AliJJet;
          new( fPartonOrgGroup[2*index0+idx1] ) AliJBaseTrack( *(AliJBaseTrack*)fParticle[ig1] );
          AliJJet * jet = (AliJJet*) fPartonGroup[2*index0+idx1];;
          for( UInt_t ij=0;ij<vec.size();ij++ ){
              jet->AddConstituent( fParticle[ij] );
          }
          jet->ReSum();
      }
    }
    for( int ij=0;ij<fPartonGroup.GetEntriesFast();ij++ ){
        TObject * o = fPartonGroup[ij];
        if( !o ) new( fPartonGroup[ij] ) AliJJet;
        TObject * o1 = fPartonOrgGroup[ij];
        if( !o1) new (fPartonOrgGroup[ij]) AliJBaseTrack;
    }

}

//==== iGroup
//==== stringGroup
//==== particleID
//==== mother
//==== org   : after any merging. org means orignal particle index
//==== status2
void AliJPythiaTool::ScanFinalParton( vector<int>* vec, int particleID){
    //= fSeenScanFinalParton was resized by number of particles
    if( particleID >= int(fSeenScanFinalParton.size()) ) return;
    //= If seen but not merged before

    Pythia8::Event & event = (*fEvent);
    Pythia8::Particle & p = event[particleID];
    int status = p.statusAbs();
    int d1 = p.daughter1();
    int m1 = p.mother1();

    //= stop if seen and hadron
    if( fSeenScanFinalParton[particleID] &&  status >=80 ) return;
    //= Check as Seen


    if( status < 40 ){
        if( status==12 || status==23 || status==33 ){
            if( fSeenScanFinalParton[particleID] ) return;
            int ig0 = -1;
            if( status == 12 ){
                ig0 = 1;
            }else{
                ig0  = event[event[m1].daughter1()].index();
            }
            int ig1 = particleID;
            vec = &fPartonGroupMap[ig0][ig1];
            /*  
                if( fGroupMap.find(ig0) == fGroupMap.end() ){
                iGroup = fGroupMap.size();
                fGroupMap[ig0] = iGroup;
                fGroupHeader.push_back( ig0 );
                fPartonGroup.resize( iGroup+1 );
                fGroupSecond.resize( iGroup+1 );
                stringGroup = -1;
                }
                stringGroup++;
                fGroupSecond.push_back( ig1 );
                fPartonGroup[iGroup].resize( stringGroup + 1 );
                */
        }
    }

    //= parton just before hadronization
    if( status<80 && ( event[d1].statusAbs() >=80 || event[d1].statusAbs()==73) )
    {
        if( fSeenScanFinalParton[particleID] )
            cout<<"DEBUG_B_1 Seen particle!!"<<endl;
        //AliJBaseTrack * trk = (AliJBaseTrack*) fParticle[particleID];
        //trk->SetLabel( iGroup*1000+stringGroup );
        vec->push_back( particleID );
        fSeenScanFinalParton[particleID]++;
        return;
    }

    //= Hadrons
    if( status >= 80 ){
        fSeenScanFinalParton[particleID]++;
        return;
    }

    fSeenScanFinalParton[particleID]++;
    vector<int> daughterVec = p.daughterList();
    for( UInt_t i=0;i<daughterVec.size();i++ ){
        int id = daughterVec[i];
        ScanFinalParton( vec, id );
    }

}


void AliJPythiaTool::UnExpectedError( int error, TString str, int ip, int org ){
    cout<<"!!! ERROR"<<error<<" "<<str<< " with "<<ip<<" "<<org<<endl;
    // TODO gSystem->Exit(error);
}


void AliJPythiaTool::SumTest( int m1, int m2, int d1, int d2 ){
    TLorentzVector pm;
    TLorentzVector pd;
    for( int m=m1;m<=m2;m++ )pm += *(TLorentzVector*) fParticle[m];
    for( int d=d1;d<=d2;d++ )pd += *(TLorentzVector*) fParticle[d];
    /* 
       cout << pm.E() <<","<<pd.E()<<"\t";
       if( Compare( pm, pd , 1e-2) ) 
       {
       cout<<"================= DEBUG 10 N1="<<m2-m1<<" N2="<<d2-d1;
       cout<<Form("%d, %d, %d, %d", m1, m2, d1, d2)<<endl;
       cout<<"\t status="<<(*fEvent)[m1].status()<<" , "<<(*fEvent)[d1].status()<<endl;
       cout<<"JWARNING !! Sum Fail!!"<<endl;
       pm.Print();
       pd.Print();
    //fEvent->list();
    //gSystem->Exit(134);
    }
    */
}

int AliJPythiaTool::Compare( TLorentzVector & t1, TLorentzVector & t2, double tol ){
    TLorentzVector t = t1-t2;
    if( 
            Compare( t1.Px(), t2.Px(), tol ) 
            || Compare( t1.Py(), t2.Py(), tol ) 
            || Compare( t1.Pz(), t2.Pz(), tol ) 
            || Compare( t1.E(), t2.E(), tol ) 
      ) return 1;
    return 0;

}
int AliJPythiaTool::Compare( double x, double y, double tol){
    double d = x-y;
    if( d > tol ) return 1;
    if( d < -tol ) return -1;
    return 0;
}


void AliJPythiaTool::CreateHistos() {
    int nBINS=150;
    double logBinsX[nBINS+1], limL=0.1, LimH=100;
    double logBW = (log(LimH)-log(limL))/nBINS;
    for(int ij=0;ij<=nBINS;ij++) logBinsX[ij]=limL*exp(ij*logBW);

    double ptbin[] = { 0, 1, 2, 3, 4, 5, 10, 15, 20, 30, 40, 60, 80, 100, 150, 200 };
    int nptbin = sizeof(ptbin)/sizeof(ptbin[0]);
    TDirectory * owd = gDirectory;
    TDirectory * cwd = (TDirectory*) owd->Get("pythia");
    if( !cwd )  cwd = owd->mkdir("pythia");
    if( !cwd )  {
        cout<<"Can't make directory pythia"<<endl;
        gSystem->Exit(-10);
    }
    cwd->cd();

    fParton23Pt = new TH1D("fParton23Pt","fParton23Pt", nBINS, logBinsX );
    fParton23Eta = new TH1D("fParton23Eta","",100,-6,6 );

    // EACH Group
    for( int i=0;i<5;i++ ){
        TString idx1 = Form("%02d",i);
        fhPartonJetPt[i] = new TH1D(Form("hPartonJetPt%02d",i),
                Form("pt of 4Jet. iJet=%d",i),
                nBINS, logBinsX);
        for( int i2=0;i2<nptbin-1;i2++ ){
            TString idx2 = Form("%02d",i2);
            TString tit2 = Form(" %.0f<p_{T}<%.0f", ptbin[i2], ptbin[i2+1] );
            fhPartonJetPhi[i][i2] = new TH1D("hPartonJetPhi"+idx1+idx2,"hPartonJetPhi"+idx1+tit2,500,-0.5,1.5);
            fhPartonJetEta[i][i2] = new TH1D("hPartonJetEta"+idx1+idx2,"hPartonJetEta"+idx1+tit2,500,-5,5);
            fhPartonJetDeltaEtaDeltaPhi[i][i2] = new TH2D("hPartonJetDeltaEtaDeltaPhi"+idx1+idx2, "#Delta#eta vs #Delta#phi of String groups"+idx1+tit2,500,-5,5, 500,-0.5,1.5 );
        }
    }

    //=== DiJet
    for( int i=0;i<3;i++ ){
        TString idx1 = Form("%02d",i);
        fhPartonJetPtPair[i] = new TH1D("hPartonJetPtPair"+idx1, "Jet Pt Pair "+idx1, nBINS, logBinsX);
        fhPartonJetDPhiPt[i] = new TH2D("hPartonJetDPhiPt"+idx1,"hPartonJetDPhi"+idx1,500,-0.5,1.5, 200, 0, 200);
        fhPartonJetDPhiE[i] = new TH2D("hPartonJetDPhiE"+idx1,"hPartonJetDPhi"+idx1,500,-0.5,1.5, 200, 0, 200);
        for( int i2=0;i2<nptbin-1;i2++ ){
            TString idx2 = Form("%02d",i2);
            TString tit2 = Form(" %.0f<p_{T}<%.0f", ptbin[i2], ptbin[i2+1] );
            fhPartonJetDPhi[i][i2] = new TH1D("hPartonJetDPhi"+idx1+idx2,"hPartonJetDPhi"+idx1+tit2,500,-0.5,1.5);
            fhPartonJetAssym[i][i2] = new TH1D("hPartonJetAssym"+idx1+idx2,"hPartonJetAssym"+idx1+tit2, 100, 0, 1);
        }
    }
    fhStringGroupMultiplicity = new TH1D("hStringGroupMultiplicity","hStringGroupMultiplicity",30,0-.5,30-.5);

    fhJetMassCor = new TH2D("hJetMassCor", "hJetMassCor", 800, 0, 200, 800, 0, 200 );
    fhPythiaDiJetMass = new TH1D("hPythiaDiJetMass", "hPythiaDiJetMass", 5000, 0, 500 );
    fhPythiaPtPairComp = new TH2D("hPythiaPtPairComp", "hPythiaPtPairComp", 1200, 0, 300, 1200, 0, 300 );

    owd->cd();
}

void AliJPythiaTool::PrintEvent(){
    Pythia8::Event & event = *fEvent;

}

const vector<double>& AliJPythiaTool::GetPartonGroupProbability( int index ){
    AliJBaseTrack * trk = (AliJBaseTrack*) fParticle[index];
    //int stringGroup = trk->GetLabel();
    //return fPartonStringGroup2[stringGroup];
    vector<double> * tmp = new vector<double>;
    return *tmp;


}
