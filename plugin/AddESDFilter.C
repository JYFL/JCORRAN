AliAnalysisFilter *AddESDFilter( AliJCORRANTask* jctask, Bool_t enableTPCOnlyAODTracks = kTRUE, Int_t year = 2010 )
{
	AliAnalysisFilter* trackFilter = 0;
	
	AliJFilter *jfilter = jctask->GetFilter();

	Bool_t enableTPCOnlyAODTracksLocalFlag = enableTPCOnlyAODTracks;

	if( year == 2010 ){
		// Cuts on primary tracks
		AliESDtrackCuts* esdTrackCutsL = AliESDtrackCuts::GetStandardTPCOnlyTrackCuts();
		
		// ITS stand-alone tracks
		AliESDtrackCuts* esdTrackCutsITSsa = new AliESDtrackCuts("ITS stand-alone Track Cuts", "ESD Track Cuts");
		esdTrackCutsITSsa->SetRequireITSStandAlone(kTRUE);
		
		// Pixel OR necessary for the electrons
		AliESDtrackCuts *itsStrong = new AliESDtrackCuts("ITSorSPD", "pixel requirement for ITS");
		itsStrong->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
		
		
		// PID for the electrons
		AliESDpidCuts *electronID = new AliESDpidCuts("Electrons", "Electron PID cuts");
		electronID->SetTPCnSigmaCut(AliPID::kElectron, 3.);
		
		// tighter cuts on primary particles for high pT tracks
		// take the standard cuts, which include already 
		// ITSrefit and use only primaries...
		
		// ITS cuts for new jet analysis 
		//  gROOT->LoadMacro("$ALICE_ROOT/PWGJE/macros/CreateTrackCutsPWGJE.C");
		//  AliESDtrackCuts* esdTrackCutsHG0 = CreateTrackCutsPWGJE(10001006);

		AliESDtrackCuts *jetCuts1006 = new AliESDtrackCuts("AliESDtrackCuts"); 

		TFormula *f1NClustersTPCLinearPtDep = new TFormula("f1NClustersTPCLinearPtDep","70.+30./20.*x");
		jetCuts1006->SetMinNClustersTPCPtDep(f1NClustersTPCLinearPtDep,20.);
		jetCuts1006->SetMinNClustersTPC(70);
		jetCuts1006->SetMaxChi2PerClusterTPC(4);
		jetCuts1006->SetRequireTPCStandAlone(kTRUE); //cut on NClustersTPC and chi2TPC Iter1
		jetCuts1006->SetAcceptKinkDaughters(kFALSE);
		jetCuts1006->SetRequireTPCRefit(kTRUE);
		jetCuts1006->SetMaxFractionSharedTPCClusters(0.4);
		// ITS
		jetCuts1006->SetRequireITSRefit(kTRUE);
		//accept secondaries
		jetCuts1006->SetMaxDCAToVertexXY(2.4);
		jetCuts1006->SetMaxDCAToVertexZ(3.2);
		jetCuts1006->SetDCAToVertex2D(kTRUE);
		//reject fakes
		jetCuts1006->SetMaxChi2PerClusterITS(36);
		jetCuts1006->SetMaxChi2TPCConstrainedGlobal(36);

		jetCuts1006->SetRequireSigmaToVertex(kFALSE);

		jetCuts1006->SetEtaRange(-0.9,0.9);
		jetCuts1006->SetPtRange(0.15, 1E+15.);

		AliESDtrackCuts* esdTrackCutsHG0 = jetCuts1006->Clone("JetCuts10001006");
		esdTrackCutsHG0->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);


		// throw out tracks with too low number of clusters in
		// the first pass (be consistent with TPC only tracks)
		// N.B. the number off crossed rows still acts on the tracks after
		// all iterations if we require tpc standalone, number of clusters
		// and chi2 TPC cuts act on track after the first iteration
		//   esdTrackCutsH0->SetRequireTPCStandAlone(kTRUE);
		//   esdTrackCutsH0->SetMinNClustersTPC(80); // <--- first pass
		
		
		// the complement to the one with SPD requirement
		//  AliESDtrackCuts* esdTrackCutsHG1 = CreateTrackCutsPWGJE(10011006);
		AliESDtrackCuts* esdTrackCutsHG1 = jetCuts1006->Clone("JetCuts10011006");
		esdTrackCutsHG1->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kNone);

		// the tracks that must not be taken pass this cut and
		// non HGC1 and HG
		//  AliESDtrackCuts* esdTrackCutsHG2 = CreateTrackCutsPWGJE(10021006);
		AliESDtrackCuts* esdTrackCutsHG2 = jetCuts1006->Clone("JetCuts10021006");
		esdTrackCutsHG2->SetMaxChi2PerClusterITS(1E10);


		// standard cuts also used in R_AA analysis
		//   "Global track RAA analysis QM2011 + Chi2ITS<36";
		//  AliESDtrackCuts* esdTrackCutsH2 = CreateTrackCutsPWGJE(1000);
		AliESDtrackCuts* esdTrackCutsH2 = AliESDtrackCuts::GetStandardITSTPCTrackCuts2010(kTRUE,1);
		esdTrackCutsH2->SetMinNCrossedRowsTPC(120);
		esdTrackCutsH2->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
		esdTrackCutsH2->SetMaxChi2PerClusterITS(36);
		esdTrackCutsH2->SetMaxFractionSharedTPCClusters(0.4);
		esdTrackCutsH2->SetMaxChi2TPCConstrainedGlobal(36);

		esdTrackCutsH2->SetEtaRange(-0.9,0.9);
		esdTrackCutsH2->SetPtRange(0.15, 1e10);


		//  AliESDtrackCuts* esdTrackCutsGCOnly = CreateTrackCutsPWGJE(10041006);
		AliESDtrackCuts* esdTrackCutsGCOnly = jetCuts1006->Clone("JetCuts10041006");
		esdTrackCutsGCOnly->SetRequireITSRefit(kFALSE);



		// TPC only tracks
		AliESDtrackCuts* esdTrackCutsTPCCOnly = AliESDtrackCuts::GetStandardTPCOnlyTrackCuts();
		esdTrackCutsTPCCOnly->SetMinNClustersTPC(70);
		
		// Compose the filter
		trackFilter = new AliAnalysisFilter("trackFilter");
		// 1, 1<<0
		trackFilter->AddCuts(esdTrackCutsL);
		// 2 1<<1
		trackFilter->AddCuts(esdTrackCutsITSsa);
		// 4 1<<2
		trackFilter->AddCuts(itsStrong);
		itsStrong->SetFilterMask(1);        // AND with Standard track cuts 
		// 8 1<<3
		trackFilter->AddCuts(electronID);
		electronID->SetFilterMask(4);       // AND with Pixel Cuts
		// 16 1<<4
		trackFilter->AddCuts(esdTrackCutsHG0);
		// 32 1<<5
		trackFilter->AddCuts(esdTrackCutsHG1);
		// 64 1<<6
		trackFilter->AddCuts(esdTrackCutsHG2);
		// 128 1<<7
		trackFilter->AddCuts(esdTrackCutsTPCCOnly); // add QM TPC only track cuts
		// 256 1<<8
		trackFilter->AddCuts(esdTrackCutsGCOnly);
		// 512 1<<9                         
		AliESDtrackCuts* esdTrackCutsHG1_tmp = new AliESDtrackCuts(*esdTrackCutsHG1); // avoid double delete
		trackFilter->AddCuts(esdTrackCutsHG1_tmp); // add once more for tpc only tracks
		// 1024 1<<10                        
		trackFilter->AddCuts(esdTrackCutsH2); // add r_aa cuts

        // 2048 1<<11
        AliESDtrackCuts* tpc2A  = (AliESDtrackCuts*) esdTrackCutsTPCCOnly->Clone("tpc2A");
        tpc2A->SetMinNCrossedRowsTPC(120);
        tpc2A->SetMaxChi2PerClusterTPC(4);
        tpc2A->SetAcceptKinkDaughters(kFALSE);
        tpc2A->SetMaxDCAToVertexZ(2);
        tpc2A->SetMaxDCAToVertexXY(2.4);
        tpc2A->SetDCAToVertex2D(kTRUE);
        tpc2A->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8); 
        tpc2A->SetMaxChi2TPCConstrainedGlobal(36);
        trackFilter->AddCuts(tpc2A);

        // 1<<12
        AliESDtrackCuts* tpcPlus = (AliESDtrackCuts*) tpc2A->Clone("tpcPlus");
        tpcPlus->SetMaxDCAToVertexXYPtDep("0.04+0.264/pt^1.832");
        trackFilter->AddCuts( tpcPlus );
        
        // 1<<13 RAA for 2011?
        AliESDtrackCuts* esdTrackCutsH22011 = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011();
        trackFilter->AddCuts( esdTrackCutsH22011 );

        // HYBRID 2011
        // bit 8 in 2011
		// Extra cuts for hybrids
		// first the global tracks we want to take
		AliESDtrackCuts* esdTrackCutsHTG = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(kFALSE); 
		esdTrackCutsHTG->SetName("Global Hybrid tracks, loose DCA");
		esdTrackCutsHTG->SetMaxDCAToVertexXY(2.4);
		esdTrackCutsHTG->SetMaxDCAToVertexZ(3.2);
		esdTrackCutsHTG->SetDCAToVertex2D(kTRUE);
		esdTrackCutsHTG->SetMaxChi2TPCConstrainedGlobal(36);
        // 1<<14
        trackFilter->AddCuts( esdTrackCutsHTG );
		
        // bit 9 in 2011
		// Than the complementary tracks which will be stored as global
		// constraint, complement is done in the ESDFilter task
		AliESDtrackCuts* esdTrackCutsHTGC = new AliESDtrackCuts(*esdTrackCutsHTG);
		esdTrackCutsHTGC->SetName("Global Constraint Hybrid tracks, loose DCA no it requirement");
		esdTrackCutsHTGC->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kOff);
		esdTrackCutsHTGC->SetRequireITSRefit(kFALSE);
        // 1<<15
        trackFilter->AddCuts( esdTrackCutsHTGC );


        // bit 5 in 2011
		// standard cuts with tight DCA cut
		AliESDtrackCuts* esdTrackCutsH22011 = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011();
        // 1 << 16
        trackFilter->AddCuts( esdTrackCutsH22011 );


        // bit 6 in 2011
		AliESDtrackCuts* esdTrackCutsH32011 = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(); 
		esdTrackCutsH32011->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kNone);
		esdTrackCutsH32011->SetClusterRequirementITS(AliESDtrackCuts::kSDD, AliESDtrackCuts::kFirst);
        // 1 << 17
        trackFilter->AddCuts( esdTrackCutsH22011 );

		//     esdfilter->SetTPCConstrainedFilterMask(1<<11); // these tracks are written out as tpc constrained tracks

		jfilter->SetTrackFilter(trackFilter);
	}
	else if( year == 2011 ){
		// Cuts on primary tracks
		AliESDtrackCuts* esdTrackCutsL = AliESDtrackCuts::GetStandardTPCOnlyTrackCuts();

		// ITS stand-alone tracks
		AliESDtrackCuts* esdTrackCutsITSsa = new AliESDtrackCuts("ITS stand-alone Track Cuts", "ESD Track Cuts");
		esdTrackCutsITSsa->SetRequireITSStandAlone(kTRUE);

		// Pixel OR necessary for the electrons
		AliESDtrackCuts *itsStrong = new AliESDtrackCuts("ITSorSPD", "pixel requirement for ITS");
		itsStrong->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);


		// PID for the electrons
		AliESDpidCuts *electronID = new AliESDpidCuts("Electrons", "Electron PID cuts");
		electronID->SetTPCnSigmaCut(AliPID::kElectron, 3.);

		// standard cuts with very loose DCA
		AliESDtrackCuts* esdTrackCutsH = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(kFALSE); 
		esdTrackCutsH->SetMaxDCAToVertexXY(2.4);
		esdTrackCutsH->SetMaxDCAToVertexZ(3.2);
		esdTrackCutsH->SetDCAToVertex2D(kTRUE);

		// standard cuts with tight DCA cut
		AliESDtrackCuts* esdTrackCutsH2 = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011();

		// standard cuts with tight DCA but with requiring the first SDD cluster instead of an SPD cluster
		// tracks selected by this cut are exclusive to those selected by the previous cut
		AliESDtrackCuts* esdTrackCutsH3 = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(); 
		esdTrackCutsH3->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kNone);
		esdTrackCutsH3->SetClusterRequirementITS(AliESDtrackCuts::kSDD, AliESDtrackCuts::kFirst);
	
		// TPC only tracks: Optionally enable the writing of TPConly information
		// constrained to SPD vertex in the filter below
		AliESDtrackCuts* esdTrackCutsTPCOnly = AliESDtrackCuts::GetStandardTPCOnlyTrackCuts();
		// The following line is needed for 2010 PbPb reprocessing and pp, but not for 2011 PbPb
		//esdTrackCutsTPCOnly->SetMinNClustersTPC(70);

		// Extra cuts for hybrids
		// first the global tracks we want to take
		AliESDtrackCuts* esdTrackCutsHTG = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(kFALSE); 
		esdTrackCutsHTG->SetName("Global Hybrid tracks, loose DCA");
		esdTrackCutsHTG->SetMaxDCAToVertexXY(2.4);
		esdTrackCutsHTG->SetMaxDCAToVertexZ(3.2);
		esdTrackCutsHTG->SetDCAToVertex2D(kTRUE);
		esdTrackCutsHTG->SetMaxChi2TPCConstrainedGlobal(36);
		
		// Than the complementary tracks which will be stored as global
		// constraint, complement is done in the ESDFilter task
		AliESDtrackCuts* esdTrackCutsHTGC = new AliESDtrackCuts(*esdTrackCutsHTG);
		esdTrackCutsHTGC->SetName("Global Constraint Hybrid tracks, loose DCA no it requirement");
		esdTrackCutsHTGC->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kOff);
		esdTrackCutsHTGC->SetRequireITSRefit(kFALSE);

		// standard cuts with tight DCA cut, using cluster cut instead of crossed rows (a la 2010 default)
		AliESDtrackCuts* esdTrackCutsH2Cluster = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(kTRUE, 0);

		// Compose the filter
		trackFilter = new AliAnalysisFilter("trackFilter");
		// 1, 1<<0
		trackFilter->AddCuts(esdTrackCutsL);
		// 2, 1<<1
		trackFilter->AddCuts(esdTrackCutsITSsa);
		// 4, 1<<2
		trackFilter->AddCuts(itsStrong);
		itsStrong->SetFilterMask(1);        // AND with Standard track cuts 
		// 8, 1<<3
		trackFilter->AddCuts(electronID);
		electronID->SetFilterMask(4);       // AND with Pixel Cuts
		// 16, 1<<4
		trackFilter->AddCuts(esdTrackCutsH);
		// 32, 1<<5
		trackFilter->AddCuts(esdTrackCutsH2);
		// 64, 1<<6
		trackFilter->AddCuts(esdTrackCutsH3);
		// 128 , 1 << 7
		trackFilter->AddCuts(esdTrackCutsTPCOnly);
		// 256, 1 << 8 Global Hybrids
		trackFilter->AddCuts(esdTrackCutsHTG);
		// 512, 1<< 9 GlobalConstraint Hybrids
		trackFilter->AddCuts(esdTrackCutsHTGC);
		// 1024, 1<< 10 // tight DCA cuts
		trackFilter->AddCuts(esdTrackCutsH2Cluster);
		jfilter->SetTrackFilter(trackFilter);

	}
	else{
		cout << "============ ERROR = ESD filter run with wrong year " << endl;
	}

  return trackFilter;
}

