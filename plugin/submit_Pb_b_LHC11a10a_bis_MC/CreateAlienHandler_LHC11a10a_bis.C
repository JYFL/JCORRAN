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

  plugin->SetNtestFiles(6); // Relevant only for run mode "test" 

  //========================================
  // Set versions of used packages
  //========================================
  plugin->SetAPIVersion("V1.1x");
  plugin->SetROOTVersion("v5-34-07");
  plugin->SetAliROOTVersion("v5-04-61-AN");

  //========================================
  // Declare input data to be processed.
  //========================================

    // LHC10e
 // plugin->SetRunPrefix("000");   // real data
    plugin->SetGridDataDir("/alice/sim/LHC11a10a_bis");
    plugin->SetDataPattern("*ESDs.root");
  
	plugin->AddRunNumber(137162);
/*
	plugin->AddRunNumber(137161);
	plugin->AddRunNumber(137231);
	plugin->AddRunNumber(137232);
	plugin->AddRunNumber(137235);
	plugin->AddRunNumber(137236);
	plugin->AddRunNumber(137243);
	plugin->AddRunNumber(137366);
	plugin->AddRunNumber(137430);
	plugin->AddRunNumber(137431);
	plugin->AddRunNumber(137432);
	plugin->AddRunNumber(137434);
	plugin->AddRunNumber(137439);
	plugin->AddRunNumber(137440);
	plugin->AddRunNumber(137441);
	plugin->AddRunNumber(137443);
	plugin->AddRunNumber(137530);
	plugin->AddRunNumber(137531);
	plugin->AddRunNumber(137539);
	plugin->AddRunNumber(137541);
	plugin->AddRunNumber(137544);
	plugin->AddRunNumber(137546);
	plugin->AddRunNumber(137549);
	plugin->AddRunNumber(137595);
	plugin->AddRunNumber(137608);
	plugin->AddRunNumber(137638);
	plugin->AddRunNumber(137639);
	plugin->AddRunNumber(137685);
	plugin->AddRunNumber(137686);
	plugin->AddRunNumber(137691);
	plugin->AddRunNumber(137692);
	plugin->AddRunNumber(137693);
	plugin->AddRunNumber(137704);
	plugin->AddRunNumber(137718);
	plugin->AddRunNumber(137722);
	plugin->AddRunNumber(137724);
	plugin->AddRunNumber(137751);
	plugin->AddRunNumber(137752);
	plugin->AddRunNumber(137844);
	plugin->AddRunNumber(137848);
	plugin->AddRunNumber(138190);
	plugin->AddRunNumber(138192);
	plugin->AddRunNumber(138197);
	plugin->AddRunNumber(138201);
	plugin->AddRunNumber(138225);
	plugin->AddRunNumber(138275);
	plugin->AddRunNumber(138364);
	plugin->AddRunNumber(138396);
	plugin->AddRunNumber(138438);
	plugin->AddRunNumber(138439);
	plugin->AddRunNumber(138442);
	plugin->AddRunNumber(138469);
	plugin->AddRunNumber(138534);
	plugin->AddRunNumber(138578);
	plugin->AddRunNumber(138579);
	plugin->AddRunNumber(138582);
	plugin->AddRunNumber(138583);
	plugin->AddRunNumber(138621);
	plugin->AddRunNumber(138624);
	plugin->AddRunNumber(138638);
	plugin->AddRunNumber(138652);
	plugin->AddRunNumber(138653);
	plugin->AddRunNumber(138662);
	plugin->AddRunNumber(138666);
	plugin->AddRunNumber(138730);
	plugin->AddRunNumber(138732);
	plugin->AddRunNumber(138837);
	plugin->AddRunNumber(138870);
	plugin->AddRunNumber(138871);
	plugin->AddRunNumber(138872);
	plugin->AddRunNumber(139028);
	plugin->AddRunNumber(139029);
	plugin->AddRunNumber(139036);
	plugin->AddRunNumber(139037);
	plugin->AddRunNumber(139038);
	plugin->AddRunNumber(139105);
	plugin->AddRunNumber(139107);
	plugin->AddRunNumber(139173);
	plugin->AddRunNumber(139309);
	plugin->AddRunNumber(139310);
	plugin->AddRunNumber(139314);
	plugin->AddRunNumber(139328);
	plugin->AddRunNumber(139329);
	plugin->AddRunNumber(139360);
	plugin->AddRunNumber(139437);
	plugin->AddRunNumber(139438);
	plugin->AddRunNumber(139465);
	plugin->AddRunNumber(139503);
	plugin->AddRunNumber(139505);
	plugin->AddRunNumber(139507);
	plugin->AddRunNumber(139510);
*/
  
  // end run list
	
  plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWGJCORRAN.par");
//plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWGJCORRAN.par");

//  plugin->SetAdditionalLibs("PWGJCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

  //========================================
  // Set Ouput Information  
  //========================================
  plugin->SetGridWorkingDir("PROD10/PbPb/sim/LHC11a10a_bis");
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
