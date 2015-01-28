/**************************************************************************
 * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// Comment describing what this class does needed!

// $Id: AliJCaloCell.cxx,v 1.5 2012/04/19 15:19:52 jkral Exp $

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

#include "AliJCaloCell.h"

ClassImp(AliJCaloCell);
//______________________________________________________________________________
AliJCaloCell::AliJCaloCell() : 
    fAbsId(-1),
    fAmplitude(-1),              
    fTime(-1),                   
  fCaloType(kUndef),
  fMcLabel(-1),
  fEFrac(0)

{
}

//_____________________________________________________________________________
AliJCaloCell::AliJCaloCell(const AliJCaloCell& a) : 
    fAbsId(a.fAbsId),
    fAmplitude(a.fAmplitude),              
    fTime(a.fTime),                   
  fCaloType(a.fCaloType),
  fMcLabel(a.fMcLabel),
  fEFrac(a.fEFrac)

{
}

//_____________________________________________________________________________
AliJCaloCell& AliJCaloCell::operator=(const AliJCaloCell& cell){
  //operator=    
  if(this != &cell){
    TObject::operator=(cell);
    fAbsId = cell.fAbsId;
    fAmplitude  = cell.fAmplitude;
    fTime    = cell.fTime;
    fCaloType    = cell.fCaloType;
    fMcLabel    = cell.fMcLabel;
    fEFrac      = cell.fEFrac;
  }

  return *this;
}


