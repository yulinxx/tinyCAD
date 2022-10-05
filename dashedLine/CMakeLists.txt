################################################################
set(INC_DIR ./include ./shader)
include_directories(${INC_DIR})

add_executable(dashedLine dashedLine.cpp)

################################################################
# find_package(OpenGL REQUIRED)
# if(OpenGL_FOUND)
#     message(STATUS  " -----OpenGL_FOUND")
#     # include_directories(${OpenGL_INCLUDE_DIRS})
#     # message(STATUS  ${OpenGL_INCLUDE_DIRS})

#     # link_directories(${OpenGL_LIBRARY_DIRS})
#     # message(STATUS  ${OpenGL_LIBRARY_DIRS})

#     # target_link_libraries(tinyCAD ${OpenGL_LIBRARIES})
#     # message(STATUS  ${OpenGL_LIBRARIES}) 
    
#     # add_definitions(${OpenGL_DEFINITIONS})
#     target_link_libraries(tinyCAD GL)
#     target_link_libraries(tinyCAD openGL)
# endif(OpenGL_FOUND)

# ################################################################
# if(WIN32)
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
#     # set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
#     # set_target_properties(GLAD PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin//Debug")
# elseif(UNIX AND NOT APPLE)
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# elseif(APPLE)
#     set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
#     set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/")
#     set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/")
# endif(WIN32)

################################################################
# #GLAD
# # add_subdirectory(./3rd/glad)
# include_directories(${CMAKE_SOURCE_DIR}/includes)
# add_library(GLAD ${CMAKE_SOURCE_DIR}/3rd/glad/glad.c)
# target_include_directories(GLAD PRIVATE ${CMAKE_SOURCE_DIR}/3rd/)
# set(LIBS ${LIBS} GLAD)

find_package(glfw3 CONFIG REQUIRED)
# find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
    message(STATUS  " -----glfw3_FOUND")
    target_link_libraries(dashedLine glfw)
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS  " -----GLEW_FOUND")
    target_link_libraries(dashedLine ${GLEW_LIBRARIES})
endif()

################################################################
target_link_libraries(dashedLine GLAD)

################################################################
if(WIN32)
    set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(GLAD PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin//Debug")
elseif(UNIX AND NOT APPLE)
elseif(APPLE)
endif(WIN32)

################################################################
# function(create_project_from_sources chapter demo)

include_directories(${CMAKE_SOURCE_DIR}/includes)
target_include_directories(dashedLine PRIVATE ${CMAKE_SOURCE_DIR}/3rd)
