#include "constant.cpp"
#include "node.cpp"
#include "state.cpp"
#include "utility.cpp"

#include <iostream> //���o��
#include <chrono>   // ���Ԍv��
#include <stdlib.h> // �������l�ϊ�
#include <vector>   // �ϒ��z��

using namespace std;

// �O���[�o���ϐ�
int mcts_duration_ms = 1000;
int original_board[BOARD_WIDTH][BOARD_HEIGHT] = {
    {1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 1, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 3, 2, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0},
    {4, 3, 4, 2, 3, 1, 5, 0, 0, 0, 0, 0, 0},
    {2, 4, 2, 3, 3, 1, 1, 5, 0, 0, 0, 0, 0},
};
int next_puyos[2] = {2, 2};
int next_next_puyos[2] = {1, 1};
int tetris_height = 15;
vector<int> garbages;

void parse_arguments(int argc, char *argv[])
{
    // �v�Z����
    mcts_duration_ms = atoi(argv[1]);

    // �Ֆ�
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            original_board[x][y] = ctoi(argv[2 + x][y]);
        }
    }

    // �l�N�X�g
    next_puyos[0] = ctoi(argv[7][0]);
    next_puyos[1] = ctoi(argv[8][0]);

    // �l�N�l�N
    next_next_puyos[0] = ctoi(argv[10][0]);
    next_next_puyos[1] = ctoi(argv[11][0]);

    // �e�g���X�̍���
    tetris_height = atoi(argv[12]);

    // ������܌v�Z
    for (int p = 13; p < argc; p++)
    {
        garbages.push_back(atoi(argv[p]));
    }
}

void MCTS()
{
    // ���[�g�m�[�h�𐶐�
    Node *root_node = new Node();
    root_node->set_as_root(original_board, tetris_height);

    // ���[�g��Ԑ���
    State *root_state = new State();
    root_state->set_as_root(root_node);

    // ���Ԃ��������T�����s��
    int counter;
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (counter = 0; chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() < mcts_duration_ms; counter++)
    {
        // ----- �I�� -----
        State *selected_state = root_state->select();

        // �T�����ł��Ȃ����̓G���[��f�����Ƃɂ���
        if (selected_state == nullptr)
        {
            cout << "Puyo Filled Error\n";
            break;
        }

        // ----- �T�� -----
        selected_state->expand();

        // ----- �V�~�����[�V����&�o�b�N�v���p�Q�[�V���� -----
        selected_state->simulate_and_backpropagate();
    }

    cout << "\n�v�Z�I��\n"
         << counter << "����s���܂����D";
}

int main(int argc, char *argv[])
{

    // �������p�[�X����
    // parse_arguments(argc, argv);

    // garbages.push_back(1);
    // garbages.push_back(2);

    MCTS();

    /**
    Node *top_node = new Node();
    top_node->set_as_root(original_board, tetris_height);

    top_node->board[0][10] = 2;
    Node::show_board(top_node->board);

    Node::drop_puyos(top_node->board);
    Node::show_board(top_node->board);

    Node::calculate_score(top_node);
    Node::show_board(top_node->board);

    cout << top_node->score;
    */

    // ����I��
    return 0;
}