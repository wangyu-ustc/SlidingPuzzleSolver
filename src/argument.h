#ifndef ARGUMENT_H_
#define ARGUMENT_H_

#include <filesystem>
#include <string>

class RunConfig {
public:
  std::string algorithm; // Astar or IDAstar
  int puzzle_size;
  std::string input_file;

  RunConfig(std::string algorithm, int puzzle_size, std::string input_file)
      : algorithm(algorithm), puzzle_size(puzzle_size), input_file(input_file) {
  }
};

RunConfig parse_arguments(int argc, char *argv[]);

#endif