cmake_minimum_required (VERSION 3.0)
project(GumOpenGL)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0090 NEW)
set(CMAKE_EXPORT_PACKAGE_REGISTRY TRUE)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

#Set OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DGUM_OS_LINUX)
    add_definitions(-DGUM_OS_UNIX)
    set(GUM_OS_LINUX true)
    set(GUM_OS_UNIX true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DGUM_OS_WINDOWS)
    add_definitions(-DGUM_OS_DOSBASED)
    set(GUM_OS_WINDOWS true)
    set(GUM_OS_DOSBASED true)

    set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Gumlib/")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGUM_OS_MACOS)
    add_definitions(-DGUM_OS_UNIX)
    set(GUM_OS_MACOS true)
    set(GUM_OS_UNIX true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_definitions(-DGUM_OS_BSD)
    add_definitions(-DGUM_OS_UNIX)
    set(GUM_OS_BSD true)
    set(GUM_OS_UNIX true)
endif()

set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
if(DEFINED GUM_OS_LINUX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -fdiagnostics-color=always")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-unused-parameter -Wno-reorder -Wno-pedantic")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC") #-Og
elseif(DEFINED GUM_OS_WINDOWS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    set (CMAKE_CXX_FLAGS "/JMC /permissive- /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope /Gd /MDd /FC /EHsc /nologo /diagnostics:column")
endif()

set(CMAKE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
set(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

message(STATUS "${CMAKE_PROJECT_NAME}: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

 
    find_package(OpenGL REQUIRED) 
    find_package(GLEW REQUIRED) 
    find_package(GumSystem REQUIRED) 
    find_package(GumMaths REQUIRED) 
    find_package(GumEssentials REQUIRED) 
    find_package(GumCodecs REQUIRED) 
    find_package(GumDesktop REQUIRED) 
    find_package(GumGLFW REQUIRED)

#add_definitions(-DDEBUG)
add_definitions(-DCHECK_GL_ERRORS)



add_subdirectory(src)
add_subdirectory(examples)
