#include "constant.cpp"

#include <iostream> //入出力
#include <chrono>   // 時間計測
#include <stdlib.h> // 文字数値変換
#include <vector>   // 可変超配列
#include <utility>  // ペアー
#include <random>   // ランダム

using namespace std;

// 乱数
random_device rd;
mt19937 mt(rd());

// グローバル変数
int mcts_duration_ms = 1000;
int required_survival_steps = 20;
int top_priority_moves = 5;
int original_board[BOARD_WIDTH][BOARD_HEIGHT] = {
    {1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 1, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 3, 2, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0},
    {4, 3, 4, 2, 3, 1, 5, 0, 0, 0, 0, 0, 0},
    {2, 4, 2, 3, 3, 1, 1, 5, 0, 0, 0, 0, 0},
};
int next_puyos[2] = {2, 2};
int next_next_puyos[2] = {1, 1};
int tetris_height = 15;
vector<int> garbages;
int action_votes[ACTION_LENGTH];

#include "node.cpp"
#include "state.cpp"
#include "utility.cpp"

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
    // ルートノードを生成
    Node *root_node = new Node();
    root_node->set_as_root(original_board, tetris_height);

    // ルート状態生成
    State *root_state = new State();

    // 時間がある限り探索を行う
    int counter;
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (counter = 0; chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() < mcts_duration_ms; counter++)
    {
        // ----- 選択 -----
        State *current_state = root_state;
        // シミュレーションが行われていないノードが現れるまで，下へ潜っていく
        while (!current_state->has_unsearched_node())
        {
            // UCT値を参照して次に進む
            current_state = current_state->get_max_uct_state();
        }

        // ----- 探索 -----
        current_state->expand();

        // ----- シミュレーション&バックプロパゲーション -----
        current_state->simulate_and_backpropagate();
    }

    cout << counter;
}

int main(int argc, char *argv[])
{

    // 引数をパースする
    // parse_arguments(argc, argv);

    // garbages.push_back(1);
    // garbages.push_back(2);

    MCTS();

    /**
    Node *top_node = new Node();
    top_node->set_as_root(original_board, tetris_height);

    top_node->board[0][10] = 2;
    Node::show_board(top_node->board);

    Node::drop_puyos(top_node->board);
    Node::show_board(top_node->board);

    Node::calculate_score(top_node);
    Node::show_board(top_node->board);

    cout << top_node->score;
    */

    // 正常終了
    return 0;
}