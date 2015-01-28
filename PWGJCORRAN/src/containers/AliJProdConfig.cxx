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

#include "AliJProdConfig.h"
AliJProdConfig::AliJProdConfig():
fRunNumber(0),
fRunType(""),
fInputFormat(kJESD),
fWithoutSPD(0),
fColType(kJPP),
fIsMC(kFALSE),
fStoreEventPlaneSource(kFALSE),
fStoreTPCTrackBitMask(1),
fStoreGCGTrackBitMask(1)
{
	;
}