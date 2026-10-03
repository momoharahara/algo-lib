#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

class Mo {
    int n_;
    int width_;
    std::vector<int> left_;
    std::vector<int> right_;
    std::vector<int> order_;

  public:
    explicit Mo(int n) : n_(n), width_(std::max(1, static_cast<int>(std::sqrt(n)))) {}

    void add_query(int l, int r) {
        left_.push_back(l);
        right_.push_back(r);
    }

    int size() const {
        return static_cast<int>(left_.size());
    }

    template <class Add, class Remove, class Answer>
    auto run(Add add, Remove remove, Answer answer) const {
        using Result = decltype(answer(0));

        int q = size();
        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int ablock = left_[a] / width_;
            int bblock = left_[b] / width_;
            if(ablock != bblock)
                return ablock < bblock;
            if(ablock & 1)
                return right_[a] < right_[b];
            return right_[a] > right_[b];
        });

        std::vector<Result> ans(q);
        int nl = 0;
        int nr = 0;
        for(int idx : order) {
            while(nl > left_[idx])
                add(--nl);
            while(nr < right_[idx])
                add(nr++);
            while(nl < left_[idx])
                remove(nl++);
            while(nr > right_[idx])
                remove(--nr);
            ans[idx] = answer(idx);
        }
        return ans;
    }
};
