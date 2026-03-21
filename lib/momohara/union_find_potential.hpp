#pragma once

#include <utility>
#include <vector>


template <class Abel>
struct UnionFindPotential {
    std::vector<int> par;
    std::vector<int> rank;
    std::vector<Abel> diff_weight;

    explicit UnionFindPotential(int n = 1, Abel sum_unity = 0) {
        init(n, sum_unity);
    }

    void init(int n = 1, Abel sum_unity = 0) {
        par.resize(n);
        rank.resize(n);
        diff_weight.resize(n);
        for(int i = 0; i < n; ++i) {
            par[i] = i;
            rank[i] = 0;
            diff_weight[i] = sum_unity;
        }
    }

    int root(int x) {
        if(par[x] == x)
            return x;
        int r = root(par[x]);
        diff_weight[x] += diff_weight[par[x]];
        return par[x] = r;
    }

    Abel weight(int x) {
        root(x);
        return diff_weight[x];
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }

    bool merge(int x, int y, Abel w) {
        w += weight(x);
        w -= weight(y);
        x = root(x);
        y = root(y);
        if(x == y)
            return false;
        if(rank[x] < rank[y]) {
            std::swap(x, y);
            w = -w;
        }
        if(rank[x] == rank[y])
            ++rank[x];
        par[y] = x;
        diff_weight[y] = w;
        return true;
    }

    Abel diff(int x, int y) {
        return weight(y) - weight(x);
    }
};

