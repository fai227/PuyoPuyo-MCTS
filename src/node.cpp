const int chain_scores[19] = {0, 8, 16, 32, 64, 96, 128, 160, 192, 224, 256, 288, 352, 320, 384, 416, 448, 480, 512};
const int multiple_color_scores[4] = {0, 3, 6, 12};
const int connected_scores[8] = {0, 2, 3, 4, 5, 6, 7, 10};

class Node
{
public:
    // 状態
    int board[BOARD_WIDTH][BOARD_HEIGHT]; // 現在の盤面
    int first_height[BOARD_WIDTH];        // 最初の高さ

    // どの状態でも持つ変数
    int tetris_height; // 相手の高さ
    int garbage;       // 降ってくるおじゃまの量

    // 子ノードのみが持つ変数
    int puyos[2];   // 現在のぷよ
    int top_action; // 最初のアクション
    int action;     // 現在のアクション　
    int score;      // 置いた際のスコア
    int value;      // 評価値

    // ゲームの状態を表す変数
    bool gameover; // ゲームオーバーかどうか
    bool win;      // 勝っているかどうか

    // ルート設定
    void set_as_root(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int tetris_height)
    {
        copy_board(original_board, this->board);

        this->tetris_height = tetris_height;
        this->garbage = 0;
    }

    // 子ノード設定  ぷよを置けるかの真偽値を返す
    bool set_as_child(Node *parent, int action, int garbage, int puyos[2])
    {
        copy_board(parent->board, this->board);

        this->tetris_height = parent->tetris_height - LINE_CLEAR_PER_ACTION;
        this->garbage = parent->garbage + garbage;

        this->puyos[0] = puyos[0];
        this->puyos[1] = puyos[1];
        this->top_action = parent->top_action;
        this->action = action;
        this->score = 0;
        this->value = 0;

        // シミュレート開始
        bool can_place = place();

        // 置くことができてなければ終了
        if (!can_place)
            return false;

        // 連鎖計算
        calculate_score(this);

        // おじゃま計算
        int attack_garbage = score / 70;
        garbage -= attack_garbage;

        // 相手に攻撃が行われる時
        if (garbage < 0)
        {
            int attack_line = convert_garbage_to_line(-garbage);
            tetris_height += attack_line;
            garbage = 0;

            // 勝利判定
            if (tetris_height >= TETRIS_GAMEOVER_LINE)
            {
                gameover = true;
                win = true;
            }
        }

        // 負け判定 これ以上計算しない
        if (board[2][11] != 0)
        {
            gameover = true;
            win = false;
            return true;
        }

        // 連鎖が起きてない時
        if (score == 0)
        {
            // 置いたぷよのくっつきの数を評価

            // 置いたぷよを消した時の評価
        }
        // 連鎖が起きた場合
        else
        {
            // 連鎖のスコアを登録
            value = score;
        }

        // 盤面の形を評価

        // 計算終了
        return true;
    }

    // ぷよをおけるかを返す
    bool place()
    {
        // ぷよの設置，設置できない場合はfalseを返す
        // 上向き設置
        if (action < 6)
        {
            int x = action;
            int first_y = get_first_y(board, x);

            // 置けない場合
            if (first_y > BOARD_HEIGHT - 1)
                return false;

            // ぷよを置く
            board[x][first_y] = puyos[0];
            board[x][first_y + 1] = puyos[1];

            return true;
        }

        // 下向き設置
        if (action < 12)
        {
            int x = action - 6;
            int first_y = get_first_y(board, x);

            // 置けない場合
            if (first_y > BOARD_HEIGHT - 1)
                return false;

            // ぷよを置く
            board[x][first_y + 1] = puyos[0];
            board[x][first_y] = puyos[1];

            return true;
        }
        // 右向き設置
        if (action < 17)
        {
            int first_x = action - 12;
            int first_y = get_first_y(board, first_x);
            int second_x = first_x + 1;
            int second_y = get_first_y(board, second_x);

            // 置けない場合
            if (first_y > BOARD_HEIGHT)
                return false;
            if (second_y > BOARD_HEIGHT)
                return false;

            // ぷよを置く
            board[first_x][first_y] = puyos[0];
            board[second_x][second_y] = puyos[1];

            return true;
        }

        // 左向き設置
        int first_x = action - 17;
        int first_y = get_first_y(board, first_x);
        int second_x = first_x + 1;
        int second_y = get_first_y(board, second_x);

        // 置けない場合
        if (first_y > BOARD_HEIGHT)
            return false;
        if (second_y > BOARD_HEIGHT)
            return false;

        // ぷよを置く
        board[first_x][first_y] = puyos[1];
        board[second_x][second_y] = puyos[0];

        return true;
    }

#pragma region Static Methods
    static void copy_board(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int copied_board[BOARD_WIDTH][BOARD_HEIGHT])
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            for (int y = 0; y < BOARD_HEIGHT; y++)
            {
                copied_board[x][y] = original_board[x][y];
            }
        }
    }

    static void show_board(int board[BOARD_WIDTH][BOARD_HEIGHT])
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

    static void drop_puyos(int board[BOARD_WIDTH][BOARD_HEIGHT])
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

    static void calculate_score(Node *node)
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

    static int get_chain_score(int chain)
    {
        if (chain >= 19)
        {
            return chain_scores[18];
        }
        return chain_scores[chain - 1];
    }

    static int get_multiple_color_score(int number_of_color)
    {
        return multiple_color_scores[number_of_color - 1];
    }

    static int get_connected_score(int number_of_connected_puyos)
    {
        int number = number_of_connected_puyos - 4;
        if (number >= 8)
        {
            return connected_scores[7];
        }
        return connected_scores[number];
    }

    static int convert_garbage_to_line(int garbage)
    {
        return garbage / 2;
    }

    static int convert_line_to_garbage(int line)
    {
        return 0;
    }
#pragma endregion
};