# About
ぷよぷよAIのための，モンテカルロ木探索を実装したものになります．


# テンプレート
- フィールド：GTR
1220000000000
1215000000000
3135000000000
4323150000000
4342315000000
2423311500000


# ぷよの色
0. なし
1. 1色目
2. 2色目
3. 3色目
4. 4色目
5. おじゃま

# 入力
- 計算時間（ms）
- 何手先まで生き延びることを要求するか
- 優先する上位の手の数
- 1列目(下から)
- 2列目(下から)
- 3列目(下から)
- 4列目(下から)
- 5列目(下から)
- 6列目(下から)
- ネクスト(スペースで区切る)
- ネクネク(スペースで区切る)
- 相手の高さ（0~20）
- 現在のおじゃま量
- 相手の火力（無限に続く，ぷよの手数が単位）

# MCTSについて
- 2手先まで全て展開する
- そこからMCTS開始
  
## ランダム探索
- ある盤面において子を展開して上位n位からランダムで選ぶ

## 探索終了後の投票
- m手以内に死ぬ場合は負の評価を，m手以上生き残る場合は正の評価を与える
- m手以上生き残る場合は，m以上に生き残った数と相手に攻撃した数値を組み合わせて評価にする

- 時間終了後は，平均を求める
- そしてそのリーフ内の最大値から1位を取っていく


# 盤面の評価について
- 得られるスコアx1
- おいた時にできた結合を消した時に得られる連鎖
- Σ連結数^2
- ちぎり
- Uの字

# ACTIONについて
- -1が一番左で左向き
- 0で一番左で上向き
- 21で一番右