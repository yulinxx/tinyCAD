cmake_minimum_required(VERSION 3.0.0)

# 使用vcpkg进行包管理
if(WIN32)
    set(VCPKG_PATH D:/Install/vcpkg/scripts/buildsystems/vcpkg.cmake)
elseif(UNIX AND NOT APPLE)
    set(VCPKG_PATH /home/x/install/vcpkg//scripts/buildsystems/vcpkg.cmake)
elseif(APPLE)
endif(WIN32)

set(CMAKE_TOOLCHAIN_FILE  ${VCPKG_PATH})

project(tinyCAD VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

################################################################
set(INC_DIR ./include ./shader)
include_directories(${INC_DIR})

set(APP_NAME tinyCAD)
aux_source_directory(./src EXE_SRC)
add_executable(${APP_NAME} ${EXE_SRC})

# add_executable(testGL ./test2/main2.cpp)

################################################################
# find_package(OpenGL REQUIRED)
# if(OpenGL_FOUND)
#     message(STATUS  " -----OpenGL_FOUND")
#     # include_directories(${OpenGL_INCLUDE_DIRS})
#     # message(STATUS  ${OpenGL_INCLUDE_DIRS})

#     # link_directories(${OpenGL_LIBRARY_DIRS})
#     # message(STATUS  ${OpenGL_LIBRARY_DIRS})

#     # target_link_libraries(${APP_NAME} ${OpenGL_LIBRARIES})
#     # message(STATUS  ${OpenGL_LIBRARIES}) 
    
#     # add_definitions(${OpenGL_DEFINITIONS})
#     target_link_libraries(${APP_NAME} GL)
#     target_link_libraries(${APP_NAME} openGL)
# endif(OpenGL_FOUND)

################################################################
if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    # set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    # set_target_properties(GLAD PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin//Debug")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
elseif(APPLE)
    set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/")
endif(WIN32)

#################### GLAD #################################
#GLAD
# add_subdirectory(./3rd/glad)
include_directories(${CMAKE_SOURCE_DIR}/includes)
add_library(GLAD ${CMAKE_SOURCE_DIR}/3rd/glad/glad.c)
target_include_directories(GLAD PRIVATE ${CMAKE_SOURCE_DIR}/3rd/)
set(LIBS ${LIBS} GLAD)
target_link_libraries(${APP_NAME}  GLAD)

add_library(STB_IMAGE ${CMAKE_SOURCE_DIR}/3rd/stb_image.cpp)
set(LIBS ${LIBS} STB_IMAGE)
target_link_libraries(${APP_NAME}  STB_IMAGE)

################### GLFW3 #################################
find_package(glfw3 CONFIG REQUIRED)
# find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
    message(STATUS  " -----glfw3_FOUND")
    #target_link_libraries(${APP_NAME} ${glfw_LIBRARIES})
    target_link_libraries(${APP_NAME} glfw)
endif()

################## GLEW ####################################
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS  " -----GLEW_FOUND")
    #include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${APP_NAME}  ${GLEW_LIBRARIES})
    # target_link_libraries(${APP_NAME} PRIVATE GLEW::GLEW)
    # target_link_libraries(${APP_NAME} PRIVATE glfw)
endif()

################# GLOG #####################################
find_package(glog CONFIG REQUIRED)
if(glog_FOUND)
    message(STATUS  " -----glog_FOUND")
    # target_link_libraries(${APP_NAME} PRIVATE glog::glog)
    # target_link_libraries(${APP_NAME} PRIVATE ${glog_LIBRARIES})
    target_link_libraries(${APP_NAME}  glog::glog)
endif()

################# Boost #####################################
find_package(Boost REQUIRED)
if(Boost_FOUND)
    message(STATUS  " -----Boost_FOUND")
 
    target_include_directories(${APP_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${APP_NAME} ${Boost_LIBRARY_DIRS})
endif()

################# ImGui #####################################
find_package(imgui CONFIG REQUIRED)
if(imgui_FOUND)
    message(STATUS  " -----imgui_FOUND")
    target_link_libraries(${APP_NAME}  imgui::imgui)
endif()

################# OUPUT ######################################
if(WIN32)
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(${APP_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin//Debug")
    set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(GLAD PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin//Debug")
elseif(UNIX AND NOT APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
elseif(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/")
endif(WIN32)

################################################################
function(FILL_COPY)

    # copy shader files to build directory
    file(GLOB SHADERS
         "shader/*.vs"
         "shader/*.frag"
         "shader/*.fs"
         "shader/*.gs"
	     "shader/*.cs"
    )

    foreach(SHADER ${SHADERS})
        if(WIN32)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/)
        elseif(APPLE)
            # create symbolic link for *.vs *.fs *.gs
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${SHADERNAME} ${APP_NAME})
        endif(WIN32)
    endforeach(SHADER)
endfunction()

FILL_COPY()

include_directories(${CMAKE_SOURCE_DIR}/includes)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rd)

################################################################
# add_subdirectory(dashedLine)
