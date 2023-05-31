int chain_scores[19] = {
    0,
    8,
    16,
    32,
    64,
    96,
    128,
    160,
    192,
    224,
    256,
    288,
    320,
    352,
    384,
    416,
    448,
    480,
    512,
};

int get_chain_score(int chain)
{
    if (chain >= 19)
    {
        return chain_scores[18];
    }
    return chain_scores[chain - 1];
}

int multiple_color_scores[4] = {0, 3, 6, 12};

int get_multiple_color_score(int number_of_color)
{
    return multiple_color_scores[number_of_color - 1];
}

int connected_scores[8] = {0, 2, 3, 4, 5, 6, 7, 10};

int get_connected_score(int number_of_connected_puyos)
{
    int number = number_of_connected_puyos - 4;
    if (number >= 8)
    {
        return connected_scores[7];
    }
    return connected_scores[number];
}