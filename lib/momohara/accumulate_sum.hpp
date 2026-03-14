#pragma once

#include <cassert>
#include <vector>

namespace momohara {

template <typename T>
struct AccumulateSum {
    std::vector<T> sum;

    explicit AccumulateSum(const std::vector<T> &a) {
        int n = static_cast<int>(a.size());
        sum.resize(n + 1);
        for(int i = 0; i < n; ++i)
            sum[i + 1] = sum[i] + a[i];
    }

    T get(int l, int r) const {
        assert(0 <= l && l <= r && r <= static_cast<int>(sum.size()));
        return sum[r] - sum[l];
    }
};

template <typename T>
struct AccumulateSum2 {
    std::vector<std::vector<T>> sum;

    explicit AccumulateSum2(const std::vector<std::vector<T>> &a) {
        assert(!a.empty());
        int n = static_cast<int>(a.size());
        int m = static_cast<int>(a[0].size());
        sum = std::vector<std::vector<T>>(n + 1, std::vector<T>(m + 1, 0));
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                sum[i + 1][j + 1] = sum[i + 1][j] + sum[i][j + 1] - sum[i][j];
                sum[i + 1][j + 1] += a[i][j];
            }
        }
    }

    T get(int yl, int xl, int yr, int xr) const {
        assert(0 <= yl && yl <= yr && yr <= static_cast<int>(sum.size()));
        assert(0 <= xl && xl <= xr && xr <= static_cast<int>(sum[0].size()));
        return sum[yr][xr] - sum[yr][xl] - sum[yl][xr] + sum[yl][xl];
    }
};

template <typename T>
struct AccumulateSum3 {
    std::vector<std::vector<std::vector<T>>> sum;

    explicit AccumulateSum3(const std::vector<std::vector<std::vector<T>>> &a) {
        assert(!a.empty());
        int n = static_cast<int>(a.size());
        int m = static_cast<int>(a[0].size());
        int l = static_cast<int>(a[0][0].size());
        sum = std::vector<std::vector<std::vector<T>>>(
            n + 1, std::vector<std::vector<T>>(m + 1, std::vector<T>(l + 1, 0)));
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                for(int k = 0; k < l; ++k) {
                    sum[i + 1][j + 1][k + 1] =
                        sum[i][j + 1][k + 1] + sum[i + 1][j][k + 1] + sum[i + 1][j + 1][k]
                        - sum[i + 1][j][k] - sum[i][j + 1][k] - sum[i][j][k + 1]
                        + sum[i][j][k];
                    sum[i + 1][j + 1][k + 1] += a[i][j][k];
                }
            }
        }
    }

    T get(int xl, int yl, int zl, int xr, int yr, int zr) const {
        assert(0 <= xl && xl <= xr && xr <= static_cast<int>(sum.size()));
        assert(0 <= yl && yl <= yr && yr <= static_cast<int>(sum[0].size()));
        assert(0 <= zl && zl <= zr && zr <= static_cast<int>(sum[0][0].size()));
        return sum[xr][yr][zr] - sum[xr][yr][zl] - sum[xr][yl][zr] - sum[xl][yr][zr]
               + sum[xr][yl][zl] + sum[xl][yr][zl] + sum[xl][yl][zr] - sum[xl][yl][zl];
    }
};

} // namespace momohara
