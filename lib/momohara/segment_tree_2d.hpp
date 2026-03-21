#pragma once

#include <algorithm>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>


template <typename T, typename F, typename U>
class SegmentTree2DInner {
    T e_;
    F op_;
    U st_;
    int n_;
    std::vector<T> dat_;

  public:
    SegmentTree2DInner(F f, U u, T e) : e_(e), op_(f), st_(u), n_(1) {}

    void init(const std::vector<T> &v) {
        int m = static_cast<int>(v.size());
        n_ = 1;
        while(n_ < m)
            n_ <<= 1;
        dat_.assign(2 * n_ - 1, e_);
        for(int i = 0; i < m; ++i)
            dat_[n_ + i - 1] = v[i];
        for(int i = n_ - 2; i >= 0; --i)
            dat_[i] = op_(dat_[i * 2 + 1], dat_[i * 2 + 2]);
    }

    void update(int i, T x) {
        i += n_ - 1;
        dat_[i] = st_(dat_[i], x);
        while(i > 0) {
            i = (i - 1) / 2;
            dat_[i] = op_(dat_[i * 2 + 1], dat_[i * 2 + 2]);
        }
    }

    T query(int l, int r) const {
        T left = e_;
        T right = e_;
        l += n_ - 1;
        r += n_ - 1;
        while(l < r) {
            if((l & 1) == 0)
                left = op_(left, dat_[l]);
            if((r & 1) == 0)
                right = op_(dat_[r - 1], right);
            l /= 2;
            r = (r - 1) / 2;
        }
        return op_(left, right);
    }
};

template <typename CandidateType, typename ValueType, typename F, typename U>
class RangeTree {
  public:
    static_assert(std::is_integral_v<CandidateType>, "Integral required.");

  private:
    using CT = CandidateType;
    using VT = ValueType;
    using pcc = std::pair<CT, CT>;

    int n_;
    int sz_;
    VT e_;
    F op_;
    U st_;
    std::vector<SegmentTree2DInner<VT, F, U>> seg_;
    std::vector<std::vector<pcc>> yx_;
    std::vector<pcc> sorted_;
    std::vector<pcc> cand_;

    void update_(int id, CT x, CT y, VT val) {
        id += n_ - 1;
        int yid = static_cast<int>(std::lower_bound(yx_[id].begin(), yx_[id].end(), pcc(y, x)) - yx_[id].begin());
        seg_[id].update(yid, val);
        while(id > 0) {
            id = (id - 1) / 2;
            yid = static_cast<int>(std::lower_bound(yx_[id].begin(), yx_[id].end(), pcc(y, x)) - yx_[id].begin());
            seg_[id].update(yid, val);
        }
    }

    VT query_impl(int lxid, int rxid, CT ly, CT ry, int k, int l, int r) const {
        if(r <= lxid || rxid <= l)
            return e_;
        if(lxid <= l && r <= rxid) {
            int lyid = static_cast<int>(std::lower_bound(yx_[k].begin(), yx_[k].end(), pcc(ly, std::numeric_limits<CT>::min())) - yx_[k].begin());
            int ryid = static_cast<int>(std::upper_bound(yx_[k].begin(), yx_[k].end(), pcc(ry, std::numeric_limits<CT>::min())) - yx_[k].begin());
            return (lyid >= ryid) ? e_ : seg_[k].query(lyid, ryid);
        }
        return op_(
            query_impl(lxid, rxid, ly, ry, 2 * k + 1, l, (l + r) / 2),
            query_impl(lxid, rxid, ly, ry, 2 * k + 2, (l + r) / 2, r));
    }

  public:
    RangeTree(F f, U u, VT e, const std::vector<pcc> &cand, const std::vector<VT> &val)
        : n_(1), sz_(static_cast<int>(cand.size())), e_(e), op_(f), st_(u), sorted_(sz_), cand_(cand) {
        while(n_ < sz_)
            n_ <<= 1;
        for(int i = 0; i < sz_; ++i)
            sorted_[i] = {cand[i].first, i};
        std::sort(sorted_.begin(), sorted_.end(), [&](const pcc &a, const pcc &b) {
            return (a.first == b.first) ? (cand[a.second].second < cand[b.second].second) : (a.first < b.first);
        });
        yx_.resize(2 * n_ - 1);
        seg_.reserve(2 * n_ - 1);
        for(int i = 0; i < 2 * n_ - 1; ++i)
            seg_.emplace_back(op_, st_, e_);

        for(int i = 0; i < sz_; ++i) {
            yx_[i + n_ - 1] = {{sorted_[i].second, sorted_[i].first}};
            std::vector<VT> arg = {val[sorted_[i].second]};
            seg_[i + n_ - 1].init(arg);
            sorted_[i].second = cand[sorted_[i].second].second;
        }
        for(int i = n_ - 2; i >= 0; --i) {
            yx_[i].resize(yx_[2 * i + 1].size() + yx_[2 * i + 2].size());
            if(yx_[i].empty())
                continue;
            std::merge(yx_[2 * i + 1].begin(), yx_[2 * i + 1].end(), yx_[2 * i + 2].begin(), yx_[2 * i + 2].end(), yx_[i].begin(), [&](const pcc &a, const pcc &b) {
                return (cand[a.first].second == cand[b.first].second) ? (a.second < b.second) : (cand[a.first].second < cand[b.first].second);
            });
            std::vector<VT> arg(yx_[i].size());
            for(int j = 0; j < static_cast<int>(yx_[i].size()); ++j)
                arg[j] = val[yx_[i][j].first];
            seg_[i].init(arg);
        }
        for(int i = 0; i < 2 * n_ - 1; ++i) {
            for(auto &e : yx_[i])
                e.first = cand[e.first].second;
        }
    }

    void update(CT x, CT y, VT val) {
        int id = static_cast<int>(std::lower_bound(sorted_.begin(), sorted_.end(), pcc(x, y)) - sorted_.begin());
        update_(id, x, y, val);
    }

    VT query(CT lx, CT ly, CT rx, CT ry) const {
        int lxid = static_cast<int>(std::lower_bound(sorted_.begin(), sorted_.end(), pcc(lx, std::numeric_limits<CT>::min())) - sorted_.begin());
        int rxid = static_cast<int>(std::upper_bound(sorted_.begin(), sorted_.end(), pcc(rx, std::numeric_limits<CT>::min())) - sorted_.begin());
        return (lxid >= rxid) ? e_ : query_impl(lxid, rxid, ly, ry, 0, 0, n_);
    }
};

