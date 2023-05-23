class Node
{
private:
    int board[BOARD_WIDTH][BOARD_HEIGHT];
    int first_empties[BOARD_WIDTH];

    int action;
    int top_action;
    int garbage;

    bool gameover;
    bool win;

    int value;

public:
    // コンストラクタ
    Node()
    {
        value = 0;
    }

    void set_as_root(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int tetris_height, int garbage)
    {
        this->garbage = garbage;
    }

    void set_as_child(Node *parent, int action, int garbage)
    {
        this->top_action = parent->get_top_action();
        this->action = action;
        this->garbage = parent->get_garbage() + garbage;

        uniform_int_distribution<int> random_test(0, 100);
        this->value = random_test(mt);
    }

    bool is_game_ended()
    {
        return false; // gameover;
    }

    bool is_win()
    {
        return false; // win;
    }

    int get_action()
    {
        return action;
    }

    void set_top_action(int top_action)
    {
        this->top_action = top_action;
    }

    int get_top_action()
    {
        return top_action;
    }

    int get_garbage()
    {
        return garbage;
    }

    int get_value()
    {
        return value;
    }

    int test()
    {
        return first_empties[0];
    }
};