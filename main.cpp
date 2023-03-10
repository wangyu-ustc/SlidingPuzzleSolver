#include <iostream>
#include <ctime>
#include "solutionIDAstar.h"
#include "solution.h"
using namespace std;

int main()
{
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
    status s("input4.txt", nullptr);
    s.set_target(target_pos);
    s.print();
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