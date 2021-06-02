#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#define REP(i, m, n) for(int i = (int)(m); i < (int)(n); ++i)
#define rep(i, n) REP(i, 0, n)
using ll = long long;

vector<int> z_algo(string s) {
    vector<int> ret(s.size());
    ret[0] = s.size();
    int i = 1, j = 0;
    while(i < s.size()) {
        while(i + j < s.size() && s[j] == s[i + j])
            ++j;
        ret[i] = j;
        if(j == 0) {
            ++i;
            continue;
        }
        int k = 1;
        while(i + k < s.size() && k + ret[k] < j)
            ret[i + k] = ret[k], ++k;
        i += k;
        j -= k;
    }
    return ret;
}

string t;
int calc(string &s) {
    vector<int> v = z_algo(t + s);
    int n = s.size(), m = t.size();
    int ret = 0;
    REP(i, m, m + n) {
        ret += v[i] >= m;
    }
    return ret;
}

int calc_over(string &s1, string &s2) {
    vector<int> v = z_algo(t + s1 + s2);
    int n1 = s1.size(), n2 = s2.size(), m = t.size();
    string u = s1 + s2;
    int ret = 0;
    for(int i = max(m, n1 + 1); i <= n1 + n2 && i < m + n1; i++) {
        ret += v[i] >= m;
    }
    return ret;
}

const int max_m = 3000;
struct genome {
    ll sz, num;
    string pre, suf;
    genome(string s) : sz(s.size()), num(calc(s)) {
        if(sz < 2 * max_m) {
            pre = s;
            suf = "";
        } else {
            pre = s.substr(0, 3000);
            suf = s.substr(s.size() - 3000);
        }
    }
    genome(string pre, string suf, ll sz, ll num) : pre(pre), suf(suf), sz(sz), num(num) {}
    genome operator+(genome &rhs) {
        if(sz < 2 * max_m && rhs.sz < 2 * max_m) {
            return genome(pre + rhs.pre);
        } else if(rhs.sz < 2 * max_m) {
            ll new_num = num + rhs.num + calc_over(suf, rhs.pre);
            string new_str = suf + rhs.pre;
            return genome(pre, new_str.substr(new_str.size() - 3000), sz + rhs.sz, new_num);
        } else if(sz < 2 * max_m) {
            ll new_num = num + rhs.num + calc_over(pre, rhs.pre);
            string new_str = pre + rhs.pre;
            return genome(new_str.substr(0, 3000), rhs.suf, sz + rhs.sz, new_num);
        } else {
            ll new_num = num + rhs.num + calc_over(suf, rhs.pre);
            return genome(pre, rhs.suf, sz + rhs.sz, new_num);
        }
    }
    genome operator*(ll num) {
        genome ret = genome("");
        genome cur = *this;
        while(num) {
            if(num & 1)
                ret = ret + cur;
            cur = cur + cur;
            num >>= 1;
        }
        return ret;
    }
};

ll parsell(string &s, int &i) {
    if(s[i] < '0' || '9' < s[i]) {
        return 1;
    }
    ll ret = 0;
    while('0' <= s[i] && s[i] <= '9') {
        ret = 10 * ret + s[i] - '0';
        ++i;
    }
    return ret;
}

genome term(string &s, int &i);
genome expr(string &s, int &i) {
    genome ret("");
    while(i < s.size() && s[i] != ')') {
        genome rhs = term(s, i);
        ret = ret + rhs;
    }
    return ret;
}

genome term(string &s, int &i) {
    ll cur = parsell(s, i);
    if(s[i] == '(') {
        ++i;
        genome res = expr(s, i);
        ++i;
        return res * cur;
    } else {
        genome res(s.substr(i, 1));
        ++i;
        return res * cur;
    }
}
void solve(string &s) {
    cin >> t;
    int i = 0;
    cout << expr(s, i).num << endl;
}
int main() {
    string s;
    while(cin >> s, s != "#") {
        solve(s);
    }
    return 0;
}
/*
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#define REP(i, m, n) for(int i = (int)(m); i < (int)(n); ++i)
#define rep(i, n) REP(i, 0, n)
using ll = long long;

const int max_m = 1000001;
struct genome {
    string s;
    genome(string _s) {
        s = _s;
        if(s.size() > max_m)
            s.resize(max_m);
    }
    genome operator+(genome &rhs) {
        return genome(s + rhs.s);
    }
    genome operator*(ll num) {
        string cur = "";
        rep(_, num) {
            if(cur.size() > max_m)
                break;
            cur += s;
        }
        return genome(cur);
    }
};

ll parsell(string &s, int &i) {
    if(s[i] < '0' || '9' < s[i]) {
        return 1;
    }
    ll ret = 0;
    while('0' <= s[i] && s[i] <= '9') {
        ret = 10 * ret + s[i] - '0';
        ++i;
    }
    return ret;
}

genome term(string &s, int &i);
genome expr(string &s, int &i) {
    genome ret("");
    while(i < s.size() && s[i] != ')') {
        genome rhs = term(s, i);
        ret = ret + rhs;
    }
    return ret;
}

genome term(string &s, int &i) {
    ll cur = parsell(s, i);
    if(s[i] == '(') {
        ++i;
        genome res = expr(s, i);
        ++i;
        return res * cur;
    } else {
        genome res(s.substr(i, 1));
        ++i;
        return res * cur;
    }
}
void solve(string &s) {
    int idx;
    cin >> idx;
    int i = 0;
    string ret = expr(s, i).s;
    cout << (ret.size() <= idx ? '0' : ret[idx]) << endl;
}
int main() {
    string s;
    while(cin >> s, s != "0") {
        solve(s);
    }
    return 0;
}

*/