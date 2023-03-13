cmake_minimum_required(VERSION 3.17)
project(SlidingPuzzle
		VERSION 1.0
		DESCRIPTION "Sliding NxN-Puzzle"
		LANGUAGES CXX
		)

# Setup compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer")

# Setup C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup source directory
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Setup library
set(LIB_FILES
		${SRC_DIR}/argument.cpp)

# Setup argparse
include(FetchContent)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/_deps/argparse-src/include)

# Setup main executable
add_executable(puzzle_solver ${SRC_DIR}/main.cpp ${LIB_FILES})
