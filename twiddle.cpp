#include "twiddle.h"

#include <math.h>

#include "move.h"
#include "typedefs.h"


namespace chessCore {

/*
// general-purpose bit twiddling functions

constexpr int count_bits_set(bitboard v) {
    // thanks Brian Kernighan
    int c;
    for (c = 0; v; c++) {
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

*/
} // end of chessCore namespace
