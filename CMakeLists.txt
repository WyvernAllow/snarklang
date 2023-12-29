cmake_minimum_required(VERSION 3.20)
project(snarkc VERSION 0.1.0 LANGUAGES C)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.c")
add_executable(${PROJECT_NAME} ${sources})

set(CMAKE_C_STANDARD 99)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4          
        /WX
        /permissive-
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wno-unused-function
        -Wno-unused-parameter
        -Wno-unused-variable
    )
endif()