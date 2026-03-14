#pragma once

#include <vector>

namespace momohara {

template <typename T>
class FenwickTree {
    std::vector<T> data_;
    int n_;
    int p_;

  public:
    explicit FenwickTree(int n) : data_(n + 1, 0), n_(n), p_(1) {
        while(p_ < static_cast<int>(data_.size()))
            p_ <<= 1;
    }

    T sum(int k) const {
        T ret = 0;
        for(; k > 0; k -= k & -k)
            ret += data_[k];
        return ret;
    }

    T sum(int l, int r) const {
        return sum(r) - sum(l);
    }

    void add(int k, T x) {
        for(++k; k <= n_; k += k & -k)
            data_[k] += x;
    }

    int lower_bound(long long w) const {
        if(w <= 0)
            return -1;
        int x = 0;
        for(int k = p_ >> 1; k > 0; k >>= 1) {
            if(x + k <= n_ && data_[x + k] < w) {
                w -= data_[x + k];
                x += k;
            }
        }
        return x;
    }
};

} // namespace momohara
