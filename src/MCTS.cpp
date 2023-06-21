#include "constant.cpp"

#include <iostream> //入出力
#include <chrono>   // 時間計測
#include <stdlib.h> // 文字数値変換
#include <vector>   // 可変超配列
#include <random>   // ランダム

using namespace std;

// 乱数
random_device rd;
mt19937 mt(rd());

// グローバル変数
int mcts_duration_ms = 1000;
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

    // 盤面
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            original_board[x][y] = ctoi(argv[2 + x][y]);
        }
    }

    // ネクスト
    next_puyos[0] = ctoi(argv[7][0]);
    next_puyos[1] = ctoi(argv[8][0]);

    // ネクネク
    next_next_puyos[0] = ctoi(argv[10][0]);
    next_next_puyos[1] = ctoi(argv[11][0]);

    // テトリスの高さ
    tetris_height = atoi(argv[12]);

    // おじゃま計算
    for (int p = 13; p < argc; p++)
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
    root_state->set_as_root(root_node);

    // 時間がある限り探索を行う
    int counter;
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (counter = 0; chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() < mcts_duration_ms; counter++)
    {
        // ----- 選択 -----
        State *selected_state = root_state->select();

        // 探索ができない時はエラーを吐くことにする
        if (selected_state == nullptr)
        {
            cout << "Puyo Filled Error\n";
            break;
        }

        // ----- 探索 -----
        selected_state->expand();

        // ----- シミュレーション&バックプロパゲーション -----
        selected_state->simulate_and_backpropagate();
    }

    cout << "計算終了\n"
         << counter << "回実行しました．";
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