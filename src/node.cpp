class Node
{
private:
    int first_empties[BOARD_WIDTH];
    int top_action;

public:
    // コンストラクタ
    Node()
    {
    }

    void SetAsRoot(int original_board[BOARD_WIDTH][BOARD_HEIGHT], int tetris_height, int garbage)
    {
    }

    void SetAsChild(Node parent, int action, int top_action, int garbage)
    {

        this->top_action = top_action;
    }

    bool isGameEnded()
    {
        return true;
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