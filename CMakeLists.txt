cmake_minimum_required(VERSION 3.10)

# Project name
project(FileCompressorTool)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories (if you have headers)
include_directories(${PROJECT_SOURCE_DIR})

# Add the source files
set(SOURCES
    compressor
    decompressor
    huffman.cpp
    encode.cpp
    decode.cpp
)

# Add the headers if needed (like huffman.hpp)
set(HEADERS
    huffman.hpp
)

# Create the executable
add_executable(file_compressor ${SOURCES})

# If you need to link any libraries, you can add them here
# target_link_libraries(file_compressor <library_name>)
