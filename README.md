# tetris_emulatorDX
テトリスエミュレーター。DXライブラリ使用。

時々雨ルル@tokidoki_rain

tetris emulatorDX(v3.7.1) latest
tetris emulatorDX(v3.6.2) recommened


無断でばらまかれると大変なことになりそう（小並感）
無料で遊べるテト〇スエミュレーター

(注意)当ソフトウェアの使用はご自身の責任で行ってください。気を付けてはいますが重大なバグやエラーが生じる可能性が十分にあります。特に、テンプレ機能はテキストファイルの記述内容や構成が正確であることを前提としており、また整合性チェックを行わないため不正なエラーの温床になりえます。十分に気を付けてください。

インストール： 
不要です。中のexe実行ファイルを実行すればOKです。当然、windows限定となります…。
アップデート：
＜注意！＞v3.6.2 <-> v3.7.1は互換性がありません！
v3.7.1を動かす際は対応するファイルセットを用いてください（リリースからセットで取得しましょう）
v3.6.2以前を動かす際も同様に、対応するファイルセットをリリースから取得してください

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

☆　パッドコントローラー対応（暫定的）
暫定的ですがパット対応しています。が、製作者にPC用ジョイパットがない（proコンは反応せず）ため機能するか知らないしコンフィグもできません…
以下switch proコンを対象にした設定内容です、参考までに（①とかはハードウェア上での識別番号です）
 A  = ② =>SPACE(ハードロ)
 
 B  = ① =>C(ホールド)
 
 X  = ④ =>
 
 Y  = ③ =>
 
 "+"  = ⑩ =>
 
 "-"  = ⑨ =>R(リセット)
 
 □ = ⑭ =>
 
 ⌂  = ⑬ =>
 
 R  = ⑥ =>↑(時計回り回転)
 
 ZR = ⑧ =>↑(時計回り回転)
 
 L  = ⑤ =>Z(反時計回り回転)
 
 ZL = ⑦ =>Z(反時計回り回転)
 
 R↓= ⑫ =>
 
 L↓= ⑪ =>

テンプレ実装しました（v3.02）
ループ探索（深度探索）5手を実装しました（v.3.4.9）

大方完成していますがAI側の問題で「選択したテンプレが組めるのに組まなかった」ことが多発します。この点は深度探索と評価を正しく設定すれば解決できそうです
深度探索と組み合わせた状態でもテンプレ機能が有効化されました。ただし、評価の問題か途中でテンプレを放棄する例があります。なんで…

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

・テンプレ適応中にお邪魔せり上がりが生じてもテンプレデータ自体の絶対座標が補正されず、、おかしな挙動になってしまう。

・テンプレはそれぞれあらかじめ指定されたミノ順序関係の条件を満たすと採用される。
TSDであればI < Z < J または I < S < L など。実際にはテンプレを組めるのに組に行かない、あるいは途中で失敗することが多い（HOLDの利用をあまり考えていなかったので）

頑張って機能拡張したい。でも脳みそが足りていない。

☆リリースノート☆

v3.7.1 0:48 2022/06/18 ・マルチスレッド化、パラメータ調整、パッド対応、FHD16:9比に対応

v3.6.2 15:12 2022/01/03 ・評価関数の強化と調整　・パフェ判定の修正
それなりに動きがよくなりましたが、一方で明らかな悪手を選んで窒息するパターンも増えました。また、ライン消去の判断が遅くなりだいぶ積み上げるようになってしまいました。

v3.4.9 22:14 2021/09/03
・祝！深度（ループ）探索実装！意外といい動きになりました。一方、テンプレが機能しなくなったほか以前評価関数が悪く悪手を連発します。



本プロジェクトではDXライブラリを使用しています(https://dxlib.xsrv.jp/index.html)

DX Library Copyright (C) 2001-2021 Takumi Yamada.

-about Copyright-
please refer to the file "copyright_list.txt".
