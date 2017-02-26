PROJECT(libmons)
FILE (GLOB_RECURSE libmons_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (libmons_LIBS core csv cpptoml Threads::Threads)
SET (libmons_BIN "mons")

ADD_LIBRARY(${libmons_BIN} ${libmons_SRCS})
TARGET_LINK_LIBRARIES(${libmons_BIN} ${libmons_LIBS})
TARGET_INCLUDE_DIRECTORIES(${libmons_BIN} PUBLIC ".")

SET_TARGET_PROPERTIES(${libmons_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${libmons_BIN} CLEAN_DIRECT_OUTPUT 1)

IF (CMAKE_CXX_COMPILER MATCHES "icpc")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -xHOST -O3 -no-prec-div -static -fp-model fast=2")
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}	-fno-math-errno -ffinite-math-only -ffast-math -Rpass-analysis=loop-vectorize -Rpass=loop-vectorize -Rpass-missed=loop-vectorize")
ENDIF()
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${libmons_BIN} DESTINATION bin)
