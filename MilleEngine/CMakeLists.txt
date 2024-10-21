#Author: DONG Frank @2024

cmake_minimum_required(VERSION 3.23) 
set(NAME VentEngine)
project(${NAME})

include_directories(external)
include_directories(external/glm)
include_directories(external/imgui)
include_directories(external/tinygltf)
include_directories(external/ktx/include) 
include_directories(external/ktx/other_include)
include_directories(VentEngine)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
    find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/VentEngine/libs/vulkan)
    if(Vulkan_LIBRARY)
        set(Vulkan_FOUND ON)
        message("Found Vulkan libs")
    endif()
endif()

if(NOT Vulkan_FOUND) 
    message(FATAL_ERROR "Could not find Vulkan libs") 
else()
    message(STATUS ${Vulkan_LIBRARY})
endif() 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")

# assets and shaders path 
add_definitions(-DVK_EXAMPLE_ASSETS_DIR=\"${CMAKE_SOURCE_DIR}/assets/\")
add_definitions(-DVK_EXAMPLE_SHADERS_DIR=\"${CMAKE_SOURCE_DIR}/demos/shaders/\")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE *.cpp )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
add_subdirectory(VentEngine)
add_subdirectory(demos)
