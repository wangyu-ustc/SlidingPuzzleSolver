#include "argument.h"
#include "solution.h"
#include "solutionIDAstar.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <set>

using namespace std;

int main(int argc, char *argv[]) {
  auto run_config = parse_arguments(argc, argv);
  std::cout << "Welcome to NxN-Puzzle Solver!" << std::endl;

  // Usage ./program input.txt N
  //   solution sol; // 使用A*算法
  //    solutionIDAstar sol;  //使用IDA*算法

  //   int N = stoi(argv[2]);
  int N = run_config.puzzle_size;
  std::cout << "N=" << N << std::endl;

  //   status s(argv[1], nullptr, N);
  status s(run_config.input_file.c_str(), nullptr, N, run_config.k);

  int maxNum = s.size - s.slot_size;
  int *target_pos = new int[maxNum + 1];
  target_pos[0] = 0;
  for (int i = 1; i <= maxNum;
       i++) { // the nums are ordered, with the slots at the end
    target_pos[i] = i - 1;
  }
  std::cout << "Initial status:" << std::endl;
  s.print();
  if (false) // comment this line if you need random target
  {

    target_pos = new int[maxNum + 1];
    target_pos[0] = 0;
    set<int> pos;
    while (pos.size() <
           maxNum) { // To random pick maxNum positions on the board
      pos.insert(std::rand() % s.size);
    }
    int *nums = new int[maxNum + 1];
    for (int i = 0; i < maxNum; i++) {
      nums[i] = i + 1;
    }
    auto rng = std::default_random_engine{};
    rng.seed(::clock());
    std::shuffle(nums, nums + maxNum, rng);
    int i = 0;
    status target(N);
    for (int x : pos) {
      target_pos[nums[i]] = x;
      target.board[x] = nums[i];
      i++;
    }
    std::cout << "Target status:" << std::endl;
    target.print();
  }

  s.set_target(target_pos);
  int starttime = clock();

  // run A* or IDA*
  if (run_config.algorithm == "Astar") {
    solution solA;
    solA.solve(s);
  } else {
    solutionIDAstar solIDA;
    solIDA.solve(s);
  }

  int endtime = clock();
  std::cout << "Time : " << (endtime - starttime) / 1000 << "ms" << std::endl;
  delete[] target_pos;
  return 0;
}

/*
0, 1, 2, 3, 4,
        10, 5, 7, 8, 9,
        12, 13, 14, 15, 16,
        17, 18, 19, 20, 21,
        22 };
*/