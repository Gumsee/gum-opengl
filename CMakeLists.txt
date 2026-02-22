cmake_minimum_required (VERSION 3.0...3.9)
project(GumOpenGL)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0090 NEW)
set(CMAKE_EXPORT_PACKAGE_REGISTRY TRUE)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(GUMOPENGL_BUILD_EXAMPLES "Should the exampleprojects be build" ON)

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

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_definitions(-DGUM_OS_ANDROID)
    add_definitions(-DGUM_OS_UNIX)
    set(GUM_OS_ANDROID true)
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

message(STATUS "${PROJECT_NAME}: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

 
    
        if(NOT OPENGL_FOUND) 
            find_package(OpenGL REQUIRED)
        endif() 
    
        if(NOT GUMGRAPHICS_FOUND) 
            find_package(GumGraphics REQUIRED)
        endif() 
    
        if(NOT GUMCODECS_FOUND) 
            find_package(GumCodecs REQUIRED)
        endif() 
    
        if(NOT GUMPRIMITIVES_FOUND) 
            find_package(GumPrimitives REQUIRED)
        endif() 
    
        if(NOT GUMESSENTIALS_FOUND) 
            find_package(GumEssentials REQUIRED)
        endif() 
    
        if(NOT GUMDESKTOP_FOUND) 
            find_package(GumDesktop REQUIRED)
        endif() 
    
        if(NOT GUMSYSTEM_FOUND) 
            find_package(GumSystem REQUIRED)
        endif() 
    
        if(NOT GUMMATHS_FOUND) 
            find_package(GumMaths REQUIRED)
        endif()

#add_definitions(-DDEBUG)
add_definitions(-DCHECK_GL_ERRORS)



add_subdirectory(src)
if(GUMOPENGL_BUILD_EXAMPLES)
    add_subdirectory(examples)
    endif()
    
