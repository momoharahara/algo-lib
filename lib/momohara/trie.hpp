#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>


class Trie {
    std::vector<std::array<int, 26>> next_;
    std::vector<int> terminal_count_;
    std::vector<int> subtree_count_;
    std::vector<int> depth_;
    int size_;

    std::pair<int, int> compress_dfs(int node) {
        int child_count = 0;
        std::pair<int, int> ret = {terminal_count_[node], node};
        for(int i = 0; i < 26; ++i) {
            if(next_[node][i] == -1)
                continue;
            auto child = compress_dfs(next_[node][i]);
            ret.first += child.first;
            next_[node][i] = child.second;
            ++child_count;
        }
        subtree_count_[node] = ret.first;
        if(terminal_count_[node] == 0 && child_count == 1) {
            for(int i = 0; i < 26; ++i) {
                if(next_[node][i] != -1) {
                    ret.second = next_[node][i];
                    break;
                }
            }
        }
        return ret;
    }

  public:
    explicit Trie(int max_nodes)
        : next_(max_nodes),
          terminal_count_(max_nodes, 0),
          subtree_count_(max_nodes, 0),
          depth_(max_nodes, 0),
          size_(1) {
        for(auto &row : next_)
            row.fill(-1);
    }

    void insert(const std::string &s) {
        int node = 0;
        for(char c : s) {
            int idx = c - 'a';
            if(next_[node][idx] == -1) {
                next_[node][idx] = size_;
                depth_[size_] = depth_[node] + 1;
                ++size_;
            }
            node = next_[node][idx];
        }
        ++terminal_count_[node];
    }

    void compress() {
        compress_dfs(0);
    }

    int solve(const std::string &s, const std::string &order, int node = 0) const {
        int pos = depth_[node];
        if(pos == static_cast<int>(s.size()))
            return 1;
        int ret = terminal_count_[node];
        for(int i = 0; i < 26; ++i) {
            if(s[pos] == order[i]) {
                ret += solve(s, order, next_[node][order[i] - 'a']);
                break;
            }
            int to = next_[node][order[i] - 'a'];
            if(to == -1)
                continue;
            ret += subtree_count_[to];
        }
        return ret;
    }
};

