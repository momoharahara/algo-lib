#pragma once

#include <map>


inline std::map<long long, long long> factoring(long long n) {
    std::map<long long, long long> mp;
    for(long long i = 2; i * i <= n; ++i) {
        while(n % i == 0) {
            mp[i]++;
            n /= i;
        }
    }
    if(n != 1)
        mp[n]++;
    return mp;
}

