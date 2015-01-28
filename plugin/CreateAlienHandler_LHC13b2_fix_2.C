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

  plugin->SetNtestFiles(5); // Relevant only for run mode "test" 

  //========================================
  // Set versions of used packages
  //========================================
  plugin->SetAPIVersion("V1.1x");
  plugin->SetROOTVersion("v5-34-07");
    //plugin->SetAliROOTVersion("v5-04-02-AN");
    plugin->SetAliROOTVersion("v5-04-61-AN");
    //plugin->SetAliROOTVersion("v5-04-03pid-AN");

  //========================================
  // Declare input data to be processed.
  //========================================
	// plugin->SetRunPrefix("000");   // real data
	plugin->SetGridDataDir("/alice/sim/2013/LHC13b2_fix_2");
	plugin->SetDataPattern("*AliESDs.root");

	// runs
	plugin->AddRunNumber(195344);
	plugin->AddRunNumber(195346);
	plugin->AddRunNumber(195351);
	plugin->AddRunNumber(195389);
	plugin->AddRunNumber(195390);
	plugin->AddRunNumber(195391);
	plugin->AddRunNumber(195478);
	plugin->AddRunNumber(195479);
	plugin->AddRunNumber(195480);
	plugin->AddRunNumber(195481);
	plugin->AddRunNumber(195482);
	plugin->AddRunNumber(195483);
	plugin->AddRunNumber(195529);
	plugin->AddRunNumber(195531);
	plugin->AddRunNumber(195566);
	plugin->AddRunNumber(195567);
	plugin->AddRunNumber(195568);
	plugin->AddRunNumber(195592);
	plugin->AddRunNumber(195593);
	plugin->AddRunNumber(195596);
	plugin->AddRunNumber(195633);
	plugin->AddRunNumber(195635);
	plugin->AddRunNumber(195644);
	plugin->AddRunNumber(195673);
	plugin->AddRunNumber(195675);
	plugin->AddRunNumber(195677);


	plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");
	//plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");

	//  plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

	//========================================
	// Set Ouput Information  
	//========================================
	plugin->SetGridWorkingDir("Efficiency/LHC13b2_fix_2_r1529");
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
	plugin->SetSplitMaxInputFileNumber(50);
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
