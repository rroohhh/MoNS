PROJECT(mons)
FILE (GLOB_RECURSE mons_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (mons_LIBS plot mons sshfile Threads::Threads readline)
SET (mons_BIN "MoNS")

ADD_EXECUTABLE(${mons_BIN} ${mons_SRCS})
TARGET_LINK_LIBRARIES(${mons_BIN} ${mons_LIBS})

SET_TARGET_PROPERTIES(${mons_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${mons_BIN} CLEAN_DIRECT_OUTPUT 1)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -parallel -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -fPIC")

# TARGET_COMPILE_FEATURES(${mons_BIN} PRIVATE cxx_constexpr)
# TARGET_COMPILE_FEATURES(${mons_BIN} PRIVATE cxx_decltype_auto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${mons_BIN} DESTINATION bin)
