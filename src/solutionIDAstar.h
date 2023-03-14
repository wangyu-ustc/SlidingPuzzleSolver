#pragma once

#include "status.h"
#include <climits>
#include <iostream>

class solutionIDAstar {
public:
  struct status_prior {
    bool operator()(const status *_Left, const status *_Right) const {
      // if (_Right->is_deprecated || _Left->is_deprecated) return true;
      return _Left->f() > _Right->f();
    }
  };

  std::priority_queue<status *, std::vector<status *>, status_prior> list;
  std::map<status, status *> list_map;

  status solve(const status &s);
  void clear();
};

status solutionIDAstar::solve(const status &s) {
  char ch;
  // 清空open_list
  while (!list.empty())
    list.pop();
  status *n;
  status *init_s = new status(s);

  int d_limit = init_s->f();

  int cnt = 0;
  int cnt_deprecated = 0;

  int next_d_limit;
  while (d_limit < INT_MAX) {
    std::cout << "D_LIMIT = " << d_limit << std::endl;
    next_d_limit = INT_MAX;

    status *init_s = new status(s);
    std::cout << "k=" << init_s->k << std::endl;

    list.push(init_s);
    list_map[*init_s] = init_s;

    while (!list.empty()) {
      cnt++;
      n = list.top();
      list.pop();
      if (n->is_deprecated) {
        delete n;
        cnt_deprecated--;
        continue;
      }
      if (cnt % 100000 == 0) {
        std::cout << "Deprecated/Total = " << cnt_deprecated << "/"
                  << list.size() << std::endl;
        std::cout << "f" << n->f() << std::endl;
      }
      if (n->f() > d_limit) {
        next_d_limit = std::min(next_d_limit, n->f());
      } else {
        if (n->h == 0) {
          std::cout << "Nodes traversed" << cnt << std::endl;
          std::cout << "Least cost = " << n->g << std::endl;
          status rst(*n);
          delete n;
          clear();
          return rst;
        } else {
          auto actions = n->get_actions();
          status *m;
          for (auto act : actions) {
            m = n->act(act);
            std::map<status, status *>::iterator mcheck;
            if ((mcheck = list_map.find(*m)) !=
                list_map.end()) // 已经在open_list中
            {
              if (m->g < mcheck->second->g) // 比之前的好，更新 g 值
              {
                cnt_deprecated++;
                mcheck->second
                    ->deprecate(); // 由于priority_queue不能改变，设置原节点废弃，留在open_list中
                mcheck->second = m; // open_list_map 跟踪新节点
                list.push(m);       // 新节点插入priority_queue
              } else                // 不如之前，销毁缓存
              {
                delete m;
              }
            } else {
              list.push(m);
              list_map[*m] = m;
            }
          }
        }
      }
      list_map.erase(*n);
      delete n;
    }
    d_limit = next_d_limit;
  }

  std::cout << "FAIL" << std::endl;

  return status(0);
}

inline void solutionIDAstar::clear() {
  while (!list.empty()) {
    delete list.top();
    list.pop();
  }
  list_map.clear();
}
