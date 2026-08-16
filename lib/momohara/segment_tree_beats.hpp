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

namespace RangeUpdateChgcdRangeMaxSum {

constexpr uint32_t BINF = 1 << 30;
struct S {
    uint32_t max; // 区間最大値
    uint32_t lcm; // min(BINF, (区間内全要素の最小公倍数))
    uint32_t sz;  // 区間要素数
    uint64_t sum; // 区間内全要素の総和
    bool fail;
    S() : max(0), lcm(1), sz(0), sum(0), fail(0) {}
    S(uint32_t x, uint32_t sz_ = 1) : max(x), lcm(x), sz(sz_), sum((uint64_t)x * sz_), fail(0) {}
};

S e() { return S(); }

S op(S l, S r) {
    if(r.sz == 0)
        return l;
    if(l.sz == 0)
        return r;
    S ret;
    ret.max = std::max(l.max, r.max);
    ret.sum = l.sum + r.sum;
    ret.lcm = std::min(uint64_t(BINF), (uint64_t)l.lcm * r.lcm / std::__gcd(l.lcm, r.lcm));
    ret.sz = l.sz + r.sz;
    return ret;
}

struct F {
    uint32_t dogcd, reset;
    F() : dogcd(0), reset(0) {}
    F(uint32_t g, uint32_t upd) : dogcd(g), reset(upd) {}
    static F gcd(uint32_t g) noexcept { return F(g, 0); }
    static F update(uint32_t a) noexcept { return F(0, a); }
};

F composition(F fnew, F fold) {
    if(fnew.reset)
        return F::update(fnew.reset);
    else if(fold.reset) {
        return F::update(std::__gcd(fnew.dogcd, fold.reset));
    } else {
        return F::gcd(std::__gcd(fnew.dogcd, fold.dogcd));
    }
}

F id() { return F(); }

S mapping(F f, S x) {
    if(x.fail)
        return x;
    if(f.reset)
        x = S(f.reset, x.sz);
    if(f.dogcd) {
        if(x.sz == 1) {
            x = S(std::__gcd(x.max, f.dogcd));
        } else if(x.lcm == BINF or f.dogcd % x.lcm) {
            // 区間 gcd クエリによって，複数個の要素からなる区間で
            // いずれかの値が変更を受ける場合のみ計算失敗
            x.fail = true;
        }
    }
    return x;
}
using SegmentTree = SegmentTreeBeats<S, op, e, F, mapping, composition, id>;
} // namespace RangeUpdateChgcdRangeMaxSum

namespace RangeBitwiseAndOrRangeMax {
using UINT = uint32_t;
constexpr UINT digit = 20;
constexpr int mask = (1 << digit) - 1;

struct S {
    UINT max;   // 区間最大値
    UINT upper; // 区間内全要素の bitwise or
    UINT lower; // 区間内全要素の bitwise and
    bool fail;
    S(UINT x = 0) : max(x), upper(x), lower(x), fail(false) {}
};

S e() { return S(); }

S op(S l, S r) {
    l.max = std::max(l.max, r.max);
    l.upper |= r.upper;
    l.lower &= r.lower;
    return l;
}

struct F {
    UINT bit_and;
    UINT bit_or;
    F() : bit_and(mask), bit_or(0) {}
    F(UINT a, UINT o) : bit_and(a), bit_or(o) {}
    static F b_and(UINT a) noexcept { return {a, 0}; }
    static F b_or(UINT a) noexcept { return {mask, a}; }
};

F composition(F fnew, F fold) {
    return F{fnew.bit_and & fold.bit_and, fnew.bit_or | (fnew.bit_and & fold.bit_or)};
}

F id() { return F(); }

S mapping(F f, S x) {
    if((x.upper - x.lower) & (~f.bit_and | f.bit_or)) {
        // 区間内で立っている要素と立っていない要素が混在するような bit で
        // 変更が起きた場合のみ計算失敗（新たな最大値が不明なので）
        x.fail = true;
        return x;
    }
    x.upper = (x.upper & f.bit_and) | f.bit_or;
    x.lower = (x.lower & f.bit_and) | f.bit_or;
    x.max = (x.max & f.bit_and) | f.bit_or;
    return x;
}
using SegmentTree = SegmentTreeBeats<S, op, e, F, mapping, composition, id>;
} // namespace RangeBitwiseAndOrRangeMax

namespace RangeChMinMaxAddSum {
template <typename Num>
inline Num second_lowest(Num a, Num a2, Num c, Num c2) noexcept {
    // a < a2, c < c2 のとき全引数を昇順ソートして二番目の値
    return a == c ? std::min(a2, c2) : a2 <= c ? a2
                                   : c2 <= a   ? c2
                                               : std::max(a, c);
}
template <typename Num>
inline Num second_highest(Num a, Num a2, Num b, Num b2) noexcept {
    // a > a2, b > b2 のとき全引数を降順ソートして二番目の値
    return a == b ? std::max(a2, b2) : a2 >= b ? a2
                                   : b2 >= a   ? b2
                                               : std::min(a, b);
}

using BNum = long long;
constexpr BNum BINF = 1LL << 61;

struct S {
    BNum lo, hi, lo2, hi2, sum; // 区間最小・最大値，区間最小・最大から二番目の値，区間総和
    unsigned sz, nlo, nhi;      // 区間要素数，区間最小・最大値をとる要素の個数
    bool fail;
    S() : lo(BINF), hi(-BINF), lo2(BINF), hi2(-BINF), sum(0), sz(0), nlo(0), nhi(0), fail(0) {}
    S(BNum x, unsigned sz_ = 1)
        : lo(x), hi(x), lo2(BINF), hi2(-BINF), sum(x * sz_), sz(sz_), nlo(sz_), nhi(sz_), fail(0) {}
};

S e() { return S(); }

S op(S l, S r) {
    S ret;
    ret.lo = std::min(l.lo, r.lo), ret.hi = std::max(l.hi, r.hi);
    ret.lo2 = second_lowest(l.lo, l.lo2, r.lo, r.lo2);
    ret.hi2 = second_highest(l.hi, l.hi2, r.hi, r.hi2);
    ret.sum = l.sum + r.sum, ret.sz = l.sz + r.sz;
    ret.nlo = l.nlo * (l.lo <= r.lo) + r.nlo * (r.lo <= l.lo);
    ret.nhi = l.nhi * (l.hi >= r.hi) + r.nhi * (r.hi >= l.hi);
    return ret;
}

struct F {
    BNum lb, ub, bias;
    F(BNum chmax_ = -BINF, BNum chmin_ = BINF, BNum add = 0) : lb(chmax_), ub(chmin_), bias(add) {}
    static F chmin(BNum x) noexcept { return F(-BINF, x, BNum(0)); }
    static F chmax(BNum x) noexcept { return F(x, BINF, BNum(0)); }
    static F add(BNum x) noexcept { return F(-BINF, BINF, x); };
};

F composition(F fnew, F fold) {
    F ret;
    ret.lb = std::max(std::min(fold.lb + fold.bias, fnew.ub), fnew.lb) - fold.bias;
    ret.ub = std::min(std::max(fold.ub + fold.bias, fnew.lb), fnew.ub) - fold.bias;
    ret.bias = fold.bias + fnew.bias;
    return ret;
}

F id() { return F(); }

S mapping(F f, S x) {
    if(x.sz == 0)
        return e();
    else if(x.lo == x.hi or f.lb == f.ub or f.lb >= x.hi or f.ub <= x.lo) {
        return S(std::min(std::max(x.lo, f.lb), f.ub) + f.bias, x.sz);
    } else if(x.lo2 == x.hi) {
        x.lo = x.hi2 = std::max(x.lo, f.lb) + f.bias;
        x.hi = x.lo2 = std::min(x.hi, f.ub) + f.bias;
        x.sum = x.lo * x.nlo + x.hi * x.nhi;
        return x;
    } else if(f.lb < x.lo2 and f.ub > x.hi2) {
        BNum nxt_lo = std::max(x.lo, f.lb), nxt_hi = std::min(x.hi, f.ub);
        x.sum += (nxt_lo - x.lo) * x.nlo - (x.hi - nxt_hi) * x.nhi + f.bias * x.sz;
        x.lo = nxt_lo + f.bias, x.hi = nxt_hi + f.bias, x.lo2 += f.bias, x.hi2 += f.bias;
        return x;
    }
    x.fail = 1;
    return x;
}
using SegmentTree = SegmentTreeBeats<S, op, e, F, mapping, composition, id>;
} // namespace RangeChMinMaxAddSum

// https://rsm9.hatenablog.com/entry/2021/02/01/220408#Library-Checker-Range-Chmin-Chmax-Add-Range-Sum