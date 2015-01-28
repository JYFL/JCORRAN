/* Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice */

// Short comment describing what this class does needed!

////////////
// Is this class really used?
////////////


#ifndef ALIJPRODCONFIG_H
#define ALIJPRODCONFIG_H
#include "TNamed.h"

class AliJProdConfig {
public:
	AliJProdConfig();
	UInt_t  GetRunNumber()const{return fRunNumber;}
	void	SetRunNumber(UInt_t run){ fRunNumber=run; }
	
	TString GetRunType() const { return fRunType;}
	void    SetRunType( const TString type ){ fRunType = type; }

	Int_t   GetInputFormat() const { return fInputFormat; }
	void    SetInputFormat( Int_t v ){ fInputFormat = v; }
	Bool_t  FromESD() const{ return fInputFormat==kJESD; }
	Bool_t  FromAOD() const{ return fInputFormat==kJAOD; }

	Bool_t  IsMC()const{ return fIsMC; }
	void    SetIsMC(Bool_t b){ fIsMC=b; }

	Int_t   GetColType()const{ return fColType; }
	void    SetColType(int t){ fColType = t; }

	Bool_t  GetWithoutSPD()const{ return fWithoutSPD; }
	void    SetWithoutSPD( bool b ){ fWithoutSPD = b; }


	void 	SetStoreEventPlaneSource(Bool_t dostore ){ fStoreEventPlaneSource = dostore; }
	Bool_t 	GetStoreEventPlaneSource() const { return fStoreEventPlaneSource; };

	UInt_t 	SetStoreTPCTrackBitMask()const{ return fStoreTPCTrackBitMask }
	void 	SetStoreTPCTrackBitMask( UInt_t mask ){ fStoreTPCTrackBitMask = mask; }

	UInt_t 	SetStoreGCGTrackBitMask()const{ return fStoreGCGTrackBitMask }
	void 	SetStoreGCGTrackBitMask( UInt_t mask ){ fStoreGCGTrackBitMask = mask; }

private:
	UInt_t		fRunNumber;   // number of the current run
	TString   	fRunType;   // ex) LHC10h
	Int_t     	fInputFormat; // specify the input data format (kJESD or kJAOD)
	Bool_t    	fWithoutSPD;  // Is silicon pixel detector used or not
	Int_t     	fColType;     // collision type
	Bool_t		fIsMC;          // real data or MC data
	Bool_t 		fStoreEventPlaneSource; // store event plane
	UInt_t 		fStoreTPCTrackBitMask;  // time projection chamber bit mask
	UInt_t 		fStoreGCGTrackBitMask;  // GCG bit mask

};
#endif