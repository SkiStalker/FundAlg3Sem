cmake_minimum_required(VERSION 3.16)
project(ProjectName LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-std=c++20 -Wall -Wextra -Wpedantic -Werror)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fsanitize=leak)
    add_link_options(-fsanitize=address -fsanitize=leak)
endif()

add_subdirectory(utils)
add_subdirectory(ipc)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(subprocesses)
