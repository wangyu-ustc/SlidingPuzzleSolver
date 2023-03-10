#pragma once

#include<map>
#include<set>
#include<list>
#include<tuple>
#include<queue>
#include<fstream>

typedef unsigned char uchar;

typedef std::tuple<int, uchar, char> TAction;

struct status
{
    static const int N = 5;
    static const int size = 25;
    int invalid = -1;

    int* board = nullptr;       // 棋盘状况
    int slot[2];                // 空槽位置
    int hash;                   // 哈希数值
    int h;                      // 启发式值

    int* target_pos;

    TAction last_action;

    bool is_deprecated = false;

    status* father = nullptr;

    int g;      //已消耗代价
    status() { board = new int[size]; g = 0; }

    status(const status& s) {
        board = new int[size];
        for (int i = 0; i < size; i++)
            board[i] = s.board[i];
        g = s.g;
        h = s.h;
        target_pos = s.target_pos;
        slot[0] = s.slot[0];
        slot[1] = s.slot[1];
        father = s.father;
        last_action = s.last_action;
        hash = s.hash;
    }

    void operator =(const status& s)
    {
        delete board;
        board = new int[size];
        for (int i = 0; i < size; i++)
            board[i] = s.board[i];
        g = s.g;
        h = s.h;
        target_pos = s.target_pos;
        slot[0] = s.slot[0];
        slot[1] = s.slot[1];
        father = s.father;
        last_action = s.last_action;
        hash = s.hash;
    }
    status(const char* path, int* _target_pos)
    {
        board = new int[size];
        g = 0;
        std::fstream ifs;
        ifs.open(path);
        if (!ifs.is_open())
        {
            printf("FILE ERROR");
            return;
        }
        char ch;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N - 1; j++)
                ifs >> board[i * N + j] >> ch;
            ifs >> board[i * N + N - 1];
        }
        int cnt = 0;
        for (int i = 0; i < size; i++)
        {
            if (board[i] == 0)
                slot[cnt++] = i;
        }
        get_hash();
        if (_target_pos != nullptr)
            get_herustic();
        else
            h = 0;
        target_pos = _target_pos;
        last_action = TAction(0, 0, 0);
        father = nullptr;
    }

    ~status() { delete[] board; }

    //废弃，并部分释放内存
    void deprecate()
    {
        is_deprecated = true;
        delete[] board;
        board = nullptr;
    }

    void set_target(int* _tar)
    {
        target_pos = _tar;
        get_herustic();
    }
    constexpr int& operator()(const int row, const int col)const {
        return board[row * N + col];
    }
    constexpr int& operator[](const int idx)const {
        return board[idx];
    }

    void get_hash()
    {
        hash = 0;
        int x = 1;
        for (int i = 0; i < size; i++)
        {
            hash += board[i] * x;
            x *= 21;
            x %= 100000223;
            hash %= 100000223;
        }
    }

    static std::tuple<int, int> coord(int x) { return { x / N, x % N }; }

    int& up(int x) {
        if (x < 5)
            return invalid;
        else
            return board[x - 5];
    }
    int& down(int x) {
        if (x > 20)
            return invalid;
        else
            return board[x + 5];
    }
    int& left(int x) {
        if (x % 5 == 0)
            return invalid;
        else
            return board[x - 1];
    }
    int& right(int x) {
        if (x % 5 == 4)
            return invalid;
        else
            return board[x + 1];
    }
    //用于集合的排序
    friend bool operator<(const status& s1, const status& s2)
    {
        if (s1.hash < s2.hash)
            return true;
        else if (s1.hash > s2.hash)
            return false;

        for (int i = 0; i < size; i++)
            if (s1.board[i] < s2.board[i])
                return true;
            else if (s1.board[i] > s2.board[i])
                return false;

        return false;
    }

    //获得当前状态可行的动作 {int 数码, uchar 槽, char 方向}
    std::list<std::tuple<int, uchar, char>> get_actions()
    {
        std::list<std::tuple<int, uchar, char>> rst;
        int temp;
        for (uchar i = 0; i < 2; i++)
        {
            if ((temp = left(slot[i])) > 0)
                if (temp != 7 && check_norepeat(temp, 'l'))
                    rst.push_back({ temp, i, 'r' });
            if ((temp = right(slot[i])) > 0)
                if (temp != 7 && check_norepeat(temp, 'r'))
                    rst.push_back({ temp, i, 'l' });
            if ((temp = up(slot[i])) > 0)
                if (temp != 7 && check_norepeat(temp, 'u'))
                    rst.push_back({ temp, i, 'd' });
            if ((temp = down(slot[i])) > 0)
                if (temp != 7 && check_norepeat(temp, 'd'))
                    rst.push_back({ temp, i, 'u' });

        }
        if (left(slot[0]) == 7 && left(slot[1]) == 7 && check_norepeat(7, 'l'))
            rst.push_back({ 7, slot[0] < slot[1] ? 0 : 1, 'r' });
        if (right(slot[0]) == 7 && right(slot[1]) == 7 && check_norepeat(7, 'r'))
            rst.push_back({ 7, slot[0] < slot[1] ? 0 : 1, 'l' });
        if (up(slot[0]) == 7 && up(slot[1]) == 7 && check_norepeat(7, 'u'))
            rst.push_back({ 7, slot[0] < slot[1] ? 0 : 1, 'd' });
        if (down(slot[0]) == 7 && down(slot[1]) == 7 && check_norepeat(7, 'd'))
            rst.push_back({ 7, slot[0] < slot[1] ? 0 : 1, 'u' });
        return rst;
        
    }

    bool check_norepeat(int fig, char dir) const
    {
        auto [a, b, c] = last_action;
        return !(fig == a && dir == c);
    }

    status* act(const std::tuple<int, uchar, char>& _action)
    {
        status* ret = new status(*this);
        ret->g = g + 1;
        ret->father = this;
        auto [fig, slot, dir] = _action;
        if (fig != 7)
        {
            int pos = ret->slot[slot];
            (*ret)[pos] = fig;
            switch (dir)
            {
            case 'l':
                ret->slot[slot] += 1;
                break;
            case 'r':
                ret->slot[slot] += -1;
                break;
            case 'u':
                ret->slot[slot] += 5;
                break;
            case 'd':
                ret->slot[slot] += -5;
                break;
            default:
                printf("小朋友你是否有许多???\n");
                break;
            }
            (*ret)[ret->slot[slot]] = 0;
        }
        else
        {
            int pos1 = ret->slot[slot];
            (*ret)[ret->slot[0]] = 7; (*ret)[ret->slot[1]] = 7;
            switch (dir)
            {
            case 'l':
                ret->slot[0] = pos1 + 2;
                ret->slot[1] = ret->slot[0] + 5;
                break;
            case 'r':
                ret->slot[0] = pos1 - 2;
                ret->slot[1] = ret->slot[0] + 6;
                break;
            case 'u':
                ret->slot[0] = pos1 + 5;
                ret->slot[1] = ret->slot[0] + 6;
                break;
            case 'd':
                ret->slot[0] = pos1 - 5;
                ret->slot[1] = ret->slot[0] + 1;
                break;
            default:
                printf("小朋友你是否有许多???\n");
                break;
            }
            (*ret)[ret->slot[0]] = 0; (*ret)[ret->slot[1]] = 0;
        }
        ret->get_hash();
        ret->get_herustic();
        ret->last_action = _action;
        return ret;
    }

    void get_herustic()
    {
        h = 0;
        bool count7 = false;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if ((*this)(i, j) == 0 || ((*this)(i, j) == 7 && count7))
                    continue;
                if ((*this)(i, j) == 7)
                    count7 = true;
                if (target_pos[(*this)(i, j)]>=0)
                {
                    auto [x, y] = status::coord(target_pos[(*this)(i, j)]);
                    h += abs(i - x) + abs(j - y);
                }
            }
        }
    }

    int f() const{
        return g + 2 * h;
        //return g + 2 * h 对于input3.txt，使用这个启发式！
    }
    //打印
    void print() const
    {
        std::cout << "   --- g = " << g << "---    "<< std::endl;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
                printf("%3d", (*this)(i, j));
            printf("\n");
        }
    }
};