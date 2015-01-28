// Script to recover AliEMCALRecParam from OCDB
// Author: Jiri Kral

#if !defined(__CINT__)
#include <TString.h>
#include <TH2.h>
#include <TF1.h>

#include "AliRun.h"
#include "AliCaloCalibPedestal.h"
#include "AliEMCALGeoParams.h"
#include "AliEMCALRecParam.h"
#include "AliCDBMetaData.h"
#include "AliCDBId.h"
#include "AliCDBEntry.h"
#include "AliCDBManager.h"
#include "AliCDBStorage.h"
#endif


AliEMCALRecParam* GetOCDBRecParam( Int_t runNum, char *datatype="pp", Bool_t grid=kTRUE ){

	AliCDBManager *man;

	AliEMCALRecParam *pars;
	TObjArray *arr;
	
	if( grid ){
		cout << "======= Will get reco params from OCDB on grid ============= " << endl;
		if(!gGrid) {
			if (!TGrid::Connect("alien://")){
				cout << "Error: can't connect to grid" << endl;
				exit(1);
			}
		}
	}
	else{
		cout << "======= Will get reco params from local ODCB =============== " << endl;
	}
	
	// created the OCDB manager
	man = AliCDBManager::Instance();

	// point it to local storage
	// !!! careful, one must build an exact path of OCDB directories
	// and store the file in those
	// here "./OCDB/EMCAL/Calib/Pedestals/Run*.root) for masks
	if( grid )
		man->SetDefaultStorageFromRun( runNum );
	else
		man->SetDefaultStorage( "local://$ALICE_ROOT/OCDB" );
	
	// load the file data
	arr = (TObjArray*)(man->Get("EMCAL/Calib/RecoParam", runNum)->GetObject());

  if( !strcmp( datatype, "pp" ))
		pars = (AliEMCALRecParam*)arr->FindObject( "Low Flux - p+p" );
	else
		pars = (AliEMCALRecParam*)arr->FindObject( "High Flux - Pb+Pb" );

	// cleanup
	arr->Clear();
	delete arr;
	man->ClearCache();
	man->DestroyActiveStorages();
	man->Destroy();

	cout << "======= Reco parames retrieval finished ==================== " << endl;
	
	return pars;
}
