cmake_minimum_required(VERSION 3.0.0)
project(tinyCAD VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

################################################################
set(INC_DIR ./include )
include_directories(${INC_DIR})

aux_source_directory(./src EXE_SRC)
add_executable(tinyCAD ${EXE_SRC})

################################################################
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message(STATUS  " -----OpenGL_FOUND")
    # include_directories(${OpenGL_INCLUDE_DIRS})
    # message(STATUS  ${OpenGL_INCLUDE_DIRS})

    # link_directories(${OpenGL_LIBRARY_DIRS})
    # message(STATUS  ${OpenGL_LIBRARY_DIRS})

    # target_link_libraries(tinyCAD ${OpenGL_LIBRARIES})
    # message(STATUS  ${OpenGL_LIBRARIES}) 
    
    # add_definitions(${OpenGL_DEFINITIONS})
    target_link_libraries(tinyCAD GL)
endif(OpenGL_FOUND)

################################################################
#GLAD
# add_subdirectory(./3rd/glad)
add_library(GLAD ${CMAKE_SOURCE_DIR}/3rd/glad/glad.c)
target_include_directories(GLAD PRIVATE ${CMAKE_SOURCE_DIR}/3rd/glad)
set(LIBS ${LIBS} GLAD)

find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
    message(STATUS  " -----glfw3_FOUND")
    #target_link_libraries(tinyCAD ${glfw_LIBRARIES})
    target_link_libraries(tinyCAD glfw)
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS  " -----GLEW_FOUND")
    #include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(tinyCAD ${GLEW_LIBRARIES})
endif()

################################################################
target_link_libraries(tinyCAD GLAD)
