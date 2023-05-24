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

    // 子ノード設定
    void set_as_child(Node *parent, int action, int garbage, int puyos[2])
    {
        copy_board(parent->board, this->board);

        this->tetris_height = parent->tetris_height;
        this->garbage = parent->garbage + garbage;

        this->puyos[0] = puyos[0];
        this->puyos[1] = puyos[1];
        this->top_action = parent->top_action;
        this->action = action;
    }

    void simulate()
    {
    }

    void place()
    {
    }
};