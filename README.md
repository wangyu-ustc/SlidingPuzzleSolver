# NxN-Puzzle Solver

This is a C++ program that solves the NxN puzzle game using two different search algorithms, `A*` and `IDA*`.
This project was developed as a final project for CSE 202.

## 1. Requirements

To run the program, you need to have a GCC C++ or Clang compiler and CMake installed on your system.

### 1.1 For MacOS users

<!-- #### 1. Installing requirements -->

Firstly, for users using homebrew, you can install CMake by running the following command:

```bash
brew install cmake
```

*(Optional) Note that for macOS, we recommend using Clang as your preferred compiler. Also, to ensure coding style consistency, we suggest installing clang-format using brew.*

<!-- #### 2. Compiling on MacOS -->

Next, to compile the program, follow these steps:

1. Create a build folder

```sh
mkdir build && cd build
```

2. Configure the project

```sh
export CC=clang
export CXX=clang++
cmake -DCMAKE_BUILD_TYPE=Release ..
```

3. Build the project

```sh
cmake --build .
```

## 3. Usage

The program can be used with the following arguments:

```bash
Usage: puzzle_solver [--help] [--version] [--algorithm VAR] --NxN-puzzle VAR --input_file VAR

Optional arguments:
  -h, --help            shows help message and exits 
  -v, --version         prints version information and exits 
  -a, --algorithm       specify the algorithm to use. The available algorithms are: Astar and IDAstar. The default algorithm is Astar. [default: "Astar"]
  -n, --NxN-puzzle      specify the dimension of puzzle grid [required]
  -i, --input_file      specify the input file. The input file should be in the format of NxN puzzle grid, with each row separated by a newline character, and each number separated by a comma. The number 0 represents the empty slot. [required]
```

An example usage case is provided in `example_run.sh`. To generate files with random input status for a given puzzle grid dimension, a Python script genInput.py is included in the `./res` folder.