cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

project(xxd LANGUAGES C CXX)

file(GLOB SOURCES
    "src/*.c"
)

set(EXE xxd)

add_executable(${EXE} ${SOURCES})

add_custom_command(
        TARGET xxd POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/xxd.exe        
                ${CMAKE_SOURCE_DIR}
                )

