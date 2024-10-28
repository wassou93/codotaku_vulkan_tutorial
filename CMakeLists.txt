cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(PROJECT_NAME codotaku_vulkan_tutorial)

project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C)

# Add source files
set(SOURCES
    src/main.c
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Specify multiple include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    include
)

# Link Vulkan libraries
target_link_libraries(${PROJECT_NAME} glfw vulkan)
