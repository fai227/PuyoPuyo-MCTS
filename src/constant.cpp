#pragma once

const int BOARD_HEIGHT = 13;
const int BOARD_WIDTH = 6;
const int ACTION_LENGTH = 22;

const int REQUIRED_SURVIVAL_STEPS = 20;
const int LINE_CLEAR_PER_ACTION = 1;
const int TETRIS_GAMEOVER_LINE = 25;

const int chain_scores[19] = {0, 8, 16, 32, 64, 96, 128, 160, 192, 224, 256, 288, 352, 320, 384, 416, 448, 480, 512};
const int multiple_color_scores[4] = {0, 3, 6, 12};
const int connected_scores[8] = {0, 2, 3, 4, 5, 6, 7, 10};
