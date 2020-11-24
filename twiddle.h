// bit twiddling functions

#ifndef __TWIDDLE_H
#define __TWIDDLE_H

#include <math.h>
#include "move.h"
#include "typedefs.h"

namespace chessCore {

#define FSB first_set_bit
#define ITER_BITBOARD(ind, bb) \
    for(ind = FSB(bb); bb; bb &= (bb - 1ULL), ind = FSB(bb))



// constant masks
/**@{*/
/** Some constant bitboard mask, corresponding to useful files and rows. */
constexpr bitboard fileA = 0x0101010101010101;
constexpr bitboard fileB = 0x0202020202020202;
constexpr bitboard fileG = 0x4040404040404040;
constexpr bitboard fileH = 0x8080808080808080;
constexpr bitboard rankOne = 0x00000000000000FF;
constexpr bitboard rankTwo = 0x000000000000FF00;
constexpr bitboard rankSeven = 0x00FF000000000000;
constexpr bitboard rankEight = 0xFF00000000000000;
constexpr bitboard notFileA = ~fileA;
constexpr bitboard notFileB = ~fileB;
constexpr bitboard notFileG = ~fileG;
constexpr bitboard notFileH = ~fileH;
constexpr bitboard notRankOne = ~rankOne;
constexpr bitboard notRankEight = ~rankEight;
/**@}*/

constexpr int count_bits_set(bitboard v) {
    // thanks Brian Kernighan
    int c=0;
    for (; v; c++) {
        v &= v - 1;
    }
    return c;
}

constexpr int first_set_bit(bitboard v) {
    //returns the position of the rightmost set bit
    return log2(v & -v);
}

constexpr int last_set_bit(bitboard v) {
    //returns the position of the leftmost set bit
    return log2(v);
}

constexpr bool is_bit_set(bitboard v, int i) {
    return ( v & (1ULL << i));
}

constexpr bitboard set_bit(bitboard v, int i) {
    return v | (1ULL << i);
}

constexpr bitboard unset_bit(bitboard v, int i) {
    return ~((~v) | (1ULL << i));
}

// moving functions

constexpr bitboard oneN(bitboard b) {
    return b << N;
}

constexpr bitboard oneS(bitboard b) {
    return b >> -S;
}

constexpr bitboard oneE(bitboard b) {
    return (b << E) & notFileA;
}

constexpr bitboard oneW(bitboard b) {
    return (b >> -W) & notFileH;
}

constexpr bitboard oneNE(bitboard b) {
    return (b << NE) & notFileA;
}

constexpr bitboard oneSE(bitboard b) {
    return (b >> -SE) & notFileA;
}

constexpr bitboard oneSW(bitboard b) {
    return (b >> -SW) & notFileH;
}

constexpr bitboard oneNW(bitboard b) {
    return (b << NW) & notFileH;
}

constexpr bitboard oneGeneral8(bitboard b, int d) {
    switch (d) {
    case 0:
        return oneN(b);
        break;
    case 1:
        return oneNE(b);
        break;
    case 2:
        return oneE(b);
        break;
    case 3:
        return oneSE(b);
        break;
    case 4:
        return oneS(b);
        break;
    case 5:
        return oneSW(b);
        break;
    case 6:
        return oneW(b);
        break;
    case 7:
        return oneNW(b);
        break;
    default:
        return b;
        break;
    }
}

constexpr bitboard twoN(bitboard b) {
    return b << 2 * N;
}

constexpr bitboard twoS(bitboard b) {
    return b >> -2 * S;
}

constexpr bitboard oneNNE(bitboard b) {
    return (b << NNE) & notFileA;
}

constexpr bitboard oneENE(bitboard b) {
    return (b << ENE) & notFileA & notFileB;
}

constexpr bitboard oneESE(bitboard b) {
    return (b >> -ESE) & notFileA & notFileB;
}

constexpr bitboard oneSSE(bitboard b) {
    return (b >> -SSE) & notFileA;
}

constexpr bitboard oneSSW(bitboard b) {
    return (b >> -SSW) & notFileH;
}

constexpr bitboard oneWSW(bitboard b) {
    return (b >> -WSW) & notFileH & notFileG;
}

constexpr bitboard oneWNW(bitboard b) {
    return (b << WNW) & notFileH & notFileG;
}

constexpr bitboard oneNNW(bitboard b) {
    return (b << NNW) & notFileH;
}



} // end of chessCore namespace

#endif
