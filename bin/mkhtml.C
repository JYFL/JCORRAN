#include <THtml.h>
#include <TROOT.h>
#include <TSystem.h>

void mkhtml(char *macro=0, Int_t force=1) {
    // to run this macro, you must have the correct .rootrc file
    // in your galice directory.
    // The gAlice classes summary documentation go to directory html
    // The gAlice classes source  documentation go to directory html/src
    // The example macros documentation go to directory html/examples

    // gROOT->LoadMacro("loadlibs.C");
    // loadlibs();
    THtml html;
    html.SetProductName("JCORRAN");
    if(macro) {
        gROOT->LoadMacro(macro);
        html.Convert(macro,"Example Macro");
    } else {
        gSystem->Load("PWGJCORRAN/src/libJCORRAN");
        gSystem->Load("PWGJCORRAN/src/containers/libJCORRANContainers");
        html.MakeAll(force,"[A-Z]*");
        html.MakeClass("AliJBin");

    }
    html.Convert("local/jtkt_alice.cxx","jtkt_alice.cxx");
}
