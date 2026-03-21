#pragma once

#include <vector>


template <class T>
struct Combination {
    std::vector<T> fact_;
    std::vector<T> inv_;
    std::vector<T> finv_;
    int mod_ = 0;

    Combination() = default;

    explicit Combination(int n) {
        init(n + 1);
    }

    void init(int n) {
        fact_.assign(n, T(1));
        inv_.assign(n, T(1));
        finv_.assign(n, T(1));
        mod_ = T::mod();
        for(int i = 2; i < n; ++i) {
            fact_[i] = fact_[i - 1] * i;
            inv_[i] = -inv_[mod_ % i] * (mod_ / i);
            finv_[i] = finv_[i - 1] * inv_[i];
        }
    }

    T nPr(int n, int k) const {
        if(n < k || n < 0 || k < 0)
            return 0;
        return fact_[n] * finv_[n - k];
    }

    T nCr(int n, int k) const {
        if(n < k || n < 0 || k < 0)
            return 0;
        if(n < mod_)
            return fact_[n] * finv_[k] * finv_[n - k];

        T ret = 1;
        while(n || k) {
            int nn = n % mod_;
            int kk = k % mod_;
            n /= mod_;
            k /= mod_;
            ret *= nCr(nn, kk);
        }
        return ret;
    }

    T nHr(int n, int k) const {
        if(n <= 0 || k < 0)
            return 0;
        return nCr(n + k - 1, k);
    }

    T fact(int n) const {
        return n < 0 ? T(0) : fact_[n];
    }

    T inv(int n) const {
        return n < 0 ? T(0) : inv_[n];
    }

    T finv(int n) const {
        return n < 0 ? T(0) : finv_[n];
    }
};

