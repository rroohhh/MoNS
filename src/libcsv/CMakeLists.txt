PROJECT(libcsv)
FILE (GLOB_RECURSE libcsv_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (libcsv_LIBS fmt)
SET (libcsv_BIN "csv")

ADD_LIBRARY(${libcsv_BIN} ${libcsv_SRCS})
TARGET_LINK_LIBRARIES(${libcsv_BIN} ${libcsv_LIBS})
TARGET_INCLUDE_DIRECTORIES(${libcsv_BIN} PUBLIC ".")

SET_TARGET_PROPERTIES(${libcsv_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${libcsv_BIN} CLEAN_DIRECT_OUTPUT 1)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -parallel -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -fPIC")

TARGET_COMPILE_FEATURES(${libcsv_BIN} PRIVATE cxx_constexpr)
TARGET_COMPILE_FEATURES(${libcsv_BIN} PRIVATE cxx_decltype_auto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${libcsv_BIN} DESTINATION bin)
