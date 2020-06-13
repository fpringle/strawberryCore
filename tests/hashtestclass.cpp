
/*
 * File:   hashtestclass.cpp
 * Author: freddy
 *
 * Created on 09-Jun-2020, 07:52:00
 */

#include "hashtestclass.h"
#include "board.h"
#include "hash.h"
#include "init.h"
#include "move.h"
#include "action.h"
#include "play.h"

CPPUNIT_TEST_SUITE_REGISTRATION(hashtestclass);

hashtestclass::hashtestclass() {
}

hashtestclass::~hashtestclass() {
}

void hashtestclass::setUp() {
}

void hashtestclass::tearDown() {
}

void hashtestclass::testInit_gen() {
    init_gen(0);
    std::stringstream ss;
    
    uint64_t real_ans[4] = { 0, 7679169796915961517ULL,
                                9928827280107095932ULL,
                                1835933433882300461ULL };
    
    for ( int i=0; i<4; i+=1) {
        ss << "MT wrong at index " << i << "\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << MT[i*100];
        CPPUNIT_ASSERT_MESSAGE( ss.str(), MT[i*100] == real_ans[i] );
        ss.str("");
    }
}

void hashtestclass::testExtract_number() {
    init_gen(0);
    std::stringstream ss;
    uint64_t ex;
    
    uint64_t real_ans[4] = { 10470742564043646991ULL,
                             18332391566201596577ULL,
                             12416377439523956594ULL,
                             10573902219460348369ULL  };
    
    for ( int i=0; i<4; i+=1) {
        ex = extract_number();
        ss << "extract_number #" << i << " wrong\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << ex;
        //std::cout << ss.str() << std::endl;
        CPPUNIT_ASSERT_MESSAGE( ss.str(), ex == real_ans[i] );
        ss.str("");
    }
}

void hashtestclass::testInit_keys() {
    init_keys(0);
    std::stringstream ss;
    
    uint64_t real_ans[5] = { 10470742564043646991ULL,
                              3678968663291648592ULL,
                              7913751394762574702ULL,
                              3587419481263851835ULL,
                                 55218894715056074ULL };
    
    for ( int i=0; i<5; i++ ) {
        ss << "keys wrong at index " << i*100 << "\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << zobristKeys[i*100];        
        CPPUNIT_ASSERT_MESSAGE( ss.str(), zobristKeys[i*100] == real_ans[i] );
        ss.str("");
    }
    
}

void hashtestclass::testZobrist_hash() {
    init_keys(0);
    
    board _board;
    
    CPPUNIT_ASSERT_MESSAGE ( "zobrist hash for starting board is wrong", _board.zobrist_hash() == 9597562740140802116ULL );
}


void hashtestclass::testIncremental_hash() {
    init(0);
    
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
    
    move_t moves[13] = { e2e4, e7e5, b1c3, f8b4, c3b5, b4d2, c1d2,
                         d8g5, d2g5, e8d8, d1d7, e8d7, e1c1       };
    
    uint64_t hsh, real_hsh;
    
    for ( int i=0; i<13; i++ ) {
        _board.print_board();
        _board = doMove( _board, moves[i] );
        _board.getHash( &hsh );
        real_hsh = _board.zobrist_hash();
//        std::cout << "calculated hash:  " << real_hsh <<std::endl;
//        std::cout << "incremental hash: " <<      hsh <<std::endl;
        CPPUNIT_ASSERT( hsh == real_hsh );
    }
    
//    _board.print_board();

}


void hashtestclass::testIncremental_hash2() {
    init(0);
    
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
    
    uint64_t hsh, real_hsh;
    
    for ( int i=0; i<9; i++ ) {
        ss << "Incremental evaluation failure at move " << i;
        pos3 = doMove( pos3, moves[i] );
        pos3.getHash( &hsh );
        real_hsh = pos3.zobrist_hash();
//        std::cout << "calculated hash:  " << real_hsh <<std::endl;
//        std::cout << "incremental hash: " <<      hsh <<std::endl;
        CPPUNIT_ASSERT_MESSAGE( ss.str(), hsh == real_hsh );
        ss.str("");
    }
    
//    pos3.print_board();

}