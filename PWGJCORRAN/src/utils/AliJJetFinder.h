// $Id: jet_alice.C,v 1.1 2010/02/18 10:09:32 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file jet_alice.C
  \brief
  \author J. Rak, D.J.Kim, F.Krizek (University of Jyvaskyla)
  \email: djkim@cc.jyu.fi
  \version $Revision: 1.1 $
  \date $Date: 2010/02/18 10:09:32 $
  */
////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// includes from JCORRAN
#include "AliJConst.h"
#include "AliJBaseCard.h"
#include "AliJBaseTrack.h"
#include "AliJJet.h"

#include "TClonesArray.h"
// Fastjet
#include "fastjet/PseudoJet.hh"
//#include "fastjet/PseudoJet.hh"
#include <TLorentzVector.h>


#ifndef JJETFINDER_H
#define JJETFINDER_H

// bschang trimming #include "fastjet/QCDTrimmingFast.hh"
//#include "fastjet/GaussianFilterPlugin.hh"
#include "fastjet/ClusterSequence.hh"
#include <iostream>
namespace fastjet {
    class ClusterSequence;
    class ClusterSequenceArea;
}
using namespace fastjet;
using namespace std;

class AliJJetFinder {
 public :
  typedef vector<fastjet::PseudoJet> JETS;

  AliJJetFinder( vector<fastjet::PseudoJet>& input_particles, JetAlg fJetalg, bool jettrimming = false, bool usearea = true);
  ~AliJJetFinder();

  AliJJetFinder( AliJBaseCard *card );

  JETS find();	

  double get_R() { return fR; };
  void set_R( double r ) { fR =r; };
  void set_InputPtMin( double ptmin ) {  fInputPtMin=ptmin; };

  void set_JetAlg( JetAlg falg ) { fJetalg =falg; };
  JetAlg get_JetAlg() { return fJetalg; };

  double get_ptmin() { return fpTMin ; };
  void set_ptmin( double ptmin ) { fpTMin=ptmin; };

  bool useArea(){ return fuseArea; }
  bool useArea( bool usearea ){ return fuseArea = usearea; }

  void set_LeadingPaticleIndex( int indexLP ) { fIndexLP=indexLP; };

  void SetUseCharged(bool ch ){ fUseCharged=ch; }
  void SetRCharged(double r, bool ch){ set_R( r); SetUseCharged(ch); }
  JETS get_jets() { return fJets; }
  JETS get_BgSubstracted_jets() { return fBgSubstractedJets; }
  JETS get_SubArea4vectors() { return fSubArea4vectors; }
  fastjet::ClusterSequenceArea* get_ClusterSequenceArea(){ return fClust_seq_area; };
  fastjet::ClusterSequence* get_ClusterSequence(){ return fClust_seq; };
  //fastjet::JetDefinition get_JetDefinition(){ return jet_def; };
  //fastjet::JetDefinition::Plugin * get_plugin() { return plugin; };
  TClonesArray& RecoJets( TObjArray * inputlist,  vector<int>*sublist);
  int GetJetIDofTrack(int trackid);
  AliJJet * GetMotherJet( AliJBaseTrack* tk );
  void PrintConfig(const char *message) const;
  double get_rho() { return frho; };
  double get_sigma() { return fsigma; };
  vector<int> GetConstituents(int jetid);
  vector<int> GetConstituentsSimpleCone() { return IndexArrayOfJetConstituentsSC; };
  void Clear();

  TObjArray * GetJetList(){ return &fJetList; }
  vector<int> * GetJetTrackMap(){ return &fJetTrackMap; }

 private:
  bool fEnabled;
  JetAlg  fJetalg;
  bool 	fJetTrimming;
  bool	fuseArea;
  double fR;
  double fpTMin;
  double fJetEtaRange;
  double fInputPtMin;
  int fIndexLP;//for simple Cone alg
  JETS fInput_particles;
  JETS fJets;
  JETS fBgSubstractedJets;
  JETS fSubArea4vectors;
  fastjet::ClusterSequence *fClust_seq;
  fastjet::ClusterSequenceArea *fClust_seq_area;
  //fastjet::JetDefinition jet_def;
  //fastjet::JetDefinition::Plugin *plugin;
  //fastjet::AreaDefinition fArea_def;
  TObjArray *fInputList;
  vector<int> * fSubList;
  TClonesArray fJetList;
  vector<int> fJetTrackMap;
  vector<int> IndexArrayOfJetConstituentsSC;

  bool fUseCharged;
  double frho;
  double fsigma;


};


#endif
