
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
    std::stringstream ss;
    std::stringstream ss2;
    char real[64][2];
    std::string real_string[64];
    for ( i=0; i<8; i++ ) {
        for ( j=0; j<8; j++ ) {
            real[i*8 + j][0] = 'a' + j;
            real[i*8 + j][1] = '1' + i;
            real_string[i*8+j] = "  ";
            real_string[i*8+j][0] = 'a'+j;
            real_string[i*8+j][1] = '1'+i;
        }
    }
    
    char dest[2];
    std::string _string = "  ";
    
    for ( i=0;i<64;i++ ) {
        ss << "wrong string at square ";
        ss << real[i][0] << real[i][1];
        itos( i, dest );
        _string = itos(i);
        itos(i,ss2);
        CPPUNIT_ASSERT_MESSAGE ( ss.str(), dest[0] == real[i][0] );
        CPPUNIT_ASSERT_MESSAGE ( ss.str(), dest[1] == real[i][1] );
        CPPUNIT_ASSERT_MESSAGE ( ss.str(), _string == real_string[i] );
        CPPUNIT_ASSERT_MESSAGE ( ss.str(), _string == ss2.str() );
        ss.str("");
        ss2.str("");
    }
}

void movetestclass::testStoi() {
    int i,j;
    std::stringstream ss;
    char real[64][2];
    for ( i=0; i<8; i++ ) {
        for ( j=0; j<8; j++ ) {
            real[i*8 + j][0] = 'a' + j;
            real[i*8 + j][1] = '1' + i;
        }
    }
    
    for ( i=0; i<64; i++ ) {
        ss << "wrong index at square ";
        ss << real[i][0] << real[i][1];
        CPPUNIT_ASSERT_MESSAGE( ss.str(), _stoi( real[i] ) == i );
        ss.str("");
    }
}

bool test_move(int from, int to, bool prom, bool cap, bool s1, bool s0) {
    struct move_t _move ( from, to, prom, cap, s1, s0);
    bool is_quiet = ! ( prom || cap || s1 || s0 );
    int data = from + (to * (1ULL<<6))  + (cap * (1ULL<<12)) +
               (s0 * (1ULL<<13)) + (s1 * (1ULL<<14)) + (prom * (1ULL<<15)); 
    
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

void movetestclass::testMove_t() {
    // normal capture
    CPPUNIT_ASSERT( test_move(10, 19, false,  true, false, false) );
    // promotion
    CPPUNIT_ASSERT( test_move(15,  7,  true, false, false, false) );
    // kingside castle
    CPPUNIT_ASSERT( test_move( 4,  6, false, false,  true, false) );
    // queenside castle
    CPPUNIT_ASSERT( test_move(60, 57, false, false,  true,  true) );
    // ep capture
    CPPUNIT_ASSERT( test_move(38, 45, false,  true, false,  true) );
    // double pawn push
    CPPUNIT_ASSERT( test_move(50, 34, false, false, false,  true) );
}

void movetestclass::testOperatorPrint() {
    move_t move1(10, 19, false,  true, false, false);
    move_t move2(15,  7,  true, false, false, false);
    move_t move3( 4,  6, false, false,  true, false);
    move_t move4(60, 57, false, false,  true,  true);
    move_t move5(38, 45, false,  true, false,  true);
    move_t move6(50, 34, false, false, false,  true);
    
    std::cout << move1 << std::endl
              << move2 << std::endl
              << move3 << std::endl
              << move4 << std::endl
              << move5 << std::endl
              << move6 << std::endl;
}

void movetestclass::testInit_rays() {
    init_rays();
    
    CPPUNIT_ASSERT( rays[0][ 5] == 0x2020202020202000 );
    CPPUNIT_ASSERT( rays[1][17] == 0x4020100804000000 );
    CPPUNIT_ASSERT( rays[2][63] == 0x0000000000000000 );
    CPPUNIT_ASSERT( rays[3][11] == 0x0000000000000010 );
    CPPUNIT_ASSERT( rays[4][32] == 0x0000000001010101 );
    CPPUNIT_ASSERT( rays[5][ 0] == 0x0000000000000000 );
    CPPUNIT_ASSERT( rays[6][ 9] == 0x0000000000000100 );
    CPPUNIT_ASSERT( rays[7][55] == 0x4000000000000000 );
    
}

void movetestclass::testPawnPushNaive() {
    int sq = 19;
    bitboard blockers = ( 1ULL << 11 ) | ( 1ULL << 25 ) |
                        ( 1ULL << 28 ) | ( 1ULL << 35 ) |
                        ( 1ULL << 39 ) | ( 1ULL << 42 ) |
                        ( 1ULL << 51 ) | ( 1ULL << 62 ) ;
    colour mc = white;
    bitboard bb_real = 0x0000000008000000;
    
    CPPUNIT_ASSERT( pawnPushNaive( sq, blockers, mc ) == bb_real ); 
}

void movetestclass::testPawnAttackNaive() {
    int sq = 19;
    colour mc = white;
    bitboard bb_real = 0x0000000014000000;
    
    CPPUNIT_ASSERT( pawnAttackNaive( sq, mc ) == bb_real ); 
}

void movetestclass::testPawnAttacks() {
    int sq = 19;
    colour mc = white;
    bitboard _white = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) ;
    
    bitboard _black = ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = 0x0000000010000000;
    
    CPPUNIT_ASSERT( pawnAttacks( sq, _white, _black, mc ) == bb_real );
}

void movetestclass::testPawnTargets() {
    int sq = 19;
    colour mc = white;
    bitboard _white = ( 1ULL << 11 ) | ( 1ULL << 35 ) |
                      ( 1ULL << 42 ) | ( 1ULL << 62 ) ;
    
    bitboard _black = ( 1ULL << 25 ) | ( 1ULL << 28 ) |
                      ( 1ULL << 31 ) | ( 1ULL << 51 ) ;
    
    bitboard bb_real = 0x0000000018000000;
    
    CPPUNIT_ASSERT( pawnTargets( sq, _white, _black, mc ) == bb_real );
}

void movetestclass::testKnightPushNaive() {
    int sq = 9;
    bitboard bb_real = ( 1ULL <<  3 ) | ( 1ULL << 19 ) |
                       ( 1ULL << 26 ) | ( 1ULL << 24 ) ;
    
    CPPUNIT_ASSERT( knightPushNaive( sq ) == bb_real ) ; 
}

void movetestclass::testKnightTargets() {
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

void movetestclass::testKingPushNaive() {
    int sq = 19;
    bitboard bb_real = ( 1ULL << 27 ) | ( 1ULL << 11 ) |
                       ( 1ULL << 20 ) | ( 1ULL << 18 ) |
                       ( 1ULL << 28 ) | ( 1ULL << 10 ) |
                       ( 1ULL << 26 ) | ( 1ULL << 12 ) ;
    
    CPPUNIT_ASSERT( kingPushNaive( sq ) == bb_real ); 
}

void movetestclass::testKingTargets() {
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

void movetestclass::testBishopPushNaive() {
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

void movetestclass::testBishopTargets() {
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
    
    CPPUNIT_ASSERT( bishopTargets( sq, _white, _black, mc ) == bb_real ); 
}