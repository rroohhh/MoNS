PROJECT(libmons)
FILE (GLOB_RECURSE libmons_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (libmons_LIBS fmt csv cpptoml Threads::Threads)
SET (libmons_BIN "mons")

ADD_LIBRARY(${libmons_BIN} ${libmons_SRCS})
TARGET_LINK_LIBRARIES(${libmons_BIN} ${libmons_LIBS})
TARGET_INCLUDE_DIRECTORIES(${libmons_BIN} PUBLIC ".")

SET_TARGET_PROPERTIES(${libmons_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${libmons_BIN} CLEAN_DIRECT_OUTPUT 1)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -parallel -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -fPIC")

TARGET_COMPILE_FEATURES(${libmons_BIN} PRIVATE cxx_constexpr)
TARGET_COMPILE_FEATURES(${libmons_BIN} PRIVATE cxx_decltype_auto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${libmons_BIN} DESTINATION bin)
