cmake_minimum_required(VERSION 3.20)
project(Lab_04 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно указываем файлы вместо GLOB
set(SOURCES
        main.cpp
        # Добавьте другие .cpp файлы, если есть
)

set(HEADERS
        Window.cpp
        Window.h
        InputDevice.cpp
        InputDevice.h
        # Добавьте .h файлы, если есть
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        UNICODE
        _UNICODE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        user32
        gdi32
)