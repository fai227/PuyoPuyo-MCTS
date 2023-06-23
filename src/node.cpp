#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>

#include "game_state.cpp"
#include "constant.cpp"
#include "generator.cpp"

const float c_constant = sqrt(2);

using namespace std;

class Node
{
public:
    Node *parent_node;
    Game_State *game_state;
    vector<Node *> child_nodes;

    string name;
    int action;
    int level;

    int n_state_trials;
    float w_win_values;
    float UCT_value;

    bool searched;

    Node()
    {
        searched = false;
        n_state_trials = 0;
    }

    void set_as_root(Game_State *game_state)
    {
        this->level = -1;
        this->game_state = game_state;
        this->name = "Root";
    }

    void set_as_child(Node *parent_node, int action)
    {
        this->parent_node = parent_node;
        this->name = parent_node->name + "-" + to_string(action);
        this->level = parent_node->level + 1;
        this->action = action;

        Game_State *game_state = new Game_State();
        game_state->set_as_child(parent_node->game_state, action, get_garbage(level), get_puyo(level));
    }

    Node *select()
    {
        // When unsearched node found
        if (!searched)
        {
            return this;
        }

        // check from heighest uct in childs
        for (Node *child_node : child_nodes)
        {
            Node *check_node = child_node->select();
            if (check_node != nullptr)
            {
                return check_node;
            }
        }

        // no node fulfills
        return nullptr;
    }

            void expand()
            {
                // check all actions
                for (int i = 0; i < ACTION_LENGTH; i++)
                {
                    // skip when cannot place
                    if (!game_state->can_place(i))
                        continue;

                    // generate next node
                    Node *next_node = new Node();
                    next_node->set_as_child(this, i);

                    // playout and calculate win value
                    for (int i = 0; i < MCTS_RANDOM_TRIALS; i++)
                    {
                        float win_value = playout(next_node);
                    }
                }

                searched = true;
            }

        void simulate_and_backpropagate()
        {
        }

        void recalculate_uct()
        {
            float win_rate = w_win_values / n_state_trials;
            float unsearched_rate = c_constant * sqrt(log(parent_node->n_state_trials) / n_state_trials);

            this->UCT_value = win_rate + unsearched_rate;
        }

        // defeat enemy = 1, endure desired turns = height of tetri / tetris deadline, lose = -1
        static float playout(Node * node)
        {
            Game_State *current_state = node->game_state;

            int turn;
            for (turn = 1; turn <= REQUIRED_SURVIVAL_STEPS && !current_state->gameover; turn++)
            {
                // find placable action
                int action = rand() % ACTION_LENGTH;
                while (!current_state->can_place(action))
                {
                    action = rand() % ACTION_LENGTH;
                }

                // simulate next turn
                Game_State *new_state = new Game_State();
                int current_turn = node->level + turn;
                new_state->set_as_child(current_state, action, get_garbage(current_turn), get_puyo(current_turn));

                current_state = new_state;
            }

            if (current_state->gameover)
            {
                return current_state->win ? 1.0 : -1.0;
            }

            return current_state->tetris_height / TETRIS_GAMEOVER_LINE;
        }
};
