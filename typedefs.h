#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <cstdint>
#include <iostream>
#include <map>

namespace chessCore {

/**
 *  A 64-bit unsigned integer used to represent the locations of
 *  a particular piece on the board.
 */
using bitboard = uint64_t;

/** A signed integer used for evaluation. */
using value_t = int16_t;

using move_t = uint16_t;

/**
 *  \enum colour
 *  An Enum to represents the two colours of pieces.
 */
enum colour {white,black};

/**
 *  Print the name of a colour to an output stream.
 *
 *  \param out          The output stream to print to.
 *  \param colour       The colour to pretty-print.
 *  \return             The output stream.
 */
inline std::ostream& operator<<(std::ostream& out, const colour& colour) {
    out << ((colour == white) ? "white" : "black");
    return out;
}
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

struct record_t;
using TransTable = std::map<uint32_t, record_t>;


} // end of chessCore namespace

#endif
