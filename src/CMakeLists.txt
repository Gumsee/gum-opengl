#
# Source
#
file(GLOB_RECURSE SRC 
"./*.cpp"

)
add_library(${CMAKE_PROJECT_NAME}_trgt STATIC ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/external/)

 
        if(OPENGL_FOUND)
            include_directories(${OPENGL_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for OPENGL cannot not found)
        endif() 
        if(GUMCODECS_FOUND)
            include_directories(${GUMCODECS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMCODECS cannot not found)
        endif() 
        if(GUMPRIMITIVES_FOUND)
            include_directories(${GUMPRIMITIVES_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMPRIMITIVES cannot not found)
        endif() 
        if(GUMESSENTIALS_FOUND)
            include_directories(${GUMESSENTIALS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMESSENTIALS cannot not found)
        endif() 
        if(GUMDESKTOP_FOUND)
            include_directories(${GUMDESKTOP_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMDESKTOP cannot not found)
        endif() 
        if(GUMSYSTEM_FOUND)
            include_directories(${GUMSYSTEM_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMSYSTEM cannot not found)
        endif() 
        if(GUMGLFW_FOUND)
            include_directories(${GUMGLFW_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMGLFW cannot not found)
        endif() 
        if(GUMMATHS_FOUND)
            include_directories(${GUMMATHS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMMATHS cannot not found)
        endif()

 

install(
    FILES gum-opengl.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/
) 

file(GLOB HEADERS OpenGL/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/OpenGL/
)




set(COMBINED_LIB ${CMAKE_BINARY_DIR}/lib${CMAKE_PROJECT_NAME}.a)

add_custom_target(createcombineddir
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/combined/
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
    DEPENDS ${CMAKE_PROJECT_NAME}_trgt
    VERBATIM
)

add_custom_command( #combinedtarget ALL
    OUTPUT ${COMBINED_LIB}
    
    COMMAND ar -x $<TARGET_FILE:${CMAKE_PROJECT_NAME}_trgt>
    COMMAND sh -c "ar -qcs \"${COMBINED_LIB}\" *.o"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/combined/
    DEPENDS  createcombineddir ${CMAKE_PROJECT_NAME}_trgt
    COMMENT "Combining libs"
    VERBATIM
)

add_custom_target(combinedlibtarget ALL DEPENDS ${COMBINED_LIB})

# Create an library target out of the library compilation result
add_library(${CMAKE_PROJECT_NAME} STATIC IMPORTED GLOBAL)
add_dependencies(${CMAKE_PROJECT_NAME} combinedlibtarget)

# Specify where the library is and where to find the headers
set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    IMPORTED_LOCATION "${COMBINED_LIB}"
)

install(FILES ${COMBINED_LIB}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})

#install(
#    EXPORT ${CMAKE_PROJECT_NAME}Targets
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
#    FILE ${CMAKE_PROJECT_NAME}Targets.cmake
#)

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

