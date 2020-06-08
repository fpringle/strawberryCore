
/*
 * File:   boardtestclass.cpp
 * Author: freddy
 *
 * Created on 08-Jun-2020, 16:34:45
 */

#include "boardtestclass.h"
#include "board.h"
#include <sstream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(boardtestclass);

boardtestclass::boardtestclass() {
}

boardtestclass::~boardtestclass() {
}

void boardtestclass::setUp() {
}

void boardtestclass::tearDown() {
}

void boardtestclass::testBoard() {
    board _board;
    bitboard real_pb[12];
    bool castling[4];
    bool ep;
    int clk;
    colour side;
    uint32_t value;
    
    int i;
    
    bitboard whitePawnStart   = 0x000000000000ff00;
    bitboard whiteRookStart   = 0x0000000000000081;
    bitboard whiteKnightStart = 0x0000000000000042;
    bitboard whiteBishopStart = 0x0000000000000024;
    bitboard whiteQueenStart  = 0x0000000000000008;
    bitboard whiteKingStart   = 0x0000000000000010;
    
    bitboard blackPawnStart   = 0x00ff000000000000;
    bitboard blackRookStart   = 0x8100000000000000;
    bitboard blackKnightStart = 0x4200000000000000;
    bitboard blackBishopStart = 0x2400000000000000;
    bitboard blackQueenStart  = 0x0800000000000000;
    bitboard blackKingStart   = 0x1000000000000000;
    
    bitboard pb[12];
    
    pb[0]  = whitePawnStart;
    pb[1]  = whiteRookStart;
    pb[2]  = whiteKnightStart;
    pb[3]  = whiteBishopStart;
    pb[4]  = whiteQueenStart;
    pb[5]  = whiteKingStart;
    pb[6]  = blackPawnStart;
    pb[7]  = blackRookStart;
    pb[8]  = blackKnightStart;
    pb[9]  = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;
    
    _board.getBitboards( real_pb );
    _board.getCastlingRights( castling );
    _board.getEP( &ep );
    _board.getClock( &clk );
    _board.getSide( &side );
    value = _board.getValue( );
    
    for ( i=0;i<12;i++) {
        if ( pb[i] != real_pb[i] ) {
            CPPUNIT_ASSERT(false);            
        }
    }
    
    for ( i=0; i<4; i++) {
        if ( ! castling[i] ) {
            CPPUNIT_ASSERT(false);
        }
    }
    
    if ( ep || clk || value || side != white ) {
        CPPUNIT_ASSERT(false);
    }
}

void boardtestclass::testBoard2() {
    //board _board;
    bool castling[4] = {1,0,1,0};
    bool castling_real[4];
    bool ep = true;
    bool ep_real;
    int dPPFile = 3;
    int dPPFile_real;
    int clk = 12;
    int clk_real;
    colour side = black;
    colour side_real;
    uint32_t value = -251;
    uint32_t value_real;
    
    int i;
    
    bitboard whitePawnStart   = 0x000000a00000ff00;
    bitboard whiteRookStart   = 0x0032000000000081;
    bitboard whiteKnightStart = 0x00000aaa00000042;
    bitboard whiteBishopStart = 0x00a37d0000000024;
    bitboard whiteQueenStart  = 0x0000000000000008;
    bitboard whiteKingStart   = 0x0000345000000010;
    
    bitboard blackPawnStart   = 0x00ff000000000000;
    bitboard blackRookStart   = 0x8100001000a000b0;
    bitboard blackKnightStart = 0x4200aa0bb0cc0100;
    bitboard blackBishopStart = 0x24000011000abc03;
    bitboard blackQueenStart  = 0x08000008810a0000;
    bitboard blackKingStart   = 0x10000bbbbcbcb000;
    
    bitboard pb[12];
    bitboard pb_real[12];
    
    pb[0]  = whitePawnStart;
    pb[1]  = whiteRookStart;
    pb[2]  = whiteKnightStart;
    pb[3]  = whiteBishopStart;
    pb[4]  = whiteQueenStart;
    pb[5]  = whiteKingStart;
    pb[6]  = blackPawnStart;
    pb[7]  = blackRookStart;
    pb[8]  = blackKnightStart;
    pb[9]  = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;
    
    board _board(pb,castling,ep,dPPFile,clk,side,value);
    
    _board.getBitboards( pb_real );
    _board.getCastlingRights( castling_real );
    _board.getEP( &ep_real );
    _board.getClock( &clk_real );
    _board.getSide( &side_real );
    value_real = _board.getValue( );
    
    for ( i=0;i<12;i++) {
        if ( pb[i] != pb_real[i] ) {
            CPPUNIT_ASSERT(false);            
        }
    }
    
    for ( i=0; i<4; i++) {
        if ( castling[i] != castling_real[i] ) {
            CPPUNIT_ASSERT(false);
        }
    }
    
    if ( ep != ep_real || clk != clk_real || value != value_real || side != side_real ) {
        CPPUNIT_ASSERT(false);
    }
}

void boardtestclass::testBoard3() {
    //board _board;
    bool castling[4] = {1,0,1,0};
    bool castling_real[4];
    bool ep = true;
    bool ep_real;
    int dPPFile = 3;
    int dPPFile_real;
    int clk = 12;
    int clk_real;
    colour side = black;
    colour side_real;
    uint32_t value = -251;
    uint32_t value_real;
    
    int i;
    
    bitboard whitePawnStart   = 0x000000a00000ff00;
    bitboard whiteRookStart   = 0x0032000000000081;
    bitboard whiteKnightStart = 0x00000aaa00000042;
    bitboard whiteBishopStart = 0x00a37d0000000024;
    bitboard whiteQueenStart  = 0x0000000000000008;
    bitboard whiteKingStart   = 0x0000345000000010;
    
    bitboard blackPawnStart   = 0x00ff000000000000;
    bitboard blackRookStart   = 0x8100001000a000b0;
    bitboard blackKnightStart = 0x4200aa0bb0cc0100;
    bitboard blackBishopStart = 0x24000011000abc03;
    bitboard blackQueenStart  = 0x08000008810a0000;
    bitboard blackKingStart   = 0x10000bbbbcbcb000;
    
    bitboard pb[12];
    bitboard pb_real[12];
    
    pb[0]  = whitePawnStart;
    pb[1]  = whiteRookStart;
    pb[2]  = whiteKnightStart;
    pb[3]  = whiteBishopStart;
    pb[4]  = whiteQueenStart;
    pb[5]  = whiteKingStart;
    pb[6]  = blackPawnStart;
    pb[7]  = blackRookStart;
    pb[8]  = blackKnightStart;
    pb[9]  = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;
    
    board _board2(pb,castling,ep,dPPFile,clk,side,value);
    board _board = _board2;
    
    if ( _board != _board2 ) CPPUNIT_ASSERT(false);
    
    _board.getBitboards( pb_real );
    _board.getCastlingRights( castling_real );
    _board.getEP( &ep_real );
    _board.getClock( &clk_real );
    _board.getSide( &side_real );
    value_real = _board.getValue( );
    
    for ( i=0;i<12;i++) {
        if ( pb[i] != pb_real[i] ) {
            CPPUNIT_ASSERT(false);            
        }
    }
    
    for ( i=0; i<4; i++) {
        if ( castling[i] != castling_real[i] ) {
            CPPUNIT_ASSERT(false);
        }
    }
    
    if ( ep != ep_real || clk != clk_real || value != value_real || side != side_real ) {
        CPPUNIT_ASSERT(false);
    }
}

void boardtestclass::testPrint_board() {
    board _board;
    std::stringstream ss;
    _board.print_board( ss );
    
    std::string real = "   A B C D E F G H\n\n"
            
                       "8  R N B Q K B N R  8\n"
                       "7  P P P P P P P P  7\n"
                       "6  . . . . . . . .  6\n"
                       "5  . . . . . . . .  5\n"
                       "4  . . . . . . . .  4\n"
                       "3  . . . . . . . .  3\n"
                       "2  p p p p p p p p  2\n"
                       "1  r n b q k b n r  1\n\n"
            
                       "   A B C D E F G H\n";
    
    if ( ss.str() != real ) CPPUNIT_ASSERT(false);
}


void boardtestclass::testPrint_all() {
    board _board;
    std::stringstream ss;
    _board.print_all( ss );
    
    std::string real = "   A B C D E F G H\n\n"
            
                       "8  R N B Q K B N R  8\n"
                       "7  P P P P P P P P  7\n"
                       "6  . . . . . . . .  6\n"
                       "5  . . . . . . . .  5\n"
                       "4  . . . . . . . .  4\n"
                       "3  . . . . . . . .  3\n"
                       "2  p p p p p p p p  2\n"
                       "1  r n b q k b n r  1\n\n"
            
                       "   A B C D E F G H\n\n"
            
                       "Side to move:\n"
                       "  White\n\n"
            
                       "Castling rights:\n"
                       "  White can castle kingside\n"
                       "  White can castle queenside\n"
                       "  Black can castle kingside\n"
                       "  Black can castle queenside\n\n"
            
                       "En passant:\n"
                       "  last move was not a double pawn push\n\n"
            
                       "Halfmove Clock:\n"
                       "  0\n";
    
    if ( ss.str() != real ) CPPUNIT_ASSERT(false);
}


void boardtestclass::testFEN() {
    board _board;
    
    char c1[55] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0";
    char c2[55];
    
    _board.FEN( c2 );
    c2[54] = '\0';
    
    std::cout << c1 << std::endl;
    std::cout << c2 << std::endl;
    
    for (int i=0;i<54;i++) {
        if ( c1[i] != c2[i] ) CPPUNIT_ASSERT(false);
    }
    
}

void boardtestclass::testSet_piece() {
    bitboard bb[12];
    bb[0]   = 0x000000000001ff00;
    bb[1]   = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4]  = 0x0000000000000008;
    bb[5]   = 0x1000000000000010;

    bb[6]   = 0x00ff000000000000;
    bb[7]   = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10]  = 0x0800000000000000;
    bb[11]   = 0x1000000000000000;
    
    bool castling[] = {1,1,1,1};
    
    board _board1;
    board _board2(bb,castling,0,0,0,white,0);
    
    _board1.set_piece( whitePawn, 16 );
    _board1.set_piece( whiteKing, 60 );
    
    if ( _board1 != _board2 ) CPPUNIT_ASSERT(false);
}


void boardtestclass::testSet_side() {
    bitboard bb[12];
    bb[0]   = 0x000000000001ff00;
    bb[1]   = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4]  = 0x0000000000000008;
    bb[5]   = 0x1000000000000010;

    bb[6]   = 0x00ff000000000000;
    bb[7]   = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10]  = 0x0800000000000000;
    bb[11]   = 0x1000000000000000;
    
    bool castling[] = {1,1,1,1};
    
    board _board1(bb,castling,0,0,0,black,0);
    board _board2(bb,castling,0,0,0,white,0);
    
    _board1.set_side( white );
    
    if ( _board1 != _board2 ) CPPUNIT_ASSERT(false);
}

void boardtestclass::testWhiteSquares() {
    board _board;
    if ( _board.whiteSquares() != 0x000000000000ffff ) CPPUNIT_ASSERT(false);
}

void boardtestclass::testBlackSquares() {
    board _board;
    if ( _board.blackSquares() != 0xffff000000000000 ) CPPUNIT_ASSERT(false);
}

void boardtestclass::testTakenSquares() {
    board _board;
    if ( _board.takenSquares() != 0xffff00000000ffff ) CPPUNIT_ASSERT(false);
}

void boardtestclass::testEmptySquares() {
    board _board;
    if ( _board.emptySquares() != 0x0000ffffffff0000 ) CPPUNIT_ASSERT(false);
}