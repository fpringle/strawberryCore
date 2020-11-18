
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
#include <cstdint>

using namespace chessCore;

CPPUNIT_TEST_SUITE_REGISTRATION(boardtestclass);

boardtestclass::boardtestclass() { }

boardtestclass::~boardtestclass() { }

void boardtestclass::setUp() { }

void boardtestclass::tearDown() { }

void boardtestclass::testBoard() {
    board _board;
    bitboard pb_real[12];
    bool castling[4];
    bool ep;
    uint8_t clk;
    colour side;
    uint32_t value;

    int i;

    bitboard whitePawnStart = 0x000000000000ff00;
    bitboard whiteRookStart = 0x0000000000000081;
    bitboard whiteKnightStart = 0x0000000000000042;
    bitboard whiteBishopStart = 0x0000000000000024;
    bitboard whiteQueenStart = 0x0000000000000008;
    bitboard whiteKingStart = 0x0000000000000010;

    bitboard blackPawnStart = 0x00ff000000000000;
    bitboard blackRookStart = 0x8100000000000000;
    bitboard blackKnightStart = 0x4200000000000000;
    bitboard blackBishopStart = 0x2400000000000000;
    bitboard blackQueenStart = 0x0800000000000000;
    bitboard blackKingStart = 0x1000000000000000;

    bitboard pb[12];

    pb[0] = whitePawnStart;
    pb[1] = whiteRookStart;
    pb[2] = whiteKnightStart;
    pb[3] = whiteBishopStart;
    pb[4] = whiteQueenStart;
    pb[5] = whiteKingStart;
    pb[6] = blackPawnStart;
    pb[7] = blackRookStart;
    pb[8] = blackKnightStart;
    pb[9] = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;

    _board.getBitboards(pb_real);
    _board.getCastlingRights(castling);
    _board.getEP(&ep);
    _board.getClock(&clk);
    _board.getSide(&side);
    value = _board.getValue();

    std::stringstream ss;

    for (i = 0; i < 12; i++) {
        ss << "pieceBoard comparison failed for colourPiece " << i;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), pb[i] == pb_real[i]);
        ss.str("");
    }

    for (i = 0; i < 4; i++) {
        ss << "castling wrong at index " << i;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), castling[i]);
        ss.str("");
    }

    if (ep || clk || value || side != white) {
        CPPUNIT_FAIL("flags wrong");
    }
}

void boardtestclass::testBoard2() {
    //board _board;
    bool castling[4] = {1, 0, 1, 0};
    bool castling_real[4];
    bool ep = true;
    bool ep_real;
    int dPPFile = 3;
    int dPPFile_real;
    uint8_t clk = 12;
    uint8_t clk_real;
    uint8_t full_clk = 30;
    uint8_t full_clk_real;
    colour side = black;
    colour side_real;
    uint32_t open_value = -251;
    uint32_t end_value = -200;
    uint32_t open_value_real;
    uint32_t end_value_real;

    int i;

    bitboard whitePawnStart = 0x000000a00000ff00;
    bitboard whiteRookStart = 0x0032000000000081;
    bitboard whiteKnightStart = 0x00000aaa00000042;
    bitboard whiteBishopStart = 0x00a37d0000000024;
    bitboard whiteQueenStart = 0x0000000000000008;
    bitboard whiteKingStart = 0x0000345000000010;

    bitboard blackPawnStart = 0x00ff000000000000;
    bitboard blackRookStart = 0x8100001000a000b0;
    bitboard blackKnightStart = 0x4200aa0bb0cc0100;
    bitboard blackBishopStart = 0x24000011000abc03;
    bitboard blackQueenStart = 0x08000008810a0000;
    bitboard blackKingStart = 0x10000bbbbcbcb000;

    bitboard pb[12];
    bitboard pb_real[12];

    pb[0] = whitePawnStart;
    pb[1] = whiteRookStart;
    pb[2] = whiteKnightStart;
    pb[3] = whiteBishopStart;
    pb[4] = whiteQueenStart;
    pb[5] = whiteKingStart;
    pb[6] = blackPawnStart;
    pb[7] = blackRookStart;
    pb[8] = blackKnightStart;
    pb[9] = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;

    board _board(pb, castling, ep, dPPFile, clk, full_clk, side, open_value, end_value);

    _board.getBitboards(pb_real);
    _board.getCastlingRights(castling_real);
    _board.getEP(&ep_real);
    _board.getClock(&clk_real);
    _board.getFullClock(&full_clk_real);
    _board.getSide(&side_real);
    open_value_real = _board.getOpeningValue();
    end_value_real = _board.getEndgameValue();

    std::stringstream ss;

    for (i = 0; i < 12; i++) {
        ss << "pieceBoard comparison failed for colourPiece " << i;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), pb[i] == pb_real[i]);
        ss.str("");
    }

    for (i = 0; i < 4; i++) {
        ss << "castling wrong at index " << i;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), castling[i] == castling_real[i]);
        ss.str("");
    }

    CPPUNIT_ASSERT_MESSAGE("ep wrong", ep == ep_real);
    CPPUNIT_ASSERT_MESSAGE("half move clock wrong", clk == clk_real);
    CPPUNIT_ASSERT_MESSAGE("full move clock wrong", full_clk == full_clk_real);
    CPPUNIT_ASSERT_MESSAGE("opening value wrong", open_value == open_value_real);
    CPPUNIT_ASSERT_MESSAGE("endgame value wrong", end_value == end_value_real);
    CPPUNIT_ASSERT_MESSAGE("side wrong", side == side_real);
}

void boardtestclass::testBoard3() {
    //board _board;
    bool castling[4] = {1, 0, 1, 0};
    bool castling_real[4];
    bool ep = true;
    bool ep_real;
    int dPPFile = 3;
    int dPPFile_real;
    uint8_t clk = 12;
    uint8_t clk_real;
    uint8_t full_clk = 30;
    uint8_t full_clk_real;
    colour side = black;
    colour side_real;
    uint32_t open_value = -251;
    uint32_t end_value = -200;
    uint32_t open_value_real;
    uint32_t end_value_real;

    int i;

    bitboard whitePawnStart = 0x000000a00000ff00;
    bitboard whiteRookStart = 0x0032000000000081;
    bitboard whiteKnightStart = 0x00000aaa00000042;
    bitboard whiteBishopStart = 0x00a37d0000000024;
    bitboard whiteQueenStart = 0x0000000000000008;
    bitboard whiteKingStart = 0x0000345000000010;

    bitboard blackPawnStart = 0x00ff000000000000;
    bitboard blackRookStart = 0x8100001000a000b0;
    bitboard blackKnightStart = 0x4200aa0bb0cc0100;
    bitboard blackBishopStart = 0x24000011000abc03;
    bitboard blackQueenStart = 0x08000008810a0000;
    bitboard blackKingStart = 0x10000bbbbcbcb000;

    bitboard pb[12];
    bitboard pb_real[12];

    pb[0] = whitePawnStart;
    pb[1] = whiteRookStart;
    pb[2] = whiteKnightStart;
    pb[3] = whiteBishopStart;
    pb[4] = whiteQueenStart;
    pb[5] = whiteKingStart;
    pb[6] = blackPawnStart;
    pb[7] = blackRookStart;
    pb[8] = blackKnightStart;
    pb[9] = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;

    board _board2(pb, castling, ep, dPPFile, clk, full_clk, side, open_value, end_value);
    board _board = _board2;

    CPPUNIT_ASSERT_MESSAGE("_board1, _board2 not equal", _board == _board2);

    _board.getBitboards(pb_real);
    _board.getCastlingRights(castling_real);
    _board.getEP(&ep_real);
    _board.getClock(&clk_real);
    _board.getFullClock(&full_clk_real);
    _board.getSide(&side_real);
    open_value_real = _board.getOpeningValue();
    end_value_real = _board.getEndgameValue();

    std::stringstream ss;

    for (i = 0; i < 12; i++) {
        ss << "pieceBoard comparison failed for colourPiece " << i;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), pb[i] == pb_real[i]);
        ss.str("");
    }

    for (i = 0; i < 4; i++) {
        ss << "castling wrong at index " << i;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), castling[i] == castling_real[i]);
        ss.str("");
    }

    CPPUNIT_ASSERT_MESSAGE("ep wrong", ep == ep_real);
    CPPUNIT_ASSERT_MESSAGE("half move clock wrong", clk == clk_real);
    CPPUNIT_ASSERT_MESSAGE("full move clock wrong", full_clk == full_clk_real);
    CPPUNIT_ASSERT_MESSAGE("opening value wrong", open_value == open_value_real);
    CPPUNIT_ASSERT_MESSAGE("endgame value wrong", end_value == end_value_real);
    CPPUNIT_ASSERT_MESSAGE("side wrong", side == side_real);
}

void boardtestclass::testPrint_bb() {
    board b;
    bitboard pb[12];
    b.getBitboards(pb);
    std::stringstream ss;
    std::stringstream ss2;
    std::string pb_real[12];
    pb_real[0] = ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            "x x x x x x x x \n"
            ". . . . . . . . \n\n";

    pb_real[1] = ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            "x . . . . . . x \n\n";

    pb_real[2] = ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". x . . . . x . \n\n";

    pb_real[3] = ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . x . . x . . \n\n";

    pb_real[4] = ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . x . . . . \n\n";

    pb_real[5] = ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . x . . . \n\n";

    pb_real[6] = ". . . . . . . . \n"
            "x x x x x x x x \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n\n";

    pb_real[7] = "x . . . . . . x \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n\n";

    pb_real[8] = ". x . . . . x . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n\n";

    pb_real[9] = ". . x . . x . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n\n";

    pb_real[10] = ". . . x . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n\n";

    pb_real[11] = ". . . . x . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n"
            ". . . . . . . . \n\n";

    for (int i = 0; i < 12; i++) {
        print_bb(pb[i], 'x', ss);
        ss2 << "printed wrong bitboard for pieceColour " << i;
        if (ss.str() != pb_real[i]) std::cout << ss.str() << pb_real[i];
        CPPUNIT_ASSERT_MESSAGE(ss2.str(), ss.str() == pb_real[i]);
        ss.str("");
        ss2.str("");
    }
}

void boardtestclass::testPrint_board() {
    board _board;
    std::stringstream ss;
    _board.print_board(ss);

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

    CPPUNIT_ASSERT_MESSAGE("error printing initial board", ss.str() == real);
}

void boardtestclass::testPrint_board_operator() {
    board _board;
    std::stringstream ss;
    ss << _board;

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

    CPPUNIT_ASSERT_MESSAGE("error printing initial board", ss.str() == real);
}

void boardtestclass::testPrint_all() {
    board _board;
    std::stringstream ss;
    _board.print_all(ss);

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
            "  0\n"

            "Fullmove Clock:\n"
            "  0\n";

    CPPUNIT_ASSERT_MESSAGE("Error print_all-ing inital board", ss.str() == real);
}

void boardtestclass::testFEN() {
    board _board;

    std::stringstream ss, ss2;
    std::string s2;

    std::string s = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    s2 = _board.FEN();

    if (s != s2) {
        std::cout << s << std::endl;
        std::cout << s2 << std::endl;
    }

    CPPUNIT_ASSERT_MESSAGE("FEN output test 1 failed", s2 == s);

    ss.str("");
    board _board2(s);
    _board.print_all(ss);
    _board2.print_all(ss2);
    //    _board.print_all( );
    //    _board2.print_all( );

    if (ss.str() != ss2.str()) {
        std::cout << ss.str() << std::endl;
        std::cout << ss2.str() << std::endl;
    }

    CPPUNIT_ASSERT_MESSAGE("FEN input test 2 failed", ss.str() == ss2.str());
}

void boardtestclass::testSet_piece() {
    bitboard bb[12];
    bb[0] = 0x000000000001ff00;
    bb[1] = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4] = 0x0000000000000008;
    bb[5] = 0x1000000000000010;

    bb[6] = 0x00ff000000000000;
    bb[7] = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10] = 0x0800000000000000;
    bb[11] = 0x1000000000000000;

    bool castling[] = {1, 1, 1, 1};

    board _board1;
    board _board2(bb, castling, 0, 0, 0, 0, white, 0, 0);

    _board1.set_piece(whitePawn, 16);
    _board1.set_piece(whiteKing, 60);

    CPPUNIT_ASSERT(_board1 == _board2);
}

void boardtestclass::testSet_side() {
    bitboard bb[12];
    bb[0] = 0x000000000001ff00;
    bb[1] = 0x0000000000000081;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4] = 0x0000000000000008;
    bb[5] = 0x1000000000000010;

    bb[6] = 0x00ff000000000000;
    bb[7] = 0x8100000000000000;
    bb[8] = 0x4200000000000000;
    bb[9] = 0x2400000000000000;
    bb[10] = 0x0800000000000000;
    bb[11] = 0x1000000000000000;

    bool castling[] = {1, 1, 1, 1};

    board _board1(bb, castling, 0, 0, 0, 0, black, 0, 0);
    board _board2(bb, castling, 0, 0, 0, 0, white, 0, 0);

    _board1.set_side(white);

    CPPUNIT_ASSERT(_board1 == _board2);
}

void boardtestclass::testWhiteSquares() {
    board _board;
    CPPUNIT_ASSERT(_board.whiteSquares() == 0x000000000000ffff);
}

void boardtestclass::testBlackSquares() {
    board _board;
    CPPUNIT_ASSERT(_board.blackSquares() == 0xffff000000000000);
}

void boardtestclass::testTakenSquares() {
    board _board;
    CPPUNIT_ASSERT(_board.takenSquares() == 0xffff00000000ffff);
}

void boardtestclass::testEmptySquares() {
    board _board;
    CPPUNIT_ASSERT(_board.emptySquares() == 0x0000ffffffff0000);
}
