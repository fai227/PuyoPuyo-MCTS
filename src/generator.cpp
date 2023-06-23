#pragma once

#include <vector>
#include <cstdlib>

using namespace std;

vector<int *> puyo_list;
vector<int> garbage_list;

void reset_all(int next[2], int next_next[2], vector<int> garbages)
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
    int *next = puyo_list.at(0);
    int next_one = next[0];
    int next_two = next[1];

    int *next_next = puyo_list.at(1);
    int next_next_one = next_next[0];
    int next_next_two = next_next[1];

    puyo_list.clear();

    int next_puyos[2] = {next_one, next_two};
    int next_next_puyos[2] = {next_next_one, next_next_two};
    puyo_list.push_back(next_puyos);
    puyo_list.push_back(next_next_puyos);
}

int *get_puyo(int turn)
{
    // generate next puyos
    while (turn > puyo_list.size())
    {
        int puyo[2] = {rand() % 4, rand() % 4};
        puyo_list.push_back(puyo);
    }
    return puyo_list.at(turn);
}

int get_garbage(int turn)
{
    return garbage_list.at(turn % garbage_list.size());
}
