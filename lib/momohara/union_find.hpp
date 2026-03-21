#pragma once

#include <utility>
#include <vector>


class UnionFind {
    std::vector<int> data_;
    int components_;

  public:
    explicit UnionFind(int size) : data_(size, -1), components_(size) {}

    bool merge(int x, int y) {
        x = root(x);
        y = root(y);
        if(x == y)
            return false;
        if(data_[y] < data_[x])
            std::swap(x, y);
        data_[x] += data_[y];
        data_[y] = x;
        --components_;
        return true;
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }

    int root(int x) {
        return data_[x] < 0 ? x : data_[x] = root(data_[x]);
    }

    int size(int x) {
        return -data_[root(x)];
    }

    int count() const {
        return components_;
    }
};

