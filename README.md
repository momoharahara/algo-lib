## ACL 利用について

lib 配下ですでに clone した ac-library のフォルダを指定してリンク作成

```
ln -sL ~/dev/ac-library/atcoder/ atcoder
```

## venv 環境について

- venv で仮想環境構築

```
python3 -m venv venv
```

- venv 環境に入る

```
source venv/bin/activate
```

- venv 環境から出る

```
deactivate
```

## pip でパッケージ管理

- インストールしたパッケージのバージョン確認

```
pip freeze
```

## online-judge-tools

- oj のインストール

```sh
pip install online-judge-tools
```

- oj のログイン、確認
  oj login -u ユーザー名 -p パスワード "https://atcoder.jp/"
  oj login --check "https://atcoder.jp/"
