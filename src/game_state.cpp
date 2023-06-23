#pragma once

#include "constant.cpp"
#include "utility.cpp"

using namespace std;

class Game_State
{
public:
#pragma region Field
    int board[BOARD_WIDTH][BOARD_HEIGHT];
    int first_heights[BOARD_WIDTH];

    int tetris_height;
    int garbage;

    int puyos[2];
    int action;
    int score;

    bool gameover;
    bool win;
#pragma endregion

#pragma region Methods
    void set_as_root(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int tetris_height)
    {
        copy_board(original_board, this->board);

        this->tetris_height = tetris_height;
        this->garbage = 0;

        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            first_heights[x] = get_first_y(board, x);
        }
    }

    void set_as_child(Game_State *parent, int action, int garbage, int puyos[2])
    {
        copy_board(parent->board, this->board);

        this->tetris_height = parent->tetris_height - LINE_CLEAR_PER_ACTION;
        this->garbage = parent->garbage + garbage;

        this->puyos[0] = puyos[0];
        this->puyos[1] = puyos[1];

        this->action = action;
        this->score = 0;

        simulate();
    }

    void simulate()
    {
        place();

        calculate_score(this);

        simulate_garbage_and_check_gameover();
    }

    void place()
    {
        if (action < 6)
        {
            int x = action;
            int first_y = first_heights[x];

            board[x][first_y] = puyos[0];
            board[x][first_y + 1] = puyos[1];

            first_heights[x] += 2;

            return;
        }

        if (action < 12)
        {
            int x = action - 6;
            int first_y = first_heights[x];

            board[x][first_y + 1] = puyos[0];
            board[x][first_y] = puyos[1];

            first_heights[x] += 2;

            return;
        }

        if (action < 17)
        {
            int first_x = action - 12;
            int first_y = first_heights[first_x];

            int second_x = first_x + 1;
            int second_y = first_heights[second_x];

            board[first_x][first_y] = puyos[0];
            board[second_x][second_y] = puyos[1];

            first_heights[first_x]++;
            first_heights[second_x]++;
        }

        int first_x = action - 17;
        int first_y = first_heights[first_x];

        int second_x = first_x + 1;
        int second_y = first_heights[second_x];

        board[first_x][first_y] = puyos[1];
        board[second_x][second_y] = puyos[0];

        first_heights[first_x]++;
        first_heights[second_x]++;
    }

    void simulate_garbage_and_check_gameover()
    {
        int attack_garbage = score / 70;
        garbage -= attack_garbage;

        // attacking
        if (garbage < 0)
        {
            int attack_line = convert_garbage_to_line(-garbage);
            tetris_height += attack_line;
            garbage = 0;

            // check win
            if (tetris_height >= TETRIS_GAMEOVER_LINE)
            {
                gameover = true;
                win = true;
            }
        }
        else
        {
            int current_garbage = garbage;
            if (current_garbage > 30)
            {
                current_garbage = 30;
                garbage -= 30;
            }
            else
            {
                garbage = 0;
            }

            // drop garbages
            int garbage_line = current_garbage / 6;
            int garbage_puyo = current_garbage % 6;
            if (garbage_line > 0)
            {
                for (int y = 0; y < garbage_line; y++)
                {
                    for (int x = 0; x < BOARD_WIDTH; x++)
                    {
                        if (first_heights[x] >= BOARD_HEIGHT)
                            continue;

                        board[x][first_heights[x]] = 5;
                        first_heights[x]++;
                    }
                }
            }
            if (garbage_puyo > 0)
            {
                vector<int> x_positions{0, 1, 2, 3, 4, 5};
                vector<int> garbage_positions;
                for (int x = 0; x < garbage_puyo; x++)
                {
                    int index = rand() % x_positions.size();
                    garbage_positions.push_back(x_positions.at(index));
                    x_positions.erase(cbegin(x_positions) + index);
                }
                for (int x : garbage_positions)
                {
                    if (first_heights[x] >= BOARD_HEIGHT)
                        continue;
                    board[x][first_heights[x]] = 5;
                    first_heights[x]++;
                }
            }
        }

        // check lose
        if (board[2][11] != 0)
        {
            gameover = true;
            win = false;
        }
    }

    bool can_place(int action)
    {
        if (action < 6)
        {
            int x = action;
            return first_heights[x] < 12;
        }
        if (action < 12)
        {
            int x = action - 6;
            return first_heights[x] < 12;
        }
        if (action < 17)
        {
            int first_x = action - 12;
            int second_x = first_x + 1;

            return first_heights[first_x] < 13 && first_heights[second_x] < 13;
        }

        int first_x = action - 17;
        int second_x = first_x + 1;
        return first_heights[first_x] < 13 && first_heights[second_x] < 13;
    }
#pragma endregion

#pragma region Static Mehods
    static int get_first_y(int board[BOARD_WIDTH][BOARD_HEIGHT], int x)
    {
        int first_y;
        for (first_y = 0; first_y < BOARD_HEIGHT; first_y++)
        {
            if (board[x][first_y] == 0)
                break;
        }

        return first_y;
    }

    static void calculate_score(Game_State *state)
    {
        int score = 0;
        int chain = 1;

        while (true)
        {
            bool is_chained = false;
            bool colors[4] = {false};
            int vanish_puyo = 0;
            int connected_bonus = 0;

            // check connection
            bool checked_board[BOARD_WIDTH][BOARD_HEIGHT] = {false};

            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                for (int y = 0; y < BOARD_HEIGHT; y++)
                {
                    // skip when already checked
                    if (checked_board[x][y])
                        continue;

                    int check_point = 0;
                    int check_color = state->board[x][y];

                    // skip when empty or garbage
                    if (check_color == 0 || check_color == 5)
                    {
                        checked_board[x][y] = true;
                        continue;
                    }

                    vector<pair<int, int>> check_queue;
                    pair<int, int> current_position(x, y);
                    check_queue.push_back(current_position);

                    while (check_queue.size() > check_point)
                    {
                        pair<int, int> check_position = check_queue.at(check_point);
                        check_point++;

                        int check_x = check_position.first;
                        int check_y = check_position.second;

                        checked_board[check_x][check_y] = true;

                        // check right
                        if (check_x < BOARD_WIDTH - 1)
                        {
                            if (!checked_board[check_x + 1][check_y])
                            {
                                if (state->board[check_x + 1][check_y] == check_color)
                                {
                                    pair<int, int> right_position(check_x + 1, check_y);
                                    check_queue.push_back(right_position);
                                }
                            }
                        }

                        // check left
                        if (check_x > 0)
                        {
                            if (!checked_board[check_x - 1][check_y])
                            {
                                if (state->board[check_x - 1][check_y] == check_color)
                                {
                                    pair<int, int> left_position(check_x - 1, check_y);
                                    check_queue.push_back(left_position);
                                }
                            }
                        }

                        // check up
                        if (check_y < BOARD_HEIGHT - 1)
                        {
                            if (!checked_board[check_x][check_y + 1])
                            {
                                if (state->board[check_x][check_y + 1] == check_color)
                                {
                                    pair<int, int> up_position(check_x, check_y + 1);
                                    check_queue.push_back(up_position);
                                }
                            }
                        }

                        // check down
                        if (check_y > 0)
                        {
                            if (!checked_board[check_x][check_y - 1])
                            {
                                if (state->board[check_x][check_y - 1] == check_color)
                                {
                                    pair<int, int> down_position(check_x, check_y - 1);
                                    check_queue.push_back(down_position);
                                }
                            }
                        }
                    }

                    // when found 4 or more connection
                    if (check_queue.size() >= 4)
                    {
                        is_chained = true;
                        colors[check_color - 1] = true;
                        vanish_puyo += check_queue.size();
                        connected_bonus += get_connected_score(check_queue.size());

                        // vanish puyos
                        for (pair<int, int> check_position : check_queue)
                        {
                            int check_x = check_position.first;
                            int check_y = check_position.second;

                            state->board[check_x][check_y] = 0;

                            // vanish garbage around
                            // right
                            if (check_x < BOARD_WIDTH - 1)
                                if (state->board[check_x + 1][check_y] == 5)
                                    state->board[check_x + 1][check_y] = 0;
                            // left
                            if (check_x > 0)
                                if (state->board[check_x - 1][check_y] == 5)
                                    state->board[check_x - 1][check_y] = 0;
                            // up
                            if (check_y < BOARD_HEIGHT - 1)
                                if (state->board[check_x][check_y + 1] == 5)
                                    state->board[check_x][check_y + 1] = 0;
                            // down
                            if (check_y > 0)
                                if (state->board[check_x][check_y - 1] == 5)
                                    state->board[check_x][check_y - 1] = 0;
                        }
                    }
                }
            }

            if (is_chained)
            {
                // colors
                int color = 0;
                for (bool is_color : colors)
                    if (is_color)
                        color++;

                // score calculation
                int bonus = (get_chain_score(chain) + connected_bonus + get_multiple_color_score(color));
                if (bonus == 0)
                    bonus++;
                score += vanish_puyo * 10 * bonus;

                chain++;
            }
            else
            {
                // no chains
                break;
            }

            drop_puyos(state);
        }

        state->score = score;
    }

    static void drop_puyos(Game_State *state)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            int first_y = get_first_y(state->board, x);

            for (int y = first_y; y < BOARD_HEIGHT; y++)
            {
                if (state->board[x][y] != 0)
                {
                    state->board[x][first_y] = state->board[x][y];
                    state->board[x][y] = 0;
                    first_y++;
                }
            }

            state->first_heights[x] = first_y;
        }
    }

    static int get_chain_score(int chain)
    {
        if (chain >= 19)
        {
            return CHAIN_SCORES[18];
        }
        return CHAIN_SCORES[chain - 1];
    }

    static int get_multiple_color_score(int number_of_color)
    {
        return MULTIPLE_COLOR_SCORES[number_of_color - 1];
    }

    static int get_connected_score(int number_of_connected_puyos)
    {
        int number = number_of_connected_puyos - 4;
        if (number >= 8)
        {
            return CONNECTED_SCORES[7];
        }
        return CONNECTED_SCORES[number];
    }

    static int convert_garbage_to_line(int garbage)
    {
        int line = 0;
        int score = garbage * 70;

        while (score >= GARBAGE_TO_LINE_CONVERSION_SCORES[0])
        {
            for (int i = 1; i < 10; i++)
            {
                if (GARBAGE_TO_LINE_CONVERSION_SCORES[i] > score)
                {
                    score -= GARBAGE_TO_LINE_CONVERSION_SCORES[i - 1];
                    line += i;
                    break;
                }
                else if (i == 9)
                {
                    score -= GARBAGE_TO_LINE_CONVERSION_SCORES[9];
                    line += 10;
                    break;
                }
            }
        }

        return line;
    }

#pragma endregion
};