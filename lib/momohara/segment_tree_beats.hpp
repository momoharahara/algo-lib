#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>


inline int ceil_pow2(int n) {
    int x = 0;
    while((1U << x) < static_cast<unsigned int>(n))
        ++x;
    return x;
}

inline int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct SegmentTreeBeats {
  public:
    SegmentTreeBeats() : SegmentTreeBeats(0) {}
    explicit SegmentTreeBeats(int n) : SegmentTreeBeats(std::vector<S>(n, e())) {}
    explicit SegmentTreeBeats(const std::vector<S> &v) : _n(static_cast<int>(v.size())) {
        log = ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        lz = std::vector<F>(size, id());
        for(int i = 0; i < _n; ++i)
            d[size + i] = v[i];
        for(int i = size - 1; i >= 1; --i)
            update(i);
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        for(int i = log; i >= 1; --i)
            push(p >> i);
        d[p] = x;
        for(int i = 1; i <= log; ++i)
            update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        for(int i = log; i >= 1; --i)
            push(p >> i);
        return d[p];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        if(l == r)
            return e();

        l += size;
        r += size;

        for(int i = log; i >= 1; --i) {
            if(((l >> i) << i) != l)
                push(l >> i);
            if(((r >> i) << i) != r)
                push(r >> i);
        }

        S sml = e(), smr = e();
        while(l < r) {
            if(l & 1)
                sml = op(sml, d[l++]);
            if(r & 1)
                smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for(int i = log; i >= 1; --i)
            push(p >> i);
        d[p] = mapping(f, d[p]);
        for(int i = 1; i <= log; ++i)
            update(p >> i);
    }

    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if(l == r)
            return;

        l += size;
        r += size;

        for(int i = log; i >= 1; --i) {
            if(((l >> i) << i) != l)
                push(l >> i);
            if(((r >> i) << i) != r)
                push((r - 1) >> i);
        }

        int l2 = l, r2 = r;
        while(l < r) {
            if(l & 1)
                all_apply(l++, f);
            if(r & 1)
                all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
        l = l2;
        r = r2;

        for(int i = 1; i <= log; ++i) {
            if(((l >> i) << i) != l)
                update(l >> i);
            if(((r >> i) << i) != r)
                update((r - 1) >> i);
        }
    }

    template <bool (*g)(S)>
    int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }

    template <class G>
    int max_right(int l, G g) {
        assert(0 <= l && l <= _n);
        assert(g(e()));
        if(l == _n)
            return _n;
        l += size;
        for(int i = log; i >= 1; --i)
            push(l >> i);
        S sm = e();
        do {
            while(l % 2 == 0)
                l >>= 1;
            if(!g(op(sm, d[l]))) {
                while(l < size) {
                    push(l);
                    l = 2 * l;
                    if(g(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        ++l;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            ++l;
        } while((l & -l) != l);
        return _n;
    }

    template <bool (*g)(S)>
    int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }

    template <class G>
    int min_left(int r, G g) {
        assert(0 <= r && r <= _n);
        assert(g(e()));
        if(r == 0)
            return 0;
        r += size;
        for(int i = log; i >= 1; --i)
            push((r - 1) >> i);
        S sm = e();
        do {
            --r;
            while(r > 1 && (r % 2))
                r >>= 1;
            if(!g(op(d[r], sm))) {
                while(r < size) {
                    push(r);
                    r = 2 * r + 1;
                    if(g(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        --r;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if(k < size) {
            lz[k] = composition(f, lz[k]);
            if(d[k].fail)
                push(k), update(k);
        }
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
};

namespace RangeUpdateDivideRangeSum {

constexpr uint32_t BINF = 1U << 30;

struct S {
    uint32_t mi;
    uint32_t ma;
    uint64_t sum;
    uint32_t sz;
    bool fail;
    S() : mi(0), ma(0), sum(0), sz(0), fail(false) {}
    S(uint32_t x, uint32_t sz = 1) : mi(x), ma(x), sum(static_cast<uint64_t>(x) * sz), sz(sz), fail(false) {}
};

inline S e() { return S(); }

inline S op(S l, S r) {
    if(r.sz == 0)
        return l;
    if(l.sz == 0)
        return r;
    S ret;
    ret.mi = std::min(l.mi, r.mi);
    ret.ma = std::max(l.ma, r.ma);
    ret.sum = l.sum + r.sum;
    ret.sz = l.sz + r.sz;
    return ret;
}

struct F {
    uint32_t upd;
    uint32_t inv;
    F() : upd(static_cast<uint32_t>(-1)), inv(0) {}
    F(uint32_t upd, uint32_t inv) : upd(upd), inv(inv) {}
    static F update(uint32_t upd) noexcept { return F(upd, 0); }
    static F divide(uint32_t inv) noexcept { return F(static_cast<uint32_t>(-1), inv); }
};

inline F composition(F fnew, F fold) {
    if(fnew.upd != static_cast<uint32_t>(-1)) {
        return F::update(fnew.upd);
    } else if(fnew.inv != 0) {
        if(fold.upd != static_cast<uint32_t>(-1)) {
            return F::update(fold.upd / fnew.inv);
        } else if(fold.inv != 0) {
            return F::divide(std::min<long long>(1LL * fold.inv * fnew.inv, BINF));
        } else {
            return fnew;
        }
    }
    return fold;
}

inline F id() { return F(); }

inline S mapping(F f, S x) {
    if(f.upd != static_cast<uint32_t>(-1)) {
        x = S(f.upd, x.sz);
    } else if(f.inv != 0) {
        if(x.ma / f.inv == x.mi / f.inv) {
            x = S(x.ma / f.inv, x.sz);
        } else {
            x.fail = true;
        }
    }
    return x;
}

using SegmentTree = SegmentTreeBeats<S, op, e, F, mapping, composition, id>;

} // namespace RangeUpdateDivideRangeSum

