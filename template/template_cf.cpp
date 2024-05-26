#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using P = pair<ll, ll>;
using tp = tuple<ll, ll, ll>;

template <class T>
using vec = vector<T>;
template <class T>
using vvec = vector<vec<T>>;

#define all(hoge) (hoge).begin(), (hoge).end()
#define en '\n'
#define rep(i, m, n) for(ll i = (ll)(m); i < (ll)(n); ++i)
#define rep2(i, m, n) for(ll i = (ll)(n) - 1; i >= (ll)(m); --i)
#define REP(i, n) rep(i, 0, n)
#define REP2(i, n) rep2(i, 0, n)

constexpr long long INF = 1LL << 60;
constexpr int INF_INT = 1 << 25;
// constexpr long long MOD = (ll)1e9 + 7;
constexpr long long MOD = 998244353LL;
static const ld pi = 3.141592653589793L;

#if !defined(DEBUG)
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#endif

// デバッグ系 https://www.creativ.xyz/dump-cpp-652/

#define repi(itr, ds) for(auto itr = ds.begin(); itr != ds.end(); itr++)

// vector
template <typename T>
istream &operator>>(istream &is, vector<T> &vec) {
    for(T &x : vec)
        is >> x;
    return is;
}
// pair
template <typename T, typename U>
ostream &operator<<(ostream &os, pair<T, U> &pair_var) {
    os << "(" << pair_var.first << ", " << pair_var.second << ")";
    return os;
}
// vector
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &vec) {
    os << "{";
    for(int i = 0; i < vec.size(); i++) {
        os << vec[i] << (i + 1 == vec.size() ? "" : ", ");
    }
    os << "}";
    return os;
}
// map
template <typename T, typename U>
ostream &operator<<(ostream &os, map<T, U> &map_var) {
    os << "{";
    repi(itr, map_var) {
        os << *itr;
        itr++;
        if(itr != map_var.end())
            os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
// set
template <typename T>
ostream &operator<<(ostream &os, set<T> &set_var) {
    os << "{";
    repi(itr, set_var) {
        os << *itr;
        itr++;
        if(itr != set_var.end())
            os << ", ";
        itr--;
    }
    os << "}";
    return os;
}

#define DUMPOUT cerr

void dump_func() {
    DUMPOUT << endl;
}
template <class Head, class... Tail>
void dump_func(Head &&head, Tail &&...tail) {
    DUMPOUT << head;
    if(sizeof...(Tail) > 0) {
        DUMPOUT << ", ";
    }
    dump_func(std::move(tail)...);
}
#ifdef DEBUG
#define DEB
#define dump(...)                                                       \
    DUMPOUT << "  " << string(#__VA_ARGS__) << ": "                     \
            << "[" << to_string(__LINE__) << ":" << __FUNCTION__ << "]" \
            << endl                                                     \
            << "    ",                                                  \
        dump_func(__VA_ARGS__)
#else
#define DEB if(false)
#define dump(...)
#endif
// デバッグ系おわり

template <class T>
inline bool chmin(T &a, T b) {
    if(a > b) {
        a = b;
        return true;
    }
    return false;
}

template <class T>
inline bool chmax(T &a, T b) {
    if(a < b) {
        a = b;
        return true;
    }
    return false;
}

struct Edge {
    int to, rev;
    ll cap;
    Edge(int _to, int _rev, ll _cap) : to(_to), rev(_rev), cap(_cap) {}
};

typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

void add_edge(Graph &G, int from, int to, ll cap, bool revFlag, ll revCap) {
    G[from].push_back(Edge(to, (int)G[to].size(), cap));
    if(revFlag)
        G[to].push_back(Edge(from, (int)G[from].size() - 1, revCap));
}

void solve() {
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    // cout << fixed << setprecision(10);

    ll t;
    cin >> t;
    REP(i, t - 1) {
        solve();
    }

    solve();

    return 0;
}
