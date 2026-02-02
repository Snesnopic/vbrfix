cmake_minimum_required(VERSION 3.20)
project(vbrfix CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
        src/core
        src/lib
        include
)

file(GLOB CORE_SOURCES "src/core/*.cpp")
add_library(vbrfix_core STATIC ${CORE_SOURCES})
target_include_directories(vbrfix_core PUBLIC src/core)

add_library(vbrfix_lib STATIC src/lib/vbrfix_lib.cpp)
target_link_libraries(vbrfix_lib PRIVATE vbrfix_core)
target_include_directories(vbrfix_lib PUBLIC include)

add_executable(vbrfix src/cli/main.cpp src/cli/ConsoleFixer.cpp src/cli/CommandReader.cpp
)
target_link_libraries(vbrfix PRIVATE vbrfix_core)
target_include_directories(vbrfix PRIVATE src/cli)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(vbrfix PRIVATE stdc++fs)
    target_link_libraries(vbrfix_lib PRIVATE stdc++fs)
endif()

include(GNUInstallDirs)

install(TARGETS vbrfix DESTINATION ${CMAKE_INSTALL_BINDIR})

install(TARGETS vbrfix_lib DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/vbrfix DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})