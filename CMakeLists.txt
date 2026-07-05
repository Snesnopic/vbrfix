cmake_minimum_required(VERSION 3.20)
project(vbrfix VERSION 0.34 LANGUAGES CXX)

if(MSVC)
    # force static msvc runtime linkage (/mt for release, /mtd for debug)
    if(POLICY CMP0091)
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Core Library ---
file(GLOB CORE_SOURCES "src/core/*.cpp")
add_library(vbrfix_core STATIC ${CORE_SOURCES})
target_include_directories(vbrfix_core PUBLIC src/core)
target_compile_definitions(vbrfix_core PRIVATE VBRFIX_VERSION="${PROJECT_VERSION}")

# --- CLI Executable ---
add_executable(vbrfix src/cli/main.cpp src/cli/ConsoleFixer.cpp src/cli/CommandReader.cpp)
target_link_libraries(vbrfix PRIVATE vbrfix_core)
target_include_directories(vbrfix PRIVATE src/cli)

include(GNUInstallDirs)
install(TARGETS vbrfix DESTINATION ${CMAKE_INSTALL_BINDIR})