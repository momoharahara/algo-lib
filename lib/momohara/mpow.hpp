#pragma once

#include <vector>


template <class S>
std::vector<std::vector<S>> mIdentity(int n) {
    std::vector<std::vector<S>> a(n, std::vector<S>(n));
    for(int i = 0; i < n; ++i)
        a[i][i] = 1;
    return a;
}

template <class S>
std::vector<std::vector<S>> mMul(const std::vector<std::vector<S>> &a, const std::vector<std::vector<S>> &b) {
    std::vector<std::vector<S>> c(a.size(), std::vector<S>(b[0].size()));
    for(int i = 0; i < static_cast<int>(c.size()); ++i) {
        for(int j = 0; j < static_cast<int>(c[i].size()); ++j) {
            for(int k = 0; k < static_cast<int>(a[i].size()); ++k)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    return c;
}

template <class S>
std::vector<std::vector<S>> mPow(std::vector<std::vector<S>> a, long long e) {
    auto ret = mIdentity<S>(static_cast<int>(a.size()));
    while(e > 0) {
        if(e & 1)
            ret = mMul(ret, a);
        a = mMul(a, a);
        e >>= 1;
    }
    return ret;
}

