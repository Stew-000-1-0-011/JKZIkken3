# TL; DR
- MSYS2を入れよう
- avr-gccの新しめのをインストールしよう
- Arduino IDEでビルドするときのコンパイラオプションを変えよう
- avr-gccの最新バージョンをビルドしよう

# MSYS2を入れよう
[MSYS2(公式ページ)](https://www.msys2.org/)
特に罠もなかったと思う。QiitaとかZennとか参考に素直に入れれば良い。
`pacman -Sy`, `pacman -Su`を忘れないこと(aptのupdate, upgradeに相当)

あ、環境変数は設定しておくと良い。
私はWindowsのユーザー環境変数`Path`に`＜msysのルートディレクトリ＞/mingw64/bin`を加えてる。逆にMSYS2のほうの環境変数`PATH`は~/.bashrcで必要なのだけ加えている。

# avr-gccを入れよう
MSYS2のMinGW64を使って、
```bash
# やってなかったらやる
pacman -Sy
pacman -Su

# 入れたいパッケージのちゃんとした名前を検索
pacman -Ss avr-gcc

# 結果を見た感じ、avr-toolchainを入れれば良さげ
pacman -S mingw-w64-x86_64-avr-toolchain

# 最後にバージョンをチェック、正しく表示されればヨシ。
avr-gcc --version
```

# Arduino IDEのコンパイラオプションの変更
もしかしたら人によるかもしれない。私の場合、Arduinoのコード類はユーザーフォルダ内の`AppData/Local/Arduino15`にあった(**隠しファイルを可視化してないと表示されないので注意**)。

さらに`./packages/arduino/hardware/avr/1.8.6`を見ていく。最後のバージョン番号は違っているかも。

ここにplatform.txtがある。これを編集すればよい。

**かならず元の行をコピペし、`#`でコメントアウトしてもとに戻せるようにしておくこと。**
コンパイラパスをMSYS2のほうに変更、最後にC/C++のそれぞれのバージョンを17, 20にして保存。(その時点で最新かその一つ前くらいがいい)

ここで必ず**Arduino IDEを再起動**すること。

# 注意
Arduino IDEはplatform.txtの変更を感知しない。Arduino IDEの起動時に一度読まれるだけっぽい。注意。
(なんかビルド結果もキャッシュしてそう。なんか怪しい場合は一々再起動してビルドしたほうがいいかも)

# avr-gcc-13.2.0の入れ方
MSYS2で入るavr-gcc-12.2.0だと`Serial.print(<コンパイル時定数でないもの>)`で最適化中に*internal compiler error*が出る。
そこで何とかしてgcc-13.2.0を入れる必要がある。
(注：2023/10/18時点。既にデフォで新しいのが入るならこんなことすべきじゃない)

正攻法でないかもしれないが、MSYS2のavr-gccのPKGBUILDを少し弄ってmakepkgにかけることでavr-gcc-13.2.0を入れることができた。
ほとんどChatGPTにお祈りしてやってる。あまりあてにならない。

## 手順
MSYS2が入っている前提。

1. avr-gccのPKGBUILDをコピペし、新たに用意した作業ディレクトリに置く。
2. 中身のpkgverを13.2.0に変更する。
3. sha256sumsの上のほうもSKIPにする。
4. いちどmakepkgをかける。失敗するはず。
5. 出てきたエラーにある公開鍵を`gpg --recv-keys <KeyID>`(ここ非常に怪しい)
6. もう一度makepkgをかける(少なくとも2時間半以上かかる。寝落ちしている間に終わっていた)
7. できたpkg.tar.zstをインストールする。`pacman -U <pkg.tar.zst>`

動かした。`Serial.print(<コンパイル時定数でないもの>)`が通るようになった。(なんかエラーじゃないっぽい出力吐いてるけど)
