cmake_minimum_required (VERSION 3.0)
project(GumOpenGL)
cmake_policy(SET CMP0072 NEW)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

#Set OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DGUM_OS_LINUX)
    set(GUM_OS_LINUX true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DGUM_OS_WINDOWS)
    set(GUM_OS_WINDOWS true)

    set(CMAKE_INSTALL_PREFIX "/Program Files (x86)/Gumlib/")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGUM_OS_MACOS)
    set(GUM_OS_MACOS true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_definitions(-DGUM_OS_BSD)
    set(GUM_OS_BSD true)
endif()

 
find_package(OpenGL REQUIRED)
              
find_package(GLEW REQUIRED)
              
find_package(GumSystem REQUIRED)
              
find_package(GumMaths REQUIRED)
              
find_package(GumEssentials REQUIRED)
             

#add_definitions(-DDEBUG)
add_definitions(-DCHECK_GL_ERRORS)



add_subdirectory(src)
add_subdirectory(examples)
