cmake_minimum_required(VERSION 3.20)
project(vbrfix CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Fixer)

set(FIXER_SOURCES
        Fixer/ApeTag.cpp
        Fixer/CrcCalc.cpp
        Fixer/EndianHelper.cpp
        Fixer/FileBuffer.cpp
        Fixer/FixerSettings.cpp
        Fixer/Id3Tags.cpp
        Fixer/Mp3FileObject.cpp
        Fixer/Mp3Frame.cpp
        Fixer/Mp3Header.cpp
        Fixer/Mp3Reader.cpp
        Fixer/ReadSettings.cpp
        Fixer/UnknownDataObject.cpp
        Fixer/VbrFixer.cpp
        Fixer/VbriFrame.cpp
        Fixer/Version.cpp
        Fixer/XingFrame.cpp
        Fixer/LyricsTag.cpp
        Fixer/BitReader.h
        Fixer/FeedBackInterface.h
        Fixer/FrameObjectChecker.h
        Fixer/GenHelpers.h
        Fixer/Mp3FileObjectChecker.h
)

add_library(FixerLib STATIC ${FIXER_SOURCES})

set(CONSOLE_SOURCES
        ConsoleFixer/main.cpp
        ConsoleFixer/ConsoleFixer.cpp
        ConsoleFixer/CommandReader.cpp
)

add_executable(vbrfix ${CONSOLE_SOURCES})

target_link_libraries(vbrfix PRIVATE FixerLib)
target_include_directories(vbrfix PRIVATE Fixer)