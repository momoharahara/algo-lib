#pragma once

#include <algorithm>
#include <cassert>
#include <vector>


template <typename T, const T id>
class ConvexHullTrick {
    struct Line {
        T a;
        T b;
        Line(T a = 0, T b = 0) : a(a), b(b) {}
        T get(T x) const {
            return a * x + b;
        }
    };

    struct Node {
        Line l;
        Node *lch;
        Node *rch;
        explicit Node(Line l) : l(l), lch(nullptr), rch(nullptr) {}
        ~Node() {
            delete lch;
            delete rch;
        }
    };

    int n_;
    std::vector<T> pos_;
    Node *root_;

    Node *modify(Node *p, int lb, int ub, Line &l) {
        if(!p)
            return new Node(l);
        if(p->l.get(pos_[lb]) >= l.get(pos_[lb]) && p->l.get(pos_[ub]) >= l.get(pos_[ub]))
            return p;
        if(p->l.get(pos_[lb]) <= l.get(pos_[lb]) && p->l.get(pos_[ub]) <= l.get(pos_[ub])) {
            p->l = l;
            return p;
        }
        int c = (lb + ub) / 2;
        if(p->l.get(pos_[c]) < l.get(pos_[c]))
            std::swap(p->l, l);
        if(p->l.get(pos_[lb]) <= l.get(pos_[lb])) {
            p->lch = modify(p->lch, lb, c, l);
        } else {
            p->rch = modify(p->rch, c + 1, ub, l);
        }
        return p;
    }

    T sub(Node *p, int lb, int ub, int t) const {
        if(!p)
            return id;
        if(ub - lb == 0)
            return p->l.get(pos_[t]);
        int c = (lb + ub) / 2;
        if(t <= c)
            return std::max(p->l.get(pos_[t]), sub(p->lch, lb, c, t));
        return std::max(p->l.get(pos_[t]), sub(p->rch, c + 1, ub, t));
    }

  public:
    explicit ConvexHullTrick(const std::vector<T> &pos) : n_(static_cast<int>(pos.size())), pos_(pos), root_(nullptr) {}

    ~ConvexHullTrick() {
        delete root_;
    }

    void insert(T a, T b) {
        Line l(a, b);
        root_ = modify(root_, 0, n_ - 1, l);
    }

    T get(T x) const {
        int t = static_cast<int>(std::lower_bound(pos_.begin(), pos_.end(), x) - pos_.begin());
        assert(t < n_ && pos_[t] == x);
        return sub(root_, 0, n_ - 1, t);
    }
};

