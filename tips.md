# Tips

## Output

- `fixed << setprecision(n)` で小数点以下 `n` 桁出力
- `setfill('0') << setw(12) << num` で 0 埋め 12 桁出力

## Standard Library

- `lower_bound(all(a), x)` は `x` 以上の最初の要素
- `upper_bound(all(a), x)` は `x` より大きい最初の要素
- `set` / `map` は専用の `lower_bound` がある
- `next_permutation(all(r))` は次の順列に並べ替える
- `next_permutation` を使う前はソートしておく
- `priority_queue<P, vector<P>, greater<P>>` で昇順 priority queue

## General

- 調和級数は `n/1 + n/2 + n/3 + ...` でだいたい `n log n`
- 約数を見るときは `sqrt(n)` まででよい
- 鳩ノ巣を疑う
- ビット演算は括弧を付ける
- 問題を飛ばすときは順位表を確認する
- `1 << n` ではなく `1LL << n`
- 数え上げや最適化はまず `DP` を疑う

## Two Pointers

- 連続区間で条件を満たす最長長さは尺取りを疑う
- 右へ伸ばして、壊れたら左端を落とす
- オーバーフローに注意

## Game

- 後ろから考える
- 逆操作を考える
- Grundy 数を使えるか確認する
- `mex` 高速化: <https://atcoder.jp/contests/arc038/submissions/9163705>

## Graph

- 頂点数 × 状態数 のグラフ化を疑う
- 条件 `l` 以下で通れるなら、`l` 以下だけ通る補助グラフ再構築も考える
- 森なら `連結成分数 = 頂点数 - 辺数`

## Arithmetic

- `floor(a / b) = (a - a % b) / b`
- `floor` 系は周期性を持つことが多い
- 桁和の桁和は順序が関係ないことがある
- 桁和まわりは周期 `9` を疑う
- 平均が `x` になる条件は `値 - x` の総和で見る
- 正負に分けると見やすいことがある

## Bit / XOR

- 部分集合列挙は `O(3^n)` の形になることがある
- 部分集合上の最大値などは高速ゼータ変換も候補

### XOR

- `A xor A = 0`
- `A xor B = C` なら `A = B xor C`
- `A or B >= A xor B`
- ビットごとに分けて考える
- `0/1` で範囲分けしたいときは、ソートして再帰で区間ごとに見る
- `A xor B = A + B` は繰り上がりがない場合
- `A + B = (A and B) * 2 + (A xor B)`
- `sum = xor + and * 2 = or + and`
- `or = xor + and`
- `mod 2` 上の演算は上位 bit から貪欲が効きやすい

## DP

- 区間 DP を疑う
- 包除原理は余事象から引く形も考える
- 未確定要素を持つ DP を考える
- 同じサイズの集合を重複して数えるときは基準点を固定する
- 「最大が `x`」は「`x` 以下」から「`x-1` 以下」を引く

## Counting

- `x * y * z` の総和は、各要素から 1 個選ぶ場合の数として見直せる
- `x + y + z = n` 系は丸と仕切りを使う
- 偶数条件があるときは 2 個まとめて考える
- ランダムな 2 種類の操作はグリッド経路で見られることがある
- `A の数 <= B の数` になる経路数はカタラン数を疑う
- `N <= 40` は半分全列挙を疑う
- 半分全列挙の典型はナップザック、使えないペアのある問題
- `nCr` の sum が `n` 次式になることがあるのでラグランジュ補間を疑う

## Errors

### CE

- Unix 依存のものや使えない変数名を疑う

### RE

- 範囲外参照
- 配列長の取りすぎ
- DP 配列長のミス
- 入力配列長の確認漏れ
- 無限ループ
- コーナーケース漏れ

### TLE

- 参照渡しになっていない
- `map` を `unordered_map` にできないか
- `priority_queue` の順序ミス
- `set` の定数倍が重い

### WA

- 制約見間違い
- `n / x` は切り捨て
- `=` と `==`
- 負 index などの未定義動作
- オーバーフロー
- マクロ内のオーバーフロー
- 初期値設定漏れ
- iterator を読む前に `end()` か確認

## Data Structure

- order statistic tree があると、`log n` で挿入、`k` 番目取得、順位取得ができる
- 参考: <https://xuzijian629.hatenablog.com/entry/2018/12/01/000010>
- 参考: <https://atcoder.jp/contests/arc033/submissions/3643481>

## Command Memo

```sh
cd "/Users/momohara/Desktop/dev/atcoder/" && g++ AtCoder2.cpp -std=c++17 -o a && /Users/momohara/Desktop/dev/atcoder/a
cd "/Users/momohara/Desktop/dev/atcoder/" && g++ b.cpp -std=c++17 -o a && /Users/momohara/Desktop/dev/atcoder/a
```
