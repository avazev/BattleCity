cmake_minimum_required(VERSION 4.2.1 FATAL_ERROR)

project(GLAD)

add_library(glad STATIC include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include)
