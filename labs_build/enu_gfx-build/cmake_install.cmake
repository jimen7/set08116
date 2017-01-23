# Install script for directory: C:/Users/40204497/Desktop/set08116/labs_build/enu_gfx-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/set08116_graphics")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/40204497/Desktop/set08116/labs_build/zlib-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/glfw-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/glew-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/glm-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/assimp-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/libpng-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/libjpeg-build/cmake_install.cmake")
  include("C:/Users/40204497/Desktop/set08116/labs_build/devil-build/cmake_install.cmake")

endif()

