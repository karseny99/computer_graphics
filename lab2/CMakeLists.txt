cmake_minimum_required(VERSION 3.0.0)
project(lab2)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(main
    src/config.h
    src/main.cpp 
    src/glad.c
)

target_include_directories(main 
    PRIVATE
    dependencies 
)

target_link_libraries(main
    glfw
    OpenGL::GL 
)