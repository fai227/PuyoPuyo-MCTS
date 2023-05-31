void copy_board(int[BOARD_WIDTH][BOARD_HEIGHT], int[BOARD_WIDTH][BOARD_HEIGHT]);
int get_first_y(int[BOARD_WIDTH][BOARD_HEIGHT], int);

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

        this->tetris_height = parent->tetris_height;
        this->garbage = parent->garbage + garbage;

        this->puyos[0] = puyos[0];
        this->puyos[1] = puyos[1];
        this->top_action = parent->top_action;
        this->action = action;

        // シミュレート開始
        bool can_place = place();

        // 置くことができてなければ終了
        if (!can_place)
            return false;

        // 連鎖計算
        calculate_score(this);

        // ゲームオーバー判定
        gameover = board[2][11] != 0;

        // ゲームオーバーの場合はこれ以上計算しない
        if (gameover)
            return true;

        // 相手を倒したかをチェック

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
};