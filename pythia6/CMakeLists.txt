include ( ${JCORRAN_CMAKE_DIR}/JCORRAN_CMake.cmake  OPTIONAL )
# !!!! DON'T TOUCH ABOVE !!!!

PROJECT ( jtkt_pythia6 )
JCORRAN_PACKAGE ( jtkt_pythia6 )

set ( SRCS ${JCORRAN_PACKAGE}.cxx )

JCORRAN_ADD_LIB_DIR( $ENV{PWD} )

JCORRAN_ADD_SUBDIR( src )

JCORRAN_ADD_DIR( ../PWGJCORRAN/src ../PWGJCORRAN/src/containers ../PWGJCORRAN/src/utils src )
JCORRAN_ADD_LIB( JCORRANpythia6 Pythia6 EGPythia6  )
JCORRAN_BUILD_EXE()
