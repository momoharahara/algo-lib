#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

namespace momohara {

template <typename T, const T id>
class convex_hull_trick {
    struct line {
        T a;
        T b;
        line(T a = 0, T b = 0) : a(a), b(b) {}
        T get(T x) const {
            return a * x + b;
        }
    };

    struct node {
        line l;
        node *lch;
        node *rch;
        explicit node(line l) : l(l), lch(nullptr), rch(nullptr) {}
        ~node() {
            delete lch;
            delete rch;
        }
    };

    int n_;
    std::vector<T> pos_;
    node *root_;

    node *modify(node *p, int lb, int ub, line &l) {
        if(!p)
            return new node(l);
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

    T sub(node *p, int lb, int ub, int t) const {
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
    explicit convex_hull_trick(const std::vector<T> &pos) : n_(static_cast<int>(pos.size())), pos_(pos), root_(nullptr) {}

    ~convex_hull_trick() {
        delete root_;
    }

    void insert(T a, T b) {
        line l(a, b);
        root_ = modify(root_, 0, n_ - 1, l);
    }

    T get(T x) const {
        int t = static_cast<int>(std::lower_bound(pos_.begin(), pos_.end(), x) - pos_.begin());
        assert(t < n_ && pos_[t] == x);
        return sub(root_, 0, n_ - 1, t);
    }
};

} // namespace momohara
