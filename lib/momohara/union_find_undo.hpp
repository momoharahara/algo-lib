#pragma once

#include <utility>
#include <vector>


class UnionFindUndo {
    std::vector<int> data_;
    std::vector<std::pair<int, int>> history_;
    int components_;

  public:
    explicit UnionFindUndo(int size) : data_(size, -1), components_(size) {}

    bool merge(int x, int y) {
        x = root(x);
        y = root(y);
        if(x == y)
            return false;
        history_.push_back({x, data_[x]});
        history_.push_back({y, data_[y]});
        if(data_[y] < data_[x])
            std::swap(x, y);
        data_[x] += data_[y];
        data_[y] = x;
        --components_;
        return true;
    }

    bool same(int x, int y) const {
        return root(x) == root(y);
    }

    int root(int x) const {
        return data_[x] < 0 ? x : root(data_[x]);
    }

    int size(int x) const {
        return -data_[root(x)];
    }

    int count() const {
        return components_;
    }

    void snapshot() {
        history_.clear();
    }

    void undo() {
        data_[history_.back().first] = history_.back().second;
        history_.pop_back();
        data_[history_.back().first] = history_.back().second;
        history_.pop_back();
        ++components_;
    }

    void rollback() {
        while(!history_.empty())
            undo();
    }
};

