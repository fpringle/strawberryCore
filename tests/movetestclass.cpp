
/*
 * File:   movetestclass.cpp
 * Author: freddy
 *
 * Created on 09-Jun-2020, 09:10:00
 */

#include "movetestclass.h"
#include "board.h"
#include "move.h"
#include "twiddle.h"
#include <string>
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION(movetestclass);

movetestclass::movetestclass() {
}

movetestclass::~movetestclass() {
}

void movetestclass::setUp() {
}

void movetestclass::tearDown() {
}

void movetestclass::testItos() {
    int i,j;
    char real[64][2];
    for ( i=0; i<8; i++ ) {
        for ( j=0; j<8; j++ ) {
            real[i*8 + j][0] = 'a' + i;
            real[i*8 + j][1] = '1' + j;
        }
    }
    
    char dest[2];
    
    for ( i=0;i<64;i++ ) {
        itos( i, dest );
        if ( dest[0] != real[i][0] ) CPPUNIT_ASSERT(false);
        if ( dest[1] != real[i][1] ) CPPUNIT_ASSERT(false);
    }
}

void movetestclass::testStoi() {
    int i,j;
    char real[64][2];
    for ( i=0; i<8; i++ ) {
        for ( j=0; j<8; j++ ) {
            real[i*8 + j][0] = 'a' + i;
            real[i*8 + j][1] = '1' + j;
        }
    }
    
    for ( i=0; i<64; i++ ) {
        if ( stoi( real[i] ) != i ) CPPUNIT_ASSERT(false);
    }
}

bool test_move(int from, int to, bool prom, bool cap, bool s1, bool s2) {
    struct move_t _move ( from, to, prom, cap, s1, s1);
    bool is_quiet = ! ( prom || cap || s1 || s2 );
    int data = from + (to * (2**6))  + (cap * (2**12)) +
               (s0 * (2**13)) + (s1 * (2**14)) + (prom * (2**15)); 
    
    if ( _move.from_sq() != from ) return false;
    if ( _move.to_sq() != to ) return false;
    if ( _move.is_quiet() != is_quiet ) return false;
    if ( _move.is_promotion() != prom ) return false;
    if ( _move.is_capture() != cap ) return false;
    if ( _move.is_ep_capture() != ( cap && s0 ) ) return false;
    if ( _move.is_doublePP() != ( s0 && ! ( s1 || cap || prom ) ) ) return false;
    if ( _move.is_kingCastle() != ( s1 && ! ( s0 || cap || prom ) ) ) return false;
    if ( _move.is_queenCastle() != ( s1 && s0 && ! ( cap || prom ) ) ) return false;
    if ( _move.is_castle() != ( s1 && ! ( cap || prom ) ) ) return false;
    if ( _move.give() != data ) return false;
    
    return true;
}

void movetestclass::testMove_T() {
    // normal capture
    CPPUNIT_ASSERT(test_move(10, 19, false,  true, false, false));
    // promotion
    CPPUNIT_ASSERT(test_move(15,  7,  true, false, false, false));
    // kingside castle
    CPPUNIT_ASSERT(test_move( 4,  6, false, false,  true, false));
    // queenside castle
    CPPUNIT_ASSERT(test_move(60, 57, false, false,  true,  true));
    // ep capture
    CPPUNIT_ASSERT(test_move(38, 45, false,  true, false,  true));
    // double pawn push
    CPPUNIT_ASSERT(test_move(50, 34, false, false, false,  true));
}

void movetestclass::testInit_rays() {
    init_rays();
    
    if ( rays[0][ 5] != 0x2020202020202000 ) CPPUNIT_ASSERT(false);
    if ( rays[1][17] != 0x4020100804000000 ) CPPUNIT_ASSERT(false);
    if ( rays[2][63] != 0x0000000000000000 ) CPPUNIT_ASSERT(false);
    if ( rays[3][11] != 0x0000000000000010 ) CPPUNIT_ASSERT(false);
    if ( rays[4][32] != 0x0000000001010101 ) CPPUNIT_ASSERT(false);
    if ( rays[5][ 0] != 0x0000000000000000 ) CPPUNIT_ASSERT(false);
    if ( rays[6][ 9] != 0x0000000000000100 ) CPPUNIT_ASSERT(false);
    if ( rays[7][55] != 0x4000000000000000 ) CPPUNIT_ASSERT(false);
    
}

void movestestclass::testPawnPushNaive() {
    int sq = 19;
    bitboard blockers = ( 1ULL << 11 ) | ( 1ULL << 25 ) |
                        ( 1ULL << 28 ) | ( 1ULL << 35 ) |
                        ( 1ULL << 39 ) | ( 1ULL << 42 ) |
                        ( 1ULL << 51 ) | ( 1ULL << 62 ) ;
    colour mc = white;
    bitboard bb_real = 0x0000000008000000;
    
    CPPUNIT_ASSERT( pawnPushNaive( sq, blockers, mc ) == bb_real ); 
}

void movestestclass::testPawnAttackNaive() {
    int sq = 19;
    colour mc = white;
    bitboard bb_real = 0x0000000014000000;
    
    CPPUNIT_ASSERT( pawnAttackNaive( sq, mc ) == bb_real ); 
}

void movestestclass::testPawnAttacks() {
    int sq = 19;
    colour mc = white;
    bitboard _white = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) ;
    
    bitboard _black = ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = 0x0000000010000000;
    
    CPPUNIT_ASSERT( pawnAttacks( sq, _white, _black, mc ) == bb_real );
}

void movestestclass::testPawnTargets() {
    int sq = 19;
    colour mc = white;
    bitboard _white = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) ;
    
    bitboard _black = ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = 0x0000000018000000;
    
    CPPUNIT_ASSERT( pawnTargets( sq, _white, _black, mc ) == bb_real );
}

void movestestclass::testKnightPushNaive() {
    int sq = 9;
    bitboard bb_real = ( 1ULL <<  3 ) | ( 1ULL << 19 ) |
                       ( 1ULL << 26 ) | ( 1ULL << 24 ) ;
    
    CPPUNIT_ASSERT( knightPushNaive( sq ) == bb_real ) ; 
}

void movestestclass::testKnightTargets() {
    int sq = 9;
    colour mc = white;
    bitboard _white = ( 1ULL << 24 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) ;
    
    bitboard _black = ( 1ULL << 26 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = ( 1ULL <<  3 ) | ( 1ULL << 19 ) |
                       ( 1ULL << 26 ) ;
    
    CPPUNIT_ASSERT( knightTargets( sq, _white, _black, mc ) == bb_real ); 
}

void movestestclass::testKingPushNaive() {
    int sq = 19;
    bitboard bb_real = ( 1ULL << 27 ) | ( 1ULL << 11 ) |
                       ( 1ULL << 20 ) | ( 1ULL << 18 ) |
                       ( 1ULL << 28 ) | ( 1ULL << 10 ) |
                       ( 1ULL << 26 ) | ( 1ULL << 12 ) ;
    
    CPPUNIT_ASSERT( kingPushNaive( sq ) == bb_real ); 
}

void movestestclass::testKingTargets() {
    int sq = 19;
    colour mc = white;
    bitboard _white = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 );
    
    bitboard _black = ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = ( 1ULL << 27 ) |
                       ( 1ULL << 20 ) | ( 1ULL << 18 ) |
                       ( 1ULL << 28 ) | ( 1ULL << 10 ) |
                       ( 1ULL << 26 ) | ( 1ULL << 12 ) ;
    
    CPPUNIT_ASSERT( kingTargets( sq, _white, _black, mc ) == bb_real ); 
}

void movestestclass::testBishopPushNaive() {
    int sq = 19;
    bitboard blockers = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) |
                      ( 1ULL << 62 ) |
                      ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = ( 1ULL <<  1 ) | ( 1ULL << 10 ) |
                       ( 1ULL << 12 ) | ( 1ULL <<  5 ) |
                       ( 1ULL << 28 ) | ( 1ULL << 26 ) |
                       ( 1ULL << 33 ) ;
    
    CPPUNIT_ASSERT( bishopPushNaive( sq, blockers ) == bb_real ); 
}

void movestestclass::testBishopTargets() {
    int sq = 19;
    colour mc = white;
    bitboard _white = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) |
                      ( 1ULL << 62 ) ;
    
    bitboard _black = ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = ( 1ULL << 27 ) |
                       ( 1ULL << 20 ) | ( 1ULL << 18 ) |
                       ( 1ULL << 28 ) | ( 1ULL << 10 ) |
                       ( 1ULL << 26 ) | ( 1ULL << 12 ) ;
    
    CPPUNIT_ASSERT( bishopPushNaiveTargets( sq, _white, _black, mc ) == bb_real ); 
}