#pragma once

#include <algorithm>

namespace momohara {

template <typename T, const T id>
struct LiChaoTree {
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
        Node *lhs;
        Node *rhs;
        explicit Node(Line l) : l(l), lhs(nullptr), rhs(nullptr) {}
        ~Node() {
            delete lhs;
            delete rhs;
        }
    };

    T mi;
    T ma;
    Node *root = nullptr;

    LiChaoTree(T mi = 0, T ma = (1LL << 30)) : mi(mi), ma(ma) {}

    ~LiChaoTree() {
        delete root;
    }

    Node *insert(Node *p, T lb, T ub, Line &l) {
        if(!p)
            return new Node(l);
        if(p->l.get(lb) >= l.get(lb) && p->l.get(ub) >= l.get(ub))
            return p;
        if(p->l.get(lb) <= l.get(lb) && p->l.get(ub) <= l.get(ub)) {
            p->l = l;
            return p;
        }
        T mid = (lb + ub) / 2;
        if(p->l.get(mid) < l.get(mid))
            std::swap(p->l, l);
        if(p->l.get(lb) <= l.get(lb)) {
            p->lhs = insert(p->lhs, lb, mid, l);
        } else {
            p->rhs = insert(p->rhs, mid, ub, l);
        }
        return p;
    }

    void insert(T a, T b) {
        Line l(a, b);
        root = insert(root, mi, ma, l);
    }

    void insert(Line l) {
        root = insert(root, mi, ma, l);
    }

    T query(Node *p, T lb, T ub, T t) const {
        if(!p)
            return id;
        T mid = (lb + ub) / 2;
        if(t <= mid)
            return std::max(p->l.get(t), query(p->lhs, lb, mid, t));
        return std::max(p->l.get(t), query(p->rhs, mid, ub, t));
    }

    T query(T x) const {
        return query(root, mi, ma, x);
    }
};

} // namespace momohara
