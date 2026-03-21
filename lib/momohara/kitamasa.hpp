#pragma once

#include <cstdint>
#include <vector>


template <typename T>
struct Kitamasa {
    std::vector<T> a;
    std::vector<T> d;
    int k;

    Kitamasa(std::vector<T> a, std::vector<T> d) : a(std::move(a)), d(std::move(d)), k(static_cast<int>(this->a.size())) {}

    std::vector<T> dfs(int64_t n) const {
        if(n == k)
            return d;
        std::vector<T> res(k);
        if((n & 1) || n < k * 2) {
            std::vector<T> x = dfs(n - 1);
            for(int i = 0; i < k; ++i)
                res[i] = d[i] * x[k - 1];
            for(int i = 0; i + 1 < k; ++i)
                res[i + 1] += x[i];
        } else {
            std::vector<std::vector<T>> x(k, std::vector<T>(k));
            x[0] = dfs(n >> 1);
            for(int i = 0; i + 1 < k; ++i) {
                for(int j = 0; j < k; ++j)
                    x[i + 1][j] = d[j] * x[i][k - 1];
                for(int j = 0; j + 1 < k; ++j)
                    x[i + 1][j + 1] += x[i][j];
            }
            for(int i = 0; i < k; ++i) {
                for(int j = 0; j < k; ++j)
                    res[j] += x[0][i] * x[i][j];
            }
        }
        return res;
    }

    T calc(int64_t n) const {
        if(n < k)
            return a[n];
        std::vector<T> x = dfs(n);
        T res = 0;
        for(int i = 0; i < k; ++i)
            res += x[i] * a[i];
        return res;
    }
};

