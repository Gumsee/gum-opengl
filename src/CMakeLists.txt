set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -Wextra -Wno-unused-parameter -Wno-reorder -Wno-pedantic")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0") #-Og

message(STATUS "GumOpenGL: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${GUMMATH_DIRECTORY})
include_directories(${GUMESSENTIALS_DIRECTORY})

#
# Source
#
file(GLOB_RECURSE SRC "./*.cpp")

add_library(GumOpenGL STATIC ${SRC})

file(GLOB HEADERS "./OpenGL/*.h")
install(
        FILES ${HEADERS}
	DESTINATION ${GUM_INSTALLDIR}/OpenGL/
)

install(
        FILES "./gum-opengl.h"
        DESTINATION ${GUM_INSTALLDIR}/
)

install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)