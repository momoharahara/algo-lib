#pragma once

#include <algorithm>
#include <vector>


inline std::vector<long long> divisor(long long n) {
    std::vector<long long> ret;
    for(long long i = 1; i * i <= n; ++i) {
        if(n % i == 0) {
            ret.push_back(i);
            if(i * i != n)
                ret.push_back(n / i);
        }
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

