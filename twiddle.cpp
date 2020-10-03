#include "board.h"
#include "move.h"
#include "twiddle.h"

#include <math.h>




// initialise constants declared in twiddle.h
const bitboard fileA        = 0x0101010101010101;
const bitboard fileB        = 0x0202020202020202;
const bitboard fileG        = 0x4040404040404040;
const bitboard fileH        = 0x8080808080808080;
const bitboard rankOne      = 0x00000000000000FF;
const bitboard rankTwo      = 0x000000000000FF00;
const bitboard rankSeven    = 0x00FF000000000000;
const bitboard rankEight    = 0xFF00000000000000;
const bitboard notFileA     = ~ fileA;
const bitboard notFileB     = ~ fileB;
const bitboard notFileG     = ~ fileG;
const bitboard notFileH     = ~ fileH;
const bitboard notRankOne   = ~ rankOne;
const bitboard notRankEight = ~ rankEight;

// general-purpose bit twiddling functions
int count_bits_set(bitboard v) {
    // thanks Brian Kernighan
    int c;
    for (c=0;v;c++) {
        v &= v-1;
    }
    return c;
}

int first_set_bit(bitboard v) {
    //returns the position of the rightmost set bit
    return log2(v & -v);
}

int last_set_bit(bitboard v) {
    //returns the position of the leftmost set bit
    return log2(v);
}

bool is_bit_set(bitboard v, int i) {
    return ( v & ( 1ULL << i ) );
}

bitboard set_bit(bitboard v, int i) {
    return v | (1ULL << i);
}

bitboard unset_bit (bitboard v, int i) {
    return ~ ( (~v) | (1ULL << i));
}

// moving functions
bitboard oneN(bitboard b) { return b << N; }
bitboard oneS(bitboard b) { return b >> -S; }
bitboard oneE(bitboard b) { return (b << E) & notFileA; }
bitboard oneW(bitboard b) { return (b >> -W) & notFileH; }

bitboard oneNE(bitboard b) { return (b << NE) & notFileA; }
bitboard oneSE(bitboard b) { return (b >> -SE) & notFileA; }
bitboard oneSW(bitboard b) { return (b >> -SW) & notFileH; }
bitboard oneNW(bitboard b) { return (b << NW) & notFileH; }

bitboard oneGeneral8(bitboard b, int d) {
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

bitboard twoN(bitboard b) { return b << 2*N; }
bitboard twoS(bitboard b) { return b >> -2*S; }

bitboard oneNNE(bitboard b) { return (b << NNE) & notFileA; }
bitboard oneENE(bitboard b) { return (b << ENE) & notFileA & notFileB; }
bitboard oneESE(bitboard b) { return (b >> -ESE) & notFileA & notFileB; }
bitboard oneSSE(bitboard b) { return (b >> -SSE) & notFileA; }
bitboard oneSSW(bitboard b) { return (b >> -SSW) & notFileH; }
bitboard oneWSW(bitboard b) { return (b >> -WSW) & notFileH & notFileG; }
bitboard oneWNW(bitboard b) { return (b << WNW) & notFileH & notFileG; }
bitboard oneNNW(bitboard b) { return (b << NNW) & notFileH; }
