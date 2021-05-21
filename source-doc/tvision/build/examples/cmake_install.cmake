# Install script for directory: /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvedit/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdemo/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvforms/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvhc/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/mmenu/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/palette/cmake_install.cmake")
  include("/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/avscolor/cmake_install.cmake")

endif()

