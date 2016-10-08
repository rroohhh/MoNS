PROJECT(libsshfile)
FILE (GLOB_RECURSE libsshfile_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (libsshfile_LIBS ${LIBSSH_SHARED_LIBRARY} mons Threads::Threads util)
SET (libsshfile_BIN "sshfile")

ADD_LIBRARY(${libsshfile_BIN} ${libsshfile_SRCS})
TARGET_LINK_LIBRARIES(${libsshfile_BIN} ${libsshfile_LIBS})
TARGET_INCLUDE_DIRECTORIES(${libsshfile_BIN} PUBLIC ".")
TARGET_INCLUDE_DIRECTORIES(${libsshfile_BIN} PUBLIC ${LIBSSH_PUBLIC_INCLUDE_DIRS})

SET_TARGET_PROPERTIES(${libsshfile_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${libsshfile_BIN} CLEAN_DIRECT_OUTPUT 1)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -parallel -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -fPIC")

# SET_TARGET_PROPERTIES(${libsshfile_BIN} PROPERTIES LINK_FLAGS -Wl,-e,_main)
TARGET_COMPILE_FEATURES(${libsshfile_BIN} PRIVATE cxx_constexpr)
TARGET_COMPILE_FEATURES(${libsshfile_BIN} PRIVATE cxx_decltype_auto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${libsshfile_BIN} DESTINATION bin)
