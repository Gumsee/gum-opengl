set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -Wextra -Wno-pedantic -Wno-unused-but-set-variable -Wno-unused-parameter")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0") #Optimization

message(STATUS "Examples: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

link_directories(${CMAKE_SOURCE_DIR}/lib/)

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${GUMMATH_DIRECTORY})
include_directories(${GUMESSENTIALS_DIRECTORY})

add_definitions(-DEXAMPLES_PATH=\"${CMAKE_SOURCE_DIR}/examples/\")

set(EXAMPLE_FILE_LIST 
    ObjectConversion
)

set(GUMOPENGL_LIBRARIES 
    GumEssentials
    GumMaths
    GLEW 
    GL
    ${CMAKE_THREAD_LIBS_INIT}
)

foreach(EXAMPLE ${EXAMPLE_FILE_LIST})
    message(STATUS "Adding target ${EXAMPLE}")
    add_executable(Example_${EXAMPLE} "./${EXAMPLE}.cpp")
    target_link_libraries(Example_${EXAMPLE} ${GUMOPENGL_LIBRARIES})
endforeach()