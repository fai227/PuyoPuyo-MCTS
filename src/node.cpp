class Node
{
private:
    int first_empties[BOARD_WIDTH];
    int top_action;

    bool gameover;
    bool win;

public:
    // コンストラクタ
    Node()
    {
    }

    void set_as_root(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int tetris_height, int garbage)
    {
    }

    void set_as_child(Node parent, int action, int top_action, int garbage)
    {

        this->top_action = top_action;
    }

    bool is_game_ended()
    {
        return true; // gameover;
    }

    bool is_win()
    {
        return true; // win;
    }

    int get_top_action()
    {
        return top_action;
    }

    int test()
    {
        return first_empties[0];
    }
};