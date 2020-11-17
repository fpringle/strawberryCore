
/*
 * File:   evaltestclass.cpp
 * Author: freddy
 *
 * Created on 08-Jun-2020, 20:21:02
 */

#include "evaltestclass.h"
#include "board.h"
#include "move.h"
#include "eval.h"
#include "init.h"
#include "action.h"

using namespace chessCore;

CPPUNIT_TEST_SUITE_REGISTRATION(evaltestclass);

evaltestclass::evaltestclass() { }

evaltestclass::~evaltestclass() { }

void evaltestclass::setUp() { }

void evaltestclass::tearDown() { }

void _testPERFTeval(board _board, int depth, int basedepth) {

    std::stringstream ss;
    ss << "Incremental evaluation wrong at depth " << basedepth - depth
       << std::endl;
    _board.print_board(ss);
    ss << "\nReal value:        " << _board.evaluate()
       << "\nIncremental value: " << _board.getValue();

    CPPUNIT_ASSERT_MESSAGE(ss.str(), _board.getValue() == _board.evaluate());

    if (depth == 0) return;
    move_t moves[256];
    int n_moves = _board.gen_legal_moves(moves);
    board child;

    for (int i = 0; i < n_moves; i++) {
        child = doMove(_board, moves[i]);
        _testPERFTeval(child, depth - 1, basedepth);
    }
}

void testPERFTeval(board _board, int depth) {
    init();
    _board.update_value();
    _testPERFTeval(_board, depth, depth);
}

void evaltestclass::incrementalEvaluationStartBoard() {
    board _board;
    for (int i = 0; i < 6; i++) {
        testPERFTeval(_board, i);
        std::cout << "verified incremental evaluation at depth " << i << std::endl;
    }
}

void evaltestclass::incrementalEvaluationPos2() {
    board _board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 0");
    for (int i = 0; i < 6; i++) {
        testPERFTeval(_board, i);
        std::cout << "verified incremental evaluation at depth " << i << std::endl;
    }
}

void evaltestclass::incrementalEvaluationPos3() {
    board _board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    for (int i = 0; i < 6; i++) {
        testPERFTeval(_board, i);
        std::cout << "verified incremental evaluation at depth " << i << std::endl;
    }
}

void evaltestclass::incrementalEvaluationPos4() {
    board _board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    for (int i = 0; i < 6; i++) {
        testPERFTeval(_board, i);
        std::cout << "verified incremental evaluation at depth " << i << std::endl;
    }
}

void evaltestclass::incrementalEvaluationPos5() {
    board _board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    for (int i = 0; i < 6; i++) {
        testPERFTeval(_board, i);
        std::cout << "verified incremental evaluation at depth " << i << std::endl;
    }
}

void evaltestclass::incrementalEvaluationPos6() {
    board _board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    for (int i = 0; i < 6; i++) {
        testPERFTeval(_board, i);
        std::cout << "verified incremental evaluation at depth " << i << std::endl;
    }
}
