#include ( ${JCORRAN_CMAKE_DIR}/JCORRAN_CMake.cmake  OPTIONAL )
# !!!! DON'T TOUCH ABOVE !!!!

#include ( JCORRAN_CMake.cmake  OPTIONAL )
PROJECT ( JCORRANContainers )
JCORRAN_PACKAGE ( JCORRANContainers )

set ( SRCS
    AliJBaseEventHeader.cxx
    AliJBaseTrack.cxx
    AliJEventHeader.cxx
    AliJJet.cxx
    AliJRunHeader.cxx
    AliJTrack.cxx
    AliJPhoton.cxx
    AliJPiZero.cxx
    AliJCaloCell.cxx
    )
#AliJMCTrack.cxx
set ( SRCSN )

string ( REPLACE ".cxx" ".h" HDRS "${SRCS}" )
set ( DHDR  lib${JCORRAN_PACKAGE}LinkDef.h )


#===============================================
if( JCORRAN_BUILD_MODE )
    set( JCORRANPACKAGE JCORRAN )
    JCORRAN_BUILD_LIB()
endif( JCORRAN_BUILD_MODE )
