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

  plugin->SetNtestFiles(1); // Relevant only for run mode "test" 

  //========================================
  // Set versions of used packages
  //========================================
  plugin->SetAPIVersion("V1.1x");
  plugin->SetROOTVersion("v5-34-02-1");
    plugin->SetAliROOTVersion("v5-04-02-AN");
    //plugin->SetAliROOTVersion("v5-04-03pid-AN");

  //========================================
  // Declare input data to be processed.
  //========================================

    // LHC10e
  plugin->SetRunPrefix("000");   // real data
  plugin->SetGridDataDir("/alice/data/2010/LHC10e");
  plugin->SetDataPattern("*ESDs/pass2/*/AliESDs.root");
  
  // runs
  
  plugin->AddRunNumber(127712);
  plugin->AddRunNumber(127714);
  plugin->AddRunNumber(127718);
  plugin->AddRunNumber(127822);
  plugin->AddRunNumber(127933);
  plugin->AddRunNumber(127935);
  plugin->AddRunNumber(127936);
  plugin->AddRunNumber(127937);
  plugin->AddRunNumber(127940);
  plugin->AddRunNumber(127941);
  plugin->AddRunNumber(127942);
  plugin->AddRunNumber(128185);
  plugin->AddRunNumber(128186);
  plugin->AddRunNumber(128189);
  plugin->AddRunNumber(128191);
  
 // plugin->AddRunNumber(128192);
 // plugin->AddRunNumber(128260);
 //plugin->AddRunNumber(128366);
 //plugin->AddRunNumber(128452);
 //plugin->AddRunNumber(128483);
 //plugin->AddRunNumber(128486);
 //plugin->AddRunNumber(128494);
 //plugin->AddRunNumber(128495);
 //plugin->AddRunNumber(128503);
 //plugin->AddRunNumber(128504);
 //plugin->AddRunNumber(128507);
 //plugin->AddRunNumber(128582);
//plugin->AddRunNumber(128605);
//plugin->AddRunNumber(128609);
//plugin->AddRunNumber(128611);
//plugin->AddRunNumber(128615);
//plugin->AddRunNumber(128677);
//plugin->AddRunNumber(128678);
//plugin->AddRunNumber(128777);
//plugin->AddRunNumber(128778);
//plugin->AddRunNumber(128820);
//plugin->AddRunNumber(128823);
//plugin->AddRunNumber(128824);
//plugin->AddRunNumber(128835);
//plugin->AddRunNumber(128836);
//plugin->AddRunNumber(128843);
//plugin->AddRunNumber(128853);
//plugin->AddRunNumber(128855);
//plugin->AddRunNumber(128913);
//plugin->AddRunNumber(129512);
//plugin->AddRunNumber(129513);
//plugin->AddRunNumber(129514);
//plugin->AddRunNumber(129520);
//plugin->AddRunNumber(129523);
//plugin->AddRunNumber(129527);
//plugin->AddRunNumber(129528);
//plugin->AddRunNumber(129540);
//plugin->AddRunNumber(129586);
//plugin->AddRunNumber(129587);
//plugin->AddRunNumber(129599);
//plugin->AddRunNumber(129639);
//plugin->AddRunNumber(129641);
//plugin->AddRunNumber(129647);
//plugin->AddRunNumber(129650);
//plugin->AddRunNumber(129652);
//plugin->AddRunNumber(129653);
//plugin->AddRunNumber(129654);
//plugin->AddRunNumber(129659);
//plugin->AddRunNumber(129666);
//plugin->AddRunNumber(129723);
//plugin->AddRunNumber(129725);
// plugin->AddRunNumber(129726);
// plugin->AddRunNumber(129729);
// plugin->AddRunNumber(129735);
// plugin->AddRunNumber(129736);
// plugin->AddRunNumber(129738);
// plugin->AddRunNumber(129742);
// plugin->AddRunNumber(129744);
// plugin->AddRunNumber(129959);
// plugin->AddRunNumber(129960);
// plugin->AddRunNumber(129961);
// plugin->AddRunNumber(129983);
// plugin->AddRunNumber(130149);
// plugin->AddRunNumber(130157);
// plugin->AddRunNumber(130158);
// plugin->AddRunNumber(130172);
// plugin->AddRunNumber(130178);
// plugin->AddRunNumber(130179);
// plugin->AddRunNumber(130342);
// plugin->AddRunNumber(130343);
// plugin->AddRunNumber(130354);
// plugin->AddRunNumber(130356);
// plugin->AddRunNumber(130480);
// plugin->AddRunNumber(130517);
// plugin->AddRunNumber(130519);
// plugin->AddRunNumber(130696);
// plugin->AddRunNumber(130704);
// plugin->AddRunNumber(130793);
// plugin->AddRunNumber(130795);
// plugin->AddRunNumber(130798);
// plugin->AddRunNumber(130799);
// plugin->AddRunNumber(130834);  
  
  // end run list
	
  plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");
//plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");

//  plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

  //========================================
  // Set Ouput Information  
  //========================================
//  plugin->SetGridWorkingDir("work/production/11a_pass2_without_SDD");
//  plugin->SetGridWorkingDir("work/production/11h_pass2_AOD095_EMCal_set2");
  //plugin->SetGridWorkingDir("work/productionT02/data/LHC11a");
  plugin->SetGridWorkingDir("work/productionT02/sim/LHC11b10a");
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
