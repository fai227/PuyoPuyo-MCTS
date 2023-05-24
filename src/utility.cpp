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
