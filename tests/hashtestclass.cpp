
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

void _testPERFThash( board _board, int depth, int basedepth ) {
    uint64_t _hash;
    _board.getHash( &_hash );
    CPPUNIT_ASSERT( _board.zobrist_hash() == _hash );
    if ( depth==0 ) return;
    move_t moves[256];
    int n_moves = _board.gen_legal_moves( moves );
    board child;
    
    for ( int i=0; i<n_moves; i++ ) {
        child = doMove( _board, moves[i] );
        _testPERFThash( child, depth-1, basedepth );
    }
}

void testPERFThash( board _board, int depth ) {
    init();
    _board.update_hash();
    _testPERFThash( _board, depth, depth );
}

void hashtestclass::incrementalHashStartBoard() {
    board _board;
    for ( int i=0; i<6; i++ ) {
        testPERFThash( _board, i );
        std::cout << "verified incremental hash at depth " << i << std::endl;
    }
}

void hashtestclass::incrementalHashPos2() {
    board _board ( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 0" );
    for ( int i=0; i<6; i++ ) {
        testPERFThash( _board, i );
        std::cout << "verified incremental hash at depth " << i << std::endl;
    }
}

void hashtestclass::incrementalHashPos3() {
    board _board ( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1" );
    for ( int i=0; i<6; i++ ) {
        testPERFThash( _board, i );
        std::cout << "verified incremental hash at depth " << i << std::endl;
    }
}

void hashtestclass::incrementalHashPos4() {
    board _board ( "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" );
    for ( int i=0; i<6; i++ ) {
        testPERFThash( _board, i );
        std::cout << "verified incremental hash at depth " << i << std::endl;
    }
}

void hashtestclass::incrementalHashPos5() {
    board _board ( "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" );
    for ( int i=0; i<6; i++ ) {
        testPERFThash( _board, i );
        std::cout << "verified incremental hash at depth " << i << std::endl;
    }
}

void hashtestclass::incrementalHashPos6() {
    board _board ( "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" );
    for ( int i=0; i<6; i++ ) {
        testPERFThash( _board, i );
        std::cout << "verified incremental hash at depth " << i << std::endl;
    }
}

bool perftHash( board b, int depth ) {
    uint64_t _hsh;
    b.getHash( &_hsh );
    if ( _hsh != b.zobrist_hash() ) return false;
    if ( depth==0 ) return true;
    
    move_t moves[256];
    int n_moves = b.gen_legal_moves( moves );
    board child;
    
    for ( int i=0; i<n_moves; i++ ) {
        child = doMove( b, moves[i] );
        if ( ! perftHash( child, depth-1 ) ) return false;
    }
    
    return true;
}

void divideHash( board b, int depth ) {
    move_t moves[256];
    int n_moves = b.gen_legal_moves( moves );
    board child;
    
    for ( int i=0; i<n_moves; i++ ) {
        child = doMove( b, moves[i] );
        print_move( moves[i] );
        if ( perftHash( child, depth-1 ) ) std::cout << "   success\n";
        else                               std::cout << "   fail\n";
    }
}

void hashtestclass::dividePos2() {
    init();
    board _board( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 0" );
    
    move_t e5g6 ( 36, 46, 0, 1, 0, 0 );
    move_t h3g2 ( 23, 14, 0, 1, 0, 0 );
    move_t g6h8 ( 46, 63, 0, 1, 0, 0 );
    
//    _board = doMove( _board, e5g6 );
//    _board = doMove( _board, h3g2 );   
//    _board = doMove( _board, g6h8 );   
    
    _board.print_all(); 
    
    uint64_t hsh;
    _board.getHash( &hsh );
    CPPUNIT_ASSERT( hsh == _board.zobrist_hash() );
    
    divideHash( _board, 4 );
}