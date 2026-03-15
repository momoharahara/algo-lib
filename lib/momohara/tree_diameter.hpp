#pragma once

#include <tuple>
#include <vector>

#include "dijkstra.hpp"

namespace momohara {

template <typename T>
std::tuple<T, int, int> tree_diameter(const Graph<T> &g, T inf = std::numeric_limits<T>::max() / 4) {
    std::vector<T> d1;
    Dijkstra(g, 0, d1, inf);
    T far1_dist = 0;
    int far1 = 0;
    for(int v = 0; v < static_cast<int>(g.size()); ++v) {
        if(far1_dist < d1[v]) {
            far1_dist = d1[v];
            far1 = v;
        }
    }

    std::vector<T> d2;
    Dijkstra(g, far1, d2, inf);
    T far2_dist = 0;
    int far2 = far1;
    for(int v = 0; v < static_cast<int>(g.size()); ++v) {
        if(far2_dist < d2[v]) {
            far2_dist = d2[v];
            far2 = v;
        }
    }
    return {far2_dist, far1, far2};
}

} // namespace momohara
