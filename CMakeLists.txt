cmake_minimum_required(VERSION 3.20)

project(Chip8Emulator CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})

set(SOURCES src/main.cpp 
            src/Chip8.cpp 
            src/Chip8.h
            src/SDLWrapper/Display.cpp
            src/SDLWrapper/Display.h
            src/SDLWrapper/Input.cpp
            src/SDLWrapper/Input.h
            src/SDLWrapper/SDLWrapper.cpp
            src/SDLWrapper/SDLWrapper.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})