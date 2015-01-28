void SETUP() {
  // Load the BASE library
//bschang//  CheckLoadLibrary("libPWG4JCORRAN");
  TString ocwd = gSystem->WorkingDirectory();// bschang
  CheckLoadLibrary(ocwd+"/libPWG4JCORRAN");//bschang

  gROOT->ProcessLine(".include PWG4JCORRAN");
  // Set our location, so that other packages can find us
  gSystem->Setenv("PWG4JCORRAN_INCLUDE", "PWG4JCORRAN");
}

Int_t CheckLoadLibrary(const char* library) {
  // checks if a library is already loaded, if not loads the library
  
  if (strlen(gSystem->GetLibraries(Form("%s.so", library), "", kFALSE)) > 0)
    return 1;
  
  return gSystem->Load(library);
}
