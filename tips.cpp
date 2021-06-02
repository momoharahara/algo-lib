/*
#include <bits/stdc++.h>
using namespace std;

#define rep(i,m,n) for(int (i)=(int)(m);i<(int)(n);i++)
#define rep2(i,m,n) for(int (i)=(int)(n)-1;i>=(int)(m);i--)
#define REP(i,n) rep(i,0,n)
#define REP2(i,n) rep2(i,0,n)
#define all(hoge) (hoge).begin(),(hoge).end()
#define en '\n'
using ll = long long;
using ull = unsigned long long;
template <class T> using vec = vector<T>;
template <class T> using vvec = vector<vec<T>>;
typedef pair<ll, ll> P;
constexpr long long INF = 1LL << 60;
constexpr int INF_INT = 1 << 25;
//constexpr long long MOD = (ll) 1e9 + 7;
constexpr long long MOD = 998244353LL;

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

void solve(){

}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    solve();

    return 0;
}

template<class T> inline bool chmin(T& a, T b) {
 if (a > b) {
  a = b;
  return true;
 }
 return false;
}
template<class T> inline bool chmax(T& a, T b) {
 if (a < b) {
  a = b;
  return true;
 }
 return false;
}

//グラフ関連
struct Edge {
 ll to, cap, rev;
 Edge(ll _to, ll _cap, ll _rev) {
  to = _to; cap = _cap; rev = _rev;
 }
};
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

void add_edge(Graph& G, ll from, ll to, ll cap, bool revFlag, ll revCap) {
 G[from].push_back(Edge(to, cap, (ll)G[to].size()));
 if (revFlag)G[to].push_back(Edge(from, revCap, (ll)G[from].size() - 1));
}


// long dobule
struct Edge {
    ll to, rev;
    long double cap;
    Edge(ll _to, long double _cap, ll _rev) {
        to = _to;
        cap = _cap;
        rev = _rev;
    }
};
using Edges = vector<Edge>;
using Graph = vector<Edges>;

void add_edge(Graph &G, ll from, ll to, long double cap, bool revFlag,
              long double revCap) {
    G[from].push_back(Edge(to, cap, (ll)G[to].size()));
    if(revFlag)
        G[to].push_back(Edge(from, revCap, (ll)G[from].size() - 1));
}


void add_edge(Graph& G, ll from, ll to, ll cap, bool revFlag, ll revCap) {
 G[from].push_back(Edge(to, cap, (ll)G[to].size()));
 if (revFlag)G[to].push_back(Edge(from, revCap, (ll)G[from].size() - 1));
 //最大フロー・最小カットの場合逆辺は0にする
}

ll max_flow_dfs(Graph& G, ll v, ll t, ll f, vector<bool>& used)
{
 if (v == t)
  return f;
 used[v] = true;
 for (int i = 0; i < G[v].size(); ++i) {
  Edge& e = G[v][i];
  if (!used[e.to] && e.cap > 0) {
   ll d = max_flow_dfs(G, e.to, t, min(f, e.cap), used);
   if (d > 0) {
    e.cap -= d;
    G[e.to][e.rev].cap += d;
    return d;
   }
  }
 }
 return 0;
}

//最大フローを求める Ford-fulkerson
//二部マッチングにも使える
ll max_flow(Graph& G, ll s, ll t)
{
 ll flow = 0;
 for (;;) {
  vector<bool> used(G.size(),false);
  ll f = max_flow_dfs(G, s, t, INF, used);
  if (f == 0) {
   return flow;
  }
  flow += f;
 }
}

//フロー関連
//最大流
template< typename flow_t >
struct Dinic {
  const flow_t INF;

  struct edge {
    int to;
    flow_t cap;
    int rev;
    bool isrev;
    int idx;
  };

  vector< vector< edge > > graph;
  vector< int > min_cost, iter;

  Dinic(int V) : INF(numeric_limits< flow_t >::max()), graph(V) {}

  void add_edge(int from, int to, flow_t cap, int idx = -1) {
    graph[from].emplace_back((edge) {to, cap, (int) graph[to].size(), false, idx});
    graph[to].emplace_back((edge) {from, 0, (int) graph[from].size() - 1, true, idx});
  }

  bool bfs(int s, int t) {
    min_cost.assign(graph.size(), -1);
    queue< int > que;
    min_cost[s] = 0;
    que.push(s);
    while(!que.empty() && min_cost[t] == -1) {
      int p = que.front();
      que.pop();
      for(auto &e : graph[p]) {
        if(e.cap > 0 && min_cost[e.to] == -1) {
          min_cost[e.to] = min_cost[p] + 1;
          que.push(e.to);
        }
      }
    }
    return min_cost[t] != -1;
  }

  flow_t dfs(int idx, const int t, flow_t flow) {
    if(idx == t) return flow;
    for(int &i = iter[idx]; i < graph[idx].size(); i++) {
      edge &e = graph[idx][i];
      if(e.cap > 0 && min_cost[idx] < min_cost[e.to]) {
        flow_t d = dfs(e.to, t, min(flow, e.cap));
        if(d > 0) {
          e.cap -= d;
          graph[e.to][e.rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }

  flow_t max_flow(int s, int t) {
    flow_t flow = 0;
    while(bfs(s, t)) {
      iter.assign(graph.size(), 0);
      flow_t f = 0;
      while((f = dfs(s, t, INF)) > 0) flow += f;
    }
    return flow;
  }

  void output() {
    for(int i = 0; i < graph.size(); i++) {
      for(auto &e : graph[i]) {
        if(e.isrev) continue;
        auto &rev_e = graph[e.to][e.rev];
        cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << e.cap + rev_e.cap << ")" << endl;
      }
    }
  }
};

//最小費用流量
template< typename flow_t, typename cost_t >
struct PrimalDual {
  const cost_t INF;

  struct edge {
    int to;
    flow_t cap;
    cost_t cost;
    int rev;
    bool isrev;
  };
  vector< vector< edge > > graph;
  vector< cost_t > potential, min_cost;
  vector< int > prevv, preve;

  PrimalDual(int V) : graph(V), INF(numeric_limits< cost_t >::max()) {}

  void add_edge(int from, int to, flow_t cap, cost_t cost) {
    graph[from].emplace_back((edge) {to, cap, cost, (int) graph[to].size(), false});
    graph[to].emplace_back((edge) {from, 0, -cost, (int) graph[from].size() - 1, true});
  }

  cost_t min_cost_flow(int s, int t, flow_t f) {
    int V = (int) graph.size();
    cost_t ret = 0;
    using Pi = pair< cost_t, int >;
    priority_queue< Pi, vector< Pi >, greater< Pi > > que;
    potential.assign(V, 0);
    preve.assign(V, -1);
    prevv.assign(V, -1);

    while(f > 0) {
      min_cost.assign(V, INF);
      que.emplace(0, s);
      min_cost[s] = 0;
      while(!que.empty()) {
        Pi p = que.top();
        que.pop();
        if(min_cost[p.second] < p.first) continue;
        for(int i = 0; i < graph[p.second].size(); i++) {
          edge &e = graph[p.second][i];
          cost_t nextCost = min_cost[p.second] + e.cost + potential[p.second] - potential[e.to];
          if(e.cap > 0 && min_cost[e.to] > nextCost) {
            min_cost[e.to] = nextCost;
            prevv[e.to] = p.second, preve[e.to] = i;
            que.emplace(min_cost[e.to], e.to);
          }
        }
      }
      if(min_cost[t] == INF) return -1;
      for(int v = 0; v < V; v++) potential[v] += min_cost[v];
      flow_t addflow = f;
      for(int v = t; v != s; v = prevv[v]) {
        addflow = min(addflow, graph[prevv[v]][preve[v]].cap);
      }
      f -= addflow;
      ret += addflow * potential[t];
      for(int v = t; v != s; v = prevv[v]) {
        edge &e = graph[prevv[v]][preve[v]];
        e.cap -= addflow;
        graph[v][e.rev].cap += addflow;
      }
    }
    return ret;
  }

  void output() {
    for(int i = 0; i < graph.size(); i++) {
      for(auto &e : graph[i]) {
        if(e.isrev) continue;
        auto &rev_e = graph[e.to][e.rev];
        cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << rev_e.cap + e.cap << ")" << endl;
      }
    }
  }
};

template<typename W>
struct range_edge_graph {
    int n;
    struct edge { int to; W weight;};
    vector<vector<edge>> g;
    
    range_edge_graph(int n) : n(n) {
        g.resize(4*n);
        for (int i = 1; i < n; ++i) {
            int cl = i<<1|0, cr = i<<1|1;
            g[i].push_back({cl, 0});
            g[i].push_back({cr, 0});
            g[cl+2*n].push_back({i+2*n, 0});
            g[cr+2*n].push_back({i+2*n, 0});
        }
        for (int i = n; i < 2*n; ++i) g[i].push_back({i+2*n, 0});
    }

    // from [l1, r1) to [l2, r2)
    void add_edge(int l1, int r1, int l2, int r2, W w) {
        int idx = g.size();
        for (l1 += n, r1 += n; l1 < r1; l1 >>= 1, r1 >>= 1) {
            if (l1 & 1) g[l1+2*n].push_back({idx, 0}), l1++;
            if (r1 & 1) --r1, g[r1+2*n].push_back({idx, 0});
        }
        vector<edge> node;
        for (l2 += n, r2 += n; l2 < r2; l2 >>= 1, r2 >>= 1) {
            if (l2 & 1) node.push_back({l2++, w});
            if (r2 & 1) node.push_back({--r2, w});
        }
        g.push_back(node);
    }

    vector<W> dijkstra(int s) {
        s += n;
        vector<W> dist(g.size(), numeric_limits<W>::max());
        dist[s] = 0;
        using P = pair<W, int>;
        priority_queue<P, vector<P>, greater<P>> que;
        que.emplace(0, s);
        while (!que.empty()) {
            P p = que.top();
            que.pop();
            int v = p.second;
            if (dist[v] < p.first) continue;
            for (edge& e : g[v]) {
                if (dist[e.to] > dist[v] + e.weight) {
                    dist[e.to] = dist[v] + e.weight;
                    que.emplace(dist[e.to], e.to);
                }
            }
        }
        vector<W> res(dist.begin() + n, dist.begin() + 2*n);
        return res;
    }
};

//最短経路（負閉路検出）Eが小さい時
void BellmanFord(Graph& G, ll s, Array& d, Array& negative) {//O(|E||V|)
 d.resize(G.size());
 negative.resize(G.size());
 REP(i, d.size())d[i] = INF;
 REP(i, d.size())negative[i] = false;
 d[s] = 0;
 REP(k, G.size() - 1) {
  REP(i, G.size()) {
   REP(j, G[i].size()) {
    if (d[i] != INF && d[G[i][j].to] > d[i] + G[i][j].cap) {
     d[G[i][j].to] = d[i] + G[i][j].cap;
    }
   }
  }
 }
 REP(k, G.size() - 1) {
  REP(i, G.size()) {
   REP(j, G[i].size()) {
    if (d[i] != INF && d[G[i][j].to] > d[i] + G[i][j].cap) {
     d[G[i][j].to] = d[i] + G[i][j].cap;
     negative[G[i][j].to] = true;
    }
    if (negative[i] == true)negative[G[i][j].to] = true;
   }
  }
 }
}

//最短経路（負の辺が含まれていない場合）
//O(V^2)のものもある
void Dijkstra(Graph& G, ll s, Array& d) {//O(|E|log|V|)
  d.resize(G.size());
  REP(i, d.size())d[i] = INF;
  d[s] = 0;
  priority_queue<P, vector<P>, greater<P>> q;
  q.push(make_pair(0, s));
  while (!q.empty()) {
    P a = q.top();
    q.pop();
    if (d[a.second] < a.first)continue;
    REP(i, G[a.second].size()) {
      Edge e = G[a.second][i];
      if (d[e.to] > d[a.second] + e.cap) {
        d[e.to] = d[a.second] + e.cap;
        q.push(make_pair(d[e.to], e.to));
      }
    }
  }
}

//最短経路（0、１の辺のみ）O(|E|)
void bfs01(Graph& G, ll s, Array& d) {
 d.resize(G.size());
 REP(i, d.size())d[i] = INF;
 d[s] = 0;
 deque<ll> deq;
 deq.push_front(s);
 while (deq.size() > 0) {
  ll v = deq.front();
  //cout << v << endl;
  deq.pop_front();
  for (auto e : G[v]) {
   if (d[v] + e.cap < d[e.to]) {
    d[e.to] = d[v] + e.cap;
    if (e.cap == 0) {
     deq.push_front(e.to);
    }
    else {
     deq.push_back(e.to);
    }
   }
  }
 }
}

//最短経路
//負閉路があると、d[i][i]は負になる
void WarshallFloyd(Graph &G, Matrix &d) { //O(V^3)
    d.resize(G.size());
    REP(i, d.size())
    d[i].resize(G.size());
    REP(i, d.size()) {
        REP(j, d[i].size()) {
            d[i][j] = INF;
        }
    }
    REP(i, d.size()) {
        d[i][i] = 0;
    }
    REP(i, G.size()) {
        REP(j, G[i].size()) {
            d[i][G[i][j].to] = G[i][j].cap;
        }
    }
    REP(i, G.size()) {
        REP(j, G.size()) {
            REP(k, G.size()) {
                chmin(d[j][k], d[j][i] + d[i][k]);
            }
        }
    }
}

//強連結成分分解
class SCC{
	Graph g,rg;
	vector<int> cmp,used,vs;
	int size;
public:
	SCC(Graph _g):g(_g),rg(g.size()),cmp(g.size(),-1),used(g.size(),false){
		REP(i,g.size()){
			for(auto e:g[i]){
				add_edge(rg,e.to,i,1,false,0);//逆辺のグラフ
			}
		}

		REP(i,g.size()){
			if(!used[i]) dfs(i);
		}
		int k=0;
		
		REP(i,vs.size()){
			ll v=vs[vs.size()-1-i];
			if(used[v]){
				rdfs(v,k);
				k++;
			}
		}
		size=k;
	}

	void dfs(int v){
		//帰りがけ順に番号を振る
		used[v]=true;
		for(auto e:g[v]){
			if(!used[e.to]) dfs(e.to);
		}
		vs.push_back(v);
	}

	void rdfs(int v, int k){
		//通ることができた頂点に番号を振る
		used[v]=false;
		cmp[v]=k;
		for(auto e:rg[v]){
			if(used[e.to]) rdfs(e.to,k);
		}
	}

	int getSize(){
		return size;
	}

	int group(int v){
		return cmp[v];
	}

  Graph rebuild(){
		//縮約したグラフを返す
		Graph ret(size);
		REP(i,g.size()){
			int u=cmp[i];
			for(auto e:g[i]){
				int v=cmp[e.to];
				if(u==v) continue;
				add_edge(ret,u,v,e.cap,false,0);
			}
		}
		return ret;
	}
};

//トポロジカルソートO(E+V)
//閉路検出にも使える
bool tsort(Graph& graph, Array& order) { 
 int n = graph.size(), k = 0;
 Array in(n);
 for (auto& es : graph)
  for (auto& e : es)in[e.to]++;
 priority_queue<ll, Array, greater<ll>> que;
 REP(i, n)
  if (in[i] == 0)que.push(i);
 while (que.size()) {
  int v = que.top();
  que.pop();
  order.push_back(v);
  for (auto& e : graph[v])
   if (--in[e.to] == 0)que.push(e.to);
 }
 if (order.size() != n)return false;
 else return true;
}

class UnionFind {
 vector<int> data;
 int num;
public:
 UnionFind(int size) : data(size, -1), num(size) { }
 bool unionSet(int x, int y) {
  x = root(x); y = root(y);
  if (x != y) {
   if (data[y] < data[x]) swap(x, y);
   data[x] += data[y]; data[y] = x;
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

class UnionFindUndo {
    vector<int> data;
    vector<pair<int, int>> history;
    int num;

  public:
    UnionFindUndo(int size) : data(size, -1), num(size) {}
    bool unionSet(int x, int y) {
        x = root(x);
        y = root(y);
        if(x != y) {
            history.push_back({x, data[x]});
            history.push_back({y, data[y]});
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
        return data[x] < 0 ? x : root(data[x]);
    }
    int size(int x) {
        return -data[root(x)];
    }
    int numSet() {
        return num;
    }

    void snapshot() {
        history.clear();
    }
    void undo() {
        data[history.back().first] = history.back().second;
        history.pop_back();
        data[history.back().first] = history.back().second;
        history.pop_back();
    }
    void rollback() {
        while(history.size())
            undo();
    }
};

template<class Abel> struct UnionFind {
    vector<int> par;
    vector<int> rank;
    vector<Abel> diff_weight;

    UnionFind(int n = 1, Abel SUM_UNITY = 0) {
        init(n, SUM_UNITY);
    }

    void init(int n = 1, Abel SUM_UNITY = 0) {
        par.resize(n); rank.resize(n); diff_weight.resize(n);
        for (int i = 0; i < n; ++i) par[i] = i, rank[i] = 0, diff_weight[i] = SUM_UNITY;
    }

    int root(int x) {
        if (par[x] == x) {
            return x;
        }
        else {
            int r = root(par[x]);
            diff_weight[x] += diff_weight[par[x]];
            return par[x] = r;
        }
    }

    Abel weight(int x) {
        root(x);
        return diff_weight[x];
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    bool merge(int x, int y, Abel w) {
        w += weight(x); w -= weight(y);
        x = root(x); y = root(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) swap(x, y), w = -w;
        if (rank[x] == rank[y]) ++rank[x];
        par[y] = x;
        diff_weight[y] = w;
        return true;
    }

    Abel diff(int x, int y) {
        return weight(y) - weight(x);
    }
};


class persistentUF {
public:
  int now;         // 現在時刻
  vector<int> par;  // 各頂点の親
  vector<int> rank; // その頂点を根とする木の深さ
  vector<int> time; // 親がいつ更新されたか
  vector<vector<pair<int, int>>> num;// (時刻, 頂点数)を要素にもつvector
    
    explicit persistentUF(int n):rank(n,0),time(n,INF_INT),par(n,0),
			now(0),num(n,vector<pair<int,int>>(1,make_pair(0,1))) {
        for (int i = 0; i < n; i++) {
            par[i] = i;
        }
    }
    
    // 時刻tにおけるxの親を返す
    int root(int x, int t) {
        if (t < time[x]) {
            return x;
        } else {
            return root(par[x], t);
        }
    }
    
    // 時刻tにてxとyが同じ木に属するか判定
    bool findSet(int x, int y, int t) {
        return root(x, t) == root(y, t);
    }
    
    // 頂点xとyを繋げる
    void unionSet(int x, int y) {
        ++now;
        
        x = root(x, now);
        y = root(y, now);

        if (x == y) return;

        if (rank[x] < rank[y]) swap(x, y);

		num[x].push_back(make_pair(now, size(x, now) + size(y, now)));
        
        par[y] = x;
        time[y] = now;
        if (rank[x] == rank[y]) ++rank[x];
    }

	// 時刻tにおいて、頂点xを含む木の要素数を返す
    int size(int x, int t) {
        x = root(x, t);
        
        int ok = 0, ng = num[x].size();
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (num[x][mid].first <= t) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        
        return num[x][ok].second;
    }
};

template <typename T, typename F>
class SegmentTree {
  private:
    T e;
    F op;
    ll n;
    vector<T> dat;

  public:
    SegmentTree(F f, T _e, vector<T> v) : op(f), e(_e) {
        int _n = v.size();
        n = 1;
        while(n < _n)
            n *= 2;
        dat.resize(2 * n - 1, e);
        REP(i, _n)
        dat[n + i - 1] = v[i];
        for(int i = n - 2; i >= 0; i--)
            dat[i] = op(dat[i * 2 + 1], dat[i * 2 + 2]);
    }
    SegmentTree(F f, T _e, int _n) : op(f), e(_e) {
        n = 1;
        while(n < _n)
            n *= 2;
        dat.resize(2 * n - 1, e);
        for(int i = n - 2; i >= 0; i--)
            dat[i] = op(dat[i * 2 + 1], dat[i * 2 + 2]);
    }
    void update(int i, T x) {
        i += n - 1;
        dat[i] = x;
        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = op(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }
    T get(int i) {
        i += n - 1;
        return dat[i];
    }
    T query(int l, int r) {
        //[l,r)でのクエリ処理
        T left = e, right = e;
        l += n - 1;
        r += n - 1;
        while(l < r) {
            if((l & 1) == 0)
                left = op(left, dat[l]);
            if((r & 1) == 0)
                right = op(dat[r - 1], right);
            l = l / 2;
            r = (r - 1) / 2;
        }
        return op(left, right);
    }
};

auto op = [](bool a, bool b) {return a || b; };
SegmentTree<bool, decltype(op)> seg(op,false,n);

using S=ll;
S id=-INF;
auto op = [](S a, S b) {return max(a,b); };
SegmentTree<S, decltype(op)> seg(op,id,n);

auto op = [](P a, P b) { return max(a, b); };
SegmentTree<P, decltype(op)> seg(op, make_pair(-INF, -INF), n);


//k番目の値を求める時
//https://atcoder.jp/contests/arc033/submissions/8290634

class SumSegTree {
private:
	
	ll _sum(ll a, ll b, ll k, ll l, ll r) {
		if (r <= a || b <= l)return 0;
		if (a <= l && r <= b)return dat[k];
		else {
			ll s1 = _sum(a, b, 2 * k + 1, l, (l + r) / 2);
			ll s2 = _sum(a, b, 2 * k + 2, (l + r) / 2, r);
			return s1 + s2;
		}
	}
public:
	ll n, height;
	vector<ll> dat;

	SumSegTree(ll _n) {
		n = 1;
		height = 1;
		while (n < _n) {
			n *= 2;
			height++;
		}
		dat = vector<ll>(2 * n - 1,0);
	}

  //場所i(0-indexed)にxを足す
	void add(ll i, ll x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

  //区間[a,b)の総和
	ll sum(ll a, ll b) {
		return _sum(a, b, 0, 0, n);
	}
};

//最小値の位置を返す
//https://yukicoder.me/submissions/395659

class RmqTree {
private:
	P _find(ll a, ll b, ll k, ll l, ll r) {
		//区間[l,r)の最小値を持つk番目のノードを探索
		if (r <= a || b <= l)return make_pair(INF,-1);    // 交差しない
		if (a <= l && r <= b)return dat[k];   // 完全に含む
		else {
			P s1 = _find(a, b, 2 * k + 1, l, (l + r) / 2); // 左の子
			P s2 = _find(a, b, 2 * k + 2, (l + r) / 2, r); // 右の子
			return min(s1, s2);
		}
	}

public:
	ll n, height;
	vector<P> dat;

	// 初期化（_nは最大要素数）
	RmqTree(ll _n) {
		n = 1;
		height = 1;
		while (n < _n) {
			n *= 2;
			height++;
		}
		dat = vector<P>(2 * n - 1, make_pair(INF,-1));
	}

	// i番目の値(0-indexed)をxに変更
	void update(ll i, ll x) {
		i += n - 1; // i番目の葉ノードへ
		dat[i] = make_pair(x,i-(n-1));
		while (i > 0) { // 登りながら更新
			i = (i - 1) / 2;//親ノードのインデックス
			dat[i] = min(dat[i * 2 + 1], dat[i * 2 + 2]);
			//子ノードの小さい方の値を代入
		}
	}

	// 区間[a,b)の最小値の取得
	P find(ll a, ll b) {
		return _find(a, b, 0, 0, n);
	}
};

class RmqTree {
private:
 ll _find(ll a, ll b, ll k, ll l, ll r) {
  if (r <= a || b <= l)return INF;
  if (a <= l && r <= b)return dat[k];
  else {
   ll s1 = _find(a, b, 2 * k + 1, l, (l + r) / 2);
   ll s2 = _find(a, b, 2 * k + 2, (l + r) / 2, r);
   return min(s1, s2);
  }
 }
public:
 ll n, height;
 vector<ll> dat;

 RmqTree(ll _n) {
  n = 1;
  height = 1;
  while (n < _n) {
   n *= 2;
   height++;
  }
  dat = vector<ll>(2 * n - 1, INF);
 }

 //場所iにxを代入する
 void update(ll i, ll x) {
  i += n - 1;
  dat[i] = x;
  while (i > 0) {
   i = (i - 1) / 2;
   dat[i] = min(dat[i * 2 + 1], dat[i * 2 + 2]);
  }
 }

 //区間[a,b)の最小値
 ll find(ll a, ll b) {
  return _find(a, b, 0, 0, n);
 }
};



template<typename T,typename E,typename F,typename G,typename H,typename P>
class LazySegmentTree {
private:
    const T identity_t;
    const E identity_e;
    F f_tt;
    G f_te;
    H f_ee;
    P f_ep;//区間に比例する場合
    vector<T> dat;
    vector<E> lazy;
    int n;
    void eval(int k, int len) {
        if (lazy[k] == identity_e)return;
        if (k < n - 1) {
            lazy[2 * k + 1] = f_ee(lazy[2 * k + 1], lazy[k]);
            lazy[2 * k + 2] = f_ee(lazy[2 * k + 2], lazy[k]);
        }
        dat[k] = f_te(dat[k], f_ep(lazy[k], len));
        lazy[k] = identity_e;
    }
    void update(int a, int b, E x, int k, int l, int r) {
        eval(k, r - l);
        if (a <= l && r <= b) {
            lazy[k] = f_ee(lazy[k], x);
            eval(k, r - l);
        }
        else if (a < r && l < b) {
            update(a, b, x, k * 2 + 1, l, (l + r) / 2);
            update(a, b, x, k * 2 + 2, (l + r) / 2, r);
            dat[k] = f_tt(dat[2 * k + 1], dat[2 * k + 2]);
        }
    }
    T query(int a, int b, int k,int l,int r) {
        eval(k,r-l);
        if (r <= a || b <= l)return identity_t;
        else if (a <= l && r <= b)return dat[k];
        else {
            T vl = query(a, b, 2*k + 1, l, (l + r) / 2);
            T vr = query(a, b, 2 * k + 2, (l + r) / 2, r);
            return f_tt(vl, vr);
        }
    }
public:
    LazySegmentTree(vector<T> v, T id_t, E id_e, F f, G g, H h, P p) :
        identity_t(id_t), identity_e(id_e), f_tt(f), f_te(g), f_ee(h), f_ep(p) {
        n = 1;
        while (n < v.size())n *= 2;
        dat.resize(2 * n - 1, identity_t);
        lazy.resize(2 * n - 1, identity_e);
        REP(i, v.size())dat[n + i - 1] = v[i];
        for (int i = n - 2; i >= 0; i--)dat[i] = f_tt(dat[i * 2 + 1], dat[i * 2 + 2]);

    }
    void update(int a, int b, E x) { update(a, b, x, 0, 0, n); }
    T query(int a, int b) { return query(a, b, 0, 0, n); }
};

*区間add区間max
ll id_t = -INF, id_e = 0;
auto f = [](ll a, ll b) {return max(a, b); };
auto g = [](ll a, ll b) {return a + b; };
auto h = [](ll a, ll b) {return a + b; };
auto p = [](ll a, ll b) {return a; };
LazySegmentTree<ll, ll,decltype(f),decltype(g),decltype(h),decltype(p)> seg(Array(n + 1, 0), id_t, id_e, f, g, h, p);

*区間add区間sum
ll id_t = 0, id_e = 0;
auto f = [](ll a, ll b) {return a + b; };
auto g = [](ll a, ll b) {return a + b; };
auto h = [](ll a, ll b) {return a + b; };
auto p = [](ll a, ll b) {return a * b; };
LazySegmentTree<ll, ll,decltype(f),decltype(g),decltype(h),decltype(p)> seg(Array(n + 1, 0), id_t, id_e, f, g, h, p);



//*一次関数(区間 add、区間消去、区間sum)
using S=ll;
using L=P;
    S id_t = 0;
    L id_e = {1,0};
    auto f = [](S a, S b) {return a+b; };//dat->dat
    auto gg = [](S a, L b) {return a*b.first+b.second; };//lazy->dat
    auto h = [](L a, L b) {return make_pair(a.first*b.first, a.second*b.first+b.second); };//lazy->lazy
    auto p = [](L a, ll b) {return make_pair(a.first,a.second*b); };//lazyが区間に比例するか
    LazySegmentTree<S, L,decltype(f),decltype(gg),decltype(h),decltype(p)> seg(Array(n+1,0), id_t, id_e, f, gg, h, p);


//BIT
template< typename T >
class FenwickTree {
	vector< T > data;
	int n;
	int p;
public:
	FenwickTree(int n) :n(n) { 
		data.resize(n + 1LL, 0);
		p = 1;
		while (p < data.size())p *= 2;
	}
	T sum(int k) {
		T ret = 0;
		for (; k > 0; k -= k & -k) ret += data[k];
		return (ret);
	}
	
	T sum(int a, int b) { return sum(b) - sum(a); }//[a,b)
 
	void add(int k, T x) {
		for (++k; k <= n; k += k & -k) data[k] += x;
	}
 
	int lower_bound(ll w) {
		if (w <= 0)return -1;
		int x = 0;
		for (int k = p / 2; k > 0; k /= 2) {
			if (x + k <= n && data[x + k] < w)w -= data[x + k], x += k;
		}
		return x;
	}
};


ll mod_pow(ll x, ll n, ll mod) {
 ll res = 1LL;
 while (n > 0) {
  if (n & 1) res = res * x % mod;
  x = x * x % mod;
  n >>= 1;
 }
 return res;
}

ll mod_inv(ll x, ll mod) {
 return mod_pow(x, mod - 2, mod);
}

ll nCr(ll n, ll r) {
    if(n < r)
        return 0;

    ll ret = 1;
    REP(i, r) {
        (ret *= (n - i) % MOD) %= MOD;
        (ret *= mod_inv(i + 1, MOD)) %= MOD;
    }
    return ret;
}

//ラグランジュ補間(d+1)点をとおるd次式を求める
ll LagrangeInterpolation(vector<ll> y, ll t) {
    int n = y.size() - 1;
    if(t <= n)
        return y[t];
    ll fi = 1, ft = 1;
    for(ll i = 0; i <= n; i++) {
        if(i)
            (fi *= MOD - i) %= MOD;
        (ft *= t - i) %= MOD;
    }
    ll ret = 0;
    for(ll i = 0; i <= n; i++) {
        (ret += y[i] * ft % MOD * mod_inv(t - i, MOD) % MOD * mod_inv(fi, MOD) % MOD) %= MOD;
        (fi *= (i + 1) * mod_inv(MOD - n + i, MOD) % MOD) %= MOD;
    }
    return ret;
}

//MODが大きめの時(1777777777のとき)はmintがばぐるので下つかう
    vec<ll> fact(k + 1, 1), finv(k + 1, 0);
    REP(i, k) {
        fact[i + 1] = fact[i] * (i + 1) % MOD;
    }
    finv[k] = mod_inv(fact[k], MOD);
    REP2(i, k) {
        finv[i] = finv[i + 1] * (i + 1) % MOD;
    }

    auto mCr = [&](ll m, ll r) -> ll {
        return fact[m] * finv[m - r] % MOD * finv[r] % MOD;
    };

template< int mod >
struct ModInt {
  int x;

  ModInt() : x(0) {}

  ModInt(int64_t y) : x(y >= 0 ? y % mod : (mod - (-y) % mod) % mod) {}

  ModInt &operator+=(const ModInt &p) {
    if((x += p.x) >= mod) x -= mod;
    return *this;
  }

  ModInt &operator-=(const ModInt &p) {
    if((x += mod - p.x) >= mod) x -= mod;
    return *this;
  }

  ModInt &operator*=(const ModInt &p) {
    x = (int) (1LL * x * p.x % mod);
    return *this;
  }

  ModInt &operator/=(const ModInt &p) {
    *this *= p.inverse();
    return *this;
  }

  ModInt operator-() const { return ModInt(-x); }

  ModInt operator+(const ModInt &p) const { return ModInt(*this) += p; }

  ModInt operator-(const ModInt &p) const { return ModInt(*this) -= p; }

  ModInt operator*(const ModInt &p) const { return ModInt(*this) *= p; }

  ModInt operator/(const ModInt &p) const { return ModInt(*this) /= p; }

  bool operator==(const ModInt &p) const { return x == p.x; }

  bool operator!=(const ModInt &p) const { return x != p.x; }

  ModInt inverse() const {
    int a = x, b = mod, u = 1, v = 0, t;
    while(b > 0) {
      t = a / b;
      swap(a -= t * b, b);
      swap(u -= t * v, v);
    }
    return ModInt(u);
  }

  ModInt pow(int64_t n) const {
    ModInt ret(1), mul(x);
    while(n > 0) {
      if(n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }

  friend ostream &operator<<(ostream &os, const ModInt &p) {
    return os << p.x;
  }

  friend istream &operator>>(istream &is, ModInt &a) {
    int64_t t;
    is >> t;
    a = ModInt< mod >(t);
    return (is);
  }

  static int get_mod() { return mod; }
};

using mint = ModInt<MOD>;


// 二項係数ライブラリ
template <class T>
struct Combination {
    vector<T> fact_, inv_, finv_;
    constexpr Combination() {}
    constexpr Combination(int n) noexcept : fact_(n, 1), inv_(n, 1), finv_(n, 1) {
        init(n + 1);
    }
    constexpr void init(int n) noexcept {
        fact_.assign(n, 1), inv_.assign(n, 1), finv_.assign(n, 1);
        int MOD = fact_[0].get_mod();
        for(int i = 2; i < n; i++) {
            fact_[i] = fact_[i - 1] * i;
            inv_[i] = -inv_[MOD % i] * (MOD / i);
            finv_[i] = finv_[i - 1] * inv_[i];
        }
    }
    constexpr T nPr(int n, int k) const noexcept {
        if(n < k || n < 0 || k < 0)
            return 0;
        return fact_[n] * finv_[n - k];
    }
    constexpr T nCr(int n, int k) const noexcept {
        if(n < k || n < 0 || k < 0)
            return 0;
        return fact_[n] * finv_[k] * finv_[n - k];
    }
    constexpr T nHr(int n, int k) const noexcept {
        if(n <= 0 || k < 0)
            return 0;
        return nCr(n + k - 1, k);
    }
    constexpr T fact(int n) const noexcept {
        if(n < 0)
            return 0;
        return fact_[n]; //n!
    }
    constexpr T inv(int n) const noexcept {
        if(n < 0)
            return 0;
        return inv_[n]; //1/n
    }
    constexpr T finv(int n) const noexcept {
        if(n < 0)
            return 0;
        return finv_[n]; //1/n!
    }
};
Combination<mint> com(n);


//nCr(modなし)https://atcoder.jp/contests/abc057/submissions/8435522?lang=ja
// Combination Table//パスカルの三角形 O(n^2)

ll C[51][51]; // C[n][k] -> nCk
void comb_table(int N){
 for(int i=0;i<=N;++i){
  for(int j=0;j<=i;++j){
   if(j==0 or j==i){
    C[i][j]=1LL;
   }else{
    C[i][j]=(C[i-1][j-1]+C[i-1][j]);
   }
  }
 }
}


//nCr,nPr,modの逆元
//経路探索(0,0)->(dx,dy) nHr(dx,dy-1)  (配列確保が２＊n必要なので注意)
class Combination {
public:
 Array fact;//n！
 Array inv;//n!の逆元
 ll mod;
 Combination(ll n, ll _mod) {
  mod = _mod;
  fact.resize(n + 1);
  fact[0] = 1;
  REP(i, n) {
   fact[i + 1] = (fact[i] * (i + 1LL)) % mod;
  }
  inv.resize(n + 1);
  inv[n] = mod_inv(fact[n]);
  for(int i=n;i>0;i--) {
   inv[i - 1] = inv[i] * i % mod;
  }
 }
 //modの逆元
 ll mod_inv(ll x) {
  ll n = mod - 2;
  ll res = 1LL;
  while (n > 0) {
   if (n & 1) res = res * x % mod;
   x = x * x % mod;
   n >>= 1;
  }
  return res;
 }
 ll nCr(ll n, ll r) {
  return ((fact[n] * inv[r] % mod) * inv[n - r]) % mod;
 }
 ll nPr(ll n, ll r) {
  return (fact[n] * inv[n - r]) % mod;
 }
 ll nHr(ll n, ll r) {
  //重複組み合わせ：n種類のものからr種類選ぶ
  return nCr(r + n - 1, r);
 }
};



//約数
void divisor(ll n, vector<ll>& ret) {
 for (ll i = 1; i * i <= n; i++) {
  if (n % i == 0) {
   ret.push_back(i);
   if (i * i != n) ret.push_back(n / i);
  }
 }
 sort(ret.begin(), ret.end());
}

//素因数分解
void factoring(ll n, map<ll, ll>& mp) {
 	for (ll i = 2; i * i <= n; i++) {
  		while (n % i == 0) {
   			mp[i]++;
   			n /= i;
  		}
 	}
 	if (n != 1) mp[n]++;
}

//素数列挙
void Eratosthenes(ll n, vec<int> &arr) {
    arr.resize(n + 1, 1);
    for(ll i = 2; i * i <= n; i++) {
        if(arr[i]) {
            for(ll j = 2; i * j <= n; j++) {
                arr[i * j] = 0;
            }
        }
    }
}
//2~nの素数に対してそれぞれ処理してく時
arr.resize(n + 1, 1);
for(ll i = 2; i * 2 <= n; i++) {
    if(arr[i]) {
        for(ll j = 2; i * j <= n; j++) {
            arr[i * j] = 0;
        }
    }
}


//素数判定
bool IsPrime(ll n)
{
 if (n < 2) return false;
 else if (n % 2 == 0) return false; // �����͂��炩���ߏ���

 for (ll i = 3; i*i <= n; i += 2)
 {
  if (n % i == 0) return false;
 }
 return true;
}

//最大公約数
ll gcd(ll m, ll n) {
 if (n == 0)return m;
 return gcd(n, m % n);
}//gcd

//最小公倍数
ll lcm(ll m, ll n) {
 return m / gcd(m, n) * n;
}

//繰り返し２乗法
ll pow2(ll x, ll n) {
 ll ans = 1ll;
 while (n > 0) {
  if ((n & 1) == 1) {
   ans = ans * x;
  }
  x = x * x;
  n >>= 1;
 }
 return ans;
}

ll pow2(ll x, ll n, ll ma) {
    ll ans = 1ll;
    bool flag = true;
    bool flag2 = false;
    while(n > 0) {
        if((n & 1) == 1) {
            ans = ans * x;
            if(ans > ma or flag2)
                return ma;
        }
        x = x * x;
        if(x > ma)
            flag2 = true;
        n >>= 1;
    }
    return ans;
}

ll pow2_m(ll x, ll n, ll mod) {
 ll ans = 1ll;
 while (n > 0) {
  if ((n & 1) == 1) {
   ans = ans * x % mod;
  }
  x = x * x % mod;
  n >>= 1;
 }
 return ans;
}

vector<ll> lis_fast(const vector<ll>& a) {//最長部分増加列
 const ll n = a.size();
 vector<ll> A(n, INT_MAX);
 vector<ll> id(n);
 for (int i = 0; i < n; ++i) {
  //id[i] = distance(A.begin(), upper_bound(A.begin(), A.end(), a[i]));//最長部分非減少率
  id[i] = distance(A.begin(), lower_bound(A.begin(), A.end(), a[i]));
  A[id[i]] = a[i];
 }
 ll m = *max_element(id.begin(), id.end());
 vector<ll> b(m + 1);
 for (int i = n - 1; i >= 0; --i)
  if (id[i] == m) b[m--] = a[i];
 return b;
}



//文字列比較
bool z_algorithm(string& str, vector<int>& z, ll s) {//s+tを渡して、tにsが含まれているかを返す
 const int L = str.size();
 z.resize(str.size());
 for (int i = 1, left = 0, right = 0; i < L; i++) {
  if (i > right) {
   left = right = i;
   for (; right < L && str[right - left] == str[right]; right++);
   z[i] = right - left;
   right--;
  }
  else {
   int k = i - left;
   if (z[k] < right - i + 1) {
    z[i] = z[k];
   }
   else {
    left = i;
    for (; right < L && str[right - left] == str[right]; right++);
    z[i] = right - left;
    right--;
   }
  }
  if (z[i] == s)return true;
 }
 return false;
}
bool z_algorithm(string& str, vector<int>& z) {//z[i]==|s|の時sが含まれる
 const int L = str.size();
 z.resize(str.size());
 for (int i = 1, left = 0, right = 0; i < L; i++) {
  if (i > right) {
   left = right = i;
   for (; right < L && str[right - left] == str[right]; right++);
   z[i] = right - left;
   right--;
  }
  else {
   int k = i - left;
   if (z[k] < right - i + 1) {
    z[i] = z[k];
   }
   else {
    left = i;
    for (; right < L && str[right - left] == str[right]; right++);
    z[i] = right - left;
    right--;
   }
  }
 }
 return true;
}

//ローリングハッシュ
//二分探索でLCPを求める機能つき
struct RollingHash {
 static const int base1 = 1007, base2 = 2009;
 static const int mod1 = 1000000007, mod2 = 1000000009;
 vector<long long> hash1, hash2, power1, power2;

 // construct
 RollingHash(const string& S) {
  int n = (int)S.size();
  hash1.assign(n + 1, 0);
  hash2.assign(n + 1, 0);
  power1.assign(n + 1, 1);
  power2.assign(n + 1, 1);
  for (int i = 0; i < n; ++i) {
   hash1[i + 1] = (hash1[i] * base1 + S[i]) % mod1;
   hash2[i + 1] = (hash2[i] * base2 + S[i]) % mod2;
   power1[i + 1] = (power1[i] * base1) % mod1;
   power2[i + 1] = (power2[i] * base2) % mod2;
  }
 }

 // get hash of S[left:right]
 inline pair<long long, long long> get(int l, int r) const {
  long long res1 = hash1[r] - hash1[l] * power1[r - l] % mod1;
  if (res1 < 0) res1 += mod1;
  long long res2 = hash2[r] - hash2[l] * power2[r - l] % mod2;
  if (res2 < 0) res2 += mod2;
  return { res1, res2 };
 }

 // get lcp of S[a:] and T[b:]
 inline int getLCP(int a, int b) const {
  int len = min((int)hash1.size() - a, (int)hash1.size() - b);
  int low = 0, high = len;
  while (high - low > 1) {
   int mid = (low + high) >> 1;
   if (get(a, a + mid) != get(b, b + mid)) high = mid;
   else low = mid;
  }
  return low;
 }
};


//行列演算
Matrix mIdentity(ll n) {
 Matrix A(n, Array(n));
 for (int i = 0; i < n; ++i) A[i][i] = 1;
 return A;
}

Matrix mMul(const Matrix& A, const Matrix& B) {
 Matrix C(A.size(), Array(B[0].size()));
 for (int i = 0; i < C.size(); ++i)
  for (int j = 0; j < C[i].size(); ++j)
   for (int k = 0; k < A[i].size(); ++k)
    (C[i][j] += (A[i][k] % MOD) * (B[k][j] % MOD)) %= MOD;
 return C;
}
// O( n^3 log e )
Matrix mPow(const Matrix& A, ll e) {
 return e == 0 ? mIdentity(A.size()) :
  e % 2 == 0 ? mPow(mMul(A, A), e / 2) : mMul(A, mPow(A, e - 1));
}

//行列演算（mint用）
using S=mint;
vvec<S> mIdentity(ll n) {
    vvec<S> A(n, vec<S>(n));
    for(int i = 0; i < n; ++i)
        A[i][i] = 1;
    return A;
}

vvec<S> mMul(const vvec<S> &A, const vvec<S> &B) {
    vvec<S> C(A.size(), vec<S>(B[0].size()));
    for(int i = 0; i < C.size(); ++i)
        for(int j = 0; j < C[i].size(); ++j)
            for(int k = 0; k < A[i].size(); ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}
// O( n^3 log e )
vvec<S> mPow(const vvec<S> &A, ll e) {
    return e == 0 ? mIdentity(A.size()) : e % 2 == 0 ? mPow(mMul(A, A), e / 2) : mMul(A, mPow(A, e - 1));
}



class Trie {
  private:
    vvec<int> nxt;
    vec<int> end; //suffix link,そのノードに存在する文字列の数
    int root, p;
    int maxn; //文字の合計サイズ
    vec<int> num, len;

  public:
    Trie(int maxn) : maxn(maxn), nxt(maxn, vec<int>(26, -1)), end(maxn, 0), p(1), root(0), num(maxn, 0), len(maxn, 0) {}

    void insert(const string &s) {
        int now = root;
        for(int i = 0; i < s.size(); i++) {
            if(nxt[now][s[i] - 'a'] == -1)
                nxt[now][s[i] - 'a'] = p, len[p++] = len[now] + 1;
            now = nxt[now][s[i] - 'a'];
        }
        end[now]++;
    }

    pair<int, int> dfs(int now) {
        int con = 0;
        pair<int, int> ret = {end[now], now};
        REP(i, 26) {
            if(nxt[now][i] == -1)
                continue;
            auto tmp = dfs(nxt[now][i]);
            ret.first += tmp.first;
            nxt[now][i] = tmp.second;
            con++;
        }
        num[now] = ret.first;
        if(!end[now] and con == 1) {
            REP(i, 26) {
                if(nxt[now][i] != -1)
                    ret.second = nxt[now][i];
            }
        }
        return ret;
    }

    int solve(string &s, string &t, int now) {
        int pos = len[now];
        if(pos == s.size())
            return 1;
        int ret = end[now];
        REP(i, 26) {
            if(s[pos] == t[i]) {
                ret += solve(s, t, nxt[now][t[i] - 'a']);
                break;
            }
            if(nxt[now][t[i] - 'a'] == -1)
                continue;
            ret += num[nxt[now][t[i] - 'a']];
        }
        return ret;
    }
};


class AhoCorasick {
  private:
    vvec<int> nxt;
    vec<int> fail, end; //suffix link,そのノードに存在する文字列の数
    int root, p;
    int maxn; //文字の合計サイズ
  public:
    AhoCorasick(int maxn) : maxn(maxn), nxt(maxn, vec<int>(26, -1)), fail(maxn), end(maxn, 0), p(1), root(0) {}

    void insert(const string &s) {
        int now = root;
        for(int i = 0; i < s.size(); i++) {
            if(nxt[now][s[i] - 'a'] == -1)
                nxt[now][s[i] - 'a'] = p++;
            now = nxt[now][s[i] - 'a'];
        }
        end[now]++;
    }

    void build() {
        queue<int> que;
        fail[root] = root;
        for(int i = 0; i < 26; ++i) {
            if(nxt[root][i] == -1)
                nxt[root][i] = root;
            else {
                fail[nxt[root][i]] = root;
                que.push(nxt[root][i]);
            }
        }
        while(!que.empty()) {
            int now = que.front();
            que.pop();
            for(int i = 0; i < 26; ++i) {
                if(nxt[now][i] == -1)
                    nxt[now][i] = nxt[fail[now]][i];
                else {
                    fail[nxt[now][i]] = nxt[fail[now]][i];
                    que.push(nxt[now][i]);
                }
            }
        }
    }

    ll query(const string &s) { //マッチングの総数
        int now = root;
        ll cnt = 0;
        for(int i = 0; i < s.size(); ++i) {
            now = nxt[now][s[i] - 'a'];
            int tmp = now;
            while(tmp != root && end[tmp] != -1) {
                cnt += end[tmp];
                end[tmp] = -1;
                tmp = fail[tmp];
            }
        }
        return cnt;
    }
};



struct Mo {
    int width;
    vector<int> left, right, order;
    vector<bool> v;

    Mo(int N, int Q, vector<int> _c) : width((int)sqrt(N)), order(Q), v(N) {
        iota(begin(order), end(order), 0);
    }

    void add(int l, int r) { // [l, r)
        left.emplace_back(l);
        right.emplace_back(r);
    }

    void add(int idx) {
        //区間に足す処理
    }
    void del(int idx) {
        //区間から消す処理
    }
    void rem(int idx) {
        //答え記録
    }

    int run() {
        assert(left.size() == order.size());
        sort(begin(order), end(order), [&](int a, int b) {
            int ablock = left[a] / width, bblock = left[b] / width;
            if(ablock != bblock)
                return ablock < bblock;
            if(ablock & 1)
                return right[a] < right[b];
            return right[a] > right[b];
        });
        int nl = 0, nr = 0;
        auto push = [&](int idx) {
            v[idx].flip();
            if(v[idx])
                add(idx);
            else
                del(idx);
        };
        for(auto idx : order) {
            while(nl > left[idx])
                push(--nl);
            while(nr < right[idx])
                push(nr++);
            while(nl < left[idx])
                push(nl++);
            while(nr > right[idx])
                push(--nr);
            rem(idx);
        }
    }
};


template <class T>class RectangleSum {
public:
 vector<vector<T>> sum;
 T GetSum(int left, int right, int top, int bottom) { //[left, right], [top, bottom]
  T res = sum[bottom][right];
  if (left > 0) res -= sum[bottom][left - 1];
  if (top > 0) res -= sum[top - 1][right];
  if (left > 0 && top > 0) res += sum[top - 1][left - 1];
  return res;
 }
 RectangleSum(const vector<vector<T>>& s, int h, int w) {
  sum.resize(h);
  for (int i = 0; i < h; i++) sum[i].resize(w, 0);
  for (int y = 0; y < h; y++) {
   for (int x = 0; x < w; x++) {
    sum[y][x] = s[y][x];
    if (y > 0) sum[y][x] += sum[y - 1][x];
    if (x > 0) sum[y][x] += sum[y][x - 1];
    if (y > 0 && x > 0) sum[y][x] -= sum[y - 1][x - 1];
   }
  }
 }
};




//NTT
template <int mod>
struct NumberTheoreticTransform {

    vector<int> rev, rts;
    int base, max_base, root;

    NumberTheoreticTransform() : base(1), rev{0, 1}, rts{0, 1} {
        assert(mod >= 3 && mod % 2 == 1);
        auto tmp = mod - 1;
        max_base = 0;
        while(tmp % 2 == 0)
            tmp >>= 1, max_base++;
        root = 2;
        while(mod_pow(root, (mod - 1) >> 1) == 1)
            ++root;
        assert(mod_pow(root, mod - 1) == 1);
        root = mod_pow(root, (mod - 1) >> max_base);
    }

    inline int mod_pow(int x, int n) {
        int ret = 1;
        while(n > 0) {
            if(n & 1)
                ret = mul(ret, x);
            x = mul(x, x);
            n >>= 1;
        }
        return ret;
    }

    inline int inverse(int x) {
        return mod_pow(x, mod - 2);
    }

    inline unsigned add(unsigned x, unsigned y) {
        x += y;
        if(x >= mod)
            x -= mod;
        return x;
    }

    inline unsigned mul(unsigned a, unsigned b) {
        return 1ull * a * b % (unsigned long long)mod;
    }

    void ensure_base(int nbase) {
        if(nbase <= base)
            return;
        rev.resize(1 << nbase);
        rts.resize(1 << nbase);
        for(int i = 0; i < (1 << nbase); i++) {
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
        }
        assert(nbase <= max_base);
        while(base < nbase) {
            int z = mod_pow(root, 1 << (max_base - 1 - base));
            for(int i = 1 << (base - 1); i < (1 << base); i++) {
                rts[i << 1] = rts[i];
                rts[(i << 1) + 1] = mul(rts[i], z);
            }
            ++base;
        }
    }

    void ntt(vector<int> &a) {
        const int n = (int)a.size();
        assert((n & (n - 1)) == 0);
        int zeros = __builtin_ctz(n);
        ensure_base(zeros);
        int shift = base - zeros;
        for(int i = 0; i < n; i++) {
            if(i < (rev[i] >> shift)) {
                swap(a[i], a[rev[i] >> shift]);
            }
        }
        for(int k = 1; k < n; k <<= 1) {
            for(int i = 0; i < n; i += 2 * k) {
                for(int j = 0; j < k; j++) {
                    int z = mul(a[i + j + k], rts[j + k]);
                    a[i + j + k] = add(a[i + j], mod - z);
                    a[i + j] = add(a[i + j], z);
                }
            }
        }
    }

    vector<int> multiply(vector<int> a, vector<int> b) {
        int need = a.size() + b.size() - 1;
        int nbase = 1;
        while((1 << nbase) < need)
            nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        a.resize(sz, 0);
        b.resize(sz, 0);
        ntt(a);
        ntt(b);
        int inv_sz = inverse(sz);
        for(int i = 0; i < sz; i++) {
            a[i] = mul(a[i], mul(b[i], inv_sz));
        }
        reverse(a.begin() + 1, a.end());
        ntt(a);
        a.resize(need);
        return a;
    }

    vector<int> pow(vector<int> a, int k) {
        if(k <= 1)
            return a;
        if(k & 1)
            return multiply(a, pow(a, k - 1));
        else
            return pow(multiply(a, a), k / 2);
    }
};
NumberTheoreticTransform<MOD> ntt;
return ntt.multiply(L,R);

//n乗するとながさは(a.size()-1)*k+1になる
//max_baseに引っ掛かったらmodの値をみかえそう！！（1e9+7だとエラー吐く）
//複数の多項式をかけあわせるときは分割統治！
    auto cal = [&](auto &&self, int l, int r) -> vec<int> {
        if(l + 1 == r) {
            vec<int> ret(a[l] + 1, 0);
            //ここらへんいい感じに書き換える
            ret[a[l]] = 1;
            ret[0] = 1;
            return ret;
        } else {
            int m = l + r >> 1;
            vec<int> L = self(self, l, m);
            vec<int> R = self(self, m, r);
            return ntt.multiply(L, R);
        }
    };
    vec<int> con = cal(cal, 0, n);

class CentroidDecomposition {
    Graph& g;
    ll n;
    vector<int> sz, dead;
public:
    CentroidDecomposition(Graph& _g) :g(_g),n(g.size()),sz(n, 1), dead(n, 0) {}

    ll szdfs(ll v, ll par) {
        sz[v] = 1;
        for (auto e : g[v])
            if (e.to != par && !dead[e.to]) sz[v] += szdfs(e.to, v);
        return sz[v];
    }

    void find(ll v, ll par, ll tmp, Array& cs) {
        ll ok = 1;
        for (auto e : g[v]) {
            if (e.to == par || dead[e.to]) continue;
            find(e.to, v, tmp, cs);
            ok &= (sz[e.to] <= tmp / 2);
        }
        ok &= (tmp - sz[v] <= tmp / 2);
        if (ok) cs.push_back(v);
    };

    vector<ll> build(ll r) {
        ll tmp = szdfs(r, -1);
        vector<ll> cs;
        find(r, -1, tmp, cs);
        return cs;
    }

    inline ll get_sz(ll v) { return sz[v]; }

    inline void disable(ll v) { dead[v] = 1; }

    inline void enable(ll v) { dead[v] = 0; }

    inline ll alive(ll v) { return !dead[v]; }
};

//HLD
class HLD {
  private:
    Graph &g;
    int n, root;
    vector<int> sz, par, in, out, head, rev, dep, edge_in;
    void dfs_sz(int v, int p) {
        sz[v] = 1, par[v] = p;
        for(auto &e : g[v]) {
            if(e.to == p)
                swap(e, g[v].back());
            if(e.to == p)
                continue;
            dep[e.to] = dep[v] + 1;
            dfs_sz(e.to, v);
            sz[v] += sz[e.to];
            if(sz[e.to] > sz[g[v][0].to])
                swap(e, g[v][0]);
        }
    }
    void dfs_hld(int v, int p, int &t) {
        in[v] = t++;
        rev[in[v]] = v;
        for(auto e : g[v]) {
            if(e.to == p) {
                edge_in[e.cap] = in[v];
                continue;
            }
            head[e.to] = (e.to == g[v][0].to ? head[v] : e.to);
            dfs_hld(e.to, v, t);
        }
        out[v] = t;
    }

  public:
    HLD(Graph &_g, int _root = 0)
        : g(_g), n(g.size()), root(_root), sz(n), par(n, -1), head(n, -1), in(n), out(n), rev(n), dep(n, 0), edge_in(n - 1, 0) {
        dfs_sz(root, -1);
        int t = 0;
        head[root] = root;
        dfs_hld(root, -1, t);
    }
    inline int get_in(int a) { return in[a]; }
    inline int get_sz(int a) { return sz[a]; }
    inline int get_rev(int a) { return rev[a]; }
    inline int get_dep(int a) { return dep[a]; }
    inline int get_edge_in(int a) { return edge_in[a]; }
    int lca(int a, int b) {
        for(;; b = par[head[b]]) {
            if(in[a] > in[b])
                swap(a, b);
            if(head[a] == head[b])
                return a;
        }
    }
    P subtree_query(int a) { return {in[a], out[a]}; }
    vector<P> path_query(int a, int b, bool edge = false) {
        vector<P> ret;
        for(;; b = par[head[b]]) {
            if(in[a] > in[b])
                swap(a, b);
            if(head[a] == head[b])
                break;
            ret.push_back({in[head[b]], in[b] + 1});
        }
        ret.push_back({in[a] + edge, in[b] + 1});
        return ret;
    }
};


class LCA {
public:
 const int n = 0;
 const int log2_n = 0;
 std::vector<std::vector<int>> parent;
 std::vector<int> depth;

 LCA() {}

 LCA(const Graph& g, int root)
  : n(g.size()), log2_n(log2(n) + 1), parent(log2_n, std::vector<int>(n)), depth(n) {
  dfs(g, root, -1, 0);
  for (int k = 0; k + 1 < log2_n; k++) {
   for (int v = 0; v < (int)g.size(); v++) {
    if (parent[k][v] < 0)
     parent[k + 1][v] = -1;
    else
     parent[k + 1][v] = parent[k][parent[k][v]];
   }
  }
 }

 void dfs(const Graph& g, int v, int p, int d) {
  parent[0][v] = p;
  depth[v] = d;
  for (auto& e : g[v]) {
   if (e.to != p) dfs(g, e.to, v, d + 1);
  }
 }

 int get_par(int u, int i){
    for (int k = log2_n - 1; k >= 0; k--) {
        if ((i>>k)&1) {
            u = parent[k][u];
        }
    }
    return u;
 }

 int get(int u, int v) {
  if (depth[u] > depth[v]) std::swap(u, v);
  for (int k = 0; k < log2_n; k++) {
   if ((depth[v] - depth[u]) >> k & 1) {
    v = parent[k][v];
   }
  }
  if (u == v) return u;
  for (int k = log2_n - 1; k >= 0; k--) {
   if (parent[k][u] != parent[k][v]) {
    u = parent[k][u];
    v = parent[k][v];
   }
  }
  return parent[0][u];
 }
};

template <typename F, class T>
class LCAEdge {
  public:
    const int n = 0;
    const int log2_n = 0;
    std::vector<std::vector<int>> parent;
    std::vector<std::vector<T>> data;
    std::vector<int> depth;
    F op;
    int id;

    LCAEdge() {}

    LCAEdge(const Graph &g, int root, T _id, F _op)
        : n(g.size()), log2_n(log2(n) + 1), parent(log2_n, std::vector<int>(n)), data(log2_n, std::vector<T>(n, _id)), op(_op), depth(n) {
        dfs(g, root, -1, 0);
        for(int k = 0; k + 1 < log2_n; k++) {
            for(int v = 0; v < (int)g.size(); v++) {
                if(parent[k][v] < 0)
                    parent[k + 1][v] = -1;
                else
                    parent[k + 1][v] = parent[k][parent[k][v]];
                data[k + 1][v] = op(data[k][parent[k][v]], data[k][v]);
            }
        }
    }

    void dfs(const Graph &g, int v, int p, int d) {
        parent[0][v] = p;
        depth[v] = d;
        for(auto &e : g[v]) {
            if(e.to == p)
                data[0][v] = e.cap; //辺の値を保存
            else
                dfs(g, e.to, v, d + 1);
        }
    }

    int get_par(int u, int i) {
        for(int k = log2_n - 1; k >= 0; k--) {
            if((i >> k) & 1) {
                u = parent[k][u];
            }
        }
        return u;
    }

    int get(int u, int v) {
        if(depth[u] > depth[v])
            std::swap(u, v);
        T ret = 0;
        for(int k = 0; k < log2_n; k++) {
            if((depth[v] - depth[u]) >> k & 1) {
                ret = op(ret, data[k][v]);
                v = parent[k][v];
            }
        }
        if(u == v)
            return ret;
        for(int k = log2_n - 1; k >= 0; k--) {
            if(parent[k][u] != parent[k][v]) {
                ret = op(ret, data[k][u]);
                u = parent[k][u];
                ret = op(ret, data[k][v]);
                v = parent[k][v];
            }
        }
        ret = op(ret, data[0][u]);
        ret = op(ret, data[0][v]);
        return ret;
    }
};


//最大独立集合　O(2^(N/2):半分全列挙)
ll MaximumIndependentSet(const Graph &g) {
    int n = g.size();
    ll all = (1LL << n) - 1;
    vec<ll> ok(n, all), ng(n, 0);
    REP(v, n) {
        for(auto e : g[v]) {
            ok[v] ^= (1LL << e.to);
            ng[v] |= (1LL << e.to);
        }
    }

    ll l = n / 2;
    ll r = n - l;

    ll left = (1LL << l);
    vec<ll> dp(left, 0);
    REP(bit, left) {
        bool flag = true;
        REP(i, l) {
            if((bit >> i) & 1) {
                chmax(dp[bit], dp[bit ^ (1LL << i)]);
                if(bit & ng[i])
                    flag = false;
            }
        }
        if(flag)
            chmax(dp[bit], (ll)__builtin_popcountll(bit));
    }

    ll right = (1LL << r);
    ll ret = 0;
    REP(bit, right) {
        bool flag = true;
        ll mask = left - 1;
        REP(i, r) {
            if((bit >> i) & 1) {
                mask &= ok[i + l];
                if(bit & (ng[i + l] >> l))
                    flag = false;
            }
        }
        if(flag)
            chmax(ret, dp[mask] + (ll)__builtin_popcountll(bit));
    }
    return ret;
}


int popcount3(int x) {
 x = (x & 0x55555555) + (x >> 1 & 0x55555555);
 x = (x & 0x33333333) + (x >> 2 & 0x33333333);
 x = (x & 0x0F0F0F0F) + (x >> 4 & 0x0F0F0F0F);
 x = (x & 0x00FF00FF) + (x >> 8 & 0x00FF00FF);
 x = (x & 0x0000FFFF) + (x >> 16 & 0x0000FFFF);

 return x;
}

// nibutan
   ll l = -1;
   ll r = n;
   ll k = n;
   while (r - l > 1) {
    ll mid = l + r >> 1;
    if (s.sum(0,mid+1)>=x) {
     k = mid;
     r = mid;
    }
    else {
     l = mid;
    }
   }
   if(k!=n) cout << k << endl;
}

//bit全探索
 for (ll bit = 0; bit < (1LL << n); bit++) {
  REP(i, n) {
   if ((bit & (1LL << i)) != 0) {
   //1�̂Ƃ��̏���
   }
  }
 }

//map�n�F4�ߖT
 https://atcoder.jp/contests/atc001/submissions/8269997

//��ԃX�P�W���[�����O�@https://atcoder.jp/contests/abc103/submissions/8271489



//BIT
//��Ԙa
int N;
int bit[1000010];
// v[a] += w
void add(int a, int w) {
 for (int x = a; x < N; x |= x + 1) {
  bit[x] += w;
 }
}
// v[0] + ... + v[a - 1]
// [0,a)�̋�Ԙa
int sum(int a) {
 int ret = 0;
 for (int x = a - 1; x >= 0; x = (x & (x + 1)) - 1) {
  ret += bi
}
 return ret;
}

//��ԃX�P�W���[�����O
sort(all(wh), [&](P x, P y) {
  if (x.first == y.first) return x.second > y.second;
  return x.first < y.first;
  });


  int main() {
 ios::sync_with_stdio(false);
 cin.tie(nullptr);
 cout.tie(nullptr);

 ll n;
 cin >> n;
 Array t(n, 0), v(n, 0);
 Array sum_t(n+1,0);
 REP(i, n) {
  cin >> t[i];
  sum_t[i+1] += t[i]+sum_t[i];
 }
 REP(i, n) cin >> v[i];

 Array vt(sum_t[n] + 1, 0);

 ll now = 0;
 ll i = 0;
 for (int time = 0; time <= sum_t[n];time++) {
  if (time > sum_t[i + 1]) i++;
  if (now < v[i]) now++;
  if (now > v[i])now = v[i];
  vt[time] = now;
 }

 ll now = 0;
 ll i = 0;
 for (int time = sum_t[n]; time >= 0; time--) {
  if (time <= sum_t[i + 1]) i--;
  if (now < v[i]) now++;
  if (now > v[i])now = v[i];
 }

 return 0;
}

//ローリングハッシュ
template<typename T>
struct RollingHash {
	static const int base1 = 1007, base2 = 2009;
	static const int mod1 = 1000000007, mod2 = 1000000009;
	vector<long long> hash1, hash2, power1, power2;
 
	// construct
	RollingHash(const T& S) {
		int n = (int)S.size();
		hash1.assign(n + 1, 0);
		hash2.assign(n + 1, 0);
		power1.assign(n + 1, 1);
		power2.assign(n + 1, 1);
		for (int i = 0; i < n; ++i) {
			hash1[i + 1] = (hash1[i] * base1 + S[i]) % mod1;
			hash2[i + 1] = (hash2[i] * base2 + S[i]) % mod2;
			power1[i + 1] = (power1[i] * base1) % mod1;
			power2[i + 1] = (power2[i] * base2) % mod2;
		}
	}
 
 
	// get hash of S[left:right)
	inline pair<long long, long long> get(int l, int r) const {
		long long res1 = hash1[r] - hash1[l] * power1[r - l] % mod1;
		if (res1 < 0) res1 += mod1;
		long long res2 = hash2[r] - hash2[l] * power2[r - l] % mod2;
		if (res2 < 0) res2 += mod2;
		return { res1, res2 };
	}
};

bool judgeIentersected(ll ax,ll ay,ll bx,ll by,ll cx,ll cy,ll dx,ll dy) {
	ll ta,tb,tc,td;
	ta = (cx - dx) * (ay - cy) + (cy - dy) * (cx - ax);
  	tb = (cx - dx) * (by - cy) + (cy - dy) * (cx - bx);
  	tc = (ax - bx) * (cy - ay) + (ay - by) * (ax - cx);
	td = (ax - bx) * (dy - ay) + (ay - by) * (ax - dx);

  return tc * td < 0 && ta * tb < 0;
};



//桁DP
constexpr int loose = 0;
constexpr int tight = 1;
ll digit_DP(string s) {
	ll n = s.size();
	vector<Matrix> dp(2, Matrix(n + 1, Array(2, 0)));
	dp[0][0][tight] = 1;
	REP(i, n) {
		ll d = s[i] - '0';
		ll ni = i + 1;
		REP(t, 2) {
			REP(j, 10) {
				if (t == tight && d < j) break;
				ll nt = ((t == tight) && (j == d))?tight:loose;
				
				dp[1][ni][nt] += dp[1][i][t];
 
				if (j == 4|| j == 9 ) {
					dp[1][ni][nt] += dp[0][i][t];
				}
				else {
					dp[0][ni][nt] += dp[0][i][t];
				}
			}
		}
	}
 
	return  dp[1][n][loose] + dp[1][n][tight];
}

部分集合 O(3^n)
for (int j = i; j >= 0; --j) {
	j &= i;
	long long cost = 0;
	for (int k = 0; k < N; ++k) {
		if (!((i >> k) & 1)) {
			cost += std::min({ xsel[j][k], ysel[i - j][k] });
		}
	}
	ans[cnt] = std::min(ans[cnt], cost);
}

cd "/Users/momohara/Desktop/dev/atcoder/" && g++ -std=c++17 AtCoder2.cpp -o AtCoder2 && "/Users/momohara/Desktop/dev/atcoder/"AtCoder2

    ll nr,ng,nb;
    cin>>nr>>ng>>nb;
    Array r(nr),g(ng),b(nb);
    REP(i,nr) cin>>r[i];
    REP(i,ng) cin>>g[i];
    REP(i,nb) cin>>b[i];
    if()



//三分探索（範囲を３分割して、狭めていく）
閉区間[lb,ub]についてかんがえる
	ll lb = 0;
	ll ub = h[n-1];
	while(lb+2<ub){
		ll t1=(lb*2+ub)/3;
		ll t2=(lb+ub*2)/3;
		if(check(t1) < check(t2)) ub=t2;
		else lb=t1;
	}
	ll ans=INF;
	rep(i,lb,ub+1) chmin(ans,check(i));
	cout<<ans<<en;

*/

//*memo
//output :fixed  << setprecision(n) <<小数点以下n桁出力
//setfill('0') << setw(12) <<num 0埋めで12文字出力

//*func
//lower_bound(all(a),x):vector aでx以上の要素のイテレータを返す
//upper_bound(all(a,x)):vector aでxより大きい要素のイテレータを返す
//setやmapの場合は専用関数あり
//next_permutation(all(r)) : 次の順列に並べ替える．次がない場合はfalseを返す．
//開始状態はソートした後
//https://atcoder.jp/contests/abc054/submissions/8271200
//priority_queue<P, vector<P>, greater<P>> que;

//*tips
//調和関数はlogn! n/1+n/2+n/3+...
//約数をみるときは√nまででよい
//鳩ノ巣
//ビット演算ではカッコを付ける

//問題を飛ばすときは順位表確認！！！！！
//1<<nはだめ！！1LL<<nでかく！！
//数え上げや最適化⇒DP?

//連続してる区間である条件を満たす最長の長さ
//順番に操作し,満たさない場合は左端を落とす（オーバーフロー注意

//ゲーム問題
//後ろから
//逆の操作を考える
//Grundy数　（mex高速化https://atcoder.jp/contests/arc038/submissions/9163705）

//グラフ
//頂点数＊状態数がおいしい
//l以下なら通れる→l以下のルートに1の辺を貼ったGraph再構築
//森:連結成分=頂点数-辺数

//floor(a / b) = (a - a % b) / b　周期性がある

/*
bit演算
・部分集合の列挙 O(3^n)
・部分集合の中の最大値とかをもとめる（or<=kの中での最大値とか）
  ->高速ゼータ変換(O(2^n*n)):今見てるbitの1を0にした場合の集合から遷移させる

XOR
XOR:A xor A=0
A xor B = C  ->  A = B xor C
A or B >= A xor B
ビットごとにみるといい
0と1で範囲分けしたいときは、ソートしてから再帰関数で範囲ごとに見ていく。
Ai+k xor X = Biになる配列->a(i+k) xor a(i+k+1) = bi xor b(i+1)になる数列範囲をみる

A xor B = A+B(繰り上げがない場合)
A + B = (A and B)*2 + (A xor B) //xorとandのビットは被らない

 mod2上の演算は上から貪欲！！！
 v>>i = n>>i + jと上からjで差分を上の桁から見ていく時、2以上はひっくり返らない->状態数を減らせる、差分は次の桁で＊２

sum = xor + and*2 = or + and
or = xor + and

*/

//桁和の桁和
//順序が関係ない
//周期は９

//平均がxになるとき:値-xの総和で考える、正負に分けて考える

//DP
//区間DP：
//包除原理：余事象をひきながら数えてく
//箱根：未確定の要素を持つDP
//同じサイズの集合を数えちゃう問題！ -> 基準点を決める（まだ使ってない最小の頂点とか）
//最大がxになる場合の和 -> x以下の場合の数ー(x-1)以下の場合の和(これで微分して確率もとめられたりもする)

/*数え上げ
x*y*zの総和は、各要素で1個ずつ選ぶ場合の数→まると仕切りで一緒に数えられる
たとえば、x+y+z=nなら　n+2C5
(x>1になるよう-3,x,y,zの区切り2つとと、どれを選ぶかの区切り3つ)
偶数にしたい区間がある時は2こまとめて考える→奇数にする区間の選択方法で場合分け

等確率で黒白選べる、ある２種類の操作が選べる→グリッドで考える

Aの数<=Bの数になるように選択する経路数
→カタラン数cn = 2nCn/(n+1)  : (0,0)->(n,n)へi>=jの道だけ通る経路数、　括弧列の組み合わせの数

N<=40
半分全列挙
・ナップザック
・同時に使えないペアを辺で表現

nCrのsumはn次式！
->ラグランジュ補間
*/

//CE:
//unixなどは使えない変数

//RE：範囲外参照
//配列の長さとりすぎ
//配列の長さ間違え（DPのときとか）
//入力の配列の長さ確認！
//無限ループ
//コーナーケース

//TLE：参照渡しになっていないとき
//map->unordered_map
//priority_queueは降順
//setは定数倍！！setは定数倍！！



//WA:制約見間違い
//n/xは切り捨てなのできをつける
//うまく代入できてないときは==かも
//範囲外参照(負の値の時未定義)
//オーバーフロー！！（REPマクロでのオーバーフロー注意）
//初期値の設定忘れ
//イテレーターの中身を見る前にendじゃないかしらべる

//*data structure
//tree:lognで挿入し，k番目の値を取得・kが何番目かを出力することができる
//https://xuzijian629.hatenablog.com/entry/2018/12/01/000010
//https://atcoder.jp/contests/arc033/submissions/3643481

//cd "/Users/momohara/Desktop/dev/atcoder/" && g++ AtCoder2.cpp -std=c++17 -o a && /Users/momohara/Desktop/dev/atcoder/a
//cd "/Users/momohara/Desktop/dev/atcoder/" && g++ b.cpp -std=c++17 -o a && /Users/momohara/Desktop/dev/atcoder/a