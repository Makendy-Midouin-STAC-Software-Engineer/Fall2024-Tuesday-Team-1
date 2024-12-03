cmake_minimum_required(VERSION 3.15)
project(NumBrainer VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add raylib subdirectory
add_subdirectory(external/raylib)

# Source files
set(SOURCES
        main.cpp
        Game.cpp
        GameUI.h
        GameUI.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)