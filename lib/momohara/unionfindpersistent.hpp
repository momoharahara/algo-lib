#pragma once

#include <limits>
#include <utility>
#include <vector>

namespace momohara {

class persistentUF {
  public:
    int now;
    std::vector<int> par;
    std::vector<int> rank;
    std::vector<int> time;
    std::vector<std::vector<std::pair<int, int>>> num;

    explicit persistentUF(int n)
        : now(0),
          par(n, 0),
          rank(n, 0),
          time(n, std::numeric_limits<int>::max()),
          num(n, std::vector<std::pair<int, int>>(1, std::make_pair(0, 1))) {
        for(int i = 0; i < n; ++i)
            par[i] = i;
    }

    int root(int x, int t) const {
        if(t < time[x])
            return x;
        return root(par[x], t);
    }

    bool same(int x, int y, int t) const {
        return root(x, t) == root(y, t);
    }

    void merge(int x, int y) {
        ++now;
        x = root(x, now);
        y = root(y, now);
        if(x == y)
            return;
        if(rank[x] < rank[y])
            std::swap(x, y);
        num[x].push_back(std::make_pair(now, size(x, now) + size(y, now)));
        par[y] = x;
        time[y] = now;
        if(rank[x] == rank[y])
            ++rank[x];
    }

    int size(int x, int t) const {
        x = root(x, t);
        int ok = 0;
        int ng = static_cast<int>(num[x].size());
        while(ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if(num[x][mid].first <= t) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        return num[x][ok].second;
    }
};

} // namespace momohara
