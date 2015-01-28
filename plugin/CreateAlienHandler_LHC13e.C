AliAnalysisGrid* CreateAlienHandler()
{
	// Check if user has a valid token, otherwise make one. This has limitations.
	// One can always follow the standard procedure of calling alien-token-init then
	//   source /tmp/gclient_env_$UID in the current shell.
	//   if (!AliAnalysisGrid::CreateToken()) return NULL;
	AliAnalysisAlien *plugin = new AliAnalysisAlien();
	plugin->SetOverwriteMode();

	//========================================
	// SET RUN MODE :  "full",   "test",   "offline",   "submit" or "terminate"
	//========================================
	plugin->SetRunMode("full"); 

	plugin->SetNtestFiles(1); // Relevant only for run mode "test" 

	//========================================
	// Set versions of used packages
	//========================================
	plugin->SetAPIVersion("V1.1x");
	plugin->SetROOTVersion("v5-34-08");
	plugin->SetAliROOTVersion("v5-05-36-AN");

	//========================================
	// Declare input data to be processed.
	//========================================

	// set 1 runs
	///alice/data/2013/LHC13b/000195346/ESDs/pass1_wSDD
	plugin->SetGridDataDir("/alice/data/2013/LHC13e");
	//	 plugin->SetDataPattern("ESDs/pass3/*/AliESDs.root");
	plugin->SetDataPattern("/pass2/ AOD/*AOD.root");
	///alice/data/2013/LHC13b/000195344/ESDs/pass3/AOD139/
	//AOD126
	plugin->SetRunPrefix("000");   // real data

	plugin->AddRunNumber(195955);
	plugin->AddRunNumber(195958);
	plugin->AddRunNumber(196085);
	plugin->AddRunNumber(196089);
	plugin->AddRunNumber(196090);
	plugin->AddRunNumber(196091);
	plugin->AddRunNumber(196107);
	plugin->AddRunNumber(196185);
	plugin->AddRunNumber(196187);
	plugin->AddRunNumber(196194);
	plugin->AddRunNumber(196199);
	plugin->AddRunNumber(196200);
	plugin->AddRunNumber(196201);
	plugin->AddRunNumber(196214);
	plugin->AddRunNumber(196308);
	plugin->AddRunNumber(196309);
	plugin->AddRunNumber(196310);


	// end run list

	plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");
	//plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");

	//  plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

	//========================================
	// Set Ouput Information  
	//========================================
	plugin->SetGridWorkingDir("work/production/LHC13e_pass2_AOD_v5-34-08-AN_r2023");
	plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
	plugin->SetOutputToRunNo();
	plugin->SetDefaultOutputs(kFALSE);
	//plugin->SetPreferedSE("ALICE::NDGF::DCACHE");
	//plugin->SetOutputArchive("log_archive.zip:std*@disk=1 root_archive.zip:jcorran.root,AnalysisResults.root,EventStat_temp.root,*.stat@disk=2");
	plugin->SetOutputArchive("log_archive.zip:std*@disk=1 root_archive.zip:*.root,*stat,*.xml@disk=2");
	plugin->SetKeepLogs(kTRUE);
	plugin->SetOutputFiles("jcorran.root");
	plugin->SetTerminateFiles("event_stat.root");
	// plugin->SetOutputSingleFolder("output");

	//========================================
	// Optional
	//========================================
	//FK//   plugin->SetAnalysisSource("AliJCORRANTask.cxx");
	plugin->SetAnalysisMacro("TaskJC.C");
	plugin->SetSplitMaxInputFileNumber(20);
	plugin->SetExecutable("TaskJC.sh");
	//  plugin->SetExecutableCommand("export MALLOC_CHECK_=0 ; root -b -q");
	// Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
	//   plugin->SetMaxInitFailed(5);
	// Optionally resubmit threshold.
	plugin->SetMasterResubmitThreshold(95);
	// Optionally set time to live (default 30000 sec)
	plugin->SetTTL(10000);
	// Optionally set input format (default xml-single)
	plugin->SetInputFormat("xml-single");
	// Optionally modify the name of the generated JDL (default analysis.jdl)
	plugin->SetJDLName("TaskJC.jdl");
	// Optionally modify job price (default 1)
	plugin->SetPrice(1);      
	// Optionally modify split mode (default 'se')    
	plugin->SetSplitMode("se");


	return plugin;
}
