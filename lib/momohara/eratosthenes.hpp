#pragma once

#include <vector>


inline void Eratosthenes(long long n, std::vector<int> &arr) {
    arr.resize(n + 1, 1);
    for(long long i = 2; i * i <= n; ++i) {
        if(arr[i]) {
            for(long long j = 2; i * j <= n; ++j)
                arr[i * j] = 0;
        }
    }
}

