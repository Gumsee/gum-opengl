#
# Source
#
file(GLOB_RECURSE SRC 
"./*.cpp"

)
add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/external/)

 
        if(OPENGL_FOUND)
            include_directories(${OPENGL_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for OPENGL cannot not found)
        endif() 
        if(GLEW_FOUND)
            include_directories(${GLEW_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GLEW cannot not found)
        endif() 
        if(GUMSYSTEM_FOUND)
            include_directories(${GUMSYSTEM_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMSYSTEM cannot not found)
        endif() 
        if(GUMMATHS_FOUND)
            include_directories(${GUMMATHS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMMATHS cannot not found)
        endif() 
        if(GUMESSENTIALS_FOUND)
            include_directories(${GUMESSENTIALS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMESSENTIALS cannot not found)
        endif() 
        if(GUMCODECS_FOUND)
            include_directories(${GUMCODECS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMCODECS cannot not found)
        endif()

 

install(
    FILES gum-opengl.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/
) 

file(GLOB_RECURSE HEADERS OpenGL//*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/OpenGL//
)



install(TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake
)

configure_package_config_file("Config.cmake.in"
    "${CMAKE_PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_LIBDIR
)

#write_basic_package_version_file(
#	${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
#	VERSION 1.0.0
#	COMPATIBILITY SameMajorVersion
#)

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
#          "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
)

export(PACKAGE ${CMAKE_PROJECT_NAME})

