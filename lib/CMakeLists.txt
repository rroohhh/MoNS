ADD_SUBDIRECTORY(fmt)
ADD_SUBDIRECTORY(libplot)
ADD_SUBDIRECTORY(libreadline)
ADD_SUBDIRECTORY(cpptoml)
ADD_SUBDIRECTORY(libssh)
ADD_SUBDIRECTORY(sfml)

# include(ExternalProject)
# ExternalProject_Add(libedit
#   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libedit
#   CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libedit/configure --prefix=<INSTALL_DIR>
#   BUILD_COMMAND ${MAKE})
