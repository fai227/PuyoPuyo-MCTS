#pragma once

#include <vector>
#include <cstdlib>

using namespace std;

vector<int> puyo_list;
vector<int> garbage_list;

void reset_all(int next, int next_next, vector<int> garbages)
{
    puyo_list.clear();
    garbage_list.clear();

    puyo_list.push_back(next);
    puyo_list.push_back(next_next);

    for (int garbage : garbages)
    {
        garbage_list.push_back(garbage);
    }
}

void reset_random()
{
    int next = puyo_list.at(0);
    int next_next = puyo_list.at(1);

    puyo_list.clear();

    puyo_list.push_back(next);
    puyo_list.push_back(next_next);
}

int get_puyo(int turn)
{
    // generate next puyos
    while (turn >= puyo_list.size())
    {
        int puyo = ((rand() % 4 + 1) << SHIFT) + (rand() % 4 + 1);
        puyo_list.push_back(puyo);
    }
    return puyo_list.at(turn);
}

int get_garbage(int turn)
{
    return garbage_list.at(turn % garbage_list.size());
}
