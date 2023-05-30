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

// 定数
const int BOARD_HEIGHT = 13;
const int BOARD_WIDTH = 6;
const int ACTION_LENGTH = 22;

const long long FIRST_INFINITY = 10000000000000;
const long long SECOND_INFINITY = 100000000000;
const long long SIMULATION_INFINITY = 1000000000;
const long long SURVIVAL_REWARD = 10000000;

#include "utility.cpp"
#include "node.cpp"

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
    // 探索開始ノード設定
    Node *top_node = new Node();
    top_node->set_as_root(original_board, tetris_height);

    // 2手目以降のシミュレーション対象になるリスト
    vector<Node *> simulation_nodes;

    // 乱数生成用
    uniform_int_distribution<int> get_random_action(0, top_priority_moves - 1);
    uniform_int_distribution<int> get_random_puyo(1, 4);

    // 得点計算用のリストを0で初期化
    long long action_scores[ACTION_LENGTH] = {0};

    // 1手目を展開
    for (int first_action = 0; first_action < ACTION_LENGTH; first_action++)
    {
        Node *e1 = new Node();
        e1->set_as_child(top_node, first_action, garbages.at(0), next_puyos);
        e1->top_action = first_action;

        // 終了判定
        if (e1->gameover)
        {
            // 勝っている時は正の評価，負けている時は負の評価
            action_scores[first_action] += e1->win ? FIRST_INFINITY : -FIRST_INFINITY;
            continue;
        }

        // 2手目を展開
        for (int second_action = 0; second_action < ACTION_LENGTH; second_action++)
        {
            Node *e2 = new Node();
            e2->set_as_child(e1, second_action, garbages.at(1), next_next_puyos);

            // 終了判定
            if (e2->gameover)
            {
                // 勝っている時は正の評価，負けている時は負の評価
                action_scores[first_action] += e1->win ? SECOND_INFINITY : -SECOND_INFINITY;
                continue;
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
            Node *simulation_node = simulation_nodes[i];
            long long score = 0;
            int turn = 2;

            // ゲーム終了判定になるまで繰り返す
            while (!simulation_node->gameover)
            {
                // おじゃま
                int garbage = 0;
                if (turn < garbages.size())
                    garbage = garbages.at(turn);

                // 全ての手を展開する
                vector<Node *> top_moves;
                for (int action = 0; action < ACTION_LENGTH; action++)
                {
                    // ランダムなぷよの生成
                    int random_puyo[2] = {get_random_puyo(mt), get_random_puyo(mt)};

                    // 手を置く
                    Node *next_move = new Node();
                    next_move->set_as_child(simulation_node, action, garbage, random_puyo);

                    // 最善手なら残す
                    // リストが満杯のとき
                    int value = next_move->value;
                    bool need_to_delete_last_element = false;

                    if (top_moves.size() >= top_priority_moves)
                    {
                        // ランキングの最後より小さい場合はリストに入れずに継続
                        if (top_moves.back()->value > value)
                        {
                            delete next_move;
                            continue;
                        }

                        need_to_delete_last_element = true;
                    }

                    // 前からチェックしていき小さい値が存在する場合はその前に挿入
                    vector<Node *>::iterator it = top_moves.begin();
                    while (it != top_moves.end())
                    {
                        if ((*it)->value < value)
                        {
                            break;
                        }
                        ++it;
                    }
                    top_moves.insert(it, next_move);

                    if (need_to_delete_last_element)
                    {
                        // 最後尾を削除
                        top_moves.erase(top_moves.end() - 1);
                    }
                }

                // ランダム選択
                Node *rand_node = top_moves[get_random_action(mt)];

                // 1手進める
                simulation_node = rand_node;
                turn++;
            }

            // 勝っている時は正の評価，負けている時は負の評価
            simulation_scores[i] += simulation_node->win ? SIMULATION_INFINITY : -SIMULATION_INFINITY;

            // どれだけ生き延びたか得点化（100手で勝利と同じ扱い，必要手数以下なら計算しない）
            int survived_turns = (turn - required_survival_steps);
            if (survived_turns > 0)
            {
                simulation_scores[i] += survived_turns * SURVIVAL_REWARD;
            }

            // スコアを登録
            simulation_scores[i] += score;
        }
    }

    // シミュレーションの得点を計算し反映する
    for (int i = 0; i < simulation_nodes.size(); i++)
    {
        Node *simulation_node = simulation_nodes[i];
        int action = simulation_node->top_action;

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
    /**
    // 引数をパースする
    // parse_arguments(argc, argv);

    garbages.push_back(1);
    garbages.push_back(2);

    MCTS();
    */

    Node *top_node = new Node();
    top_node->set_as_root(original_board, tetris_height);

    top_node->board[0][10] = 2;
    show_board(top_node->board);

    drop_puyos(top_node->board);
    show_board(top_node->board);

    int score = calculate_score(top_node->board);
    show_board(top_node->board);

    cout << score;

    // 正常終了
    return 0;
}