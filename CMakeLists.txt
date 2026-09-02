cmake_minimum_required(VERSION 3.12)
project(glengine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

file(GLOB_RECURSE GLENGINE_SOURCE_FILES "src/glengine/*.cpp")
file(GLOB_RECURSE GLENGINE_HEADER_FILES "include/glengine/*.hpp")

include_directories("include/")

add_library(stb_image "src/stb_image.cpp" "include/stb_image.hpp")

add_library(glengine)
target_sources(
    glengine
    PUBLIC
    FILE_SET HEADERS TYPE HEADERS FILES ${GLENGINE_HEADER_FILES}
    FILE_SET SOURCES TYPE SOURCES FILES ${GLENGINE_SOURCE_FILES}
)
target_link_libraries(glengine PRIVATE GL GLEW::GLEW glfw glm assimp stb_image)
target_compile_definitions(glengine PUBLIC GLFW_INCLUDE_NONE GLEW_STATIC)

add_executable(glengine-main "src/main.cpp")
target_link_libraries(glengine-main PRIVATE glengine)
