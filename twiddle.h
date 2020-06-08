// bit twiddling functions

#ifndef __TWIDDLE_H
#define __TWIDDLE_H

#include "board.h"
#include "move.h"

// general-purpose bit twiddling functions
int count_bits_set(bitboard);
int first_set_bit(bitboard);
int last_set_bit(bitboard);
bool is_bit_set(bitboard,int);

extern const bitboard fileA;
extern const bitboard fileB;
extern const bitboard fileG;
extern const bitboard fileH;
extern const bitboard rankOne;
extern const bitboard rankTwo;
extern const bitboard rankSeven;
extern const bitboard rankEight;
extern const bitboard notFileA;
extern const bitboard notFileB;
extern const bitboard notFileG;
extern const bitboard notFileH;
extern const bitboard notRankOne;
extern const bitboard notRankEight;

// basic bitboard moving

bitboard oneN(bitboard);
bitboard oneS(bitboard);
bitboard oneE(bitboard);
bitboard oneW(bitboard);

bitboard oneNE(bitboard);
bitboard oneSE(bitboard);
bitboard oneSW(bitboard);
bitboard oneNW(bitboard);

bitboard oneGeneral8(bitboard,int);

bitboard twoN(bitboard);
bitboard twoS(bitboard);

bitboard oneNNE(bitboard);
bitboard oneENE(bitboard);
bitboard oneESE(bitboard);
bitboard oneSSE(bitboard);
bitboard oneSSW(bitboard);
bitboard oneWSW(bitboard);
bitboard oneWNW(bitboard);
bitboard oneNNW(bitboard);

#endif
