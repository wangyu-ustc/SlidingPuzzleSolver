#include <iostream>
#include <ctime>
#include "solutionIDAstar.h"
#include "solution.h"
#include <set>
#include <random>

using namespace std;

int main(int argc, char *argv[])
{
    // Usage ./program input.txt N
    solution sol;       //使用A*算法
    //solutionIDA* sol  //使用IDA*算法

    int* target_pos=new int[24]{ 0,
                                 0, 1, 2, 3, 4,
                                 10, 5, 7, 8, 9,
                                 12, 13, 14, 15, 16,
                                 17, 18, 19, 20, 21,
                                 22,
                                 6, 11
    };
    int N = stoi(argv[2]);
    std::cout << "N=" << N << std::endl;
    status s(argv[1], nullptr, N);
    std::cout << "Initial status:" << std::endl;
    s.print();
    if (N != 5) // comment this line if not needed
    {
        int maxNum = s.size - s.slot_size;
        target_pos = new int[maxNum+1];
        target_pos[0] = 0;
        set<int> pos;
        while (pos.size() < maxNum) { // To random pick maxNum positions on the board
            pos.insert(std::rand() % s.size);
        }
//        int * pos = new int[maxNum];
        int* nums = new int[maxNum + 1];
        for(int i=0; i< maxNum; i++) {
            nums[i] = i + 1;
        }
        auto rng = std::default_random_engine {};
        rng.seed(::clock());
        std::shuffle(nums, nums + maxNum, rng);
        int i = 0;
        status target(N);
        for (int x: pos) {
//            std::cout << x<<std::endl;
            target_pos[nums[i]] = x;
            target.board[x] = nums[i];
            i++;
        }
        std::cout << "Target status:" << std::endl;
        target.print();
    }


    s.set_target(target_pos);
    int starttime = clock();
    sol.solve(s);
    int endtime = clock();
    std::cout << "Time : " <<endtime-starttime<<"ms" <<std::endl;
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