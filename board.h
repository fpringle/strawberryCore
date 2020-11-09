/*
* A header file to define a "board" class to represent
* the current state of the chessboard.
*
* We are using an unsigned 64-bit integer to represent
* the positions of each type of piece. e.g. if the 10th
* bit of whiteKnight is set to 1, there is a white knight
* at position 10.
*
* For the square-mapping we use Little-Endian Rank-File,
* so 0 = A1, 7 = H1, 56 = A8, 63 = H8
*
*/

#ifndef __BOARD_H
#define __BOARD_H

#include "typedefs.h"

#include <cstdint>
#include <iostream>
#include <string>

// declare starting bitboards
// defined in board.cpp

extern bitboard whitePawnStart;
extern bitboard whiteRookStart;
extern bitboard whiteKnightStart;
extern bitboard whiteBishopStart;
extern bitboard whiteQueenStart;
extern bitboard whiteKingStart;
extern bitboard blackPawnStart;
extern bitboard blackRookStart;
extern bitboard blackKnightStart;
extern bitboard blackBishopStart;
extern bitboard blackQueenStart;
extern bitboard blackKingStart;

// define symbols for string representation
// defined in board.cpp

extern const char symbols[12];
extern const char colours[2];


// define enums for colour, piece and colourPiece

enum colour {white,black};
enum piece {pawn,rook,knight,bishop,queen,king};
enum colourPiece {whitePawn,whiteRook,whiteKnight,
                  whiteBishop,whiteQueen,whiteKing,
                  blackPawn,blackRook,blackKnight,
                  blackBishop,blackQueen,blackKing
                 };


// defined in board.cpp
void print_bb(bitboard, char c = 'x', std::ostream& cout = std::cout);

// the main event
class board {

    bitboard pieceBoards[12];

    bool castleWhiteKingSide;
    bool castleWhiteQueenSide;
    bool castleBlackKingSide;
    bool castleBlackQueenSide;

    uint8_t halfMoveClock;
    uint8_t fullMoveClock;

    bool lastMoveDoublePawnPush;
    int dPPFile;        // if lastMoveDoublePawnPush = true, dPPFile gives the file
    // of the pawn that just double moved

    colour sideToMove;

    // maintain a running value
    value_t opening_value;
    value_t endgame_value;

    // store hash of current state
    uint64_t hash_value;

public:
    // constructors
    // defined in board.cpp
    board();
    board(bitboard*, bool*, bool, int, uint8_t, uint8_t,
          colour, value_t, value_t, uint64_t hash = 0);
    board(board&);
    board(std::string);

    // operator overloading
    // defined in board.cpp
    bool operator==(const board&);
    bool operator!=(const board&);
    friend std::ostream& operator<<(std::ostream &out, const board &brd);

    // get data
    // defined in board.cpp
    void getBitboards(bitboard*) const;
    void getCastlingRights(bool*);
    void getEP(bool*);
    void getdPPFile(int*);
    void getClock(uint8_t*);
    void getFullClock(uint8_t*);
    void getSide(colour*);
    void getHash(uint64_t*);
    int num_pieces_left(colourPiece);
    int num_pieces_left();

    // evaluation
    // defined in eval.cpp
    value_t evaluate_material(int);
    value_t evaluate_pieceSquareTables(int);
    value_t getValue();
    value_t getOpeningValue();
    value_t getEndgameValue();
    value_t evaluate();
    value_t evaluateOpening();
    value_t evaluateEndgame();
    int getPhase();

    // utilities for testing
    // defined in board.cpp
    void print_board(std::ostream& cout = std::cout);
    void print_board_flipped(std::ostream& cout = std::cout);
    void print_board_indent(std::ostream& cout = std::cout, int indent = 0);
    void print_all(std::ostream& cout = std::cout);
    std::string FEN();
    //    std::string SAN_post_move(move_t);
    //    std::string SAN_pre_move (move_t);
    //    move_t move_from_SAN(std::string);
    void FEN(std::ostream&);

    // set data
    // defined in board.cpp
    void set_piece(colourPiece, int);
    void set_side(colour);
    void clear_square(int);
    void update_value();
    void update_hash();
    void set_hash(uint64_t);
    void setBitboards(bitboard*);
    void setCastlingRights(bool*);
    void setEP(bool);
    void setdPPFile(int);
    void setClock(uint8_t);
    void setFullClock(uint8_t);

    // move generation
    // defined in move.cpp
    bitboard whiteSquares();
    bitboard blackSquares();
    bitboard takenSquares();
    bitboard emptySquares();
    bool add_moves(struct move_t**, struct move_t, bool);
    int gen_moves(struct move_t*);
    bool is_legal(struct move_t);
    int gen_captures(move_t*);
    int get_out_of_check(struct move_t*, piece, int, int, bool);
    int gen_legal_moves(struct move_t*);

    // in-place action
    // defined in action.cpp
    void doMoveInPlace(move_t);

    // hashing
    // defined in hash.cpp
    uint64_t zobrist_hash();

    // check
    // defined in check.cpp
    bool is_check(colour);
    bool is_check(colour, piece*, int*, bool*);
    bool is_checkmate(colour);
    int is_checkmate();
    bool is_stalemate();
    bool gameover();
    bool was_lastmove_check(move_t);
    bool is_checking_move(move_t);
};

#endif
