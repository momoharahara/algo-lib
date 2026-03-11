#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;

/// ===== pair =====

template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

/// ===== modint =====

template <int m>
ostream &operator<<(ostream &os, const static_modint<m> &x) {
    return os << x.val();
}

ostream &operator<<(ostream &os, const modint &x) {
    return os << x.val();
}

/// ===== iterable container =====
/// vector / debug vector / set / map / deque / list / etc

template <class T>
concept Iterable =
    requires(T a) {
        begin(a);
        end(a);
    } && !std::is_same_v<T, std::string>;

template <Iterable T>
ostream &operator<<(ostream &os, const T &v) {
    os << "{";
    bool first = true;
    for(auto &&x : v) {
        if(!first)
            os << ", ";
        first = false;
        os << x;
    }
    os << "}";
    return os;
}

/// ===== debug output =====

#define DUMPOUT cerr

void dump_func() {
    DUMPOUT << endl;
}

template <class Head, class... Tail>
void dump_func(Head &&head, Tail &&...tail) {
    DUMPOUT << head;
    if(sizeof...(Tail) > 0)
        DUMPOUT << ", ";
    dump_func(std::forward<Tail>(tail)...);
}

#define dump(...)                                                 \
    DUMPOUT << "  " << #__VA_ARGS__                               \
            << " [" << __LINE__ << ":" << __FUNCTION__ << "] = ", \
        dump_func(__VA_ARGS__)