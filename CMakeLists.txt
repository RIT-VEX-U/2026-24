cmake_minimum_required(VERSION 3.16)


set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/VexV5Toolchain.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(VEX_QUIET_BUILD "Suppress compiler warnings" OFF)

if(NOT DEFINED VEX_PROJECT_NAME)
    set(VEX_PROJECT_NAME "VexProject")
endif()

set(VEX_PROJECT_NAME "${VEX_PROJECT_NAME}" CACHE STRING "Name from .vscode/vex_project_settings.json")

project(${VEX_PROJECT_NAME} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(core/include)
include_directories(vendor/eigen)
include_directories(vendor/gcem/include)

file(GLOB_RECURSE MAIN_SOURCES
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE CORE_SOURCES
    "core/src/*.cpp"
    "core/src/*.c"
)

vex_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    ${MAIN_SOURCES}
    ${CORE_SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DVexV5)
