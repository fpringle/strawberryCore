#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <cstdint>

namespace chessCore {

/**
 *  A 64-bit unsigned integer used to represent the locations of
 *  a particular piece on the board.
 */
using bitboard = uint64_t;

/** A signed integer used for evaluation. */
using value_t = int32_t;

/**
 *  \enum colour
 *  An Enum to represents the two colours of pieces.
 */
enum colour {white,black};

/**
 *  \enum piece
 *  An Enum to represents the six types of piece.
 */
enum piece {pawn,rook,knight,bishop,queen,king};

/**
 *  \enum colourPiece
 *  An Enum to represents the twelve combinations of colour and piece.
 */
enum colourPiece {whitePawn,whiteRook,whiteKnight,
                  whiteBishop,whiteQueen,whiteKing,
                  blackPawn,blackRook,blackKnight,
                  blackBishop,blackQueen,blackKing
                 };

/**
 *  \enum direction
 *  An Enum to represent directions for ray generation.
 */
enum direction {dirN, dirNE, dirE, dirSE, dirS, dirSW, dirW, dirNW};


} // end of chessCore namespace

#endif
