
/*
 * File:   evaltestclass.cpp
 * Author: freddy
 *
 * Created on 08-Jun-2020, 20:21:02
 */

#include "evaltestclass.h"
#include "board.h"
#include "eval.h"
#include "action.h"

CPPUNIT_TEST_SUITE_REGISTRATION(evaltestclass);

evaltestclass::evaltestclass() {
}

evaltestclass::~evaltestclass() {
}

void evaltestclass::setUp() {
}

void evaltestclass::tearDown() {
}

void evaltestclass::testEvaluate_material() {
    bitboard bb[12];
    bb[0]   = 0x000000000000fe00;
    bb[1]   = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4]  = 0x0000000000000008;
    bb[5]   = 0x0000000000000010;

    bb[6]   = 0x00ff000000000000;
    bb[7]   = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10]  = 0x0800000000000000;
    bb[11]   = 0x1000000000000000;
    
    bool castling[4] = {true,true,true,true};
    
    board _board( bb, castling, false, 0, 4, 4, black, -105);
    
    CPPUNIT_ASSERT( _board.evaluate_material() == -100 ) ;
}

void evaltestclass::testEvaluate_pieceSquareTables() {
    bitboard bb[12];
    bb[0]   = 0x000000000000fe00;
    bb[1]   = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4]  = 0x0000000000000008;
    bb[5]   = 0x0000000000000010;

    bb[6]   = 0x00ff000000000000;
    bb[7]   = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10]  = 0x0800000000000000;
    bb[11]   = 0x1000000000000000;
    
    bool castling[4] = {true,true,true,true};
    
    board _board( bb, castling, false, 0, 4, 4, black, -105);
    
   CPPUNIT_ASSERT( _board.evaluate_pieceSquareTables() == -5 );
}

void evaltestclass::testEvaluate() {
    bitboard bb[12];
    bb[0]   = 0x000000000000fe00;
    bb[1]   = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4]  = 0x0000000000000008;
    bb[5]   = 0x0000000000000010;

    bb[6]   = 0x00ff000000000000;
    bb[7]   = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10]  = 0x0800000000000000;
    bb[11]   = 0x1000000000000000;
    
    bool castling[4] = {true,true,true,true};
    
    board _board( bb, castling, false, 0, 4, 4, black, -105);
    
    CPPUNIT_ASSERT( _board.evaluate() == -105 );
}


void evaltestclass::testIncremental_evaluation() {

    board _board;
    
    move_t e2e4 ( 12, 28, 0, 0, 0, 1 );
    move_t e7e5 ( 52, 36, 0, 0, 0, 1 );
    move_t b1c3 (  1, 18, 0, 0, 0, 1 );
    move_t f8b4 ( 61, 25, 0, 0, 0, 1 );
    move_t c3b5 ( 18, 33, 0, 0, 0, 1 );
    move_t b4d2 ( 25, 11, 0, 0, 0, 1 );
    
    _board = doMove( e2e4 );
    _board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( e7e5 );
    _board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( b1c3 );
    _board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( f8b4 );
    _board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( c3b5 );
    _board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( b4d2 );
    _board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );

}

