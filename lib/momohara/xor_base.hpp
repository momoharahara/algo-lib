#pragma once

#include <algorithm>
#include <vector>

namespace momohara {

inline bool xor_base(long long v, std::vector<long long> &base, std::vector<long long> &vs) {
    long long reduced = v;
    for(long long e : base) {
        reduced = std::min(reduced, reduced ^ e);
    }
    if(reduced > 0) {
        base.push_back(reduced);
        vs.push_back(v);
        return true;
    }
    return false;
}

} // namespace momohara
