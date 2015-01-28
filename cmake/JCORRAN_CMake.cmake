set( CMAKE_CXX_FLAGS_DEBUG "-g" )
set(CMAKE_CXX_FLAGS "-fPIC -g ")
set(CMAKE_C_FLAGS "-fPIC -g ")
#set_property (GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
if( NOT JCORRAN_CMAKE_LOADED )
    set( JCORRAN_CMAKE_LOADED "Loaded" )
    set( JCORRAN_BUILD_MODE 1 )
    set( CMAKE_BUILD_TYPE Debug )
    set( CMAKE_CXX_FLAGS_DEBUG "-g" )
    set(CMAKE_CXX_FLAGS "-fPIC")
    set(CMAKE_C_FLAGS "-fPIC")
    #set_property (GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
    cmake_policy(SET CMP0015 OLD)
    cmake_policy(SET CMP0011 OLD)



    set( BUILD_SHARED_LIBS 1)

    #=========================================
    #    *** Manipulate Variables
    #    BUILD_MODE      : always 1 yet.
    #    JCORRAN_PACKAGE : package name
    #    ADD_INCLUDE_DIR 
    #    ADD_LIB_DIR 
    #    ADD_DIR         : add directory to includes 
    #                      and lib at the same time 
    #    ADD_LIB         : library comes with ADD_LIB_DIR
    #
    #=========================================
    MACRO( JCORRAN_BUILD_SET_MACRO name )
        MACRO( ${name} data )
            set( ${name} ${data} )
        ENDMACRO( ${name} data )
    ENDMACRO( JCORRAN_BUILD_SET_MACRO name )
    JCORRAN_BUILD_SET_MACRO( JCORRAN_BUILD_MODE )
    MACRO( JCORRAN_PACKAGE name )
        set( JCORRAN_PACKAGE ${name} )
        MESSAGE( "-- JLOG : This is ${PROJECT_NAME} - ${JCORRAN_PACKAGE}" )
    ENDMACRO( JCORRAN_PACKAGE )
    MACRO( JCORRAN_ADD_INCLUDE_DIR )
        set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${ARGV}  )
    ENDMACRO( JCORRAN_ADD_INCLUDE_DIR )
    MACRO( JCORRAN_ADD_LIB_DIR )
        set(LINK_DIRECTORIES ${LINK_DIRECTORIES} ${ARGV}  )
    ENDMACRO( JCORRAN_ADD_LIB_DIR )
    MACRO( JCORRAN_ADD_DIR data )
        JCORRAN_ADD_INCLUDE_DIR ( ${ARGV}  )
        JCORRAN_ADD_LIB_DIR ( ${ARGV}  )
    ENDMACRO( JCORRAN_ADD_DIR )
    MACRO( JCORRAN_ADD_LIB  )
        set( JCORRAN_LIB ${JCORRAN_LIB} ${ARGV}  )
    ENDMACRO( JCORRAN_ADD_LIB )

    #=========================================
    #   Load LIBRARY ( ROOT )
    #=========================================
    MACRO( JCORRAN_LOAD_ROOT )
        JCORRAN_FIND_ROOT()
        JCORRAN_ADD_INCLUDE_DIR( ${ROOT_INCLUDE_DIR} )
        JCORRAN_ADD_LIB_DIR    ( ${ROOT_LIBRARY_DIR} )
    ENDMACRO( JCORRAN_LOAD_ROOT )

    #=========================================
    #   Load LIBRARY ( PYTHIA )
    #=========================================
    MACRO( JCORRAN_LOAD_PYTHIA dir )
        JCORRAN_ADD_INCLUDE_DIR( ${dir}/include )
        JCORRAN_ADD_INCLUDE_DIR( ${dir}/include/Pythia8 )
        JCORRAN_ADD_LIB_DIR    ( ${dir}/lib/archive )
    ENDMACRO( JCORRAN_LOAD_PYTHIA )
    #=========================================
    #   Load LIBRARY ( HERWIG )
    #=========================================
    MACRO( JCORRAN_LOAD_HERWIG herwig thepeg )
        JCORRAN_ADD_INCLUDE_DIR( ${herwig}/include ${thepeg}/include)
        JCORRAN_ADD_LIB_DIR    ( ${herwig}/lib ${thepeg}/lib)
    ENDMACRO( JCORRAN_LOAD_HERWIG )

    #=========================================
    #   *** BUILD
    #   BEFORE_BUILD
    #   AFTER_BUILD
    #   BUILD_LIB
    #   BUILD_EXE
    #=========================================
    MACRO( JCORRAN_ADD_SUBDIR )
        add_subdirectory( ${ARGV0} )
    ENDMACRO( JCORRAN_ADD_SUBDIR )

    MACRO( JCORRAN_BEFORE_BUILD )
        include_directories( . ${INCLUDE_DIRECTORIES} )
        link_directories( . ${LINK_DIRECTORIES}  )
        #if( HDRS OR DHDRS )
        ROOT_GENERATE_DICTIONARY("${HDRS}" "${DHDR}" "G__${JCORRAN_PACKAGE}.cxx" "${INCLUDE_DIRECTORIES}")
        #endif( HDRS OR DHDRS )
        set( SRCS ${SRCS} ${SRCSN} )
    ENDMACRO( JCORRAN_BEFORE_BUILD )

    MACRO( JCORRAN_AFTER_BUILD )
        target_link_libraries(${JCORRAN_PACKAGE} ${ROOT_LIBRARIES} ${JCORRAN_LIB} )
    ENDMACRO( JCORRAN_AFTER_BUILD )


    MACRO(JCORRAN_BUILD_LIB)
        JCORRAN_LOAD_ROOT()
        JCORRAN_BEFORE_BUILD()
        add_library(${JCORRAN_PACKAGE} STATIC ${SRCS} G__${JCORRAN_PACKAGE} )
        #add_library(${JCORRAN_PACKAGE} SHARED ${SRCS} G__${JCORRAN_PACKAGE}.cxx )
        JCORRAN_AFTER_BUILD()
    ENDMACRO(JCORRAN_BUILD_LIB)
    MACRO(JCORRAN_BUILD_LIB_SHARED)
        JCORRAN_LOAD_ROOT()
        JCORRAN_BEFORE_BUILD()
        add_library(${JCORRAN_PACKAGE} SHARED ${SRCS} G__${JCORRAN_PACKAGE}.cxx )
        JCORRAN_AFTER_BUILD()
    ENDMACRO(JCORRAN_BUILD_LIB_SHARED)
    MACRO(JCORRAN_BUILD_EXE)
        JCORRAN_LOAD_ROOT()
        JCORRAN_BEFORE_BUILD()
        add_executable(${JCORRAN_PACKAGE} ${SRCS} )
        #TODO
        #add_executable(${JCORRAN_PACKAGE} ${SRCS} G__${JCORRAN_PACKAGE} )
        JCORRAN_AFTER_BUILD()
    ENDMACRO(JCORRAN_BUILD_EXE)

    #=========================================
    #### ROOT Finder from "FindROOT.cmake####
    #=========================================

    MACRO( JCORRAN_FIND_ROOT )
        if( NOT ROOT_LOADED )

            # - Find ROOT instalation
            # This module tries to find the ROOT installation on your system.
            # It tries to find the root-config script which gives you all the needed information.
            # If the system variable ROOTSYS is set this is straight forward.
            # If not the module uses the pathes given in ROOT_CONFIG_SEARCHPATH.
            # If you need an other path you should add this path to this varaible.  
            # The root-config script is then used to detect basically everything else.
            # This module defines a number of key variables and macros.

            # F.Uhlig@gsi.de (fairroot.gsi.de)


            MESSAGE(STATUS "Looking for Root...")

            SET(ROOT_CONFIG_SEARCHPATH
                ${SIMPATH}/tools/root/bin
                $ENV{ROOTSYS}/bin
                )

            SET(ROOT_DEFINITIONS "")

            SET(ROOT_INSTALLED_VERSION_TOO_OLD FALSE)

            SET(ROOT_CONFIG_EXECUTABLE ROOT_CONFIG_EXECUTABLE-NOTFOUND)

            FIND_PROGRAM(ROOT_CONFIG_EXECUTABLE NAMES root-config PATHS
                ${ROOT_CONFIG_SEARCHPATH}
                NO_DEFAULT_PATH)

            IF (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")
                MESSAGE( FATAL_ERROR "ROOT not installed in the searchpath and ROOTSYS is not set. Please
                set ROOTSYS or add the path to your ROOT installation in the Macro FindROOT.cmake in the
                subdirectory cmake/modules.")
            ELSE (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")
                STRING(REGEX REPLACE "(^.*)/bin/root-config" "\\1" test ${ROOT_CONFIG_EXECUTABLE}) 
                SET( ENV{ROOTSYS} ${test})
                set( ROOTSYS ${test})
            ENDIF (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")  


            IF (ROOT_CONFIG_EXECUTABLE)

                SET(ROOT_FOUND FALSE)

                EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE ROOTVERSION)

                MESSAGE(STATUS "Looking for Root... - found $ENV{ROOTSYS}/bin/root")
                MESSAGE(STATUS "Looking for Root... - version ${ROOTVERSION} ")   

                # we need at least version 5.00/00
                IF (NOT ROOT_MIN_VERSION)
                    SET(ROOT_MIN_VERSION "5.00/00")
                ENDIF (NOT ROOT_MIN_VERSION)

                # now parse the parts of the user given version string into variables
                STRING(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+" "\\1" req_root_major_vers "${ROOT_MIN_VERSION}")
                STRING(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" req_root_minor_vers "${ROOT_MIN_VERSION}")
                STRING(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+)" "\\1" req_root_patch_vers "${ROOT_MIN_VERSION}")

                # and now the version string given by qmake
                STRING(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+.*" "\\1" found_root_major_vers "${ROOTVERSION}")
                STRING(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" found_root_minor_vers "${ROOTVERSION}")
                STRING(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+).*" "\\1" found_root_patch_vers "${ROOTVERSION}")

                IF (found_root_major_vers LESS 5)
                    MESSAGE( FATAL_ERROR "Invalid ROOT version \"${ROOTERSION}\", at least major version 4 is required, e.g. \"5.00/00\"")
                ENDIF (found_root_major_vers LESS 5)

                # compute an overall version number which can be compared at once
                MATH(EXPR req_vers "${req_root_major_vers}*10000 + ${req_root_minor_vers}*100 + ${req_root_patch_vers}")
                MATH(EXPR found_vers "${found_root_major_vers}*10000 + ${found_root_minor_vers}*100 + ${found_root_patch_vers}")

                IF (found_vers LESS req_vers)
                    SET(ROOT_FOUND FALSE)
                    SET(ROOT_INSTALLED_VERSION_TOO_OLD TRUE)
                ELSE (found_vers LESS req_vers)
                    SET(ROOT_FOUND TRUE)
                ENDIF (found_vers LESS req_vers)

            ENDIF (ROOT_CONFIG_EXECUTABLE)


            IF (ROOT_FOUND)

                # ask root-config for the library dir
                # Set ROOT_LIBRARY_DIR

                EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
                    ARGS "--libdir"
                    OUTPUT_VARIABLE ROOT_LIBRARY_DIR_TMP )

                IF(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
                    SET(ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR_TMP} )
                ELSE(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
                    MESSAGE("Warning: ROOT_CONFIG_EXECUTABLE reported ${ROOT_LIBRARY_DIR_TMP} as library path,")
                    MESSAGE("Warning: but ${ROOT_LIBRARY_DIR_TMP} does NOT exist, ROOT must NOT be installed correctly.")
                ENDIF(EXISTS "${ROOT_LIBRARY_DIR_TMP}")

                # ask root-config for the binary dir
                EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE}
                    ARGS "--bindir"
                    OUTPUT_VARIABLE root_bins )
                SET(ROOT_BINARY_DIR ${root_bins})

                # ask root-config for the include dir
                EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
                    ARGS "--incdir" 
                    OUTPUT_VARIABLE root_headers )
                SET(ROOT_INCLUDE_DIR ${root_headers})
                # CACHE INTERNAL "")

                # ask root-config for the library varaibles
                EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
                    #    ARGS "--noldflags --noauxlibs --libs" 
                    ARGS "--glibs" 
                    OUTPUT_VARIABLE root_flags )

                #  STRING(REGEX MATCHALL "([^ ])+"  root_libs_all ${root_flags})
                #  STRING(REGEX MATCHALL "-L([^ ])+"  root_library ${root_flags})
                #  REMOVE_FROM_LIST(root_flags "${root_libs_all}" "${root_library}")

                SET(ROOT_LIBRARIES ${root_flags})

                # Make variables changeble to the advanced user
                MARK_AS_ADVANCED( ROOT_LIBRARY_DIR ROOT_INCLUDE_DIR ROOT_DEFINITIONS)

                # Set ROOT_INCLUDES
                SET( ROOT_INCLUDES ${ROOT_INCLUDE_DIR})

                SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${ROOT_LIBRARY_DIR})

                #######################################
                #
                #       Check the executables of ROOT 
                #          ( rootcint ) 
                #
                #######################################

                FIND_PROGRAM(ROOT_CINT_EXECUTABLE
                    NAMES rootcint
                    PATHS ${ROOT_BINARY_DIR}
                    NO_DEFAULT_PATH
                    )

            ENDIF (ROOT_FOUND)



            ###########################################
            #
            #       Macros for building ROOT dictionary
            #
            ###########################################

            MACRO (ROOT_GENERATE_DICTIONARY_OLD )

                set(INFILES "")    

                foreach (_current_FILE ${ARGN})

                    IF (${_current_FILE} MATCHES "^.*\\.h$")
                        IF (${_current_FILE} MATCHES "^.*Link.*$")
                            set(LINKDEF_FILE ${_current_FILE})
                        ELSE (${_current_FILE} MATCHES "^.*Link.*$")
                            set(INFILES ${INFILES} ${_current_FILE})
                        ENDIF (${_current_FILE} MATCHES "^.*Link.*$")
                    ELSE (${_current_FILE} MATCHES "^.*\\.h$")
                        IF (${_current_FILE} MATCHES "^.*\\.cxx$")
                            set(OUTFILE ${_current_FILE})
                        ELSE (${_current_FILE} MATCHES "^.*\\.cxx$")
                            set(INCLUDE_DIRS ${INCLUDE_DIRS} -I${_current_FILE})   
                        ENDIF (${_current_FILE} MATCHES "^.*\\.cxx$")
                    ENDIF (${_current_FILE} MATCHES "^.*\\.h$")

                endforeach (_current_FILE ${ARGN})

                #  MESSAGE("INFILES: ${INFILES}")
                #  MESSAGE("OutFILE: ${OUTFILE}")
                #  MESSAGE("LINKDEF_FILE: ${LINKDEF_FILE}")
                #  MESSAGE("INCLUDE_DIRS: ${INCLUDE_DIRS}")

                STRING(REGEX REPLACE "(^.*).cxx" "\\1.h" bla "${OUTFILE}")
                #   MESSAGE("BLA: ${bla}")
                SET (OUTFILES ${OUTFILE} ${bla})

                ADD_CUSTOM_COMMAND(OUTPUT ${OUTFILES}
                    COMMAND ${ROOT_CINT_EXECUTABLE}
                    ARGS -f ${OUTFILE} -c -DHAVE_CONFIG_H ${INCLUDE_DIRS} ${INFILES} ${LINKDEF_FILE} DEPENDS ${INFILES})

                #   MESSAGE("ROOT_CINT_EXECUTABLE has created the dictionary ${OUTFILE}")

            ENDMACRO (ROOT_GENERATE_DICTIONARY_OLD)


            MACRO (GENERATE_ROOT_TEST_SCRIPT SCRIPT_FULL_NAME)

                get_filename_component(path_name ${SCRIPT_FULL_NAME} PATH)
                get_filename_component(file_extension ${SCRIPT_FULL_NAME} EXT)
                get_filename_component(file_name ${SCRIPT_FULL_NAME} NAME_WE)
                set(shell_script_name "${file_name}.sh")

                #MESSAGE("PATH: ${path_name}")
                #MESSAGE("Ext: ${file_extension}")
                #MESSAGE("Name: ${file_name}")
                #MESSAGE("Shell Name: ${shell_script_name}")

                string(REPLACE ${PROJECT_SOURCE_DIR} 
                    ${PROJECT_BINARY_DIR} new_path ${path_name}
                    )

                #MESSAGE("New PATH: ${new_path}")

                file(MAKE_DIRECTORY ${new_path}/data)

                CONVERT_LIST_TO_STRING(${LD_LIBRARY_PATH})
                set(MY_LD_LIBRARY_PATH ${output})
                set(my_script_name ${SCRIPT_FULL_NAME})

                if(CMAKE_SYSTEM MATCHES Darwin)
                    configure_file(${PROJECT_SOURCE_DIR}/cmake/scripts/root_macro_macos.sh.in
                        ${new_path}/${shell_script_name}
                        )
                else(CMAKE_SYSTEM MATCHES Darwin)
                    configure_file(${PROJECT_SOURCE_DIR}/cmake/scripts/root_macro.sh.in
                        ${new_path}/${shell_script_name}
                        )
                endif(CMAKE_SYSTEM MATCHES Darwin)

                EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${new_path}/${shell_script_name}")

            ENDMACRO (GENERATE_ROOT_TEST_SCRIPT)
            #set( ROOT_INCLUDE_DIR ${ROOT_INCLUDE_DIR} CACHE STRING "Description")
            #set( ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR} CACHE STRING "Description")
            #set( ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR} PARENT_SCOPE)
            #set( ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR} PARENT_SCOPE)
        endif( NOT ROOT_LOADED )
        set ( ROOT_LOADED 1 PARENT_SCOPE)
        set ( ROOT_INCLUDE_DIR ${ROOT_INCLUDE_DIR} PARENT_SCOPE )
        set ( ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR} PARENT_SCOPE)

    ENDMACRO (JCORRAN_FIND_ROOT)

    ###########################################
    #
    #       Macros for building ROOT dictionary
    #
    ###########################################

    MACRO (ROOT_GENERATE_DICTIONARY INFILES LINKDEF_FILE OUTFILE INCLUDE_DIRS_IN)

        set(INCLUDE_DIRS)

        foreach (_current_FILE ${INCLUDE_DIRS_IN})
            set(INCLUDE_DIRS ${INCLUDE_DIRS} -I${_current_FILE})   
        endforeach (_current_FILE ${INCLUDE_DIRS_IN})


        #  MESSAGE("INFILES: ${INFILES}")
        #  MESSAGE("OutFILE: ${OUTFILE}")
        #  MESSAGE("LINKDEF_FILE: ${LINKDEF_FILE}")
        #  MESSAGE("INCLUDE_DIRS: ${INCLUDE_DIRS}")

        STRING(REGEX REPLACE "^(.*)\\.(.*)$" "\\1.h" bla "${OUTFILE}")
        #  MESSAGE("BLA: ${bla}")
        SET (OUTFILES ${OUTFILE} ${bla})


        if (CMAKE_SYSTEM_NAME MATCHES Linux)
            ADD_CUSTOM_COMMAND(OUTPUT ${OUTFILES}
                COMMAND LD_LIBRARY_PATH=${ROOT_LIBRARY_DIR} ROOTSYS=${ROOTSYS} ${ROOT_CINT_EXECUTABLE}
                ARGS -f ${OUTFILE} -c -DHAVE_CONFIG_H ${INCLUDE_DIRS} ${INFILES} ${LINKDEF_FILE} DEPENDS ${INFILES} ${LINKDEF_FILE})
        else (CMAKE_SYSTEM_NAME MATCHES Linux)
            if (CMAKE_SYSTEM_NAME MATCHES Darwin)
                ADD_CUSTOM_COMMAND(OUTPUT ${OUTFILES}
                    COMMAND DYLD_LIBRARY_PATH=${ROOT_LIBRARY_DIR} ROOTSYS=${ROOTSYS} ${ROOT_CINT_EXECUTABLE}
                    ARGS -f ${OUTFILE} -c -DHAVE_CONFIG_H ${INCLUDE_DIRS} ${INFILES} ${LINKDEF_FILE} DEPENDS ${INFILES} ${LINKDEF_FILE})
            endif (CMAKE_SYSTEM_NAME MATCHES Darwin)
        endif (CMAKE_SYSTEM_NAME MATCHES Linux)

    ENDMACRO (ROOT_GENERATE_DICTIONARY)

endif( NOT JCORRAN_CMAKE_LOADED )

