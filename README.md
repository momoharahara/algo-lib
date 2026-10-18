# このレポジトリについて
[AtCoder生成AIルール](https://info.atcoder.jp/entry/llm-rules-ja?_gl=1*nhqrft*_ga*MTIyMjQ1NjAzOS4xNjA2ODM2OTY1*_ga_RC512FD18N*czE3OTIzMTU2NzgkbzY4MyRnMSR0MTc5MjMyMDgxMiRqNTUkbDAkaDA.) で、ライブラリの公開が必要になったので公開しますが、基本的に他の方のライブラリのIFを変更してるくらいなので、他のライブラリを参照することをお勧めします。

## ACL 利用について

lib 配下ですでに clone した ac-library のフォルダを指定してリンク作成

```
ln -sL ~/dev/ac-library/atcoder/ atcoder
```

## online-judge-tools

- oj のインストール

```sh
uv tool install --python 3.11 online-judge-tools
```

memo: python3.12 以降は oj が対応してなくて動かなかった

- oj のログイン確認
  1. oj login "https://atcoder.jp/"
  1. ダメな場合は https://github.com/key-moon/aclogin

## 提出時の展開

`lib/momohara` や `debug` などのローカル include は提出前に 1 ファイルへ展開可能。

```sh
python3 tools/expand.py src/a.cpp -o bin/a.submit.cpp
```

- `#include "..."` は現在ファイル基準とリポジトリルート基準で解決
- リポジトリ内に実在する `#include <...>` も展開対象
- 見つからない include はそのまま残す

問題文字を指定して展開結果をクリップボードへ入れるなら:

```sh
sh tools/copy_submit.sh a
```
