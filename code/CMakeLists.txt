cmake_minimum_required(VERSION 3.14)
project(NumBrainer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

# Add executable
add_executable(${PROJECT_NAME} WIN32 code/main.cpp)

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Set Windows subsystem
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif() 