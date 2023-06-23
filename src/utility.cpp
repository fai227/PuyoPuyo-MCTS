#pragma once

#include <iostream>

#include "constant.cpp"

using namespace std;

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
