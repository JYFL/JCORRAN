#include "JPythiaDataManager.h"
#include <set>
#include <map>
#include <vector>
#include <list>

using namespace std;

//--------------------------------------------------------
//--- Search for a leading particle and parent partons ---
//--------------------------------------------------------
/*
bool JPythiaDataManager::FindLParticle(JTrack &track, int SearchParentMethod){ 

    float pT_max = -1.;     //maximal pT in the event
    indexLP      = -1;      //index of the leading particle
    //search the leading particle
    for(int i = 0; i < event.size(); i++){ 
		//cout <<event[i].name()<<"\t"<<event[i].isFinal()<<"\t"<<event[i].isCharged()<<"\t"<<event[i].isHadron()<<endl;
        if( !event[i].isFinal() || !event[i].isCharged() || !event[i].isHadron() ) continue;
		//cout <<event[i].name()<<"\t"<<event[i].isFinal()<<"\t"<<event[i].isCharged()<<"\t"<<event[i].isHadron()<<endl;
        pTtBin = card->GetBin(kTriggType, event[i].pT() );
        
        if( pTtBin<0 || !card->IsInEtaRange(event[i].eta()) ) continue;

        if(event[i].pT() > pT_max ){  //Find the leading particle in the event
            indexLP  = i;
            pT_max = event[i].pT(); 
        }
        //histos->hChargedPt->Fill(event[i].pT(), 1./event[i].pT());
    }
    if(indexLP<0 ||   (fabs(event[indexLP].eta()) > (TrackEtaRange - TriggerFiducialEtaCut)) ) return false; 
    track.SetParticle(event[indexLP],event[event[indexLP].mother1()].id());
    track.ParticleIndex = indexLP;
    if( SearchParentMethod==0) return true;

    if( FindMotherPartons(indexLP, track.particle, SearchParentMethod) ){ //has to be fixed
        track.parton71 = parton71;
        track.SetParton23(event[indexParton23]);
        track.StringIndex = mother1;
        //track.Print();
        return true;
    } else
        return false;
}
*/


//----------------------------------
//---  Search for parent partons ---
//----------------------------------
bool JPythiaDataManager::FindMotherPartons(int particleIndex, TLorentzVector particle, int SearchParentMethod){

    indexParton71 = -1;
    // find the mother string. It could be resonance (123442).
    int daughter = particleIndex;
    while(event.motherList(daughter).size()<=1) daughter = event.motherList(daughter)[0];
    mother1 = event.motherList(daughter)[0];
    mother2 = event.motherList(daughter)[0] + event.motherList(daughter).size();
    //-----
    //cout<<"particleIndex = " << particleIndex <<" "<< particle.Px() <<" "<< particle.Py()<<endl; 
    //cout<<"m1="<<mother1 <<" m2="<< mother2 <<endl; 
    //event.list();abort();
    //------

    double pptt = particle.Pt();
    double particleRadius = exp(-1.29790e-01-1.39743e-01*pptt)+4.57776e-02; //see macro dr.C (max*0.3) of dr distrib
    //double particleRadius = exp(-6.57211e-01-1.54001e-01*pptt)+2.85174e-02; //see macro dr.C (max*0.5) of dr distrib

    //find a closest parton from the string.
    vector<int> parentShower;
    double DeltaR = 0.3;
    for(int im=mother1; im<mother2; im++){
        //if( event[im].pT()<particle.Pt() ) continue;
        parton71.SetPxPyPzE(event[im].px(), event[im].py(), event[im].pz(), event[im].e());
        double dr = parton71.DeltaR(particle);
        //histos->hPartonDR->Fill(particle.Pt(), dr);
        //if(dr<DeltaR && parton71.Pt()>particle.Pt() && parton71.E() > particle.E()) {

        if( dr<particleRadius && SearchParentMethod==2 ) parentShower.push_back(im);
        if(dr<DeltaR) {
            DeltaR = dr;
            indexParton71 = im;
        }
    } 
    if(indexParton71 < 0) return false;
    if( SearchParentMethod==1 )
        parton71.SetPxPyPzE(event[indexParton71].px(), event[indexParton71].py(), event[indexParton71].pz(), event[indexParton71].e());
    else if(SearchParentMethod == 2) {
        parton71.SetPxPyPzE(0,0,0,0);
        TLorentzVector showerParton;
        for(unsigned int iparent=0; iparent<parentShower.size(); iparent++) {
            int ishower = parentShower[iparent];
            showerParton.SetPxPyPzE(event[ishower].px(), event[ishower].py(), event[ishower].pz(), event[ishower].e());
            parton71 += showerParton;
        }
        parentShower.clear();
    }

    indexParton23 = indexParton71;
    while(event.motherList(indexParton23)[0]>3) indexParton23 = event.motherList(indexParton23)[0];
    //cout<< "parton23.stat="<<event[indexParton23].statusAbs()<<endl;
    if( event[indexParton23].statusAbs() != 23) {
        //Print();event.list();abort();
        return    false;
    } else return true;
}

bool JPythiaDataManager::FindParton71(){
    fScanParton71s.clear();
    fScanStatus.clear();
    fScanStatus.resize( event.size(), 0 );
    fScanStringGroup.clear();
    fScanStringGroupFrom23.clear();
    fScanStringGroupFrom23vector.clear();
    fScanInheritFrom23=false;
  for( int it=0;it<event.size();it++ ){
    Particle &p = event[it];
    if( p.statusAbs() != 71 ) continue;
    int d1 = p.daughter1();
    fScanParton71s.push_back(it);
    fScanStringGroup[d1].push_back( it );
  }
  //cout<<"DEBUG 6 -------- "<< fScanParton71s.size() <<"\t"<<fScanStringGroup.size()<<endl;
  for(map<int, vector<int> >::iterator im=fScanStringGroup.begin();im!=fScanStringGroup.end();im++ ){
    int ig = im->first;
    vector<int>& list = im->second;
    for( UInt_t i=0;i<list.size();i++ ){
      int iter = list[i];
      while( !(event[iter].mother1()==0 || event[iter].statusAbs()==23 || iter < 2) )
        iter = event[iter].mother1();
      if( event[iter].statusAbs() ==23 ){
        fScanStringGroupFrom23.insert( ig );
        break;
      }
    }
  }
  return true;
}


/*
   bool JPythiaDataManager::FindParton71(){
   fScanParton71s.clear();
   fScanStatus.clear();
   fScanStatus.resize( event.size(), 0 );
   fScanStringGroup.clear();
   fScanStringGroupFrom23.clear();
   fScanStringGroupFrom23vector.clear();
   fScanInheritFrom23=false;
   ScanTree(1, 0);
   ScanTree(2, 0);
   for ( set<int>::iterator it=fScanStringGroupFrom23.begin() ; it != fScanStringGroupFrom23.end(); it++ )
   fScanStringGroupFrom23vector.push_back( *it );
   return true;
   }
   */


bool JPythiaDataManager::ScanTree( int it, int depth ){


  if( it < 1 ) return true;
  if( fScanStatus[it] ) return true;
  fScanStatus[it] = 1;
  Particle & p = event[it];
  int d1 = p.daughter1();
  int d2 = p.daughter2();
  int st = p.statusAbs();
  for( int i=0;i<depth;i++ ) cout<<"  ";
  cout<<Form("+-%d, st:%d, it:%d, (d1,d2)=%d, %d", depth,st, it, d1, d2)<<endl; 

  if( st >=70 && st<80  ) { 
    fScanParton71s.push_back( it );
    fScanStringGroup[d1].push_back(it);
    if( fScanInheritFrom23 ) fScanStringGroupFrom23.insert( d1 );
    return true;
  }
  if( st >= 80 ){ return true; }

  if( fScanStatus[d1] ) return true;
  if( d1 == 0 ) return true;
  if( d2 == 0 ) d2 = d1;
  if( st == 23 ) fScanInheritFrom23 = true;
  for( int i=d1;i<=d2;i++ )ScanTree( i, depth+1 );
  if( st == 23 ) fScanInheritFrom23 = false;
  return true;
}

