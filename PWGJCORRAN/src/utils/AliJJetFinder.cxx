#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
#include<iostream> // needed for io
#include<sstream>  // needed for internal io
#include<vector> 
#include <cstdio>
#include "AliJJetFinder.h"
#include "AliJBaseTrack.h"
#include "AliJJet.h"

#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/JetDefinition.hh"
// get info on how fastjet was configured
//#ifdef ENABLE_PLUGIN_SISCONE
#  include "fastjet/SISConePlugin.hh"
//#endif
#ifdef ENABLE_PLUGIN_CDFCONES
#  include "fastjet/CDFMidPointPlugin.hh"
#  include "fastjet/CDFJetCluPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_PXCONE
#  include "fastjet/PxConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_D0RUNIICONE
#  include "fastjet/D0RunIIConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_TRACKJET
#include "fastjet/TrackJetPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_ATLASCONE
#include "fastjet/ATLASConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_EECAMBRIDGE
#include "fastjet/EECambridgePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_JADE
#include "fastjet/JadePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_CMSITERATIVECONE
#include "fastjet/CMSIterativeConePlugin.hh"
#endif
using namespace std;


AliJJetFinder::AliJJetFinder( JETS& input_particles, JetAlg fJetalg, bool jettrimming, bool usearea) :
    fEnabled(1), 
    fJetalg( fJetalg ),
    fJetTrimming( jettrimming ),
    fuseArea( usearea ),
    fR(0.8),
    fpTMin(0),
    fInputPtMin(0.5),
    fIndexLP(-1),
    fInput_particles(input_particles),
    fJets(0),
    fBgSubstractedJets(0),
    fSubArea4vectors(0),
    fClust_seq(NULL),
    fClust_seq_area(NULL),
    //plugin(NULL),
    fInputList(NULL),
    fJetList("AliJJet",1000)
{
}

AliJJetFinder::AliJJetFinder( AliJBaseCard *card ) :
    fEnabled(1), 
    fJetalg(kantikt),
    fJetTrimming(false),
    fuseArea(true),
    fR(0.8),
    fpTMin(0),
    fInputPtMin(0.5),
    fIndexLP(-1),
    fInput_particles(0),
    fJets(0),
    fBgSubstractedJets(0),
    fSubArea4vectors(0),
    fClust_seq(NULL),
    fClust_seq_area(NULL),
    //plugin(NULL),
    fInputList(NULL),
    fJetList("AliJJet",1000)
{
    //BS//TODO set as Card info
    fEnabled = int( card->Get("Jet:Enable"));
    fJetalg = JetAlg(int( card->Get("Jet:Algorithm")));
    fJetTrimming = int( card->Get("Jet:Trimming" ));
    fuseArea = int( card->Get("Jet:UseArea" ));
    fR		= float( card->Get("Jet:Radius"));
    fpTMin		= double( card->Get("Jet:JetPtMin"));
    fInputPtMin	= double( card->Get("Jet:ParticlePtMin"));
    fJetEtaRange  = double( card->Get("Jet:EtaRange") );
    frho = -999;
    fsigma = -999;
}

AliJJetFinder::~AliJJetFinder(){
    if( fClust_seq ) delete fClust_seq;
    if( fClust_seq_area ) delete fClust_seq_area;
    //if( plugin ) delete plugin;
    fJetList.Delete();
}

void AliJJetFinder::Clear() {
    fJets.clear();
    fBgSubstractedJets.clear();
    fSubArea4vectors.clear();
    fInput_particles.clear();
    fJetList.Clear();
    fJetTrackMap.clear();
    IndexArrayOfJetConstituentsSC.clear();
    //if( plugin ) { delete plugin;plugin=NULL; }
    if( fClust_seq ){ delete fClust_seq;fClust_seq=NULL; }
    if( fClust_seq_area ){ delete fClust_seq_area;fClust_seq_area=NULL; }
}

TClonesArray& AliJJetFinder::RecoJets( TObjArray * inputlist, vector<int>*sublist=NULL){
    Clear(); // clear list befor doing the reco jet and also delete obj created in previous event
    // Take the input list
    fInputList = inputlist;
    if( !fEnabled ) return fJetList;

    int nTracks = 0;
    // Check sublist
    if( sublist ){
        fSubList = sublist;
        nTracks = sublist->size();
    }else{
        fSubList = NULL;
        nTracks = inputlist->GetEntriesFast() ;
    }

    if( fJetalg == kSimpleCone ){
        int indexLP = fIndexLP;// TODO
        TLorentzVector lLP = ((AliJBaseTrack*)fInputList->At(indexLP))->GetLorentzVector();
        TLorentzVector lSum = lLP;
        for( int i=0;i<nTracks;i++ ){
            int itrack = sublist?(*sublist)[i]:i;
            if( itrack == indexLP ) continue;
            TLorentzVector l = ((AliJBaseTrack*)fInputList->At(itrack))->GetLorentzVector();
            if( lLP.DeltaR( l ) > fR ) continue;
            lSum += l; 
            IndexArrayOfJetConstituentsSC.push_back(i);
        }
        fJetList.Clear();
        new ( fJetList[0] ) AliJBaseTrack( lSum ); 	
        return fJetList;
    } 

    // Make Input list for Jet Reconstruction
    for( int i=0;i<nTracks;i++ ){
        int itrack = sublist?(*sublist)[i]:i;
        // skip if not charged
        if( fUseCharged && (!((AliJBaseTrack*)fInputList->At(itrack))->GetCharge()) ) continue;
        TLorentzVector l = ((AliJBaseTrack*)fInputList->At(itrack))->GetLorentzVector();
        if(l.Pt() < fInputPtMin ) continue;
        //l.Print();
        fInput_particles.push_back( fastjet::PseudoJet(l.Px(),l.Py(),l.Pz(),l.P())); // Ptot instead of E
        fInput_particles[fInput_particles.size()-1].set_user_index(itrack);
    }

    if( fInput_particles.size() < 1 ) return fJetList;

    find();

    fJetList.Clear();
    JETS & jets = fBgSubstractedJets;
    fJetTrackMap.resize( fInputList->GetEntriesFast() );
    for( unsigned int i=0;i< jets.size();i++ ){
        fastjet::PseudoJet & j = jets[i];
        j.set_user_index(i);
        if( j.px() < 1e-3 && j.py()<1e-3 && j.pz()<1e-3 ) {
            //cout<<"WARN zero JET !! "<< i<<endl;
        }
        AliJJet jet( j.px(), j.py(), j.pz(), j.e(), -1, -1, 0 );
        jet.SetUniqueID( i );
        jet.SetID(i);
        jet.SetArea( j.area() );
        double eta = jet.Eta();
        JETS constituents;
        if(fuseArea) {
            constituents = fClust_seq_area->constituents(jets[i]);
        } else {
            constituents = fClust_seq->constituents(jets[i]);
        }
        for( unsigned int ic=0;ic< constituents.size();ic++ ){
            int index = constituents.at(ic).user_index();
            jet.AddConstituent( fInputList->At(index)) ;
            fJetTrackMap[index] = i;
        }
        new ( fJetList[i] ) AliJJet( jet );
    }
    return fJetList;
}



AliJJetFinder::JETS AliJJetFinder::find () {
    // 1.kt,2.antikt,3.siscone 4.recomE 5.recomB
    // common parameters
    //double overlap_threshold = 0.5;
  fastjet::JetDefinition jet_def;
  fastjet::JetDefinition::Plugin *plugin;
  fastjet::AreaDefinition fArea_def;

    // Check jet Algorithm
    if(fJetalg==kkt) {
        jet_def = fastjet::JetDefinition (fastjet::kt_algorithm, fR);
    } 
    if(fJetalg==kantikt) {
        jet_def = fastjet::JetDefinition (fastjet::antikt_algorithm, fR, E_scheme, Best);
    } 
    if(fJetalg==ksiscone) {
        double overlap_threshold = 0.5;
        int npass = 0;               // do infinite number of passes
        double protojet_fpTMin = 0.0; // use all protojets
        plugin =new fastjet::SISConePlugin (fR, overlap_threshold, npass, protojet_fpTMin);
        jet_def = fastjet::JetDefinition (plugin);
    } 
    if(fJetalg==krecomE) {
        fastjet::Strategy strategy = fastjet::Best;
        fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
        jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, fR, recomb_scheme, strategy);
    }
    if(fJetalg==krecomB) {
        fastjet::Strategy strategy = fastjet::Best;
        fastjet::RecombinationScheme recomb_scheme = fastjet::BIpt_scheme;
        jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, fR, recomb_scheme, strategy);
    }
    /*
       if(fJetalg==kGF) {
       plugin =new fastjet::GaussianFilterPlugin (0.5, 600, -3.141592, 3.141592, 600, -5, 5, 1);
       jet_def = fastjet::JetDefinition (plugin);
       fuseArea = false;
       }
       */

    if(fJetTrimming) {
        /* bschang trimming
           plugin= new fastjet::QCDTrimmingFast(&jet_def);
           ((fastjet::QCDTrimmingFast*)plugin)->SetRsubPtfrac(0.2, 0.001);
           jet_def = fastjet::JetDefinition (plugin);
           */
    }

    // create an object that specifies how we to define the area
    if( fuseArea ){
        // create an area definition for the clustering
        //----------------------------------------------------------
        // ghosts should go up to the acceptance of the detector or
        // (with infinite acceptance) at least 2R beyond the region
        // where you plan to investigate jets.
        double ghost_maxrap = 1.1;
        GhostedAreaSpec area_spec(ghost_maxrap);
        AreaDefinition area_def(active_area, area_spec);

        // run the jet clustering with the above jet and area definitions
        // for full event
        // ----------------------------------------------------------
        fClust_seq_area = new fastjet::ClusterSequenceArea(fInput_particles, jet_def, area_def);
        //fJets = sorted_by_E(fClust_seq_area->inclusive_jets(fpTMin));
        fJets = sorted_by_pt(fClust_seq_area->inclusive_jets(fpTMin));
        // ----------------------------------------------------------
        double ghost_area    = 0.005;// ALICE paper
        int    active_area_repeats = 1;

        // now create the object that holds info about ghosts
        fastjet::GhostedAreaSpec ghost_spec(ghost_maxrap, active_area_repeats, ghost_area);
        JetDefinition jet_def_bkgd(kt_algorithm, 0.6);
        AreaDefinition area_def_bkgd(active_area_explicit_ghosts, ghost_spec);
        double rap_max = ghost_maxrap - fR;
        Selector selector = SelectorAbsRapMax(rap_max) * (!SelectorNHardest(2));
        JetMedianBackgroundEstimator bkgd_estimator(selector, jet_def_bkgd, area_def_bkgd);

        // To help manipulate the background estimator, we also provide a
        // transformer that allows to apply directly the background
        // subtraction on the jets. This will use the background estimator
        // to compute rho for the jets to be subtracted.
        // ----------------------------------------------------------
        Subtractor subtractor(&bkgd_estimator);

        // Finally, once we have an event, we can just tell the background
        // estimator to use that list of particles
        // This could be done directly when declaring the background
        // estimator but the usage below can more easily be accomodated to a
        // loop over a set of events.
        // ----------------------------------------------------------
        bkgd_estimator.set_particles(fInput_particles);
        // get the subtracted jets
        Selector selector_ptmin = SelectorPtMin( fpTMin );
        fBgSubstractedJets = sorted_by_pt(selector_ptmin(subtractor(fJets)));

        frho = bkgd_estimator.rho();
        fsigma = bkgd_estimator.sigma();

        fSubArea4vectors.resize(fJets.size());
        for (size_t j = 0; j < fJets.size(); j++) {
            PseudoJet area_4vect = fJets[j].area_4vector();
            if (area_4vect.perp2() >= fJets[j].perp2() ||
                    area_4vect.E()     >= fJets[j].E()) {
                // if the correction is too large, set the jet to zero
                area_4vect = 0.0 * area_4vect;
                fSubArea4vectors[j] = frho*area_4vect;
            } else {
                // otherwise do an E-scheme subtraction
                fSubArea4vectors[j] = frho*area_4vect;
            }
            //cout << fBgSubstractedJets[j].perp2() <<"\t"<< area_4vect.perp2() << endl;
        }
    }else{
        fClust_seq = new fastjet::ClusterSequence(fInput_particles, jet_def);
        vector<fastjet::PseudoJet> inclusive_jets = fClust_seq->inclusive_jets(fpTMin);
        fJets = sorted_by_pt(inclusive_jets);
    }

	fJets = fBgSubstractedJets;
    return fJets;

}

int AliJJetFinder::GetJetIDofTrack(int trackid){
    int maxindex = fJetTrackMap.size()-1;
    if(trackid > maxindex  || trackid < 0 ) return -2;
    return fJetTrackMap[trackid]; 
}

AliJJet * AliJJetFinder::GetMotherJet( AliJBaseTrack* tk ){
    int jetid = GetJetIDofTrack( tk->GetID() );
    if( jetid < 0 ) return NULL;
    return (AliJJet*) fJetList[jetid];
}


vector<int> AliJJetFinder::GetConstituents(int jetid){
    JETS constituents;
    constituents = fJets[jetid].constituents();
    vector<int > IndexArrayOfJetConstituents;
    IndexArrayOfJetConstituents.clear();
    if(fJetalg == kSimpleCone) {
        return IndexArrayOfJetConstituentsSC;
    } else {
        for( unsigned int ic=0;ic< constituents.size();ic++ ){
            IndexArrayOfJetConstituents.push_back(constituents[ic].user_index());
        }
        return IndexArrayOfJetConstituents;
    }
}

void AliJJetFinder::PrintConfig(const char *message=" " ) const{
    //object print out
    cout<<message << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "Jet alg was: " << kJetAlgStrName[fJetalg]  << endl;
    if( fJetalg != kSimpleCone ) {
        //cout << "Jet definition was: " << jet_def.description() << endl;
        //cout << "Area definition was: " << fArea_def.description() << endl;
        //    cout << "Strategy adopted by FastJet was "<< fClust_seq->strategy_string()<<endl<<endl;
    } else {
        cout << "Min pt of paricle = "<< fpTMin << endl;
        cout << "Leading particle Index = "<< fIndexLP << endl;
        cout << "Vector sum of particles radius to Leading particle R = "<< fR << endl;	
    }
    cout << "--------------------------------------------------------" << endl;

}


