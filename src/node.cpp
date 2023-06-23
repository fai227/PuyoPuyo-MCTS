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

    int n_node_trials;
    float w_win_values;
    float UCT_value;

    bool searched;

    Node()
    {
        searched = false;
        UCT_value = 0.0;
        n_node_trials = 0;
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

        game_state = new Game_State();
        game_state->set_as_child(parent_node->game_state, action, get_garbage(level), get_puyo(level));
    }

    Node *select()
    {
        // When unsearched node found
        if (!searched)
        {
            return this;
        }

        // calculate all UCT value
        for(int i = 0; i < child_nodes.size(); i++) {
            Node* child_node = child_nodes.at(i);

            float win_rate = child_node->w_win_values / child_node->n_node_trials;
            float unsearched_rate = c_constant * sqrt(log(this->n_node_trials) / child_node->n_node_trials);

            child_node->UCT_value = win_rate + unsearched_rate;
        }

        // get the max node
        while(child_nodes.size() > 0) {
            // find highest
            float max_index = 0;
            float max_uct = child_nodes.at(max_index)->UCT_value;

            for(int i = 1; i < child_nodes.size(); i++) {
                if(child_nodes.at(i)->UCT_value > max_uct) {
                    max_uct = child_nodes.at(i)->UCT_value;
                    max_uct = i;
                }
            }

            Node *check_node = child_nodes.at(max_index)->select();
            if(check_node != nullptr) {
                return check_node;
            }

            child_nodes.erase(cbegin(child_nodes) + max_index);
        }

        // no node fulfills
        return nullptr;
    }

    void expand_and_simulate_and_backpropagate()
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
            child_nodes.push_back(next_node);

            // playout and calculate win value
            float win_value = 0.0;
            int trial = 0;
            for (int i = 0; i < MCTS_RANDOM_TRIALS; i++)
            {
                win_value += playout(next_node);
                trial++;
            }

            // backpropagate until root
            Node *check_node = next_node;
            while (check_node->parent_node != nullptr)
            {
                check_node->w_win_values += win_value;
                check_node->n_node_trials += trial;

                check_node = check_node->parent_node;
            }
        }

        searched = true;
    }

    /**
    void recalculate_uct()
    {
        float win_rate = w_win_values / n_node_trials;
        float unsearched_rate = c_constant * sqrt(log(parent_node->n_node_trials) / n_node_trials);

        this->UCT_value = win_rate + unsearched_rate;
    }
    */

    // defeat enemy = 1, endure desired turns = height of tetri / tetris deadline, lose = -1
    static float playout(Node *node)
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

            Game_State *tmp_state = current_state;
            current_state = new_state;

            if (turn > 1) 
            {
                delete tmp_state;
            }
        }

        if (current_state->gameover)
        {
            return current_state->win ? 1.0 : -1.0;
        }

        return current_state->tetris_height / TETRIS_GAMEOVER_LINE;
    }
};