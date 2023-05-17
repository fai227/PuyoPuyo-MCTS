#include <iostream> //入出力
#include <chrono>   // 時間計測
#include <stdlib.h> // 文字数値変換
#include <vector>   // 可変超配列

using namespace std;

// 定数
const int BOARD_HEIGHT = 13;
const int BOARD_WIDTH = 6;
const int ACTION_LENGTH = 22;

const long long FIRST_INFINITY = 10000000000000;
const long long SECOND_INFINITY = 100000000000;
const long long SIMULATION_INFINITY = 1000000000;

#include "utility.cpp"
#include "node.cpp"

// グローバル変数
int mcts_duration_ms;
int required_survival_steps;
int top_priority_moves;
int original_board[BOARD_WIDTH][BOARD_HEIGHT];
int next_puyos[2];
int next_next_puyos[2];
int tetris_height;
int now_garbage;
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

    now_garbage = atoi(argv[15]);

    // おじゃま計算
    for (int p = 16; p < argc; p++)
    {
        garbages.push_back(atoi(argv[p]));
    }
}

void MCTS()
{
    // 探索開始ノード設定
    Node top_node;
    int top_garbage = now_garbage;
    top_node.set_as_root(original_board, tetris_height, top_garbage);

    // 2手目以降のシミュレーション対象になるリスト
    vector<Node> simulation_nodes;

    // 得点計算用のリストを0で初期化
    long long action_scores[ACTION_LENGTH];
    for (int i = 0; i < ACTION_LENGTH; i++)
        action_scores[i] = 0;

    // 1手目を展開
    for (int first_action = 0; first_action < ACTION_LENGTH; first_action++)
    {
        Node e1;
        e1.set_as_child(top_node, first_action, first_action, garbages.at(0));

        // 終了判定
        if (e1.is_game_ended())
        {
            // 勝っている時は正の評価，負けている時は負の評価
            action_scores[first_action] += e1.is_win() ? FIRST_INFINITY : -FIRST_INFINITY;
        }

        // 2手目を展開
        for (int second_action = 0; second_action < ACTION_LENGTH; second_action++)
        {
            Node e2;
            e2.set_as_child(e1, second_action, first_action, garbages.at(1));

            // 終了判定
            if (e2.is_game_ended())
            {
                // 勝っている時は正の評価，負けている時は負の評価
                action_scores[first_action] += e1.is_win() ? SECOND_INFINITY : -SECOND_INFINITY;
            }

            // 探索キューに追加
            simulation_nodes.push_back(e2);
        }
    }

    // 平均計算用の得点リストを0で初期化
    long long simulation_scores[simulation_nodes.size()];
    for (int i = 0; i < simulation_nodes.size(); i++)
        simulation_scores[i] = 0;

    // 時間の限り繰り返す
    int counter;
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (counter = 0; chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() < mcts_duration_ms; counter++)
    {
        for (int i = 0; i < simulation_nodes.size(); i++)
        {
            Node simulation_node = simulation_nodes[i];
            long long score = 0;

            // ゲーム終了判定になるまで繰り返す
            while (!simulation_node.is_game_ended())
            {
                // 全ての手を展開する
            }

            // 勝っている時は正の評価，負けている時は負の評価
            simulation_scores[i] += simulation_node.is_win() ? SIMULATION_INFINITY : -SIMULATION_INFINITY;

            // スコアを登録
            simulation_scores[i] += score;
        }
    }

    // シミュレーションの得点を計算し反映する
    for (int i = 0; i < simulation_nodes.size(); i++)
    {
        Node simulation_node = simulation_nodes[i];
        int action = simulation_node.get_top_action();

        // シミュレーションの探索結果を平均してスコアに登録
        action_scores[action] = simulation_scores[i] / counter;
    }

    // スコアを表示
    for (int i = 0; i < ACTION_LENGTH; i++)
    {
        cout << action_scores[i];
        cout << "\n";
    }
    cout << counter << " times calculated";
}

int main(int argc, char *argv[])
{
    // 引数をパースする
    parse_arguments(argc, argv);

    MCTS();
    // Node tmpNode = Node();
    // cout << tmpNode.test();

    // 結果を標準出力
    return 0;
}