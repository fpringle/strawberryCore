// bit twiddling functions

#ifndef __TWIDDLE_H
#define __TWIDDLE_H

#include <math.h>
//#include "move.h"
#include "typedefs.h"

namespace chessCore {

#define FSB first_set_bit
#define ITER_BITBOARD(ind, bb) \
    for(ind = FSB(bb); bb; bb &= (bb - 1ULL), ind = FSB(bb))


/**@{*/
/** Constants for sliding pieces which moves in straight lines. */
constexpr int N = 8;
constexpr int NE = 9;
constexpr int E = 1;
constexpr int SE = -7;
constexpr int S = -8;
constexpr int SW = -9;
constexpr int W = -1;
constexpr int NW = 7;
/**@}*/

/**@{*/
/** Constants for knights. */
constexpr int NNE = 17;
constexpr int ENE = 10;
constexpr int ESE = -6;
constexpr int SSE = -15;
constexpr int SSW = -17;
constexpr int WSW = -10;
constexpr int WNW = 6;
constexpr int NNW = 15;
/**@}*/


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


/**
 *  Count the number of bits set in a bitboard.
 *
 *  \param v            The bitboard.
 *  \return             The number of bits set in v.
 */
constexpr int count_bits_set(bitboard v) {
    // thanks Brian Kernighan
    int c=0;
    for (; v; c++) {
        v &= v - 1;
    }
    return c;
}

/**
 *  Return the index of the first (rightmost) set bit in a bitboard.
 *
 *  \param v            The bitboard.
 *  \return             The index of the first bit set in v.
 */
constexpr int first_set_bit(bitboard v) {
    return log2(v & -v);
}

/**
 *  Return the index of the last (leftmost) set bit in a bitboard.
 *
 *  \param v            The bitboard.
 *  \return             The index of the last bit set in v.
 */
constexpr int last_set_bit(bitboard v) {
    return log2(v);
}

/**
 *  Check whether a particular bit of a bitboard is set.
 *
 *  \param v            The bitboard.
 *  \param i            The index of the bit to check.
 *  \return             A boolean indicating whether bit i is set in v.
 */
constexpr bool is_bit_set(bitboard v, int i) {
    return ( v & (1ULL << i));
}

/**
 *  Set a particular bit in a bitboard.
 *
 *  \param v            The bitboard.
 *  \param i            The index of the bit to set.
 *  \return             The resulting bitboard.
 */
constexpr bitboard set_bit(bitboard v, int i) {
    return v | (1ULL << i);
}

/**
 *  Unset a particular bit in a bitboard.
 *
 *  \param v            The bitboard.
 *  \param i            The index of the bit to unset.
 *  \return             The resulting bitboard.
 */
constexpr bitboard unset_bit(bitboard v, int i) {
    return ~((~v) | (1ULL << i));
}

/**
 *  Shift an entire bitboard one square north.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneN(bitboard b) {
    return b << N;
}

/**
 *  Shift an entire bitboard one square south.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneS(bitboard b) {
    return b >> -S;
}

/**
 *  Shift an entire bitboard one square east.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneE(bitboard b) {
    return (b << E) & notFileA;
}

/**
 *  Shift an entire bitboard one square west.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneW(bitboard b) {
    return (b >> -W) & notFileH;
}

/**
 *  Shift an entire bitboard one square north-east.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneNE(bitboard b) {
    return (b << NE) & notFileA;
}

/**
 *  Shift an entire bitboard one square south-east.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneSE(bitboard b) {
    return (b >> -SE) & notFileA;
}

/**
 *  Shift an entire bitboard one square south-west.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneSW(bitboard b) {
    return (b >> -SW) & notFileH;
}

/**
 *  Shift an entire bitboard one square north-west.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneNW(bitboard b) {
    return (b << NW) & notFileH;
}

/**
 *  Shift an entire bitboard one square in an arbitrary direction.
 *
 *  \param b            The bitboard to shift.
 *  \param d            The direction to shift in. 0=north, increases clockwise.
 *  \return             The shifted bitboard.
 */
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

/**
 *  Shift an entire bitboard two squares north.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard twoN(bitboard b) {
    return b << 2 * N;
}

/**
 *  Shift an entire bitboard two squares south.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard twoS(bitboard b) {
    return b >> -2 * S;
}

/**
 *  Shift an entire bitboard two squares north and one square east.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneNNE(bitboard b) {
    return (b << NNE) & notFileA;
}

/**
 *  Shift an entire bitboard two squares east and one square north.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneENE(bitboard b) {
    return (b << ENE) & notFileA & notFileB;
}

/**
 *  Shift an entire bitboard two squares east and one square south.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneESE(bitboard b) {
    return (b >> -ESE) & notFileA & notFileB;
}

/**
 *  Shift an entire bitboard two squares south and one square east.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneSSE(bitboard b) {
    return (b >> -SSE) & notFileA;
}

/**
 *  Shift an entire bitboard two squares south and one square west.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneSSW(bitboard b) {
    return (b >> -SSW) & notFileH;
}

/**
 *  Shift an entire bitboard two squares west and one square south.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneWSW(bitboard b) {
    return (b >> -WSW) & notFileH & notFileG;
}

/**
 *  Shift an entire bitboard two squares west and one square north.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneWNW(bitboard b) {
    return (b << WNW) & notFileH & notFileG;
}

/**
 *  Shift an entire bitboard two squares north and one square west.
 *
 *  \param b            The bitboard to shift.
 *  \return             The shifted bitboard.
 */
constexpr bitboard oneNNW(bitboard b) {
    return (b << NNW) & notFileH;
}



} // end of chessCore namespace

#endif
