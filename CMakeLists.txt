cmake_minimum_required (VERSION 3.0)
project (GumOpenGL)

add_definitions(-DDEBUG)

set(GUMMATH_DIRECTORY "/home/gumse/Projects/gumball/gum-maths/")
set(GUMESSENTIALS_DIRECTORY "/home/gumse/Projects/gumball/gum-essentials/")
set(GUM_INSTALLDIR "/usr/include/gumball/")

add_subdirectory(./src/)
add_subdirectory(./examples/)
