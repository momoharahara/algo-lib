#pragma once

#include <cassert>
#include <utility>

namespace momohara {

template <typename U = unsigned, int B = 32>
class lazy_binary_trie {
    struct node {
        int cnt;
        U lazy;
        node *ch[2];
        node() : cnt(0), lazy(0), ch{nullptr, nullptr} {}
    };

    node *root_;

    void push(node *t, int b) {
        if((t->lazy >> static_cast<U>(b)) & static_cast<U>(1))
            std::swap(t->ch[0], t->ch[1]);
        if(t->ch[0])
            t->ch[0]->lazy ^= t->lazy;
        if(t->ch[1])
            t->ch[1]->lazy ^= t->lazy;
        t->lazy = 0;
    }

    node *add(node *t, U val, int b = B - 1) {
        if(!t)
            t = new node;
        ++t->cnt;
        if(b < 0)
            return t;
        push(t, b);
        bool f = (val >> static_cast<U>(b)) & static_cast<U>(1);
        t->ch[f] = add(t->ch[f], val, b - 1);
        return t;
    }

    node *sub(node *t, U val, int b = B - 1) {
        assert(t);
        --t->cnt;
        if(t->cnt == 0)
            return nullptr;
        if(b < 0)
            return t;
        push(t, b);
        bool f = (val >> static_cast<U>(b)) & static_cast<U>(1);
        t->ch[f] = sub(t->ch[f], val, b - 1);
        return t;
    }

    U get_min(node *t, U val, int b = B - 1) {
        assert(t);
        if(b < 0)
            return 0;
        push(t, b);
        bool f = (val >> static_cast<U>(b)) & static_cast<U>(1);
        f ^= !t->ch[f];
        return get_min(t->ch[f], val, b - 1) | (static_cast<U>(f) << static_cast<U>(b));
    }

    U get(node *t, int k, int b = B - 1) {
        if(b < 0)
            return 0;
        push(t, b);
        int m = t->ch[0] ? t->ch[0]->cnt : 0;
        if(k < m)
            return get(t->ch[0], k, b - 1);
        return get(t->ch[1], k - m, b - 1) | (static_cast<U>(1) << static_cast<U>(b));
    }

    int count_lower(node *t, U val, int b = B - 1) {
        if(!t || b < 0)
            return 0;
        push(t, b);
        bool f = (val >> static_cast<U>(b)) & static_cast<U>(1);
        return (f && t->ch[0] ? t->ch[0]->cnt : 0) + count_lower(t->ch[f], val, b - 1);
    }

  public:
    lazy_binary_trie() : root_(nullptr) {}

    int size() const {
        return root_ ? root_->cnt : 0;
    }

    bool empty() const {
        return !root_;
    }

    void insert(U val) {
        root_ = add(root_, val);
    }

    void erase(U val) {
        root_ = sub(root_, val);
    }

    void xor_all(U val) {
        if(root_)
            root_->lazy ^= val;
    }

    U max_element(U bias = 0) {
        return get_min(root_, ~bias);
    }

    U min_element(U bias = 0) {
        return get_min(root_, bias);
    }

    int lower_bound(U val) {
        return count_lower(root_, val);
    }

    int upper_bound(U val) {
        return count_lower(root_, val + 1);
    }

    U operator[](int k) {
        assert(0 <= k && k < size());
        return get(root_, k);
    }

    int count(U val) {
        if(!root_)
            return 0;
        node *t = root_;
        for(int i = B - 1; i >= 0; --i) {
            push(t, i);
            t = t->ch[(val >> static_cast<U>(i)) & static_cast<U>(1)];
            if(!t)
                return 0;
        }
        return t->cnt;
    }
};

} // namespace momohara
