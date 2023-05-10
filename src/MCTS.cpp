#include <stdio.h>

void parse_arguments(int, char *[]);

int rankings[22];

int main(int argc, char *argv[])
{
    // 引数をパースする
    parse_arguments(argc, argv);

    // MCTS開始
    for (int MCTS_DURATION = 0; MCTS_DURATION < 1000; MCTS_DURATION++)
    {
    }

    // 結果を標準出力
    return 0;
}

void parse_arguments(int argc, char *argv[])
{
}