#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;

template <class T>
concept Iterable =
    requires(T a) {
        begin(a);
        end(a);
    } && !std::is_same_v<T, std::string>;

template <class T>
void debug_out(ostream &os, const T &x);

/// ===== pair =====

template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
    os << "(";
    debug_out(os, p.first);
    os << ", ";
    debug_out(os, p.second);
    os << ")";
    return os;
}

/// ===== iterable container =====
/// vector / debug vector / set / map / deque / list / etc

template <Iterable T>
ostream &operator<<(ostream &os, const T &v) {
    os << "{";
    bool first = true;
    for(auto &&x : v) {
        if(!first)
            os << ", ";
        first = false;
        debug_out(os, x);
    }
    os << "}";
    return os;
}

template <class T>
void debug_out(ostream &os, const T &x) {
    if constexpr(requires { x.val(); } && !Iterable<T>) {
        os << x.val();
    } else {
        os << x;
    }
}

/// ===== debug output =====

#define DUMPOUT cerr

void dump_func() {
    DUMPOUT << endl;
}

template <class Head, class... Tail>
void dump_func(Head &&head, Tail &&...tail) {
    debug_out(DUMPOUT, head);
    if(sizeof...(Tail) > 0)
        DUMPOUT << ", ";
    dump_func(std::forward<Tail>(tail)...);
}

#define dump(...)                                                 \
    DUMPOUT << "  " << #__VA_ARGS__                               \
            << " [" << __LINE__ << ":" << __FUNCTION__ << "] = ", \
        dump_func(__VA_ARGS__)
