#pragma once

#include "status.h"
#include <iostream>

class solution {
public:
  struct status_prior {
    bool operator()(const status *_Left, const status *_Right) const {
      // if (_Right->is_deprecated || _Left->is_deprecated) return true;
      return _Left->f() > _Right->f();
    }
  };

  std::set<status> close_list;
  std::priority_queue<status *, std::vector<status *>, status_prior> open_list;
  std::map<status, status *> open_list_map;
  std::list<status *> cache_status;

  solution() {}

  // 以s为初始状态求解
  status solve(const status &s);
};

status solution::solve(const status &s) {
  char ch;
  // 清空open_list和close_list
  close_list.clear();
  while (!open_list.empty())
    open_list.pop();
  status *n;
  status *init_s = new status(s);
  open_list.push(init_s);
  open_list_map[*init_s] = init_s;

  std::list<status *> cache_status;

  int cnt = 0;
  while (!open_list.empty()) {
    // std::cin >> ch;
    n = open_list.top();
    open_list.pop();
    // n->print();
    // std::cout << "h = " << herustic(*n, target_pos) << std::endl;
    if (n->is_deprecated) // 跳过废弃节点；
    {
      delete n;
      continue;
    }
    cnt++;
    if (cnt % 100000 == 0) {
      std::cout << "--- " << cnt << "# f=" << n->f() << " ---" << std::endl;
      std::cout << "h=" << n->h << ", g = " << n->g << std::endl;
      n->print();
    }

    if (n->h == 0) // 已经到达目标
    {
      std::cout << "Nodes Traversed = " << cnt << std::endl;
      std::cout << "Least cost = " << n->g << std::endl;
      status rst = *n;
      status *p = n;
      std::list<TAction> answer;
      while (p->father != nullptr) {
        answer.push_front(p->last_action);
        p = p->father;
      }
      for (auto act : answer) {
        auto [a, b, c] = act;
        std::cout << "(" << a << "," << c << ");" << std::endl;
      }
      p = new status(s);
      for (auto act : answer) {
        auto ptemp = p;
        p = p->act(act);
        delete ptemp;
      }
      p->print();
      delete p;
      while (!open_list.empty()) {
        delete open_list.top();
        open_list.pop();
      }
      while (!cache_status.empty()) {
        delete cache_status.front();
        cache_status.pop_front();
      }
      delete n;
      return rst;
    }
    std::list<std::tuple<int, uchar, char>> actions = n->get_actions();
    status *m;
    for (std::tuple<int, uchar, char> act : actions) // 所有后继状态
    {
      m = n->act(act);
      // std::cout << "new :"  << std::endl;
      // m->print();
      std::map<status, status *>::iterator mcheck;
      if ((mcheck = open_list_map.find(*m)) !=
          open_list_map.end()) // 已经在open_list中
      {
        if (m->g < mcheck->second->g) // 比之前的好，更新 g 值
        {
          mcheck->second
              ->deprecate(); // 由于priority_queue不能改变，设置原节点废弃，留在open_list中
          mcheck->second = m; // open_list_map 跟踪新节点
          open_list.push(m);  // 新节点插入priority_queue
        } else                // 不如之前，销毁缓存
        {
          delete m;
        }

      } else if (close_list.find(*m) !=
                 close_list.end()) // 已经在 close_list 中
      {
        delete m;
        continue;
      } else {
        open_list.push(m);     // 加入 open_list 中
        open_list_map[*m] = m; // 用 open_list_map 跟踪
      }
    }

    close_list.insert(*n);
    open_list_map.erase(*n);
    cache_status.push_back(n);
  }

  return status(0);
}
