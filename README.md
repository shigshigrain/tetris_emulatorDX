# tetris_emulatorDX
テトリスエミュレーター。DXライブラリ使用。

時々雨ルル@tokidoki_rain

tetris emulatorDX(v3.6.0) latest

tetris emulatorDX(v3.4.9) old


無断でばらまかれると大変なことになりそう（小並感）
無料で遊べるテト〇スエミュレーター

(注意)当ソフトウェアの使用はご自身の責任で行ってください。気を付けてはいますが重大なバグやエラーが生じる可能性が十分にあります。特に、テンプレ機能はテキストファイルの記述内容や構成が正確であることを前提としており、また整合性チェックを行わないため不正なエラーの温床になりえます。十分に気を付けてください。

インストール： 
不要です。中のexe実行ファイルを実行すればOKです。当然、windows限定となります…。

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

・壁当ての最適化動作をするときにDASが解除されず一マス戻る等が難しい
DAS等の仕方が分かってないので許し亭許して

・テンプレ適応中にお邪魔せり上がりが生じてもテンプレデータ自体の絶対座標が補正されず、、おかしな挙動になってしまう。

・テンプレはそれぞれあらかじめ指定されたミノ順序関係の条件を満たすと採用される。
TSDであればI < Z < J または I < S < L など。実際にはテンプレを組めるのに組に行かない、あるいは途中で失敗することが多い（HOLDの利用をあまり考えていなかったので）

頑張って機能拡張したい。でも脳みそが足りていない。

☆リリースノート☆

v3.6.2 15:12 2022/01/03 ・評価関数の強化と調整　・パフェ判定の修正
それなりに動きがよくなりましたが、一方で明らかな悪手を選んで窒息するパターンも増えました。また、ライン消去の判断が遅くなりだいぶ積み上げるようになってしまいました。


v3.6.0 20:06 2021/09/09
・テンプレ機能の再復活！テンプレを再度組んでくれるようになりました！しかし、TSDテンプレの場合に頻繁に失敗する（評価値の問題と思われる）。


v3.4.9 22:14 2021/09/03
・祝！深度（ループ）探索実装！意外といい動きになりました。一方、テンプレが機能しなくなったほか以前評価関数が悪く悪手を連発します。

v3.0.7 3:17 2021/08/17

・改良準備工事

v3.0.5 0:09 2021/08/14

・テンプレ機能ver2.5に移行

テンプレ機能についてはほぼ完成形。テンプレ選択条件についてより高度に指定できるする可能性あり。

評価関数を微調整したせいで弱くなりました。深度探索と合わせて改良予定　

v3.0.0 17:47 2021/08/06

・テンプレ機能ver2に移行！　その他改良準備


本プロジェクトではDXライブラリを使用しています(https://dxlib.xsrv.jp/index.html)

DX Library Copyright (C) 2001-2021 Takumi Yamada.

-about Copyright-
please refer to the file "copyright_list.txt".
