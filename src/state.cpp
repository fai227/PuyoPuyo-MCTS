#pragma once

#include <cmath>
#include <cstdlib>

#include "node.cpp"
#include "constant.cpp"

const float c_constant = sqrt(2);

class State
{
private:
    float N_simulation_time;
    float w_value;

    State()
    {
    }

public:
    bool has_unsearched_node()
    {
        return true;
    }

    State *get_max_uct_state()
    {
        State *test_state;
        return test_state;
    }

    void expand()
    {
    }

    void simulate_and_backpropagate()
    {
    }

    // プレイアウト（相手を倒したら1，指定ターン数耐えたらテトリスの高さ/25，負けたら-1）
    static float playout(State *state, int required_survival_steps)
    {
        // 盤面をコピー
        Node *current_node;
        current_node->copy(state->node);

        // 必要ターン数を超えるか，ゲームが終了するまでランダムに続ける
        int turn;
        for (turn = 1; turn <= required_survival_steps && !current_node->gameover; turn++)
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
