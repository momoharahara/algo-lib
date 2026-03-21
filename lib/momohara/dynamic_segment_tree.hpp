#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>


template <class S, S (*op)(S, S), S (*e)()>
class DynamicSegmentTree {
    struct Node;
    using NodePtr = std::unique_ptr<Node>;

    struct Node {
        std::size_t index;
        S value;
        S product;
        NodePtr left;
        NodePtr right;

        Node(std::size_t index, S value)
            : index(index), value(value), product(value), left(nullptr), right(nullptr) {}

        void update() {
            product = op(op(left ? left->product : e(), value), right ? right->product : e());
        }
    };

    const std::size_t n_;
    NodePtr root_;

  public:
    explicit DynamicSegmentTree(std::size_t n) : n_(n), root_(nullptr) {}

    void set(std::size_t p, S x) {
        assert(p < n_);
        set(root_, 0, n_, p, x);
    }

    S get(std::size_t p) const {
        assert(p < n_);
        return get(root_, 0, n_, p);
    }

    S prod(std::size_t l, std::size_t r) const {
        assert(l <= r && r <= n_);
        return prod(root_, 0, n_, l, r);
    }

    S all_prod() const {
        return root_ ? root_->product : e();
    }

    void reset(std::size_t l, std::size_t r) {
        assert(l <= r && r <= n_);
        reset(root_, 0, n_, l, r);
    }

    template <bool (*f)(S)>
    std::size_t max_right(std::size_t l) const {
        return max_right(l, [](S x) { return f(x); });
    }

    template <class F>
    std::size_t max_right(std::size_t l, const F &f) const {
        assert(l <= n_);
        S product = e();
        assert(f(product));
        return max_right(root_, 0, n_, l, f, product);
    }

    template <bool (*f)(S)>
    std::size_t min_left(std::size_t r) const {
        return min_left(r, [](S x) { return f(x); });
    }

    template <class F>
    std::size_t min_left(std::size_t r, const F &f) const {
        assert(r <= n_);
        S product = e();
        assert(f(product));
        return min_left(root_, 0, n_, r, f, product);
    }

  private:
    void set(NodePtr &t, std::size_t a, std::size_t b, std::size_t p, S x) const {
        if(!t) {
            t = std::make_unique<Node>(p, x);
            return;
        }
        if(t->index == p) {
            t->value = x;
            t->update();
            return;
        }
        std::size_t c = (a + b) >> 1;
        if(p < c) {
            if(t->index < p) {
                std::swap(t->index, p);
                std::swap(t->value, x);
            }
            set(t->left, a, c, p, x);
        } else {
            if(p < t->index) {
                std::swap(p, t->index);
                std::swap(x, t->value);
            }
            set(t->right, c, b, p, x);
        }
        t->update();
    }

    S get(const NodePtr &t, std::size_t a, std::size_t b, std::size_t p) const {
        if(!t)
            return e();
        if(t->index == p)
            return t->value;
        std::size_t c = (a + b) >> 1;
        if(p < c)
            return get(t->left, a, c, p);
        return get(t->right, c, b, p);
    }

    S prod(const NodePtr &t, std::size_t a, std::size_t b, std::size_t l, std::size_t r) const {
        if(!t || b <= l || r <= a)
            return e();
        if(l <= a && b <= r)
            return t->product;
        std::size_t c = (a + b) >> 1;
        S result = prod(t->left, a, c, l, r);
        if(l <= t->index && t->index < r)
            result = op(result, t->value);
        return op(result, prod(t->right, c, b, l, r));
    }

    void reset(NodePtr &t, std::size_t a, std::size_t b, std::size_t l, std::size_t r) const {
        if(!t || b <= l || r <= a)
            return;
        if(l <= a && b <= r) {
            t.reset();
            return;
        }
        std::size_t c = (a + b) >> 1;
        reset(t->left, a, c, l, r);
        reset(t->right, c, b, l, r);
        t->update();
    }

    template <class F>
    std::size_t max_right(const NodePtr &t, std::size_t a, std::size_t b, std::size_t l, const F &f, S &product) const {
        if(!t || b <= l)
            return n_;
        if(f(op(product, t->product))) {
            product = op(product, t->product);
            return n_;
        }
        std::size_t c = (a + b) >> 1;
        std::size_t result = max_right(t->left, a, c, l, f, product);
        if(result != n_)
            return result;
        if(l <= t->index) {
            product = op(product, t->value);
            if(!f(product))
                return t->index;
        }
        return max_right(t->right, c, b, l, f, product);
    }

    template <class F>
    std::size_t min_left(const NodePtr &t, std::size_t a, std::size_t b, std::size_t r, const F &f, S &product) const {
        if(!t || r <= a)
            return 0;
        if(f(op(t->product, product))) {
            product = op(t->product, product);
            return 0;
        }
        std::size_t c = (a + b) >> 1;
        std::size_t result = min_left(t->right, c, b, r, f, product);
        if(result != 0)
            return result;
        if(t->index < r) {
            product = op(t->value, product);
            if(!f(product))
                return t->index + 1;
        }
        return min_left(t->left, a, c, r, f, product);
    }
};

