set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -Wextra -Wno-pedantic -Wno-unused-parameter")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0") #Optimization

message(STATUS "Examples: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

list(APPEND GUM_LIBRARIES \"${CMAKE_PROJECT_NAME}\" ${CMAKE_THREAD_LIBS_INIT})

 
if(OPENGL_FOUND)
    list(APPEND GUM_LIBRARIES "${OPENGL_LIBRARIES}")
endif()
              
if(GLEW_FOUND)
    list(APPEND GUM_LIBRARIES "${GLEW_LIBRARIES}")
endif()
              
if(GUMMATHS_FOUND)
    list(APPEND GUM_LIBRARIES "${GUMMATHS_LIBRARIES}")
endif()
              
if(GUMESSENTIALS_FOUND)
    list(APPEND GUM_LIBRARIES "${GUMESSENTIALS_LIBRARIES}")
endif()
             

include_directories(${CMAKE_SOURCE_DIR}/src/)
 
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
endif()
              
if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
endif()
              
if(GUMMATHS_FOUND)
    include_directories(${GUMMATHS_INCLUDE_DIRS})
endif()
              
if(GUMESSENTIALS_FOUND)
    include_directories(${GUMESSENTIALS_INCLUDE_DIRS})
endif()
             

link_directories(${CMAKE_BINARY_DIR}/src/)

add_definitions(-DEXAMPLES_PATH=\"${CMAKE_SOURCE_DIR}/examples\")

set(EXAMPLE_FILE_LIST ObjectConversion)

foreach(EXAMPLE ${EXAMPLE_FILE_LIST})
    message(STATUS "Adding target ${EXAMPLE}")
    add_executable(Example_${EXAMPLE} "./${EXAMPLE}.cpp")
    target_link_libraries(Example_${EXAMPLE} "${GUM_LIBRARIES}")
endforeach()
