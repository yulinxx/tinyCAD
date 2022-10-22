################################################################
set(INC_DIR ./include ./shader)
include_directories(${INC_DIR})

set(CPP_DIR
    ./
)

foreach(SUB_DIRx ${CPP_DIR})
    message(STATUS "---Sub Dir: ${SUB_DIR}")

    file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR}/*.cpp")
    message(STATUS "---all cpp: ${SRC}")

    foreach(CPP ${SRC})
        message(STATUS "---Create Exe From: ${CPP}")

        STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${CPP})
        MESSAGE(" --- FILE_NAME"  ${FILE_NAME})

        add_executable(OpoenGL_${FILE_NAME} ${CPP})
        list(APPEND EXE_LIST OpoenGL_${FILE_NAME})
    endforeach(CPP)    

endforeach(SUB_DIR)


find_package(glfw3 CONFIG REQUIRED)
if(glfw3_FOUND)
    message(STATUS  " -----glfw3_FOUND")
    
    foreach(exe ${EXE_LIST})
        target_link_libraries(${exe} glfw)
    endforeach(exe ${EXE_LIST})

endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS  " -----GLEW_FOUND")
    
    foreach(exe ${EXE_LIST})
        target_link_libraries(${exe} ${GLEW_LIBRARIES})
    endforeach(exe ${EXE_LIST})
endif()

################################################################
foreach(exe ${EXE_LIST})
    target_link_libraries(${exe} GLAD)
endforeach(exe ${EXE_LIST})

################################################################
if(WIN32)
    set_target_properties(GLAD PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set_target_properties(GLAD PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin//Debug")
elseif(UNIX AND NOT APPLE)
elseif(APPLE)
endif(WIN32)

################################################################
include_directories(${CMAKE_SOURCE_DIR}/includes)

foreach(exe ${EXE_LIST})
    target_include_directories(${exe} PRIVATE ${CMAKE_SOURCE_DIR}/3rd)
endforeach(exe ${EXE_LIST})

