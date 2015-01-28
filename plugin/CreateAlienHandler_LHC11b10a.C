AliAnalysisGrid* CreateAlienHandler()
{
  // Check if user has a valid token, otherwise make one. This has limitations.
  // One can always follow the standard procedure of calling alien-token-init then
  //   source /tmp/gclient_env_$UID in the current shell.
  //   if (!AliAnalysisGrid::CreateToken()) return NULL;
  AliAnalysisAlien *plugin = new AliAnalysisAlien();
  plugin->SetOverwriteMode();

  //========================================
  // SET RUN MODE :  "full",   "est",   "offline",   "submit" or "terminate"
  //========================================
  plugin->SetRunMode("test"); 

  plugin->SetNtestFiles(1); // Relevant only for run mode "test" 

  //========================================
  // Set versions of used packages
  //========================================
  plugin->SetAPIVersion("V1.1x");
  plugin->SetROOTVersion("v5-34-07");
  //plugin->SetAliROOTVersion("v5-04-61-AN");
  plugin->SetAliROOTVersion("v5-03-Rev-23");

  //========================================
  // Declare input data to be processed.
  //========================================

// plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146746.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146747.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146748.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146801.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146802.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146803.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146804.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146805.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146806.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146807.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146817.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146824.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146856.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146858.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146859.xml");
  //plugin->AddDataFile("/alice/cern.ch/user/b/bschang/xmlCor2/LHC11b10a/146860.xml");
  //
  //p/lugin->SetRunPrefix("000");   // real data
  //plugin->SetGridDataDir("/alice/sim/LHC11b10a");
  //plugin->SetDataPattern("AOD116/*/AliAOD.root");
  //plugin->AddRunNumber(146747);
  plugin->SetGridDataDir("/alice/sim/LHC11b10a");
  plugin->SetDataPattern("AliESDs.root");
  plugin->AddRunNumber(146747);
  /*  
      plugin->SetDataPattern("AliESDs_wSDD.root");

  // runs

  plugin->AddRunNumber(146824);
  plugin->AddRunNumber(146856);
  plugin->AddRunNumber(146858);
  plugin->AddRunNumber(146859);
  plugin->AddRunNumber(146860);

  plugin->AddRunNumber(146748);
  plugin->AddRunNumber(146801);
  plugin->AddRunNumber(146802);
  plugin->AddRunNumber(146803);
  plugin->AddRunNumber(146804);
  plugin->AddRunNumber(146805);
  plugin->AddRunNumber(146806);
  plugin->AddRunNumber(146807);
  plugin->AddRunNumber(146817);

  // end run list
  plugin->AddRunNumber(146746);
  */

  plugin->SetAdditionalLibs("libGui.so libXMLParser.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");
  //plugin->SetAdditionalLibs("libGui.so libXMLIO.so libCDB.so libProof.so libRAWDatabase.so libRAWDatarec.so libSTEERBase.so libSTEER.so libTRDbase.so libTOFbase.so libTOFrec.so  libVZERObase.so libVZEROrec.so libMinuit.so  libEMCALUtils.so libEMCALraw.so libEMCALbase.so libANALYSIS.so libANALYSISalice.so libPHOSUtils.so libTENDER.so libTENDERSupplies.so libPWGflowBase.so libPWGflowTasks.so libAOD.so libCORRFW.so PWG4JCORRAN.par");

  //  plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");

  //========================================
  // Set Ouput Information
  //========================================
  plugin->SetGridWorkingDir("wrk/LHC11b10a_woSDD_kMB-v5-03-Rev-23");
  plugin->SetGridOutputDir("/alice/cern.ch/user/d/djkim/wrk/LHC11b10a_woSDD_kMB-v5-03-Rev-23/output/146746"); // In this case will be $HOME/work/output
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
