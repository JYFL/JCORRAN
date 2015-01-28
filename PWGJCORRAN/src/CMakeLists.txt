include ( ${JCORRAN_CMAKE_DIR}/JCORRAN_CMake.cmake  OPTIONAL )
# !!!! DON'T TOUCH ABOVE !!!!

PROJECT (JCORRAN)
JCORRAN_PACKAGE ( JCORRAN )

set ( SRCS
    AliJTrackCounter.cxx 
    AliJCORRAN.cxx 
    )
#AliJEfficiencyScanner.cxx 
set ( SRCSN
    AliJCorrelations.cxx 
    AliJHistos.cxx 
    AliJRunTable.cxx 
    AliJTrackCut.cxx 
    AliJDataManager.cxx 
    AliJEfficiency.cxx 
    AliJEventPool.cxx
    AliJTrackCounter.cxx
    AliJJetAnalysis.cxx
    AliJDiJetAnalysis.cxx
    AliJJetJtHistos.cxx
    AliJJetJtAnalysis.cxx
	AliJJetCORRAnalysis.cxx
    )
set ( DHDR  lib${JCORRAN_PACKAGE}LinkDef.h )

string ( REPLACE ".cxx" ".h" HDRS "${SRCS}" )


#===============================================
if( JCORRAN_BUILD_MODE )
    JCORRAN_ADD_SUBDIR( containers )
    JCORRAN_ADD_SUBDIR( utils )
    JCORRAN_ADD_DIR( containers utils )
    JCORRAN_ADD_LIB( JCORRANContainers JCORRANutils )
    JCORRAN_BUILD_LIB()
endif( JCORRAN_BUILD_MODE )

