#include "argument.h"

#include <argparse/argparse.hpp>
#include <string>
#include <vector>

RunConfig parse_arguments(int argc, char *argv[]) {
  argparse::ArgumentParser program("puzzle_solver");

  program.add_argument("-a", "--algorithm")
      .default_value("Astar")
      .help("specify the algorithm to use. The available algorithms are: "
            "Astar and IDAstar. The default algorithm is Astar.")
      .action([](const std::string &value) {
        static const std::vector<std::string> valid_algorithms = {"Astar",
                                                                  "IDAstar"};
        if (std::find(valid_algorithms.begin(), valid_algorithms.end(),
                      value) != valid_algorithms.end()) {
          return value;
        }
        return std::string("Astar");
      });

  program.add_argument("-n", "--NxN-puzzle")
      .required()
      .help("specify the dimension of puzzle grid")
      .scan<'i', int>();

  program.add_argument("-i", "--input_file")
      .required()
      .help("specify the input file. The input file should be in the format of "
            "NxN puzzle grid, with each row separated by a newline character, "
            "and each number separated by a comma. The number 0 represents the "
            "empty slot.");

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  return {program.get<std::string>("--algorithm"),
          program.get<int>("--NxN-puzzle"),
          program.get<std::string>("--input_file")};
}