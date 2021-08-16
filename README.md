# tetris_emulatorDX
テトリスエミュレーター。DXライブラリ使用。

時々雨ルル@tokidoki_rain

tetris emulatorDX(v3.0.7) latest

tetris emulatorDX(v3.0.5) stable

無断でばらまかれると大変なことになりそう（小並感）
無料で遊べるテト〇スエミュレーター

(注意)当ソフトウェアの使用はご自身の責任で行ってください。気を付けてはいますが重大なバグやエラーが生じる可能性が十分にあります。特に、テンプレ機能はテキストファイルの記述内容や構成が正確であることを前提としており、また整合性チェックを行わないため不正なエラーの温床になりえます。十分に気を付けてください。

インストール： 
不要です。中のexe実行ファイルを開けばOKです。当然、windows限定となります…。

アンインストール： 
不要です。すべてゴミ箱にいれて削除してください。


操作：クラシック？　キーコンフィグ未対応（ごめんなさい…） 

右：→ 
左：←

時計回り　：↑ 
反時計回り：Z

ホールド：C 
ハードロ：SPACE 
ソフドロ：↓

終了：ESCキー 
ソフトを終了します。できればこのキーによる終了を推奨しています。

リセット：R 
おじゃま生成：G　ランダムにせりあがる。Gを押したあとハードロするとライン消去後にせりあがる

AI開発中：M 
AIというよりCPUですが、自動でミノを配置していきます。自滅はしない程度に動きます。

テンプレ実装しました（v3.02）

大方完成していますがAI側の問題で「選択したテンプレが組めるのに組まなかった」ことが多発します。この点は深度探索と評価を正しく設定すれば解決できそうです

DAS：多分5F 
左右下移動：多分1F

PCとっても何も表示出ません（硬直0F） 
ライン消去はぷよテトに準拠？ 
ミノ操作はテトリオに近い感じ? 

DAS入力に対応？かなりガバい 
斜め下移動する（ズルい）

画像は自作なので一応

ディレクトリ構成は変更不可

画像は名前指定なので名前を変えれば好きな画像を指定できる（元の名前を損失しないように）

templateフォルダ内のテキストデータはいじらないことを推奨

記述構成が正しいことを前提に読み込みを行うため、不正なファイルを読み込むと重大なエラーにつながる可能性アリ


既知のバグ

・壁当ての最適化動作をするときにDASが解除されず一マス戻る等が難しい
DAS等の仕方が分かってないので許し亭許して

・テンプレ適応中に地形位置の変更（ライン消去・お邪魔せり上がり）が生じてもテンプレデータ自体の絶対座標が変化せず、おかしな挙動になってしまう。

：オフセットを用意し、テンプレ採用時点との絶対座標のずれを考慮に入れることで対処可能。初代テンプレ機能の時もライン消去とお邪魔せりあがりでバグってた。せり上がり未対応。

・テンプレはそれぞれあらかじめ指定されたミノ順序関係の条件を満たすと採用される。
TSDであればI < Z < J または I < S < L など。実際にはテンプレを組めるのに組に行かない、あるいは途中で失敗することが多い（HOLDの利用をあまり考えていなかったので）

あと、テンプレ通りの配置ができるにも関わらず別の手を選んでしまうこともたまにある（何故？）

頑張って機能拡張したい。でも脳みそが足りていない。

☆リリースノート☆

v3.0.7 3:17 2021/08/17

・改良準備工事

v3.0.5 0:09 2021/08/14

・テンプレ機能ver2.5に移行

テンプレ機能についてはほぼ完成形。テンプレ選択条件についてより高度に指定できるする可能性あり。

評価関数を微調整したせいで弱くなりました。深度探索と合わせて改良予定　

v3.0.0 17:47 2021/08/06

・テンプレ機能ver2に移行！　その他改良準備

v2.91 0:52 2021/08/06

・v3への移行準備、テンプレ機能の再構築、評価部分の新テンプレ対応

v2.1か2ぐらい　17:37 2021/07/30

本プロジェクトではDXライブラリを使用しています(https://dxlib.xsrv.jp/index.html)

DX Library Copyright (C) 2001-2021 Takumi Yamada.

-about Copyright-
please refer to the file "copyright_list.txt".





