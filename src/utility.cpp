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

int calculate_score(int board[BOARD_WIDTH][BOARD_HEIGHT])
{
    int score = 0;
    int chain = 1;

    while (true)
    {
        bool is_chained = false;

        // つながりチェック
        bool checked_board[BOARD_WIDTH][BOARD_HEIGHT] = {false}; // チェックしたかどうかを表す配列

        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            for (int y = 0; y < BOARD_HEIGHT; y++)
            {
                // チェック済みの場合は飛ばす
                if (checked_board[x][y])
                    continue;

                int check_point = 0;           // 現在チェックしている場所
                int check_color = board[x][y]; // チェックする色

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
                    cout << check_queue.size() << ":" << check_point << "\n";
                    pair<int, int> check_position = check_queue.at(check_point);
                    check_point++;

                    int check_x = check_position.first;
                    int check_y = check_position.second;

                    // 同じ色かをチェック
                    if (board[check_x][check_y] != check_color)
                        continue;

                    // 同じ色であることを確認済み
                    checked_board[check_x][check_y] = true;

                    // 右をチェックできるかチェック
                    if (check_x < BOARD_WIDTH - 1)
                    {
                        if (!checked_board[check_x + 1][check_y])
                        {
                            pair<int, int> right_position(check_x + 1, check_y);
                            check_queue.push_back(right_position);
                        }
                    }

                    // 左をチェックできるかチェック
                    if (check_x > 0)
                    {
                        if (!checked_board[check_x - 1][check_y])
                        {
                            pair<int, int> left_position(check_x - 1, check_y);
                            check_queue.push_back(left_position);
                        }
                    }

                    // 上をチェックできるかチェック
                    if (check_y < BOARD_HEIGHT - 1)
                    {
                        if (!checked_board[check_x][check_y + 1])
                        {
                            pair<int, int> up_position(check_x, check_y + 1);
                            check_queue.push_back(up_position);
                        }
                    }

                    // 下をチェックできるかチェック
                    if (check_y > 0)
                    {
                        if (!checked_board[check_x][check_y - 1])
                        {
                            pair<int, int> down_position(check_x, check_y - 1);
                            check_queue.push_back(down_position);
                        }
                    }
                }

                // 連鎖が起きた時
                if (check_queue.size() >= 4)
                {
                    cout << "Chain occuered";
                }
            }
        }

        // 連鎖チェック
        if (is_chained)
        {
            chain++;
        }
        else
        {
            break;
        }

        // 全てのぷよを下に落とす
        drop_puyos(board);
    }

    return score;
}