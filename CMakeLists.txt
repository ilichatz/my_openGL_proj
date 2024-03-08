cmake_minimum_required(VERSION 3.12.0)
project(my_opengl_project VERSION 0.5.4)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(my_opengl_project 
    src/config.h
    src/config.cpp
    src/main.cpp
    src/glad.h
    src/glad.c
    src/triangle_mesh.cpp
    src/material.cpp
)



target_link_libraries(my_opengl_project 
    glfw
    OpenGL::GL
    )
