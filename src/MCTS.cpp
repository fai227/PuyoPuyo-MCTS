#include <iostream> //入出力
#include <chrono>   // 時間計測
#include <stdlib.h> // 文字数値変換
#include <vector>   // 可変超配列

#include "utility.cpp"
#include "node.cpp"

using namespace std;

// 定数
const int BOARD_HEIGHT = 13;
const int BOARD_WIDTH = 6;
const int ACTION_LENGTH = 22;

// グローバル変数
int mcts_duration_ms;
int required_survival_steps;
int top_priority_moves;
int original_board[BOARD_WIDTH][BOARD_HEIGHT];
int next_puyos[2];
int next_next_puyos[2];
int tetris_height;
vector<int> garbages;
int action_votes[ACTION_LENGTH];

void parse_arguments(int argc, char *argv[])
{
    // 計算時間
    mcts_duration_ms = atoi(argv[1]);

    // 何手先まで生き延びることを要求するか
    required_survival_steps = atoi(argv[2]);

    // 優先する上位の手の数
    top_priority_moves = atoi(argv[3]);

    // 盤面
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            original_board[x][y] = ctoi(argv[4 + x][y]);
        }
    }

    // ネクスト
    next_puyos[0] = ctoi(argv[9][0]);
    next_puyos[1] = ctoi(argv[10][0]);

    // ネクネク
    next_next_puyos[0] = ctoi(argv[12][0]);
    next_next_puyos[1] = ctoi(argv[13][0]);

    // テトリスの高さ
    tetris_height = atoi(argv[14]);

    // おじゃま計算
    for (int p = 15; p < argc; p++)
    {
        garbages.push_back(atoi(argv[p]));
    }
}

void MCTS()
{
    // 子を展開
    for (int first_action = 0; first_action < ACTION_LENGTH; first_action++)
    {
    }

    // 時間の限り繰り返す
    int counter;
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (counter = 0; chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() < mcts_duration_ms; counter++)
    {
        for (int s = 0; s < 10000; s++)
        {
            int tmp = 0;
            tmp++;
            tmp <<= 2;
        }
    }
    std::cout << counter;
}

// #region 便利関数
int *copy_board(int board[BOARD_WIDTH][BOARD_HEIGHT])
{
    int copied_board[BOARD_WIDTH][BOARD_HEIGHT];
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            copied_board[x][y] = board[x][y];
        }
    }
    return *copied_board;
}

int get_score(int board[BOARD_WIDTH][BOARD_HEIGHT])
{
}
// #endregion

int main(int argc, char *argv[])
{
    // 引数をパースする
    parse_arguments(argc, argv);

    // MCTS();
    Node tmpNode = Node();
    cout << tmpNode.getC();

    // 結果を標準出力
    return 0;
}