
/*
 * File:   checktestclass.cpp
 * Author: freddy
 *
 * Created on 10-Jun-2020, 19:43:00
 */

#include "checktestclass.h"
#include "board.h"
#include "move.h"
#include "action.h"

using namespace chessCore;

CPPUNIT_TEST_SUITE_REGISTRATION(checktestclass);

checktestclass::checktestclass() { }

checktestclass::~checktestclass() { }

void checktestclass::setUp() { }

void checktestclass::tearDown() { }

void checktestclass::testIs_check() {
    init_rays();

    // board 1: black in checkmate, white nothing
    bitboard pb1[12] = {0x000000000040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0007000000000000,
        0x0000000001000000,
        0x0000000000000000,
        0x2000000000000000,
        0x0200000000000000,
        0x0400000000000000};

    bool castling1[] = {0, 0, 0, 0};
    board _board1(pb1, castling1, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is in check in test 1", _board1.is_check(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is not in check in test 1", !_board1.is_check(white));


    // board 2: black in check, white nothing
    bitboard pb2[12] = {0x000000000040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0007000000000000,
        0x0000000001000000,
        0x0000000000000000,
        0x2000000000000000,
        0x0000000000000000,
        0x0400000000000000};

    bool castling2[] = {0, 0, 0, 0};
    board _board2(pb2, castling2, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is in check in test 2", _board2.is_check(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is not in check in test 2", !_board2.is_check(white));


    // board 3: black in check, white in check
    bitboard pb3[12] = {0x000000004040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0005020000000000,
        0x0000000001000000,
        0x0000000000800000,
        0x2000000000000000,
        0x0200000000000000,
        0x0400000000000000};

    bool castling3[] = {0, 0, 0, 0};
    board _board3(pb3, castling3, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is in check in test 3", _board3.is_check(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is in check in test 3", _board3.is_check(white));


    // board 4: black in check, white in checkmate
    bitboard pb4[12] = {0x000000004040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0005020000000000,
        0x0000000001000000,
        0x0000000000800000,
        0x2000000000000000,
        0x0100000000000000,
        0x0400000000000000};

    bool castling4[] = {0, 0, 0, 0};
    board _board4(pb4, castling4, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is in check in test 4", _board4.is_check(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is in check in test 4", _board4.is_check(white));


    // board 5: black in check, white in checkmate - testing "no castling through/out of check"
    bitboard pb5[12] = {0x0000000000000000,
        0x0000000000000080,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000010,

        0x0000000000000000,
        0x0000002008000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000100000000000,
        0x8000000000000000};

    bool castling5[] = {1, 0, 0, 0};
    board _board5(pb5, castling5, 0, 0, 3, 3, white, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is in check in test 5", _board5.is_check(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is in check in test 5", _board5.is_check(white));

}

void checktestclass::testIs_checkmate() {
    init_rays();

    // board 1: black in checkmate, white nothing
    bitboard pb1[12] = {0x000000000040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0007000000000000,
        0x0000000001000000,
        0x0000000000000000,
        0x2000000000000000,
        0x0200000000000000,
        0x0400000000000000};

    bool castling1[] = {0, 0, 0, 0};
    board _board1(pb1, castling1, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is in checkmate in test 1", _board1.is_checkmate(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is not in checkmate in test 1", !_board1.is_checkmate(white));


    // board 2: black in check, white nothing
    bitboard pb2[12] = {0x000000000040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0007000000000000,
        0x0000000001000000,
        0x0000000000000000,
        0x2000000000000000,
        0x0000000000000000,
        0x0400000000000000};

    bool castling2[] = {0, 0, 0, 0};
    board _board2(pb2, castling2, 0, 0, 3, 3, black, 200, 100);

    //    if ( _board2.is_checkmate( black ) ) {
    //        _board2.print_board();
    //    }
    //    
    //    move_t c8b8 ( 58, 57, 0, 0, 0, 0 );
    //    
    //    board child;
    //    child = doMove( _board2, c8b8 );
    //    std::cout << "CHILD\n";
    //    child.print_board();

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is not in checkmate in test 2", !_board2.is_checkmate(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is not in checkmate in test 2", !_board2.is_checkmate(white));


    // board 3: black in check, white in check
    bitboard pb3[12] = {0x000000004040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0005020000000000,
        0x0000000001000000,
        0x0000000000800000,
        0x2000000000000000,
        0x0200000000000000,
        0x0400000000000000};

    bool castling3[] = {0, 0, 0, 0};
    board _board3(pb3, castling3, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is not in checkmate in test 3", !_board3.is_checkmate(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is not in checkmate in test 3", !_board3.is_checkmate(white));


    // board 4: black in check, white in checkmate
    bitboard pb4[12] = {0x000000004040a000,
        0x0000000000000020,
        0x0000000000000000,
        0x0000002000000000,
        0x0000000000000008,
        0x0000000000000040,

        0x0005020000000000,
        0x0000000001000000,
        0x0000000000800000,
        0x2000000000000000,
        0x0100000000000000,
        0x0400000000000000};

    bool castling4[] = {0, 0, 0, 0};
    board _board4(pb4, castling4, 0, 0, 3, 3, black, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is not in checkmate in test 4", !_board4.is_checkmate(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is in checkmate in test 4", _board4.is_checkmate(white));


    // board 5: black in check, white in checkmate - testing "no castling through/out of check"
    bitboard pb5[12] = {0x0000000000000000,
        0x0000000000000080,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000010,

        0x0000000000000000,
        0x0000002008000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000100000000000,
        0x8000000000000000};

    bool castling5[] = {1, 0, 0, 0};
    board _board5(pb5, castling5, 0, 0, 3, 3, white, 200, 100);

    CPPUNIT_ASSERT_MESSAGE("failed to recognise that black is not in checkmate in test 5", !_board5.is_checkmate(black));
    CPPUNIT_ASSERT_MESSAGE("failed to recognise that white is in checkmate in test 5", _board5.is_checkmate(white));

}

void checktestclass::testCheck_lastmove() {
    init_rays();
    board pos3("8/2p5/3p4/KP5r/1R2ppk1/8/4P1P1/8 b - - 0 0");
    std::stringstream ss;

    move_t d6d5(43, 35, 0, 0, 0, 0);
    move_t e2e3(12, 20, 0, 0, 0, 0);
    move_t c7c5(50, 34, 0, 0, 0, 1);
    move_t b5c6(33, 42, 0, 1, 0, 1);
    move_t f4e3(29, 20, 0, 1, 0, 0);
    move_t c6c7(42, 50, 0, 0, 0, 0);
    move_t e3e2(20, 12, 0, 0, 0, 0);
    move_t c7c8(50, 58, 1, 0, 1, 0);
    move_t e2e1(12, 4, 1, 0, 0, 1);

    move_t moves[9] = {d6d5, e2e3, c7c5, b5c6, f4e3,
        c6c7, e3e2, c7c8, e2e1};

    colour side;

    for (int i = 0; i < 9; i++) {
        pos3 = doMove(pos3, moves[i]);
        pos3.getSide(&side);
        //        pos3.print_board();
        if (pos3.is_check(side) & (!pos3.was_lastmove_check(moves[i]))) {
            ss << "False negative at move " << i;
            CPPUNIT_FAIL(ss.str());
            ss.str("");
        }
        else if ((!pos3.is_check(side)) & pos3.was_lastmove_check(moves[i])) {
            ss << "False positive at move " << i;
            CPPUNIT_FAIL(ss.str());
            ss.str("");
        }
    }
}

void checktestclass::testCheck_lastmove2() {
    init_rays();
    board _board("3k4/8/8/8/8/8/8/R3K3 w KQkq - 0 0");
    //    _board.print_board();
    std::stringstream ss;

    move_t _castle(4, 2, 0, 0, 1, 1);

    colour side;

    _board = doMove(_board, _castle);
    _board.getSide(&side);
    //    _board.print_board();
    if (_board.is_check(side) & (!_board.was_lastmove_check(_castle))) {
        CPPUNIT_FAIL("False negative");
    }
    else if ((!_board.is_check(side)) & _board.was_lastmove_check(_castle)) {
        CPPUNIT_FAIL("False positive");
    }
}
