#pragma once

int ctoi(char c)
{
    return c - '0';
}

char itoc(int i)
{
    return i + '0';
}

void copy_board(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int copied_board[BOARD_WIDTH][BOARD_HEIGHT])
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            copied_board[x][y] = original_board[x][y];
        }
    }
}

void show_board(int board[BOARD_WIDTH][BOARD_HEIGHT])
{
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
    {
        cout << "｜";
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            switch (board[x][y])
            {
            case 0:
                cout << "　";
                break;
            case 1:
                cout << "🔴";
                break;
            case 2:
                cout << "🟢";
                break;
            case 3:
                cout << "🔵";
                break;
            case 4:
                cout << "🟡";
                break;
            case 5:
                cout << "🟤";
                break;
            }
        }
        cout << "｜\n";
    }
    cout << "ーーーーーーーー\n\n";
}

int get_first_y(int board[BOARD_WIDTH][BOARD_HEIGHT], int x)
{
    int first_y;
    for (first_y = 0; first_y < BOARD_HEIGHT; first_y++)
    {
        if (board[x][first_y] == 0)
            break;
    }

    return first_y;
}

void drop_puyos(int board[BOARD_WIDTH][BOARD_HEIGHT])
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        int first_y = get_first_y(board, x);

        for (int y = first_y; y < BOARD_HEIGHT; y++)
        {
            if (board[x][y] != 0)
            {
                board[x][first_y] = board[x][y];
                board[x][y] = 0;
                first_y++;
            }
        }
    }
}

void calculate_score(Node *node)
{
    int score = 0;
    int chain = 1;

    while (true)
    {
        // 連鎖計算用
        bool is_chained = false;
        bool colors[4] = {false};
        int vanish_puyo = 0;
        int connected_bonus = 0;

        // つながりチェック
        bool checked_board[BOARD_WIDTH][BOARD_HEIGHT] = {false}; // チェックしたかどうかを表す配列

        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            for (int y = 0; y < BOARD_HEIGHT; y++)
            {
                // チェック済みの場合は飛ばす
                if (checked_board[x][y])
                    continue;

                int check_point = 0;                 // 現在チェックしている場所
                int check_color = node->board[x][y]; // チェックする色

                // 空白かおじゃまなら飛ばす
                if (check_color == 0 || check_color == 5)
                {
                    checked_board[x][y] = true;
                    continue;
                }

                // チェック用キュー
                vector<pair<int, int>> check_queue;
                pair<int, int> current_position(x, y);
                check_queue.push_back(current_position);

                while (check_queue.size() > check_point)
                {
                    pair<int, int> check_position = check_queue.at(check_point);
                    check_point++;

                    int check_x = check_position.first;
                    int check_y = check_position.second;

                    // チェック済みにして次を調べる
                    checked_board[check_x][check_y] = true;

                    // 右をチェックできるかチェック
                    if (check_x < BOARD_WIDTH - 1)
                    {
                        if (!checked_board[check_x + 1][check_y])
                        {
                            if (node->board[check_x + 1][check_y] == check_color)
                            {
                                pair<int, int> right_position(check_x + 1, check_y);
                                check_queue.push_back(right_position);
                            }
                        }
                    }

                    // 左をチェックできるかチェック
                    if (check_x > 0)
                    {
                        if (!checked_board[check_x - 1][check_y])
                        {
                            if (node->board[check_x - 1][check_y] == check_color)
                            {
                                pair<int, int> left_position(check_x - 1, check_y);
                                check_queue.push_back(left_position);
                            }
                        }
                    }

                    // 上をチェックできるかチェック
                    if (check_y < BOARD_HEIGHT - 1)
                    {
                        if (!checked_board[check_x][check_y + 1])
                        {
                            if (node->board[check_x][check_y + 1] == check_color)
                            {
                                pair<int, int> up_position(check_x, check_y + 1);
                                check_queue.push_back(up_position);
                            }
                        }
                    }

                    // 下をチェックできるかチェック
                    if (check_y > 0)
                    {
                        if (!checked_board[check_x][check_y - 1])
                        {
                            if (node->board[check_x][check_y - 1] == check_color)
                            {
                                pair<int, int> down_position(check_x, check_y - 1);
                                check_queue.push_back(down_position);
                            }
                        }
                    }
                }

                // 4つ以上連結があった場合
                if (check_queue.size() >= 4)
                {
                    // 連鎖計算用
                    is_chained = true;
                    colors[check_color - 1] = true;
                    vanish_puyo += check_queue.size();
                    connected_bonus += get_connected_score(check_queue.size());

                    // ぷよを消す
                    for (pair<int, int> check_position : check_queue)
                    {
                        int check_x = check_position.first;
                        int check_y = check_position.second;

                        node->board[check_x][check_y] = 0;

                        // 周りのおじゃまを消す
                        // 右
                        if (check_x < BOARD_WIDTH - 1)
                            if (node->board[check_x + 1][check_y] == 5)
                                node->board[check_x + 1][check_y] = 0;
                        // 左
                        if (check_x > 0)
                            if (node->board[check_x - 1][check_y] == 5)
                                node->board[check_x - 1][check_y] = 0;
                        // 上
                        if (check_y < BOARD_HEIGHT - 1)
                            if (node->board[check_x][check_y + 1] == 5)
                                node->board[check_x][check_y + 1] = 0;
                        // 下
                        if (check_y > 0)
                            if (node->board[check_x][check_y - 1] == 5)
                                node->board[check_x][check_y - 1] = 0;
                    }
                }
            }
        }

        // 連鎖チェック
        if (is_chained)
        {
            // スコア計算
            // 色数計算
            int color = 0;
            for (bool is_color : colors)
                if (is_color)
                    color++;

            // スコア計算
            int bonus = (get_chain_score(chain) + connected_bonus + get_multiple_color_score(color));
            if (bonus == 0)
                bonus++;
            score += vanish_puyo * 10 * bonus;

            // 次の連鎖準備
            chain++;
        }
        else
        {
            // 連鎖がないので終了
            break;
        }

        // 全てのぷよを下に落とす
        drop_puyos(node->board);
    }

    node->score = score;
}