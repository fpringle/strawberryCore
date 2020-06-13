
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

CPPUNIT_TEST_SUITE_REGISTRATION(checktestclass);

checktestclass::checktestclass() {
}

checktestclass::~checktestclass() {
}

void checktestclass::setUp() {
}

void checktestclass::tearDown() {
}

void checktestclass::testIs_check() {
    init_rays();
    
    // board 1: black in checkmate, white nothing
    bitboard pb1[12] = { 0x000000000040a000,
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
                         0x0400000000000000  };
    
    bool castling1[] = { 0,0,0,0 };
    board _board1 ( pb1, castling1, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is in check in test 1", _board1.is_check( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is not in check in test 1", ! _board1.is_check( white ) );
    
    
    // board 2: black in check, white nothing
    bitboard pb2[12] = { 0x000000000040a000,
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
                         0x0400000000000000  };
    
    bool castling2[] = { 0,0,0,0 };
    board _board2 ( pb2, castling2, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is in check in test 2", _board2.is_check( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is not in check in test 2", ! _board2.is_check( white ) );
    
    
    // board 3: black in check, white in check
    bitboard pb3[12] = { 0x000000004040a000,
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
                        0x0400000000000000  };
    
    bool castling3[] = { 0,0,0,0 };
    board _board3 ( pb3, castling3, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is in check in test 3", _board3.is_check( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is in check in test 3", _board3.is_check( white ) );
    
    
    // board 4: black in check, white in checkmate
    bitboard pb4[12] = { 0x000000004040a000,
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
                        0x0400000000000000  };
    
    bool castling4[] = { 0,0,0,0 };
    board _board4 ( pb4, castling4, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is in check in test 4", _board4.is_check( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is in check in test 4", _board4.is_check( white ) );
    
    
    // board 5: black in check, white in checkmate - testing "no castling through/out of check"
    bitboard pb5[12] = { 0x0000000000000000,
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
                         0x8000000000000000  };
    
    bool castling5[] = { 1,0,0,0 };
    board _board5 ( pb5, castling5, 0, 0, 3, 3, white, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is in check in test 5", _board5.is_check( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is in check in test 5", _board5.is_check( white ) );
        
}

void checktestclass::testIs_checkmate() {
    init_rays();
    
    // board 1: black in checkmate, white nothing
    bitboard pb1[12] = { 0x000000000040a000,
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
                         0x0400000000000000  };
    
    bool castling1[] = { 0,0,0,0 };
    board _board1 ( pb1, castling1, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is in checkmate in test 1", _board1.is_checkmate( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is not in checkmate in test 1", ! _board1.is_checkmate( white ) );
    
    
    // board 2: black in check, white nothing
    bitboard pb2[12] = { 0x000000000040a000,
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
                         0x0400000000000000  };
    
    bool castling2[] = { 0,0,0,0 };
    board _board2 ( pb2, castling2, 0, 0, 3, 3, black, 200 );
    
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
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is not in checkmate in test 2", ! _board2.is_checkmate( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is not in checkmate in test 2", ! _board2.is_checkmate( white ) );
    
    
    // board 3: black in check, white in check
    bitboard pb3[12] = { 0x000000004040a000,
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
                        0x0400000000000000  };
    
    bool castling3[] = { 0,0,0,0 };
    board _board3 ( pb3, castling3, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is not in checkmate in test 3", ! _board3.is_checkmate( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is not in checkmate in test 3", ! _board3.is_checkmate( white ) );
    
    
    // board 4: black in check, white in checkmate
    bitboard pb4[12] = { 0x000000004040a000,
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
                        0x0400000000000000  };
    
    bool castling4[] = { 0,0,0,0 };
    board _board4 ( pb4, castling4, 0, 0, 3, 3, black, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is not in checkmate in test 4", ! _board4.is_checkmate( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is in checkmate in test 4", _board4.is_checkmate( white ) );
    
    
    // board 5: black in check, white in checkmate - testing "no castling through/out of check"
    bitboard pb5[12] = { 0x0000000000000000,
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
                         0x8000000000000000  };
    
    bool castling5[] = { 1,0,0,0 };
    board _board5 ( pb5, castling5, 0, 0, 3, 3, white, 200 );
    
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that black is not in checkmate in test 5", ! _board5.is_checkmate( black ) );
    CPPUNIT_ASSERT_MESSAGE( "failed to recognise that white is in checkmate in test 5", _board5.is_checkmate( white ) );
        
}