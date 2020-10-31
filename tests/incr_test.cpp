#include "board.h"
#include "eval.h"
#include "action.h"
#include "play.h"
#include "move.h"
#include "twiddle.h"
#include "init.h"
#include "hash.h"
#include <iostream>
#include <cstdint>

void testIncremental_evaluation() {

    board _board;

    move_t e2e4(12, 28, 0, 0, 0, 1);
    move_t e7e5(52, 36, 0, 0, 0, 1);
    move_t b1c3(1, 18, 0, 0, 0, 1);
    move_t f8b4(61, 25, 0, 0, 0, 1);
    move_t c3b5(18, 33, 0, 0, 0, 1);
    move_t b4d2(25, 11, 0, 0, 0, 1);

    _board = doMove(_board, e2e4);
    _board.print_board();
    if (_board.getValue() != _board.evaluate()) std::cout << "FUCK";
    _board = doMove(_board, e7e5);
    _board.print_board();
    if (_board.getValue() != _board.evaluate()) std::cout << "FUCK";
    _board = doMove(_board, b1c3);
    _board.print_board();
    if (_board.getValue() != _board.evaluate()) std::cout << "FUCK";
    _board = doMove(_board, f8b4);
    _board.print_board();
    if (_board.getValue() != _board.evaluate()) std::cout << "FUCK";
    _board = doMove(_board, c3b5);
    _board.print_board();
    if (_board.getValue() != _board.evaluate()) std::cout << "FUCK";
    _board = doMove(_board, b4d2);
    _board.print_board();
    if (_board.getValue() != _board.evaluate()) std::cout << "FUCK";

}

void testIncremental_hash() {
    init(0);

    board _board;

    move_t player_move;

    uint64_t hsh;

    for (int i = 0; i < 10; i++) {
        player_move = input_move(_board);
        _board = doMove(_board, player_move);
        _board.getHash(& hsh);
        if (hsh != _board.zobrist_hash()) {
            std::cout << "FUCK\n";
            break;
        }
    }

}

int main() {

    // testIncremental_evaluation();
    testIncremental_hash();

    return 0;
}