AliAnalysisGrid* CreateAlienHandler(){
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    plugin->SetOverwriteMode();
    plugin->SetRunMode("full");

    plugin->SetAPIVersion("V1.1x");
    plugin->SetROOTVersion("v5-34-01-1");
    plugin->SetAliROOTVersion("v5-03-54-AN");

    // DATA pp 2.76 /alice/data/2011/LHC11a/000146805/ESDs/pass2_with_SDD/
    plugin->SetGridDataDir("/alice/sim/LHC11a10b");
    plugin->SetDataPattern("*ESDs.root");
    //plugin->SetRunPrefix("000");   // real data
    plugin->AddRunNumber(137366);

    plugin->SetOutputToRunNo();
    plugin->SetGridWorkingDir("CS3MC");
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    // Declare the analysis source files names separated by blancs. To be compiled runtime
    // using ACLiC on the worker nodes.
    plugin->SetAnalysisSource("AliAnalysisTaskCutStudy.cxx");
    // Declare all libraries (other than the default ones for the framework. These will be
    // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
    plugin->SetAdditionalLibs("AliAnalysisTaskCutStudy.h AliAnalysisTaskCutStudy.cxx");
    //   plugin->SetAdditionalLibs("PWG4JCORRAN.par libEMCALUtils.so libPHOSUtils.so");//AliCentralityBy1D_137161_GLAU.root AliCentralitySelectionTask.cxx");
    // Declare the output file names separated by blancs.
    // (can be like: file.root or file.root@ALICE::Niham::File)
    //   plugin->SetKeepLogs(kTRUE);
    //   plugin->SetDefaultOutputs(kFALSE);
    //   plugin->SetOutputFiles("eff.root");
    //   plugin->SetOutputArchive("root_archive.zip:*.root");
    //  plugin->SetTerminateFiles("event_stat.root"); 
    plugin->SetCheckCopy(kFALSE);//FK/
    //   plugin->SetOutputArchive("log_archive.zip:stdout,stderr root_archive.zip:*.root");
    //   plugin->SetDefaultOutputs();
    // Optionally define the files to be archived.
    //   plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
    // Optionally set a name for the generated analysis macro (default MyAnalysis.C)
    plugin->SetAnalysisMacro("CutStudyTask.C");
    // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
    plugin->SetSplitMaxInputFileNumber(100);
    // Optionally modify the executable name (default analysis.sh)
    plugin->SetExecutable("CutStudyTask.sh");
    // Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
    //   plugin->SetMaxInitFailed(5);
    // Optionally resubmit threshold.
    //   plugin->SetMasterResubmitThreshold(90);
    // Optionally set time to live (default 30000 sec)
    plugin->SetTTL(30000);
    // Optionally set input format (default xml-single)
    plugin->SetInputFormat("xml-single");
    // Optionally modify the name of the generated JDL (default analysis.jdl)
    plugin->SetJDLName("CutStudyTask.jdl");
    // Optionally modify job price (default 1)
    plugin->SetPrice(1);      
    // Optionally modify split mode (default 'se')    
    plugin->SetSplitMode("se");
    return plugin;
}
