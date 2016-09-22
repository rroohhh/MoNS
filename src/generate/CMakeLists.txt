PROJECT(generate)
FILE (GLOB_RECURSE generate_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (generate_LIBS mons)
SET (generate_BIN ${PROJECT_NAME})

ADD_EXECUTABLE(${generate_BIN} ${generate_SRCS})
TARGET_LINK_LIBRARIES(${generate_BIN} ${generate_LIBS})

SET_TARGET_PROPERTIES(${generate_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${generate_BIN} CLEAN_DIRECT_OUTPUT 1)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -parallel -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -fPIC")

# TARGET_COMPILE_FEATURES(${generate_BIN} PRIVATE cxx_constexpr)
# TARGET_COMPILE_FEATURES(${generate_BIN} PRIVATE cxx_decltype_auto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${generate_BIN} DESTINATION bin)
