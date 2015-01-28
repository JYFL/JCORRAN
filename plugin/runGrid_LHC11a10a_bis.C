const Bool_t IsMC = kTRUE; //With real data kMC = kFALSE
const TString kInputData = "ESD";
const TString kJCORRANInputFormat = "ESD"; // ESD, AODout, AODin

//_____________________________________________________________________
void runGrid_LHC11a10a_bis(){
    // Load Custom Configuration and parameters
    // override values with parameters

    UInt_t triggerSelection;

    //==== Load common libraries
    gSystem->Load("libCore.so");  
    gSystem->Load("libTree.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libVMC.so");
    gSystem->Load("libXMLIO.so");
    gSystem->Load("libPhysics.so");
    gSystem->Load("libCDB");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libSTEER");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    gSystem->Load("libANALYSIS"); 
    gSystem->Load("libANALYSISalice");   
    gSystem->Load("libEMCALbase");
    gSystem->Load("libEMCALUtils");
    gSystem->Load("libEMCALrec");
    gSystem->Load("libPHOSUtils");
    gSystem->Load("libTRDbase");
    gSystem->Load("libTENDER.so");   
    gSystem->Load("libTENDERSupplies.so"); 

    //==== Load Ours
    SetupPar("PWG4JCORRAN"); 
    //	SetupPar("JKEMCAL"); 

    // Use AliRoot includes to compile our task
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/EMCAL");
    gROOT->ProcessLine(".include $ALICE_ROOT/ANALYSIS/");
    gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT/include -I$ALICE_ROOT/EMCAL -I$ALICE_ROOT/ANALYSIS ");

    // Create and configure the alien handler plugin
    gROOT->LoadMacro("CreateAlienHandler_LHC11a10a_bis.C");
    AliAnalysisGrid *alienHandler = CreateAlienHandler();  
    if (!alienHandler) return;
    // Create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("JCHII");

    // Connect plug-in to the analysis manager
    mgr->SetGridHandler(alienHandler);

    // MC handler
    if(IsMC){
        AliMCEventHandler* mcHandler = new AliMCEventHandler();
        mcHandler->SetReadTR(kFALSE);//Do not search TrackRef file
        mgr->SetMCtruthEventHandler(mcHandler);
    }

    if(kInputData == "ESD"){
        // ESD handler
        AliESDInputHandler *esdHandler = new AliESDInputHandler();
        mgr->SetInputEventHandler(esdHandler);
    }
    if(kInputData == "AOD"){
        // AOD handler
        AliAODInputHandler *aodHandler = new AliAODInputHandler();
        mgr->SetInputEventHandler(aodHandler);
    }

    //================================================
    // TASKS
    //================================================

    // set the trigger selection
    triggerSelection =  AliVEvent::kMB
        | AliVEvent::kINT7;
    //										| AliVEvent::kHighMult 
    //										| AliVEvent::kEMCEGA;
    //										| AliVEvent::kEMCEJE
    //										| AliVEvent::kEMC1
    //										| AliVEvent::kEMC7
    //                    | AliVEvent::kCentral
    //                    | AliVEvent::kSemiCentral; 
    if(IsMC) triggerSelection = AliVEvent::kAny; 

    //   //==== EMCal tender
    //   if(kInputData == "ESD"){
    //  	// get reco params from grid OCDB
    // 		gROOT->LoadMacro("./GetOCDBRecParam.C");
    //  	// run num, data type pp/PbPb, from grid
    // // 	AliEMCALRecParam* pars = GetOCDBRecParam( 146801, "pp", kTRUE );
    // 		AliEMCALRecParam* pars = GetOCDBRecParam( 168342, "PbPb", kTRUE );
    // 		
    // 		// make necessary mods
    // 		pars->SetTimeCut(25e-9);// s
    // 		pars->SetTimeMin(-20e-9);
    // 		pars->SetTimeMax(20e-9);//s
    // 
    // 		gROOT->ProcessLine(".L ./AddTaskEMCALTender.C");
    //     // geometry EMCAL_COMPLETEV1 or EMCAL_FIRSTYEARV1;
    // 		// AliEMCALRecParam, either full object or 0 for auto OCDB load
    //     AliTender *tender = AddTaskEMCALTender( "EMCAL_COMPLETEV1", pars );
    //     tender->SetDefaultCDBStorage("raw://"); //uncomment if you work on grid
    //     //tender->SetDefaultCDBStorage("local://$ALICE_ROOT/OCDB"); //uncomment if you work local
    //     // one can sellect what collision candidates to use
    //     // triggered sample only: L1 = AliVEvent::kEMCEGA, AliVEvent::kEMCEJE; L0 = AliVEvent::kEMC1, AliVEvent::kEMC7
    //     tender->SelectCollisionCandidates( triggerSelection );
    //   }

    //==== PID
    //     gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    //     AddTaskPIDResponse(IsMC);  
    //     gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
    //     AddTaskPIDqa();
    // 
    //==== Statistics
    mgr->AddStatisticsTask();

    //==== CENTRALITY
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskCentrality.C");
    AliCentralitySelectionTask *taskCentrality = AddTaskCentrality(); 
    taskCentrality->SelectCollisionCandidates( triggerSelection );
    if(IsMC)   taskCentrality->SetMCInput();
    //  taskCentrality->SetPass(2);

    //==== Physics Selection
    gROOT->ProcessLine(".L $ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask *physSelTask = AddTaskPhysicsSelection(IsMC, kTRUE);
    physSelTask->SelectCollisionCandidates( triggerSelection );

    // 	//==== EMCAL QA class
    // 	gROOT->LoadMacro("./AddTaskJKEMCALQA.C");
    // 	AliAnalysisTaskJKEMCALQA *emcalqa = AddTaskJKEMCALQA();
    // 	emcalqa->SelectCollisionCandidates( triggerSelection );


    //============================
    //   JCORRANTask
    //===========================

    //==== JCORRAN TASK
    AliJCORRANTask *jctask = new AliJCORRANTask("PWG4JCORRANTask",kJCORRANInputFormat);

    // Additional track cuts
    // ATTENTION!!! - always check that the parameters used in
    //   AddTaskESDfilter.C  and the AOD production launcher script are compatible
    //   especially for the year parameter and enableTPCOnlyAODTracks
    // Also check the bit composition for changes with new ALIROOT versions
    //  ... and remember that any AOD was produced with a past ALIROOT version
    gROOT->ProcessLine(".L ./AddESDFilter.C");
    AliAnalysisFilter *filter = AddESDFilter( jctask, kTRUE, 2010 );
    jctask->SetOutputAODName("jcorran.root");
    jctask->SetDebugLevel(0);

    AliJRunHeader * hdr = new AliJRunHeader;
    hdr->SetIsMC( true );
    hdr->SetBeamTypeI( 1 ); // 0:pp 1:PbPb
    hdr->SetWithoutSPD(false);
    hdr->SetRunType("LHC11a10a_bis");
    hdr->SetInputFormat( 0 ); // 0: ESD;
    hdr->SetRefitESDVertexTracks(kTRUE);
    hdr->SetStoreEventPlaneSource(false);
    hdr->SetStoreTPCTrackBitMask( 2^30-1 );
    hdr->SetStoreGCGTrackBitMask( 2^30-1 );
    hdr->SetStoreEMCalInfo( kFALSE );

    jctask->SetJRunHeader( hdr );

    AliJFilter *jfilter = jctask->GetFilter();
    jfilter->SetAliJRunHeader( hdr );
    jfilter->SetClusterThreshold( 0 );
    jfilter->SetTrackThreshold( 0 );

    //==event selection
    jctask->SelectCollisionCandidates( triggerSelection );  //Apply offline trigger selection by AliPhysicsSelectionTask

    mgr->AddTask((AliAnalysisTask*) jctask);

    //==== Create containers for input/output
    AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
    AliAODHandler* aodoutHandler   = new AliAODHandler();
    aodoutHandler->SetCreateNonStandardAOD();
    mgr->SetOutputEventHandler(aodoutHandler);

    // Connect input/output
    AliAnalysisDataContainer *runinfoOutput = mgr->CreateContainer("RunInfo",  TList::Class(), AliAnalysisManager::kOutputContainer, "jcorran.root");
    AliAnalysisDataContainer *treeOutput = mgr->CreateContainer("JODTree",  TTree::Class(), AliAnalysisManager::kOutputContainer, "jcorran.root");
    AliAnalysisDataContainer *effHist = mgr->CreateContainer("EffHist",  TDirectory::Class(), AliAnalysisManager::kOutputContainer, "jcorranEff.root");
    mgr->ConnectInput(jctask, 0, cinput);
    mgr->ConnectOutput(jctask, 1, treeOutput );
    mgr->ConnectOutput(jctask, 2, runinfoOutput );
    mgr->ConnectOutput(jctask, 3, effHist );

    //===============================
    // START MANAGER
    //===============================

    // Enable debug printouts
    mgr->SetDebugLevel(0);

    if (!mgr->InitAnalysis())
        return;

    mgr->PrintStatus();
    // Start analysis in grid.
    printf("********** LD_LIBRARY_PATH *******\n");
    printf("%s\n", gSystem->Getenv("LD_LIBRARY_PATH"));
    printf("**********************************\n");


    //  printf("%s\n", gSystem->Getenv("LD_LIBRARY_PATH"));

    mgr->StartAnalysis("grid");
};




//========================================================================
// Setup Par Files
//========================================================================
void SetupPar(char* pararchivename)
{
    //Load par files, create analysis libraries
    //For testing, if par file already decompressed and modified
    //classes then do not decompress.

    TString cdir(Form("%s", gSystem->WorkingDirectory() )) ;
    TString parpar(Form("%s.par", pararchivename)) ;
    // create par file if it not exist
    if ( gSystem->AccessPathName(parpar.Data()) ) {
        gSystem->ChangeDirectory(gSystem->Getenv("ALICE_ROOT")) ;
        TString processline(Form(".! make %s", parpar.Data())) ;
        gROOT->ProcessLine(processline.Data()) ;
        gSystem->ChangeDirectory(cdir) ;
        processline = Form(".! mv /tmp/%s .", parpar.Data()) ;
        gROOT->ProcessLine(processline.Data()) ;
    }
    // decompres par file
    if ( gSystem->AccessPathName(pararchivename) ) {
        TString processline = Form(".! tar xvzf %s",parpar.Data()) ;
        gROOT->ProcessLine(processline.Data());
    }

    TString ocwd = gSystem->WorkingDirectory();
    gSystem->ChangeDirectory(pararchivename);

    // check for BUILD.sh and execute
    if (!gSystem->AccessPathName("PROOF-INF/BUILD.sh")) {
        printf("*******************************\n");
        printf("*** Building PAR archive    ***\n");
        cout<<pararchivename<<endl;
        printf("*******************************\n");

        if (gSystem->Exec("PROOF-INF/BUILD.sh")) {
            Error("runProcess","Cannot Build the PAR Archive! - Abort!");
            return -1;
        }
    }
    // check for SETUP.C and execute
    if (!gSystem->AccessPathName("PROOF-INF/SETUP.C")) {
        printf("*******************************\n");
        printf("*** Setup PAR archive       ***\n");
        cout<<pararchivename<<endl;
        printf("*******************************\n");
        gROOT->Macro("PROOF-INF/SETUP.C");
    }

    gSystem->ChangeDirectory(ocwd.Data());
    printf("Current dir: %s\n", ocwd.Data());

}


void LoadConf( TString filename ){
    ifstream in(filename.Data());
    TString str;
    str.ReadFile(in);
    str.ReplaceAll("\n",  "");
    cout<<str<<endl;

    gROOT->ProcessLine( str.Data() );
}

