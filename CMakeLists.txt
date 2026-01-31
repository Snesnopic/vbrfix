cmake_minimum_required(VERSION 3.20)
project(vbrfix CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Core Library ---
file(GLOB CORE_SOURCES "src/core/*.cpp")
add_library(vbrfix_core STATIC ${CORE_SOURCES})
target_include_directories(vbrfix_core PUBLIC src/core)

# --- CLI Executable ---
add_executable(vbrfix src/cli/main.cpp src/cli/ConsoleFixer.cpp src/cli/CommandReader.cpp)
target_link_libraries(vbrfix PRIVATE vbrfix_core)
target_include_directories(vbrfix PRIVATE src/cli)