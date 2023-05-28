## venv環境について
- venvで仮想環境構築
```
python3 -m venv venv
```
- venv環境に入る
```
source venv/bin/activate
```
- venv環境から出る
```
deactivate
```
## pipでパッケージ管理
- インストールしたパッケージのバージョン確認
```
pip freeze
```
## online-judge-tools
- ojのインストール
```sh
pip install online-judge-tools
```
- ojのログイン、確認
    oj login -u ユーザー名 -p パスワード "https://atcoder.jp/"
    oj login --check "https://atcoder.jp/"
