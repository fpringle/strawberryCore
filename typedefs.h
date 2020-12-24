#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <cstdint>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

namespace chessCore {

/**
 *  A 64-bit unsigned integer used to represent the locations of
 *  a particular piece on the board.
 */
using bitboard = uint64_t;

/** A signed integer used for evaluation. */
using value_t = int16_t;

/** A constant representing the maximum value of \ref value_t. */
constexpr value_t VAL_INFINITY = std::numeric_limits<value_t>::max() - 10;

/** A 16-bit unsigned integer representing a single move. */
using move_t = uint16_t;

/** A move_t vector for move generation and reordering. */
using MoveList = std::vector<move_t>;

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

/** A hash map used for search. */
using TransTable = std::unordered_map<uint32_t, record_t>;


} // end of chessCore namespace

#endif
