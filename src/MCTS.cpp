#include "constant.cpp"
#include "game_state.cpp"
#include "node.cpp"
#include "utility.cpp"
#include "generator.cpp"

#include <iostream> //入出力
#include <chrono>   // 時間計測
#include <stdlib.h> // 文字数値変換
#include <vector>   // 可変超配列

using namespace std;

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
int tetris_height = 15;

void parse_arguments(int argc, char *argv[])
{
    // calculation time
    mcts_duration_ms = atoi(argv[1]);

    // board
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            original_board[x][y] = ctoi(argv[2 + x][y]);
        }
    }

    // next
    int next_puyo = (ctoi(argv[7][0]) << SHIFT) + ctoi(argv[8][0]);

    // next next
    int next_next_puyo = (ctoi(argv[10][0]) << SHIFT) + ctoi(argv[11][0]);

    // height of tetris
    tetris_height = atoi(argv[12]);

    // garbage calculation
    vector<int> garbages;
    for (int p = 13; p < argc; p++)
    {
        garbages.push_back(atoi(argv[p]));
    }

    // reset generator
    reset_all(next_puyo, next_next_puyo, garbages);
}

void MCTS()
{
    // set original game state
    Game_State *root_state = new Game_State();
    root_state->set_as_root(original_board, tetris_height);

    Node *root_node = new Node();
    root_node->set_as_root(root_state);

    // search
    int counter = 0;
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (counter = 0; chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() < mcts_duration_ms; counter++)
    // while (true)
    {
        // ----- Selection -----
        Node *selected_node = root_node->select();

        // When no searchable node found
        if (selected_node == nullptr)
        {
            cout << "Puyo Filled\n";
            break;
        }

        // Display searching node
        // cout << "Node: " << selected_node->name << "\n";

        // ----- Expand, Simulate and Backpropagate -----
        selected_node->expand_and_simulate_and_backpropagate();
        // cout << "Simulated\n\n";
    }

    cout << "\nCalculation Ended\n"
         << counter << " times\n\n";

    int max_index = 0;
    float max_uct = root_node->child_nodes.at(max_index)->UCT_value;
    for(int i = 1; i < root_node->child_nodes.size(); i++) {
        if(root_node->child_nodes.at(i)->UCT_value > max_uct) {
            max_uct = root_node->child_nodes.at(i)->UCT_value;
            max_index = i;
        }
    }
    cout << "Best action is " << max_index << "\n with win rate :" << max_uct << "\n";
}

int main(int argc, char *argv[])
{

    // 引数をパースする
    // parse_arguments(argc, argv);

    vector<int> garbages;
    garbages.push_back(1);
    garbages.push_back(2);

    int next_puyo = (1 << SHIFT) + 1;
    int next_next_puyo = (2 << SHIFT) + 2;

    reset_all(next_puyo, next_next_puyo, garbages);

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