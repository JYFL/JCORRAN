#include ( ${JCORRAN_CMAKE_DIR}/JCORRAN_CMake.cmake  OPTIONAL )
# !!!! DON'T TOUCH ABOVE !!!!

PROJECT (JCORRANpythia6)
JCORRAN_PACKAGE ( JCORRANpythia6 )

set ( SRCS
     JPythia6DataManager.cxx 
     AliJCORRANpythia6.cxx 
    )
set ( DHDR  lib${JCORRAN_PACKAGE}LinkDef.h )

string ( REPLACE ".cxx" ".h" HDRS "${SRCS}" )


#===============================================
if( JCORRAN_BUILD_MODE )
    JCORRAN_ADD_DIR( ../../PWGJCORRAN/src ../../PWGJCORRAN/src/containers ../../PWGJCORRAN/src/utils )
    JCORRAN_ADD_LIB( JCORRAN )
    JCORRAN_BUILD_LIB()
endif( JCORRAN_BUILD_MODE )

