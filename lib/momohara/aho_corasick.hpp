#pragma once

#include <array>
#include <queue>
#include <string>
#include <vector>


class AhoCorasick {
    std::vector<std::array<int, 26>> next_;
    std::vector<int> fail_;
    std::vector<int> terminal_count_;
    int size_;

  public:
    explicit AhoCorasick(int max_nodes)
        : next_(max_nodes), fail_(max_nodes, 0), terminal_count_(max_nodes, 0), size_(1) {
        for(auto &row : next_)
            row.fill(-1);
    }

    void insert(const std::string &s) {
        int node = 0;
        for(char c : s) {
            int idx = c - 'a';
            if(next_[node][idx] == -1)
                next_[node][idx] = size_++;
            node = next_[node][idx];
        }
        ++terminal_count_[node];
    }

    void build() {
        std::queue<int> que;
        fail_[0] = 0;
        for(int i = 0; i < 26; ++i) {
            if(next_[0][i] == -1) {
                next_[0][i] = 0;
            } else {
                fail_[next_[0][i]] = 0;
                que.push(next_[0][i]);
            }
        }
        while(!que.empty()) {
            int node = que.front();
            que.pop();
            for(int i = 0; i < 26; ++i) {
                if(next_[node][i] == -1) {
                    next_[node][i] = next_[fail_[node]][i];
                } else {
                    fail_[next_[node][i]] = next_[fail_[node]][i];
                    que.push(next_[node][i]);
                }
            }
        }
    }

    long long query_distinct_matches(const std::string &s) const {
        std::vector<int> seen = terminal_count_;
        int node = 0;
        long long cnt = 0;
        for(char c : s) {
            node = next_[node][c - 'a'];
            int cur = node;
            while(cur != 0 && seen[cur] != -1) {
                cnt += seen[cur];
                seen[cur] = -1;
                cur = fail_[cur];
            }
        }
        return cnt;
    }
};

