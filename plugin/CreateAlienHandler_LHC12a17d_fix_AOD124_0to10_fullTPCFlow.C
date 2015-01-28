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
	plugin->SetRunMode("test"); 

	plugin->SetNtestFiles(5); // Relevant only for run mode "test" 

	//========================================
	// Set versions of used packages
	//========================================
	plugin->SetAPIVersion("V1.1x");
	plugin->SetROOTVersion("v5-34-11");
	plugin->SetAliROOTVersion("v5-05-42-AN");

	//========================================
	// Declare input data to be processed.
	//========================================
	// plugin->SetRunPrefix("000");   // real data
	plugin->SetGridDataDir("/alice/sim/2012/LHC12a17d_fix");
	plugin->SetDataPattern("/AOD124/*AliAOD.root");

	// runs
	plugin->AddRunNumber(167915);
/*
	plugin->AddRunNumber(167920);
	plugin->AddRunNumber(167985);
	plugin->AddRunNumber(167987);
	plugin->AddRunNumber(167988);
	plugin->AddRunNumber(168069);
	plugin->AddRunNumber(168076);
	plugin->AddRunNumber(168105);
	plugin->AddRunNumber(168107);
	plugin->AddRunNumber(168108);
	plugin->AddRunNumber(168115);
	plugin->AddRunNumber(168310);
	plugin->AddRunNumber(168311);
	plugin->AddRunNumber(168322);
	plugin->AddRunNumber(168325);
	plugin->AddRunNumber(168341);
	plugin->AddRunNumber(168342);
	plugin->AddRunNumber(168361);
	plugin->AddRunNumber(168362);
	plugin->AddRunNumber(168458);
	plugin->AddRunNumber(168460);
	plugin->AddRunNumber(168464);
	plugin->AddRunNumber(168467);
	plugin->AddRunNumber(168511);
	plugin->AddRunNumber(168512);
	plugin->AddRunNumber(168514);
	plugin->AddRunNumber(168777);
	plugin->AddRunNumber(168826);
	plugin->AddRunNumber(168988);
	plugin->AddRunNumber(168992);
	plugin->AddRunNumber(169035);
	plugin->AddRunNumber(169040);
	plugin->AddRunNumber(169044);
	plugin->AddRunNumber(169045);
	plugin->AddRunNumber(169091);
	plugin->AddRunNumber(169094);
	plugin->AddRunNumber(169099);
	plugin->AddRunNumber(169138);
	plugin->AddRunNumber(169144);
	plugin->AddRunNumber(169145);
	plugin->AddRunNumber(169148);
	plugin->AddRunNumber(169156);
	plugin->AddRunNumber(169160);
	plugin->AddRunNumber(169167);
	plugin->AddRunNumber(169238);
	plugin->AddRunNumber(169411);
	plugin->AddRunNumber(169415);
	plugin->AddRunNumber(169417);
	plugin->AddRunNumber(169418);
	plugin->AddRunNumber(169419);
	plugin->AddRunNumber(169420);
	plugin->AddRunNumber(169475);
	plugin->AddRunNumber(169498);
	plugin->AddRunNumber(169504);
	plugin->AddRunNumber(169506);
	plugin->AddRunNumber(169512);
	plugin->AddRunNumber(169515);
	plugin->AddRunNumber(169550);
	plugin->AddRunNumber(169553);
	plugin->AddRunNumber(169554);
	plugin->AddRunNumber(169555);
	plugin->AddRunNumber(169557);
	plugin->AddRunNumber(169586);
	plugin->AddRunNumber(169587);
	plugin->AddRunNumber(169588);
	plugin->AddRunNumber(169590);
	plugin->AddRunNumber(169591);
	plugin->AddRunNumber(169835);
	plugin->AddRunNumber(169837);
	plugin->AddRunNumber(169838);
	plugin->AddRunNumber(169846);
	plugin->AddRunNumber(169855);
	plugin->AddRunNumber(169858);
	plugin->AddRunNumber(169859);
	plugin->AddRunNumber(169923);
	plugin->AddRunNumber(169965);
	plugin->AddRunNumber(170027);
	plugin->AddRunNumber(170040);
	plugin->AddRunNumber(170081);
	plugin->AddRunNumber(170083);
	plugin->AddRunNumber(170084);
	plugin->AddRunNumber(170085);
	plugin->AddRunNumber(170088);
	plugin->AddRunNumber(170089);
	plugin->AddRunNumber(170091);
	plugin->AddRunNumber(170155);
	plugin->AddRunNumber(170159);
	plugin->AddRunNumber(170163);
	plugin->AddRunNumber(170193);
	plugin->AddRunNumber(170203);
	plugin->AddRunNumber(170204);
	plugin->AddRunNumber(170207);
	plugin->AddRunNumber(170228);
	plugin->AddRunNumber(170230);
	plugin->AddRunNumber(170268);
	plugin->AddRunNumber(170269);
	plugin->AddRunNumber(170270);
	plugin->AddRunNumber(170306);
	plugin->AddRunNumber(170308);
	plugin->AddRunNumber(170309);
	plugin->AddRunNumber(170311);
	plugin->AddRunNumber(170312);
	plugin->AddRunNumber(170313);
	plugin->AddRunNumber(170315);
	plugin->AddRunNumber(170387);
	plugin->AddRunNumber(170388);
	plugin->AddRunNumber(170572);
	plugin->AddRunNumber(170593);
*/


	plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");
	//plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");

	//  plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

	//========================================
	// Set Ouput Information  
	//========================================
	plugin->SetGridWorkingDir("Efficiency/LHC12a17d_fix_AOD124_0to10_fullTPCFlow_v5-05-42-AN_r2110");
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
