cmake_minimum_required (VERSION 3.0)
project (GumOpenGL)

add_definitions(-DDEBUG)
add_definitions(-DCHECK_GL_ERRORS)

set(GUM_INSTALLDIR "/usr/include/gumball/")
set(GUMBALL_INCLUDE_DIR "/usr/include/gumball/")
set(GUMMATH_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMESSENTIALS_DIRECTORY ${GUMBALL_INCLUDE_DIR})

add_subdirectory(./src/)
add_subdirectory(./examples/)
