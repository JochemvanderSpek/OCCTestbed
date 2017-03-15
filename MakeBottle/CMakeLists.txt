CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( MakeBottle )

#FIND_PACKAGE( OpenCASCADE REQUIRED /usr/local )
# OCC_FOUND          - system has OCC - OpenCASCADE
# OCC_INCLUDE_DIR    - where the OCC include directory can be found
# OCC_LIBRARY_DIR    - where the OCC library directory can be found
# OCC_LIBRARIES      - Link this to use OCC

IF( NOT OCC_FOUND )
	MESSAGE( "setting OCC by hand" )

	SET( OCC_INCLUDE_DIR /usr/local/include/opencascade )
	SET( OCC_LIBRARY_DIR /usr/local/lib )
	SET( OCC_LIBRARY_NAMES
	    TKBO
	    TKBool
	    TKBRep
	    TKCAF
	    TKCDF
	    TKernel
	    TKG2d
	    TKG3d
	    TKGeomAlgo
	    TKGeomBase
	    TKHLR
	    TKIGES
	    TKLCAF
	    TKMath
	    TKMesh
	    TKOffset
	    TKPrim
	    TKService
	    TKShHealing
	    TKSTEP
	    TKSTEP209
	    TKSTEPAttr
	    TKSTEPBase
	    TKSTL
	    TKTopAlgo
	    TKV3d
	    TKXCAF
	    TKXDEIGES
	    TKXDESTEP
	    TKXSBase
	    TKFillet
	)

	FOREACH( LIBNAME ${OCC_LIBRARY_NAMES} )
	    FIND_LIBRARY( ${LIBNAME} ${LIBNAME} ${OCC_LIBRARY_DIR} )
	    SET( OCC_LIBRARIES ${OCC_LIBRARIES} /usr/local/lib/lib${LIBNAME}.7.1.0.dylib )
	ENDFOREACH()
ENDIF()

INCLUDE_DIRECTORIES( ${OCC_INCLUDE_DIR} )

#ADD_SUBDIRECTORY( opencascade-7.1.0 )

SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11" )

SET( SOURCES ${SOURCES} ${PROJECT_SOURCE_DIR}/MakeBottle.cpp )

#SET( SOURCES ${SOURCES} /Users/j/Development/opencascade-7.1.0/src/BrepLib/*.cxx )
#SET( SOURCES ${SOURCES} /Users/j/Development/opencascade-7.1.0/src/NCollection/*.cxx )
#SET( SOURCES ${SOURCES} /Users/j/Development/opencascade-7.1.0/src/Standard/*.cxx )

MESSAGE( "sources: ${SOURCES}" )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OCC_LIBRARIES} )


SET( EMSCRIPTEN_TEST_DIR				${PROJECT_SOURCE_DIR}/../lib/ CACHE PATH "the path to copy the js file to (for EMSCRIPTEN only)" )
SET( EMSCRIPTEN_DEPLOYMENT_DIR	${PROJECT_SOURCE_DIR}/../lib/ CACHE PATH "the path to copy the Module.js file to (for EMSCRIPTEN only)" )

SET( EMSCRIPTEN_DIR /Users/j/Development/emsdk_portable/emscripten )
SET( INCLUDE_DIRS ${INCLUDE_DIRS} ${EMSCRIPTEN_DIR} )

#unfortunately, automatic emscripten building doesn't work...
#so you have to compile 'by hand:'
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type" )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-compat-deprecated-writable-strings" )
#SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-s NO_EXIT_RUNTIME=1" )
#SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-s ALLOW_MEMORY_GROWTH=1" )
#SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-s -Oz" )
#SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-s EXPORTED_FUNCTIONS=" )
#INCLUDE_DIRECTORIES( ${INCLUDE_DIRS} )
#ADD_EXECUTABLE( nurbstool ${SOURCES} )


#cmake -DCMAKE_TOOLCHAIN_FILE=~/Development/emsdk_portable/emscripten/1.35.0/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_MODULE_PATH=~/Development/OCCTestbed/cmake_modules/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-DIGNORE_NO_ATOMICS -I/usr/local/include" -G "Unix Makefiles" .


#check if build directory exists
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}" AND IS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" )
	SET( PRECOMMAND "pushd ${PROJECT_SOURCE_DIR}; export PATH=$PATH:${EMSCRIPTEN_DIR}; export EMSCRIPTEN=${EMSCRIPTEN_DIR}" )

	# make sources inline
	SET( SOURCES_COPY ${SOURCES} )
    SET( SOURCES "" )
	FOREACH( SRC ${SOURCES_COPY} )
	    SET( SOURCES "${SOURCES} ${SRC}" )
	ENDFOREACH( SRC )

	FOREACH( DIR ${OCC_INCLUDE_DIR} )
	    SET( INCLUDE_LINE "${INCLUDE_LINE} -I${DIR}" )
	ENDFOREACH( DIR )

  SET( INCLUDE_LINE "${INCLUDE_LINE} -L/usr/local/lib" )

  SET( OCC_DEBUG OFF )

	IF( OCC_DEBUG )
		SET( FLAGS "${FLAGS} -DOCC_DEBUG -DDEBUG" )
		SET( CREATE_MEM_FILE OFF )
		SET( FLAGS "${FLAGS} --memory-init-file 0" )
	ELSE()
		SET( FLAGS "${FLAGS} -O3" )
		#SET( FLAGS "${FLAGS} -g4" )
		SET( CREATE_MEM_FILE ON )
		SET( FLAGS "${FLAGS} --memory-init-file 1" )
	ENDIF()

	SET( FLAGS "${FLAGS} -DIGNORE_NO_ATOMICS" )


	SET( FLAGS "${FLAGS} -s USE_PTHREADS=1" )
	SET( FLAGS "${FLAGS} -std=c++11" )
	SET( FLAGS "${FLAGS} -s NO_EXIT_RUNTIME=1" )
	SET( FLAGS "${FLAGS} -s ALLOW_MEMORY_GROWTH=1" )
	SET( FLAGS "${FLAGS} -s ERROR_ON_UNDEFINED_SYMBOLS=1" )
	SET( FLAGS "${FLAGS} -s EXPORTED_FUNCTIONS=\"[ '_main' ]\"" )
	SET( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js" )

	SET( COMMAND "em++ ${SOURCES} opencascade-7.1.0/lin32/clang/libd/libTKBO.bc opencascade-7.1.0/lin32/clang/libd/libTKBool.bc  opencascade-7.1.0/lin32/clang/libd/libTKBRep.bc  opencascade-7.1.0/lin32/clang/libd/libTKCAF.bc opencascade-7.1.0/lin32/clang/libd/libTKCDF.bc opencascade-7.1.0/lin32/clang/libd/libTKernel.bc opencascade-7.1.0/lin32/clang/libd/libTKG2d.bc opencascade-7.1.0/lin32/clang/libd/libTKG3d.bc opencascade-7.1.0/lin32/clang/libd/libTKGeomAlgo.bc opencascade-7.1.0/lin32/clang/libd/libTKGeomBase.bc opencascade-7.1.0/lin32/clang/libd/libTKHLR.bc opencascade-7.1.0/lin32/clang/libd/libTKIGES.bc opencascade-7.1.0/lin32/clang/libd/libTKLCAF.bc opencascade-7.1.0/lin32/clang/libd/libTKMath.bc opencascade-7.1.0/lin32/clang/libd/libTKMesh.bc opencascade-7.1.0/lin32/clang/libd/libTKOffset.bc opencascade-7.1.0/lin32/clang/libd/libTKPrim.bc opencascade-7.1.0/lin32/clang/libd/libTKService.bc opencascade-7.1.0/lin32/clang/libd/libTKShHealing.bc opencascade-7.1.0/lin32/clang/libd/libTKSTEP.bc opencascade-7.1.0/lin32/clang/libd/libTKSTEP209.bc opencascade-7.1.0/lin32/clang/libd/libTKSTEPAttr.bc opencascade-7.1.0/lin32/clang/libd/libTKSTEPBase.bc opencascade-7.1.0/lin32/clang/libd/libTKSTL.bc opencascade-7.1.0/lin32/clang/libd/libTKTopAlgo.bc opencascade-7.1.0/lin32/clang/libd/libTKV3d.bc opencascade-7.1.0/lin32/clang/libd/libTKXCAF.bc opencascade-7.1.0/lin32/clang/libd/libTKXDEIGES.bc opencascade-7.1.0/lin32/clang/libd/libTKXDESTEP.bc opencascade-7.1.0/lin32/clang/libd/libTKXSBase.bc opencascade-7.1.0/lin32/clang/libd/libTKFillet.bc ${INCLUDE_LINE} ${FLAGS}  -o ${OUTPUT}" )

	MESSAGE( STATUS "unfortunately, building for emscripten doesn't work yet,\nuse the following line to compile 'by hand':\n\n" )
	MESSAGE( STATUS "${PRECOMMAND}; ${COMMAND}; ${POSTCOMMAND}" )

	SET( INSTALL_COMMAND "cp ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js ${EMSCRIPTEN_TEST_DIR}/")
	SET( INSTALL_COMMAND "${INSTALL_COMMAND}; cp ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js ${EMSCRIPTEN_DEPLOYMENT_DIR}/Module.js" )

	IF( ${CREATE_MEM_FILE} )
		SET( INSTALL_COMMAND "${INSTALL_COMMAND}; cp ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js.mem ${EMSCRIPTEN_TEST_DIR}/")
		SET( INSTALL_COMMAND "${INSTALL_COMMAND}; cp ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js.mem ${EMSCRIPTEN_DEPLOYMENT_DIR}/Module.js.mem")
	ENDIF()
	MESSAGE( STATUS "\nand to install:\n\n" )
	MESSAGE( STATUS "${INSTALL_COMMAND}" )

	MESSAGE( STATUS "\n\n" )
ELSE()
	MESSAGE( "the build directory ${CMAKE_CURRENT_BINARY_DIR} does not exist. exit." )
ENDIF()


