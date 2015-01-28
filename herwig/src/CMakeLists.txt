#include ( ${JCORRAN_CMAKE_DIR}/JCORRAN_CMake.cmake  OPTIONAL )
# !!!! DON'T TOUCH ABOVE !!!!

PROJECT (JCORRANherwig)
JCORRAN_PACKAGE ( JCORRANherwig )

set ( SRCS
    )
set ( SRCSN
     JHerwigDataManager.cxx 
	 JCORRANherwig.cxx
     )

set ( DHDR  lib${JCORRAN_PACKAGE}LinkDef.h )

string ( REPLACE ".cxx" ".h" HDRS "${SRCS}" )


#===============================================
if( JCORRAN_BUILD_MODE )
    JCORRAN_ADD_DIR( ../../PWGJCORRAN/src ../../PWGJCORRAN/src/containers ../../PWGJCORRAN/src/utils )
    JCORRAN_ADD_LIB( JCORRAN )
    JCORRAN_BUILD_LIB_SHARED()
endif( JCORRAN_BUILD_MODE )

