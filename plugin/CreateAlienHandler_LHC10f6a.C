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
  plugin->SetGridDataDir("/alice/sim/LHC10f6a/");
  ///alice/sim/2013/LHC10f6a/195346/AOD/066/
  plugin->SetDataPattern("/AOD041/*AliAOD.root");

  // run list
  // from: 
  // https://aliceinfo.cern.ch/Notes/sites/aliceinfo.cern.ch.Notes/files/notes/analysis/jotwinow/2013-Jun-17-analysis_note-ppReferencePaper_Note_170613.pdf

plugin->AddRunNumber(122374); 
/* 
plugin->AddRunNumber(122375); 
plugin->AddRunNumber(124751); 
plugin->AddRunNumber(125023); 
plugin->AddRunNumber(125085); 
plugin->AddRunNumber(125097); 
plugin->AddRunNumber(125100); 
plugin->AddRunNumber(125101); 
plugin->AddRunNumber(125134); 
plugin->AddRunNumber(125296); 
plugin->AddRunNumber(125628);
plugin->AddRunNumber(125630); 
plugin->AddRunNumber(125632); 
plugin->AddRunNumber(125633); 
plugin->AddRunNumber(125842); 
plugin->AddRunNumber(125843); 
plugin->AddRunNumber(125844); 
plugin->AddRunNumber(125847); 
plugin->AddRunNumber(125848); 
plugin->AddRunNumber(125849); 
plugin->AddRunNumber(125850); 
plugin->AddRunNumber(125851);
plugin->AddRunNumber(125855); 
plugin->AddRunNumber(126004); 
plugin->AddRunNumber(126007); 
plugin->AddRunNumber(126008); 
plugin->AddRunNumber(126073); 
plugin->AddRunNumber(126078); 
plugin->AddRunNumber(126081); 
plugin->AddRunNumber(126082); 
plugin->AddRunNumber(126088); 
plugin->AddRunNumber(126090); 
plugin->AddRunNumber(126097);
plugin->AddRunNumber(126158); 
plugin->AddRunNumber(126160); 
plugin->AddRunNumber(126168); 
plugin->AddRunNumber(126283); 
plugin->AddRunNumber(126284); 
plugin->AddRunNumber(126285); 
plugin->AddRunNumber(126359); 
plugin->AddRunNumber(126403); 
plugin->AddRunNumber(126404); 
plugin->AddRunNumber(126405); 
plugin->AddRunNumber(126406);
plugin->AddRunNumber(126407); 
plugin->AddRunNumber(126408); 
plugin->AddRunNumber(126409); 
plugin->AddRunNumber(126422); 
plugin->AddRunNumber(126424); 
plugin->AddRunNumber(126425); 
plugin->AddRunNumber(126432);
*/


  plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");
  //plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");

  //  plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

  //========================================
  // Set Ouput Information  
  //========================================
  plugin->SetGridWorkingDir("Efficiency/LHC10f6a_v5-05-42-AN_r2087_AOD041");
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
