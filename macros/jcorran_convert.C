void jcorran_convert(TString rootname="test.root"){
    gROOT->LoadMacro("../AliJHistManager/AliJHistManager.cxx+");
    TFile *input = new TFile(rootname,"UPDATE");

    AliJHistManager * hst = new AliJHistManager("hst");
    hst->LoadConfig();

    for( int i=0;i< hst->GetNAliJTH1(); i++ ){
      AliJTH1 * th = hst->GetAliJTH1(i);
      th->WriteAll();
    }

    input->Purge();
    input->Write(0,TObject::kOverwrite);

}
