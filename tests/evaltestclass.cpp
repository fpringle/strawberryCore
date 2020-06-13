
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
    init_rays();
    board _board;
    
    move_t e2e4 ( 12, 28, 0, 0, 0, 1 );
    move_t e7e5 ( 52, 36, 0, 0, 0, 1 );
    move_t b1c3 (  1, 18, 0, 0, 0, 0 );
    move_t f8b4 ( 61, 25, 0, 0, 0, 0 );
    move_t c3b5 ( 18, 33, 0, 0, 0, 0 );
    move_t b4d2 ( 25, 11, 0, 1, 0, 0 );
    move_t c1d2 (  2, 11, 0, 1, 0, 0 );
    move_t d8g5 ( 59, 38, 0, 0, 0, 0 );
    move_t d2g5 ( 11, 38, 0, 1, 0, 0 );
    move_t e8d8 ( 60, 59, 0, 0, 0, 0 );
    move_t d1d7 (  3, 51, 0, 1, 0, 0 );
    move_t e8d7 ( 58, 51, 0, 1, 0, 0 );
    move_t e1c1 (  4,  2, 0, 0, 1, 1 );
    
    
    _board = doMove( _board, e2e4 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, e7e5 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, b1c3 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, f8b4 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, c3b5 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, b4d2 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    CPPUNIT_ASSERT( _board.is_check( white ) );
    _board = doMove( _board, c1d2 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, d8g5 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, d2g5 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );   
    _board = doMove( _board, e8d8 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, d1d7 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );    
    CPPUNIT_ASSERT( _board.is_check( black ) );
    CPPUNIT_ASSERT( ! _board.is_checkmate( black ) );
    _board = doMove( _board, e8d7 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    _board = doMove( _board, e1c1 );
    //_board.print_board();
    CPPUNIT_ASSERT( _board.getValue() == _board.evaluate() );
    

}


void evaltestclass::testIncremental_evaluation2() {
    init_rays();
    board pos3 ( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 b - - 0 0" );
    std::stringstream ss;
    
    move_t d6d5 ( 43, 35, 0, 0, 0, 0 );
    move_t e2e3 ( 12, 20, 0, 0, 0, 0 );
    move_t c7c5 ( 50, 34, 0, 0, 0, 1 );
    move_t b5c6 ( 33, 42, 0, 1, 0, 1 );
    move_t f4e3 ( 29, 20, 0, 1, 0, 0 );
    move_t c6c7 ( 42, 50, 0, 0, 0, 0 );
    move_t e3e2 ( 20, 12, 0, 0, 0, 0 );
    move_t c7c8 ( 50, 58, 1, 0, 1, 1 );
    move_t e2e1 ( 12,  4, 1, 0, 0, 1 );
    
    move_t moves[9] = { d6d5, e2e3, c7c5, b5c6, f4e3,
                        c6c7, e3e2, c7c8, e2e1       };
    
    int real, calc;
//    pos3.print_board();
    
    for ( int i=0; i<9; i++ ) {
        ss << "Incremental evaluation failure at move " << i;
        pos3 = doMove( pos3, moves[i] );
//        pos3.print_board();
        real = pos3.evaluate();
        calc = pos3.getValue();
        CPPUNIT_ASSERT_MESSAGE( ss.str(), real == calc );
//        if ( real != calc ) {
//            std::cout << "move " << i << " - wrong :(" << std::endl;
//            std::cout << "    real: " << real << std::endl;
//            std::cout << "    calc: " << calc << std::endl;
//        }
//        else {
//            std::cout << "move " << i << " - correct!" << std::endl;
//            std::cout << "    real: " << real << std::endl;
//        }
        ss.str("");
    }
}