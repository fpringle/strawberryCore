#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <cstdint>

namespace chessCore {

using bitboard = uint64_t;
using value_t = int32_t;

enum colour {white,black};
enum piece {pawn,rook,knight,bishop,queen,king};
enum colourPiece {whitePawn,whiteRook,whiteKnight,
                  whiteBishop,whiteQueen,whiteKing,
                  blackPawn,blackRook,blackKnight,
                  blackBishop,blackQueen,blackKing
                 };

} // end of chessCore namespace

#endif
