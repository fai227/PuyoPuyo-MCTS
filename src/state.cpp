#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>

#include "node.cpp"
#include "constant.cpp"

const float c_constant = sqrt(2);

using namespace std;

class State
{
private:
    float N_simulation_time;
    float w_value;
    Node *node;

    vector<int> unsearched_action;
    vector<int> searched_action;
    vector<State *> child_states;

public:
    void set_as_root(Node *node)
    {
        this->node = node;
    }

    State *select()
    {
        // 未探索の状態があればそこを返す
        if (this->has_unsearched_node())
        {
            return this;
        }

        // 一つ先の状態で，UCT値が高いものからチェック（ソート済み）
        for (State *child_state : child_states)
        {
            State *check_state = child_state->select();
            if (check_state != nullptr)
            {
                return check_state;
            }
        }

        // 該当するものがないのでnullを返す
        return nullptr;
    }

    bool has_unsearched_node()
    {
        return true;
    }

    void expand()
    {
    }

    void simulate_and_backpropagate()
    {
    }

    Node *get_node()
    {
        return this->node;
    }

    // プレイアウト（相手を倒したら1，指定ターン数耐えたらテトリスの高さ/25，負けたら-1）
    static float playout(State *state)
    {
        // 盤面をコピー
        Node *current_node;
        current_node->copy(state->get_node());

        // 必要ターン数を超えるか，ゲームが終了するまでランダムに続ける
        int turn;
        for (turn = 1; turn <= REQUIRED_SURVIVAL_STEPS && !current_node->gameover; turn++)
        {
            // 置けるまで手を探す
            int action = rand() % ACTION_LENGTH;
            while (!current_node->can_place(action))
            {
                action = rand() % ACTION_LENGTH;
            }

            // 手を置く
            current_node->place(action);
        }

        // ゲーム終了時は，1か-1
        if (current_node->gameover)
        {
            return current_node->win ? 1.0 : -1.0;
        }

        // 継続時はテトリスの高さを正規化
        return current_node->tetris_height / TETRIS_GAMEOVER_LINE;
    }
};
