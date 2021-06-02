#include <bits/stdc++.h>
using namespace std;

using ll = long long;
template <class T>
using vec = vector<T>;
using P = pair<ll, ll>;

#define en '\n'
#define rep(i, n, m) for(ll i = n; i < m; ++i)
#define REP(i, n) rep(i, 0, n)

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

int n, m;

class UnionFind {
    vector<int> data;
    int num;

  public:
    UnionFind(int size) : data(size, -1), num(size) {}
    bool unionSet(int x, int y) {
        x = root(x);
        y = root(y);
        if(x != y) {
            if(data[y] < data[x])
                swap(x, y);
            data[x] += data[y];
            data[y] = x;
            num--;
        }
        return x != y;
    }
    bool findSet(int x, int y) {
        return root(x) == root(y);
    }
    int root(int x) {
        return data[x] < 0 ? x : data[x] = root(data[x]);
    }
    int size(int x) {
        return -data[root(x)];
    }
    int numSet() {
        return num;
    }
};

void solve() {
    using tp = tuple<ll, ll, ll>;
    vec<tp> es;
    REP(i, m) {
        ll a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        es.push_back({c, a, b});
    }
    sort(es.begin(), es.end(), greater<tp>());

    UnionFind uni(n);
    vec<vec<int>> g(2 * n);
    vec<P> data(n, make_pair(1LL << 30, -1));
    vec<ll> st(2 * n, 0); //頂点の強さ
    REP(i, n) {
        data[i].second = i;
    }

    int p = n;
    for(auto [s, u, v] : es) {
        int r1 = uni.root(u);
        int r2 = uni.root(v);
        if(uni.unionSet(u, v)) {
            int r = uni.root(u);
            auto [pre, w] = min(data[r1], data[r2]);
            if(pre != s) {
                w = p++;
            }
            if(w != data[r1].second)
                g[w].push_back(data[r1].second);
            if(w != data[r2].second)
                g[w].push_back(data[r2].second);
            data[r] = {s, w};
            st[w] = s;
        }
    }

    vec<int> num(2 * n, 0);
    auto dfs = [&](auto &&self, int v) -> void {
        if(g[v].size() == 0) {
            num[v] = 1;
            return;
        }
        for(auto to : g[v]) {
            self(self, to);
            num[v] += num[to];
        }
    };
    p--;
    dfs(dfs, p);

    auto dfs2 = [&](auto &&self, int v) -> pair<vec<ll>, vec<P>> {
        P now = {num[v], st[v]}; //数、強さ
        if(g[v].size() == 0) {
            return {{v}, {now}};
        }
        vec<ll> vs;
        vec<P> ss;
        for(auto to : g[v]) {
            auto [vs2, ss2] = self(self, to);
            if(ss == ss2) {
                if(vs.size() < vs2.size())
                    swap(vs, vs2);
                for(auto x : vs2)
                    vs.push_back(x);
            } else {
                if(ss.size() < ss2.size()) {
                    swap(ss, ss2);
                    swap(vs, vs2);
                }
                bool flag = false;
                REP(i, ss2.size()) {
                    if(ss2[ss2.size() - 1 - i] == ss[ss.size() - 1 - i])
                        continue;
                    if(ss2[ss2.size() - 1 - i] > ss[ss.size() - 1 - i]) {
                        flag = true;
                    }
                    break;
                }
                if(flag) {
                    //ss2がかったとき
                    swap(ss, ss2);
                    swap(vs, vs2);
                }
            }
        }
        ss.push_back(now);
        return {vs, ss};
    };
    auto [vs, ss] = dfs2(dfs2, p);
    sort(vs.begin(), vs.end());
    REP(i, vs.size()) {
        cout << vs[i] + 1;
        if(i == vs.size() - 1)
            break;
        cout << " ";
    }
    cout << en;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    while(cin >> n >> m) {
        if(n == 0)
            break;
        solve();
    }

    return 0;
}