#include "JParticleTools.h"
#include <set>

extern double DeltaPhi(double phi1, double phi2);
using namespace std;


JParticleTools::JParticleTools(AliJBaseCard *incard, AliJJetFinder *intriggJet):
    card(incard), 
    triggJet(intriggJet),
    indexLP(-1),
    hasLP(false),
    LeadingParticle(NULL)
{
    fAllList = NULL;
    TrackEtaRange = card->Get("EtaRange");
    TriggerFiducialEtaCut  = card->Get("TriggerFiducialEtaCut");
    fRecoJetWLP=NULL;
    fRecoLeadingJet=NULL;
    maxEtaRange   =  incard->Get("EtaRange");
    triggFiducCut =  incard->Get("TriggerFiducialEtaCut"); //FK// Fiduc cut 
    maxTriggEtaRange =  maxEtaRange - triggFiducCut; //FK// Trigger range
    random = new TRandom3(); 
    random->SetSeed(0); 
}

void JParticleTools::Clear() {
    indexLP = -1;
    hasLP = false;
    LeadingParticle = NULL;
    triggJet->Clear();
    fRecoJetWLP=NULL;
    fRecoLeadingJet=NULL;
}

void JParticleTools::Prepare(TClonesArray *inputList){ 
    fAllList = inputList;
    IndexArrayOfInputParticles.clear();
    IndexArrayOfAssocParticles.clear();
    PointerArrayOfAssocParticles.Clear();
    for(int i = 0; i < fAllList->GetEntriesFast(); i++) {
        IndexArrayOfInputParticles.push_back(i);
        AliJBaseTrack  *track = (AliJBaseTrack*)fAllList->At(i);
        PointerArrayOfAssocParticles.AddLast( track ); // for mixed event pool
    }
    hasLP = FindLParticle();
}

JParticleTools::~JParticleTools(){
}

//--------------------------------------------------------
//--- Search for a leading particle and parent partons ---
//--------------------------------------------------------
bool JParticleTools::FindLParticle(){ 

    float pT_max = -1.;     //maximal pT in the event
    indexLP      = -1;      //index of the leading particle
    double eta = -999;
    //search the leading particle
    for(int i = 0; i < fAllList->GetEntriesFast(); i++){ 
        AliJBaseTrack  *track = (AliJBaseTrack*)fAllList->At(i);
        double pt = track->Pt();
        double ieta = track->Eta();
        if(pt > pT_max ){  //Find the leading particle in the event
            indexLP  = i;
            pT_max = pt; 
            eta = ieta;
        }
    }

    if(indexLP>-1 &&   (fabs(eta) < (TrackEtaRange - TriggerFiducialEtaCut)) ) {
        LeadingParticle = (AliJBaseTrack*)fAllList->At(indexLP);
        return true;
    } else {
        return false; 
    }
}

int JParticleTools::JetSubstractedAssocList() {
    PointerArrayOfUEParticles.Clear();
    set<int> constituentsAll;
    vector<fastjet::PseudoJet> fJets;
    fJets = triggJet->get_jets();
    for(unsigned int j=0;j<fJets.size();j++) {
        vector<PseudoJet> constituents = fJets[j].constituents();
        for( unsigned int icon=0;icon<constituents.size();icon++ ){
            constituentsAll.insert(constituents[icon].user_index());//TODO
        }
    }
    for( unsigned int iassoc=0;iassoc<IndexArrayOfInputParticles.size();iassoc++){
        if( constituentsAll.count( iassoc ) == 0 ){
            AliJBaseTrack  *track = (AliJBaseTrack*)fAllList->At(IndexArrayOfInputParticles[iassoc]);
            PointerArrayOfUEParticles.Add( track );
        }
    }
    return PointerArrayOfUEParticles.GetEntries();
}

//--------------------------------------------------------
//--- Reconstuct Jets ---
//--------------------------------------------------------
void JParticleTools::ReconstructJets(){ 
    fJetConstituents.clear();
    if(fAllList->GetEntriesFast()>0) {
        if(triggJet->get_JetAlg() == kSimpleCone) triggJet->set_LeadingPaticleIndex(indexLP);
        TClonesArray &jetList = triggJet->RecoJets(fAllList, &IndexArrayOfInputParticles);
        if(hasLP && jetList.GetEntriesFast() > 0 ) {
            int jetid = triggJet->GetJetIDofTrack(indexLP);
            if(triggJet->get_JetAlg() == kSimpleCone) {
                fRecoJetWLP = (TLorentzVector*)jetList[0];
                fRecoLeadingJet = (TLorentzVector*)jetList[0];
            } else {
                fRecoJetWLP = jetid<0?NULL:(TLorentzVector*)jetList[jetid];
                fRecoLeadingJet = jetList[0]?(TLorentzVector*)jetList[0]:NULL;
            }
            if(triggJet->get_JetAlg() == kSimpleCone) {
                fJetConstituents = triggJet->GetConstituentsSimpleCone();
            } else {
                if ( jetid >= 0 ) { 
                    vector<fastjet::PseudoJet> fJets = triggJet->get_jets();
                    vector<fastjet::PseudoJet> constituents;
                    vector<fastjet::PseudoJet> constituentsLeadingJet;
                    constituents = fJets[jetid].constituents();
                    constituentsLeadingJet = fJets[0].constituents();
                    for( unsigned int ic=0;ic< constituents.size();ic++ ){
                        fJetConstituents.push_back(constituents[ic].user_index());
                    }
                    fLeadingJetConstituents.clear();
                    for( unsigned int ic=0;ic< constituentsLeadingJet.size();ic++ ){
                        fLeadingJetConstituents.push_back(constituentsLeadingJet[ic].user_index());
                    }
                }
            }
        } // for LP
    }
}

void JParticleTools::PrintJets(){ 
    if(fRecoJetWLP) { 
        cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        triggJet->PrintConfig("JParticleTools") ;  fRecoJetWLP->Print(); fRecoLeadingJet->Print();
    }  else {
        triggJet->PrintConfig("JParticleTools");
        cout<<"No Jet found for leading particle.."<<endl;
    }
}
