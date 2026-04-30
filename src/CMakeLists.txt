#
# Source
#
file(GLOB_RECURSE SRC "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC})

macro(add_guminclude LIBNAME)
  if(${LIBNAME}_FOUND)
      include_directories(${${LIBNAME}_INCLUDE_DIRS})
  else()
      message(FATAL_ERROR "Library for ${LIBNAME} cannot not be found")
  endif() 
endmacro()

add_guminclude(GUMGRAPHICS)
add_guminclude(GUMCODECS)
add_guminclude(GUMPRIMITIVES)
add_guminclude(GUMESSENTIALS)
add_guminclude(GUMDESKTOP)
add_guminclude(GUMSYSTEM)
add_guminclude(GUMMATHS)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../external/glad/include)

# Specify where the library is and where to find the headers
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib${CMAKE_PROJECT_NAME}.a"
)

install(FILES ${CMAKE_BINARY_DIR}/lib${CMAKE_PROJECT_NAME}.a
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#install(
#    EXPORT ${PROJECT_NAME}Targets
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#    FILE ${PROJECT_NAME}Targets.cmake
#)

if(NOT DEFINED DISABLE_PACKAGE_CONFIGURATION)
  configure_package_config_file("Config.cmake.in"
      "${PROJECT_NAME}Config.cmake"
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
      PATH_VARS CMAKE_INSTALL_LIBDIR
  )
endif()

#write_basic_package_version_file(
#	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#	VERSION 1.0.0
#	COMPATIBILITY SameMajorVersion
#)

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

export(PACKAGE ${PROJECT_NAME})

