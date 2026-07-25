#pragma once
#include <iterator>

struct SubsetRange {
    struct Iterator {
        int mask;
        int sub;
        bool done;

        int operator*() const { return sub; }

        Iterator &operator++() {
            if(sub == 0) {
                done = true;
            } else {
                sub = (sub - 1) & mask;
            }
            return *this;
        }

        bool operator!=(std::default_sentinel_t) const { return !done; }
    };

    int mask;
    Iterator begin() const { return Iterator{mask, mask, false}; }
    std::default_sentinel_t end() const { return {}; }
};

SubsetRange subsets(int mask) { return SubsetRange{mask}; }