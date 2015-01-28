#include ( ${JCORRAN_CMAKE_DIR}/JCORRAN_CMake.cmake  OPTIONAL )
# !!!! DON'T TOUCH ABOVE !!!!

PROJECT (JCORRANutils)
JCORRAN_PACKAGE ( JCORRANutils )

set ( SRCS
    AliJBaseCard.cxx 
    AliJCard.cxx 
    )
#AliJEfficiencyScanner.cxx 
set ( SRCSN
    AliJHistManager.cxx
    )
set ( DHDR  lib${JCORRAN_PACKAGE}LinkDef.h )

string ( REPLACE ".cxx" ".h" HDRS "${SRCS}" )


#===============================================
if( JCORRAN_BUILD_MODE )
    JCORRAN_ADD_DIR( ../containers )
    JCORRAN_ADD_LIB( JCORRANContainers )
    JCORRAN_BUILD_LIB()
endif( JCORRAN_BUILD_MODE )

