/* Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice */

// Short comment describing what this class does needed!

// $Id: AliJCaloCell.h,v 1.5 2012/04/19 15:19:52 jkral Exp $

////////////////////////////////////////////////////
/*!
  \file AliJCaloCell.h
  \brief
  \author J. Kral (University of Jyvaskyla)
  \email: jiri.kral@jyu.fi
  \version $Revision: 1.0 $
  \date $Date: 2012/04/19 15:19:52 $
  */
////////////////////////////////////////////////////

#ifndef ALIJCALOCELL_H
#define ALIJCALOCELL_H

#ifndef ROOT_TObject
#include <TObject.h>
#endif

class AliJCaloCell : public TObject {

 public:
 enum AliJCaloType {
   kUndef = -1, 
   kPHOSCalo,  
   kEMCALCalo
  };
  AliJCaloCell();     //default constructor
  AliJCaloCell(const AliJCaloCell& a); //copy constructor
  virtual ~AliJCaloCell(){  //destructor    
  }

  Short_t GetAbsId() const {return fAbsId;}
  Double32_t GetAmplitude() const {return fAmplitude;}
  Double32_t GetTime() const {return fTime;}
  Int_t GetMcLabel() const {return fMcLabel;}
  Double32_t GetEFrac() const {return fEFrac;}
  Int_t GetCaloType() const {return fCaloType;}
  
  //setters
  void  SetCaloType(AliJCaloType calo) {fCaloType = calo;}
  void SetAbsId(Short_t a) { fAbsId = a; }
  void SetAmplitude(Double32_t a) { fAmplitude = a; }
  void SetTime(Double32_t t) { fTime = t; }
  void SetMcLabel(Int_t l) { fMcLabel = l; }
  void SetEFrac(Double32_t e) { fEFrac = e; }
  
  AliJCaloCell& operator=(const AliJCaloCell& cell);

  //TODO
  Bool_t IsPHOS()  const {return fCaloType==kPHOSCalo ? kTRUE:kFALSE ;}
  Bool_t IsEMCAL() const {return fCaloType==kEMCALCalo ? kTRUE:kFALSE ;}


 private:

  Short_t    fAbsId; // cell abs id
  Double32_t   fAmplitude; // amplitude
  Double32_t   fTime;  // cell time
  AliJCaloType   fCaloType;              // PHOS or EMCAL cell
  Int_t    fMcLabel; // MC label
  Double32_t fEFrac; // embedded energy fraction

  ClassDef(AliJCaloCell,2)

};

#endif

