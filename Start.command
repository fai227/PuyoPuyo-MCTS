# ディレクトリ移動
cd "/Users/hayat/Documents/GitHub/PuyoPuyo-MCTS/src/"

# コンパイル
clang++ MCTS.cpp -o MCTS

for i in `seq 1`
do
    # 実行
    ./MCTS \
    500 \
    0000000020221 0000000005121 0000000005313 0000000513234 0000005132434 0000051133242
done