#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

namespace momohara {

template <typename mint>
struct NTT {
    static constexpr uint32_t get_pr() {
        uint32_t mod = mint::mod();
        using u64 = uint64_t;
        u64 ds[32] = {};
        int idx = 0;
        u64 m = mod - 1;
        for(u64 i = 2; i * i <= m; ++i) {
            if(m % i == 0) {
                ds[idx++] = i;
                while(m % i == 0)
                    m /= i;
            }
        }
        if(m != 1)
            ds[idx++] = m;

        uint32_t pr = 2;
        while(true) {
            bool ok = true;
            for(int i = 0; i < idx; ++i) {
                u64 a = pr, b = (mod - 1) / ds[i], r = 1;
                while(b) {
                    if(b & 1)
                        r = r * a % mod;
                    a = a * a % mod;
                    b >>= 1;
                }
                if(r == 1) {
                    ok = false;
                    break;
                }
            }
            if(ok)
                return pr;
            ++pr;
        }
    }

    static constexpr uint32_t mod = mint::mod();
    static constexpr uint32_t pr = get_pr();
    static constexpr int level = __builtin_ctzll(mod - 1);
    mint dw[level], dy[level];

    void setwy(int k) {
        mint w[level], y[level];
        w[k - 1] = mint(pr).pow((mod - 1) / (1 << k));
        y[k - 1] = w[k - 1].inv();
        for(int i = k - 2; i > 0; --i)
            w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
        dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
        for(int i = 3; i < k; ++i) {
            dw[i] = dw[i - 1] * y[i - 2] * w[i];
            dy[i] = dy[i - 1] * w[i - 2] * y[i];
        }
    }

    NTT() { setwy(level); }

    void fft4(std::vector<mint> &a, int k) {
        if(static_cast<int>(a.size()) <= 1)
            return;
        if(k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        if(k & 1) {
            int v = 1 << (k - 1);
            for(int j = 0; j < v; ++j) {
                mint ajv = a[j + v];
                a[j + v] = a[j] - ajv;
                a[j] += ajv;
            }
        }
        int u = 1 << (2 + (k & 1));
        int v = 1 << (k - 2 - (k & 1));
        mint one = mint(1);
        mint imag = dw[1];
        while(v) {
            {
                int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v;
                for(; j0 < v; ++j0, ++j1, ++j2, ++j3) {
                    mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
                }
            }
            mint ww = one, xx = one * dw[2], wx = one;
            for(int jh = 4; jh < u;) {
                ww = xx * xx, wx = ww * xx;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for(; j0 < je; ++j0, ++j2) {
                    mint t0 = a[j0], t1 = a[j0 + v] * xx, t2 = a[j2] * ww, t3 = a[j2 + v] * wx;
                    mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j0 + v] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j2 + v] = t0m2 - t1m3;
                }
                xx *= dw[__builtin_ctzll((jh += 4))];
            }
            u <<= 2;
            v >>= 2;
        }
    }

    void ifft4(std::vector<mint> &a, int k) {
        if(static_cast<int>(a.size()) <= 1)
            return;
        if(k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        int u = 1 << (k - 2);
        int v = 1;
        mint one = mint(1);
        mint imag = dy[1];
        while(u) {
            {
                int j0 = 0, j1 = v, j2 = v + v, j3 = j2 + v;
                for(; j0 < v; ++j0, ++j1, ++j2, ++j3) {
                    mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                    a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                    a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
                }
            }
            mint ww = one, xx = one * dy[2], yy = one;
            u <<= 2;
            for(int jh = 4; jh < u;) {
                ww = xx * xx, yy = xx * imag;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for(; j0 < je; ++j0, ++j2) {
                    mint t0 = a[j0], t1 = a[j0 + v], t2 = a[j2], t3 = a[j2 + v];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                    a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                    a[j0 + v] = t0m1 + t2m3, a[j2 + v] = (t0m1 - t2m3) * ww;
                }
                xx *= dy[__builtin_ctzll(jh += 4)];
            }
            u >>= 4;
            v <<= 2;
        }
        if(k & 1) {
            u = 1 << (k - 1);
            for(int j = 0; j < u; ++j) {
                mint ajv = a[j] - a[j + u];
                a[j] += a[j + u];
                a[j + u] = ajv;
            }
        }
    }

    void ntt(std::vector<mint> &a) {
        if(static_cast<int>(a.size()) > 1)
            fft4(a, __builtin_ctz(static_cast<unsigned int>(a.size())));
    }

    void intt(std::vector<mint> &a) {
        if(static_cast<int>(a.size()) <= 1)
            return;
        ifft4(a, __builtin_ctz(static_cast<unsigned int>(a.size())));
        mint iv = mint(static_cast<int>(a.size())).inv();
        for(auto &x : a)
            x *= iv;
    }

    std::vector<mint> multiply(const std::vector<mint> &a, const std::vector<mint> &b) {
        int l = static_cast<int>(a.size() + b.size() - 1);
        if(std::min<int>(a.size(), b.size()) <= 40) {
            std::vector<mint> s(l);
            for(int i = 0; i < static_cast<int>(a.size()); ++i)
                for(int j = 0; j < static_cast<int>(b.size()); ++j)
                    s[i + j] += a[i] * b[j];
            return s;
        }
        int k = 2, m = 4;
        while(m < l)
            m <<= 1, ++k;
        setwy(k);
        std::vector<mint> s(m);
        for(int i = 0; i < static_cast<int>(a.size()); ++i)
            s[i] = a[i];
        fft4(s, k);
        if(a.size() == b.size() && a == b) {
            for(int i = 0; i < m; ++i)
                s[i] *= s[i];
        } else {
            std::vector<mint> t(m);
            for(int i = 0; i < static_cast<int>(b.size()); ++i)
                t[i] = b[i];
            fft4(t, k);
            for(int i = 0; i < m; ++i)
                s[i] *= t[i];
        }
        ifft4(s, k);
        s.resize(l);
        mint invm = mint(m).inv();
        for(int i = 0; i < l; ++i)
            s[i] *= invm;
        return s;
    }

    void ntt_doubling(std::vector<mint> &a) {
        int m = static_cast<int>(a.size());
        auto b = a;
        intt(b);
        mint r = 1, zeta = mint(pr).inv().pow((mint::mod() - 1) / (m << 1));
        for(int i = 0; i < m; ++i)
            b[i] *= r, r *= zeta;
        ntt(b);
        std::copy(begin(b), end(b), back_inserter(a));
    }
};

template <typename mint>
struct FormalPowerSeries : std::vector<mint> {
    using std::vector<mint>::vector;
    using FPS = FormalPowerSeries;

    FPS &operator+=(const FPS &r) {
        if(r.size() > this->size())
            this->resize(r.size());
        for(int i = 0; i < static_cast<int>(r.size()); ++i)
            (*this)[i] += r[i];
        return *this;
    }
    FPS &operator+=(const mint &r) {
        if(this->empty())
            this->resize(1);
        (*this)[0] += r;
        return *this;
    }
    FPS &operator-=(const FPS &r) {
        if(r.size() > this->size())
            this->resize(r.size());
        for(int i = 0; i < static_cast<int>(r.size()); ++i)
            (*this)[i] -= r[i];
        return *this;
    }
    FPS &operator-=(const mint &r) {
        if(this->empty())
            this->resize(1);
        (*this)[0] -= r;
        return *this;
    }
    FPS &operator*=(const mint &v) {
        for(auto &x : *this)
            x *= v;
        return *this;
    }
    FPS &operator/=(const FPS &r) {
        if(this->size() < r.size()) {
            this->clear();
            return *this;
        }
        int n = static_cast<int>(this->size() - r.size() + 1);
        if(static_cast<int>(r.size()) <= 64) {
            FPS f(*this), g(r);
            g.shrink();
            mint coeff = g.back().inv();
            for(auto &x : g)
                x *= coeff;
            int deg = static_cast<int>(f.size() - g.size() + 1);
            int gs = static_cast<int>(g.size());
            FPS quo(deg);
            for(int i = deg - 1; i >= 0; --i) {
                quo[i] = f[i + gs - 1];
                for(int j = 0; j < gs; ++j)
                    f[i + j] -= quo[i] * g[j];
            }
            *this = quo * coeff;
            this->resize(n, mint(0));
            return *this;
        }
        return *this = ((*this).rev().pre(n) * r.rev().inv(n)).pre(n).rev();
    }
    FPS &operator%=(const FPS &r) {
        *this -= *this / r * r;
        shrink();
        return *this;
    }

    FPS operator+(const FPS &r) const { return FPS(*this) += r; }
    FPS operator+(const mint &v) const { return FPS(*this) += v; }
    FPS operator-(const FPS &r) const { return FPS(*this) -= r; }
    FPS operator-(const mint &v) const { return FPS(*this) -= v; }
    FPS operator*(const FPS &r) const { return FPS(*this) *= r; }
    FPS operator*(const mint &v) const { return FPS(*this) *= v; }
    FPS operator/(const FPS &r) const { return FPS(*this) /= r; }
    FPS operator%(const FPS &r) const { return FPS(*this) %= r; }
    FPS operator-() const {
        FPS ret(this->size());
        for(int i = 0; i < static_cast<int>(this->size()); ++i)
            ret[i] = -(*this)[i];
        return ret;
    }

    void shrink() {
        while(!this->empty() && this->back() == mint(0))
            this->pop_back();
    }
    FPS rev() const {
        FPS ret(*this);
        std::reverse(begin(ret), end(ret));
        return ret;
    }
    FPS dot(FPS r) const {
        FPS ret(std::min(this->size(), r.size()));
        for(int i = 0; i < static_cast<int>(ret.size()); ++i)
            ret[i] = (*this)[i] * r[i];
        return ret;
    }
    FPS pre(int sz) const {
        FPS ret(begin(*this), begin(*this) + std::min(static_cast<int>(this->size()), sz));
        if(static_cast<int>(ret.size()) < sz)
            ret.resize(sz);
        return ret;
    }
    FPS operator>>(int sz) const {
        if(static_cast<int>(this->size()) <= sz)
            return {};
        FPS ret(*this);
        ret.erase(ret.begin(), ret.begin() + sz);
        return ret;
    }
    FPS operator<<(int sz) const {
        FPS ret(*this);
        ret.insert(ret.begin(), sz, mint(0));
        return ret;
    }
    FPS diff() const {
        int n = static_cast<int>(this->size());
        FPS ret(std::max(0, n - 1));
        mint one(1), coeff(1);
        for(int i = 1; i < n; ++i) {
            ret[i - 1] = (*this)[i] * coeff;
            coeff += one;
        }
        return ret;
    }
    FPS integral() const {
        int n = static_cast<int>(this->size());
        FPS ret(n + 1);
        ret[0] = mint(0);
        if(n > 0)
            ret[1] = mint(1);
        auto mod = mint::mod();
        for(int i = 2; i <= n; ++i)
            ret[i] = (-ret[mod % i]) * (mod / i);
        for(int i = 0; i < n; ++i)
            ret[i + 1] *= (*this)[i];
        return ret;
    }
    mint eval(mint x) const {
        mint r = 0, w = 1;
        for(auto &v : *this)
            r += w * v, w *= x;
        return r;
    }
    FPS log(int deg = -1) const {
        assert(!this->empty() && (*this)[0] == mint(1));
        if(deg == -1)
            deg = static_cast<int>(this->size());
        return (this->diff() * this->inv(deg)).pre(deg - 1).integral();
    }
    FPS pow(int64_t k, int deg = -1) const {
        int n = static_cast<int>(this->size());
        if(deg == -1)
            deg = n;
        if(k == 0) {
            FPS ret(deg);
            if(deg)
                ret[0] = 1;
            return ret;
        }
        for(int i = 0; i < n; ++i) {
            if((*this)[i] != mint(0)) {
                mint rev = mint(1) / (*this)[i];
                FPS ret = (((*this * rev) >> i).log(deg) * k).exp(deg);
                ret *= (*this)[i].pow(k);
                ret = (ret << (i * k)).pre(deg);
                if(static_cast<int>(ret.size()) < deg)
                    ret.resize(deg, mint(0));
                return ret;
            }
            if(static_cast<__int128>(i + 1) * k >= deg)
                return FPS(deg, mint(0));
        }
        return FPS(deg, mint(0));
    }

    static void *ntt_ptr;
    static void set_fft();
    FPS &operator*=(const FPS &r);
    void ntt();
    void intt();
    void ntt_doubling();
    static int ntt_pr();
    FPS inv(int deg = -1) const;
    FPS exp(int deg = -1) const;
};

template <typename mint>
void *FormalPowerSeries<mint>::ntt_ptr = nullptr;

template <typename mint>
void FormalPowerSeries<mint>::set_fft() {
    if(!ntt_ptr)
        ntt_ptr = new NTT<mint>;
}

template <typename mint>
FormalPowerSeries<mint> &FormalPowerSeries<mint>::operator*=(const FormalPowerSeries<mint> &r) {
    if(this->empty() || r.empty()) {
        this->clear();
        return *this;
    }
    set_fft();
    auto ret = static_cast<NTT<mint> *>(ntt_ptr)->multiply(*this, r);
    return *this = FormalPowerSeries<mint>(ret.begin(), ret.end());
}

template <typename mint>
void FormalPowerSeries<mint>::ntt() {
    set_fft();
    static_cast<NTT<mint> *>(ntt_ptr)->ntt(*this);
}

template <typename mint>
void FormalPowerSeries<mint>::intt() {
    set_fft();
    static_cast<NTT<mint> *>(ntt_ptr)->intt(*this);
}

template <typename mint>
void FormalPowerSeries<mint>::ntt_doubling() {
    set_fft();
    static_cast<NTT<mint> *>(ntt_ptr)->ntt_doubling(*this);
}

template <typename mint>
int FormalPowerSeries<mint>::ntt_pr() {
    set_fft();
    return static_cast<NTT<mint> *>(ntt_ptr)->pr;
}

template <typename mint>
FormalPowerSeries<mint> FormalPowerSeries<mint>::inv(int deg) const {
    assert((*this)[0] != mint(0));
    if(deg == -1)
        deg = static_cast<int>(this->size());
    FormalPowerSeries<mint> res(deg);
    res[0] = mint(1) / (*this)[0];
    for(int d = 1; d < deg; d <<= 1) {
        FormalPowerSeries<mint> f(2 * d), g(2 * d);
        for(int j = 0; j < std::min(static_cast<int>(this->size()), 2 * d); ++j)
            f[j] = (*this)[j];
        for(int j = 0; j < d; ++j)
            g[j] = res[j];
        f.ntt();
        g.ntt();
        for(int j = 0; j < 2 * d; ++j)
            f[j] *= g[j];
        f.intt();
        for(int j = 0; j < d; ++j)
            f[j] = 0;
        f.ntt();
        for(int j = 0; j < 2 * d; ++j)
            f[j] *= g[j];
        f.intt();
        for(int j = d; j < std::min(2 * d, deg); ++j)
            res[j] = -f[j];
    }
    return res.pre(deg);
}

template <typename mint>
FormalPowerSeries<mint> FormalPowerSeries<mint>::exp(int deg) const {
    using FPS = FormalPowerSeries<mint>;
    assert(this->empty() || (*this)[0] == mint(0));
    if(deg == -1)
        deg = static_cast<int>(this->size());

    FPS inv;
    inv.reserve(deg + 1);
    inv.push_back(mint(0));
    inv.push_back(mint(1));

    auto inplace_integral = [&](FPS &f) {
        int n = static_cast<int>(f.size());
        auto mod = mint::mod();
        while(static_cast<int>(inv.size()) <= n) {
            int i = static_cast<int>(inv.size());
            inv.push_back((-inv[mod % i]) * (mod / i));
        }
        f.insert(begin(f), mint(0));
        for(int i = 1; i <= n; ++i)
            f[i] *= inv[i];
    };

    auto inplace_diff = [](FPS &f) {
        if(f.empty())
            return;
        f.erase(begin(f));
        mint coeff = 1, one = 1;
        for(int i = 0; i < static_cast<int>(f.size()); ++i) {
            f[i] *= coeff;
            coeff += one;
        }
    };

    FPS b{1, 1 < static_cast<int>(this->size()) ? (*this)[1] : 0}, c{1}, z1, z2{1, 1};
    for(int m = 2; m < deg; m <<= 1) {
        auto y = b;
        y.resize(2 * m);
        y.ntt();
        z1 = z2;
        FPS z(m);
        for(int i = 0; i < m; ++i)
            z[i] = y[i] * z1[i];
        z.intt();
        std::fill(begin(z), begin(z) + m / 2, mint(0));
        z.ntt();
        for(int i = 0; i < m; ++i)
            z[i] *= -z1[i];
        z.intt();
        c.insert(end(c), begin(z) + m / 2, end(z));
        z2 = c;
        z2.resize(2 * m);
        z2.ntt();
        FPS x(begin(*this), begin(*this) + std::min<int>(this->size(), m));
        x.resize(m);
        inplace_diff(x);
        x.push_back(mint(0));
        x.ntt();
        for(int i = 0; i < m; ++i)
            x[i] *= y[i];
        x.intt();
        x -= b.diff();
        x.resize(2 * m);
        for(int i = 0; i < m - 1; ++i)
            x[m + i] = x[i], x[i] = mint(0);
        x.ntt();
        for(int i = 0; i < 2 * m; ++i)
            x[i] *= z2[i];
        x.intt();
        x.pop_back();
        inplace_integral(x);
        for(int i = m; i < std::min<int>(this->size(), 2 * m); ++i)
            x[i] += (*this)[i];
        std::fill(begin(x), begin(x) + m, mint(0));
        x.ntt();
        for(int i = 0; i < 2 * m; ++i)
            x[i] *= y[i];
        x.intt();
        b.insert(end(b), begin(x) + m, end(x));
    }
    return FPS{begin(b), begin(b) + deg};
}

template <typename mint>
mint LinearRecurrence(long long k, FormalPowerSeries<mint> q, FormalPowerSeries<mint> p) {
    q.shrink();
    mint ret = 0;
    if(p.size() >= q.size()) {
        auto r = p / q;
        p -= r * q;
        p.shrink();
        if(k < static_cast<int>(r.size()))
            ret += r[k];
    }
    if(static_cast<int>(p.size()) == 0)
        return ret;

    FormalPowerSeries<mint>::set_fft();
    if(FormalPowerSeries<mint>::ntt_ptr == nullptr) {
        p.resize(static_cast<int>(q.size()) - 1);
        while(k) {
            auto q2 = q;
            for(int i = 1; i < static_cast<int>(q2.size()); i += 2)
                q2[i] = -q2[i];
            auto s = p * q2;
            auto t = q * q2;
            if(k & 1) {
                for(int i = 1; i < static_cast<int>(s.size()); i += 2)
                    p[i >> 1] = s[i];
                for(int i = 0; i < static_cast<int>(t.size()); i += 2)
                    q[i >> 1] = t[i];
            } else {
                for(int i = 0; i < static_cast<int>(s.size()); i += 2)
                    p[i >> 1] = s[i];
                for(int i = 0; i < static_cast<int>(t.size()); i += 2)
                    q[i >> 1] = t[i];
            }
            k >>= 1;
        }
        return ret + p[0];
    } else {
        int n = 1;
        while(n < static_cast<int>(q.size()))
            n <<= 1;

        p.resize(2 * n);
        q.resize(2 * n);
        p.ntt();
        q.ntt();
        std::vector<mint> s(2 * n), t(2 * n);

        std::vector<int> btr(n);
        for(int i = 0, logn = __builtin_ctz(static_cast<unsigned int>(n)); i < (1 << logn); ++i)
            btr[i] = (btr[i >> 1] >> 1) + ((i & 1) << (logn - 1));
        mint dw = mint(FormalPowerSeries<mint>::ntt_pr()).inv().pow((mint::mod() - 1) / (2 * n));

        while(k) {
            mint inv2 = mint(2).inv();
            t.resize(n);
            for(int i = 0; i < n; ++i)
                t[i] = q[(i << 1)] * q[(i << 1) | 1];

            s.resize(n);
            if(k & 1) {
                for(auto &i : btr) {
                    s[i] = (p[(i << 1)] * q[(i << 1) | 1] - p[(i << 1) | 1] * q[(i << 1)]) * inv2;
                    inv2 *= dw;
                }
            } else {
                for(int i = 0; i < n; ++i)
                    s[i] = (p[(i << 1)] * q[(i << 1) | 1] + p[(i << 1) | 1] * q[(i << 1)]) * inv2;
            }
            swap(p, s);
            swap(q, t);
            k >>= 1;
            if(k < n)
                break;
            p.ntt_doubling();
            q.ntt_doubling();
        }
        p.intt();
        q.intt();
        return ret + (p * (q.inv()))[k];
    }
}

template <typename mint>
mint bostan_mori(long long n, FormalPowerSeries<mint> q, FormalPowerSeries<mint> a) {
    assert(!q.empty() && q[0] != 0);
    if(n < static_cast<int>(a.size()))
        return a[n];
    assert(static_cast<int>(a.size()) >= static_cast<int>(q.size()) - 1);
    auto p = a.pre(static_cast<int>(q.size()) - 1) * q;
    p.resize(q.size() - 1);
    return LinearRecurrence<mint>(n, q, p);
}

} // namespace momohara
