#pragma once

#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

namespace momohara {

class SlopeTrick {
    std::priority_queue<long long> left_;
    std::priority_queue<long long, std::vector<long long>, std::greater<long long>> right_;
    long long add_left_ = 0;
    long long add_right_ = 0;
    long long min_ = 0;

  public:
    void init(long long x, long long a) {
        for(long long i = 0; i < a; ++i) {
            left_.push(x);
            right_.push(x);
        }
    }

    void x_minus_a(long long a) {
        left_.push(a - add_left_);
        long long r = left_.top() + add_left_ - add_right_;
        min_ += std::max(left_.top() + add_left_ - a, 0LL);
        left_.pop();
        right_.push(r);
    }

    void a_minus_x(long long a) {
        right_.push(a - add_right_);
        long long l = right_.top() + add_right_ - add_left_;
        min_ += std::max(a - (right_.top() + add_right_), 0LL);
        right_.pop();
        left_.push(l);
    }

    void shiftL(long long x) {
        add_left_ += x;
    }

    void shiftR(long long x) {
        add_right_ += x;
    }

    long long min_value() const {
        return min_;
    }
};

} // namespace momohara
