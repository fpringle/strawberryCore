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

//#include "move.h"
#include <cstdint>
#include <iostream>

// define 64-bit integer

using bitboard = unsigned long long int;

// declare starting bitboards

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

extern const char symbols[12];

extern const char colours[2];


// define enums for colour, piece and colourPiece

enum colour {white,black};
enum piece {pawn,rook,knight,bishop,queen,king};
enum colourPiece {whitePawn,whiteRook,whiteKnight,
                  whiteBishop,whiteQueen,whiteKing,
                  blackPawn,blackRook,blackKnight,
                  blackBishop,blackQueen,blackKing};

// the main event
class board {

  bitboard pieceBoards[12];
  
  bool castleWhiteKingSide;
  bool castleWhiteQueenSide;
  bool castleBlackKingSide;
  bool castleBlackQueenSide;
  
  int halfMoveClock;
  
  bool lastMoveDoublePawnPush;
  int dPPFile;        // if lastMoveDoublePawnPush = true, epFile gives the file
                      // of the pawn that just double moved
  
  colour sideToMove;

  // maintain a running value
  int32_t value = 0;

  public:
    // constructors
    board();
    board(bitboard*,bool*,bool,int,uint8_t,colour,int32_t);
    board(board&);
    
    // operator overloading
    bool operator==(const board&);
    bool operator!=(const board&);

    // get data
    void getBitboards(bitboard*);
    void getCastlingRights(bool*);
    void getEP(bool*);
    void getdPPFile(int*);
    void getClock(int*);
    void getSide(colour*);

    // evaluation
    int16_t evaluate_material();
    int16_t evaluate_pieceSquareTables();
    int32_t getValue();
    int32_t evaluate();
        //void incrementValue(int16_t);
    
    // utilities for testing
    void print_board( std::ostream& cout = std::cout);
    void print_all( std::ostream& cout = std::cout);
    int FEN(char*);

    // set data
    void set_piece(colourPiece,int);
    void set_side(colour);

    // move generation
    bitboard whiteSquares();
    bitboard blackSquares();
    bitboard takenSquares();// { return (whiteSquares() | blackSquares()); }
    bitboard emptySquares();// { return ( ~ takenSquares()); }
    int gen_moves(struct move_t*);

    // hashing
    uint64_t zobrist_hash();

    // check
    bool is_check(colour);
    bool is_checkmate(colour);
};

#endif
