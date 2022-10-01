add_library(GLAD "glad.c")
set(LIBS ${LIBS} GLAD)
include_directories(${CMAKE_SOURCE_DIR})
