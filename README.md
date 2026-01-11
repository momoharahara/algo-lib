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
