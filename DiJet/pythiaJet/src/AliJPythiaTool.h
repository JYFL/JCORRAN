#ifndef ALIJPYTHIATOOL_H
#define ALIJPYTHIATOOL_H

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <Pythia.h>
#include <TClonesArray.h>
#include <TSystem.h>
#include "AliJBaseTrack.h"
#include "AliJJet.h"

double Eta( TLorentzVector & v);
class AliJPythiaTool {
    public:
        typedef vector<int>       INTS;
        typedef map<int,INTS>     INTSMAP;
        typedef map<int,INTSMAP>  INTSMAP2;
        enum { kJNormalPythia, kJResornancePythia, kNJPythiaMode };
        AliJPythiaTool( Pythia8::Event & event, int pymode=kJNormalPythia );
        void ScanPartons();

        //==== 0~3 : Shower Group
        //====      0,1 : Beam Jet CLASS1
        //====      2,3 : OutGoing Jet CLASS1
        //==== 4~  : RF Group ( StringGroup in CLASS2 )
        //==== 0~3, hadron group is empty. 
        TObjArray       * GetAllParticle(){ return &fParticle; }
        AliJBaseTrack   * GetParticle(int i ){ return (AliJBaseTrack*)fParticle[i]; }

        //TObjArray * GetFinalPartonList(){ return &fFinalPartonList; }
        TObjArray * GetDiJet(int type);
        TClonesArray * GetPartonGroup(){ return &fPartonGroup; }
        TClonesArray * GetPartonOrgGroup(){ return &fPartonOrgGroup; }

        void SetPythiaMode(int i ){ fPythiaMode = i; }
        void PrintEvent();

        void CreateHistos();

        const vector<double>& GetPartonGroupProbability( int index );

        //vector< vector<int> >  * GetPartonGroup(){ return fPartonGroup } 


        // Utils
        void SumTest(int m1, int m2, int d1, int d2);
        int Compare( TLorentzVector & t3, TLorentzVector & t2, double tol );
        int Compare( double x, double y, double tol);

        double fDPhi;

        double GetParton23Pt(){ return (*fEvent)[fIdOf23[0]].pT() ; } 



    private:
        void ScanFinalParton( vector<int> *vec , int particleID);
        void UnExpectedError( int error, TString str, int ip, int org );

        Pythia8::Event *fEvent;
        TClonesArray fParticle;

        int fPythiaMode;

        vector<int> fSeenScanFinalParton;


        //==== 0,1 : Beam Jet CLASS1
        //==== 2,3 : OutGoing Jet CLASS1
        //==== 4~  : StringGroup in CLASS2

        INTSMAP2      fPartonGroupMap;

        TClonesArray fPartonGroup;
        TClonesArray fPartonOrgGroup;


        int fIdOf23[2];// 5,6 for normal pythia, 6,7 for resornance
        int fEventStatus;

        // HISTOS
        TH1D * fParton23Pt;
        TH1D * fParton23Eta;

        // (0,1):beam jet (2,3):outgoing (4):CLASS2
        TH1D * fhPartonJetPt[5];
        TH1D * fhPartonJetPhi[5][20];
        TH1D * fhPartonJetEta[5][20];
        TH2D * fhPartonJetDeltaEtaDeltaPhi[5][20];

        // (0):beam Jet  (1):outgoing  (2):DiJet of CLASS2
        TH1D * fhPartonJetPtPair[3];
        TH2D * fhPartonJetDPhiPt[3];
        TH2D * fhPartonJetDPhiE[3];
        TH1D * fhPartonJetDPhi[3][20];
        TH1D * fhPartonJetAssym[3][20];

        TH1D * fhStringGroupMultiplicity;
        TH2D * fhJetMassCor;
        TH1D * fhPythiaDiJetMass;
        TH2D * fhPythiaPtPairComp;



};

#endif 
