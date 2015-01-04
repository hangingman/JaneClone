cmdline_twitter
===============
Linuxのターミナルなどで使えるTwitterクライアントです。
MSYS2のターミナル(minty)でも使用できます。
1ラインで投稿とかできるので、cronと併用してBOT的な使い方もできます。


使い方
===============
コンシューマキーとシークレットの取り扱いの関係上、バイナリはありません。
使うにはまず「コンパイル」してください。
環境はLinux(Ubuntuで確認)とWindows(MSYS2を使用)でコンパイル可能です。

コンパイル
--------

cmdline_twitterは以下のパッケージを使用しています。
* g++
* STL(libstdc++)
* libcurl-dev (opensslでもnssでも何でもいい)

ので、あらかじめインストールしてください。

Ubuntuの場合は次のようにしたら良いかもしれません。
````
$ sudo apt-get install g++ libstdc++6 libcurl4-nss-dev
````

Windows環境(MSYS2)の場合は次のようにすると良いかもしれません
````
$ pacman -S gcc  gcc-libs libcurl libcurl-devel
````



次に、http://blog.uklab.jp/web/add-application-for-twitter/ のあたりを参考に、Twitterのdepeloper登録を行い、適当にアプリケーションの登録を行ってください。
コールバックURLは空欄でかまいません。

次に、sample_setkey.shを開いて、CONSUMER_KEY と CONSUMER_SECRET を書き換えます。
書き換え終わったら、
````
$ . ./sample_setkey.sh
$ make
````
でクライアントが作成されます。
. ./sample_setkey.sh は環境変数を設定しています。
一度設定したら、ターミナルを閉じるまでしなくてOKです。


コマンドライン
--------
コマンドは ctw です。
以下のオプションがあります。

* -h | --help          Print this message
* -a | --auth          [再]認証を行う<br>
                     -u オプションでエイリアスを指定できます<br>
* -D | --Direct        DM関連の操作を行う<br>
                     -p オプションと-n オプションでユーザ名指定でダイレクトメッセージを送る<br>
                     -r オプションでダイレクトメッセージを読む<br>
                     -r と -n オプションで""と指定すると自分の発言を読む<br>
                     -d と -i オプションでID指定でメッセージを消す<br>
* -p | --post status   タイムラインへ投稿
* -s | --search word   ワードで検索
* -r | --readtl        ホームのタイムラインを読む<br>
                     -x オプションでUserStreamを使って読む(以後のオプションは無視)<br>
                     -n オプションでユーザ名指定すると指定ユーザを読む<br>
                     -n オプションで""と指定すると自分の発言を読む<br>
                     -n オプションで"@"と指定すると自分へのメンションを読む<br>
* -d | --del           発言の削除 -iでID指定
* -R | --Retweet       リツイートする -iでID指定
* -F | --Fav           お気に入りに追加する -iでID指定
* -l | --list name     自分のリストnameの内容を読む<br>
                     nameで""と指定すると自分のリスト一覧を表示する<br>
* -n | --name          指定が必要な場合のユーザスクリーンネーム
* -i | --id            指定が必要な場合の発言ID
* -u | --user alies    エイリアス名指定:省略可(-a とも併用可能)
* -x | --xstream       Streaming APIを使う(使用可能な場合)
* -T | --Test          (テスト用)APIのエンドポイントを指定してAPIリクエストを行う<br>
                     -x オプションでStreaming向け接続を行う
* -v | --verbose       (デバッグ用)余計な文字を出力しまくる

まず認証する必要があるので、-a オプションで認証をしてください。
-u オプションでユーザエイリアスを指定することができます。(指定しなくても運用はできます)
エイリアスは複数アカウントを運用するときに使うと便利です。

ユーザのトークンキーとシークレットは /home/現在のLinuxログインユーザ名/.ctw/ 以下に保存されます。
(~/.ctw/ 以下です)
.authkey_エイリアス名 がそれですので、取り扱いには注意してください。

例示
--------
* エイリアス名eggで認証する<br>
ctw -u egg -a

* エイリアス名eggで投稿<br>
ctw -u egg -p "i am egg"

* エイリアス名eggのHomeタイムラインを見る<br>
ctw -u egg -r

* ｢github｣という投稿があるかどうか検索する<br>
ctw -s github


制限とか
--------
タイムラインの取得はデフォルトではREST APIを使っています。
UserStreamで取得したい場合は、-r -x としてください。

今のところ、Homeタイムラインの表示でRTや@は表示しない仕様になっています。

MSYS2環境では発言のタイムスタンプが標準時刻になっています。


その他
--------
使用している端末によっては全角文字がずれます。
例；■など
これは、Unicodeのある文字の文字幅があいまい(ambiguous)と定義されていることに由来しています。
http://ja.wikipedia.org/wiki/%E6%9D%B1%E3%82%A2%E3%82%B8%E3%82%A2%E3%81%AE%E6%96%87%E5%AD%97%E5%B9%85

ambiguous設定が指定できる端末では一応ちゃんと表示することはできます。
もし、Ubuntuを使っている場合は以下に書いてある方法で解決するかmltermを使うとよさそうです
https://gist.github.com/sgk/5991138

ただし、mlterm以外の上記方法では今度は罫線が２文字として認識されてしまうので、使用状況によっていろいろとお試しください。
(Ubuntuがデフォルトでこうなってしまうのは、おそらくユーザの使用状況によって変わるので決めかねているのかもしれません)

TODO
===============
* 画像系アップロードへ対応
* 対話モードの追加
* なるべくライブラリに依存しない
* 見栄えをよくする
* 色指定できるようにする
* 他の環境への対応

謝辞
===============
* picojsonのkazuho氏 (https://github.com/kazuho/picojson)<br>
JSONの解析に使用しています。picojsonはSTLの親和性高くて便利です

