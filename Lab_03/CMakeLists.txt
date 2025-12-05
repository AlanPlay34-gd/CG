cmake_minimum_required(VERSION 3.31)
project(Lab_03)

set(CMAKE_CXX_STANDARD 17)
add_executable(renderer
        src/main.cpp
        src/math.h
        src/rasterizer.cpp
        src/rasterizer.h
        src/camera.h
        src/shader.h
)

include_directories(third_party)
