#include "board.h"
#include "eval.h"
#include "twiddle.h"

// initialise constants declared in eval.h

const int16_t pieceValues[12] = {100,500,320,330,900,20000,-100,-500,-320,-330,-900,-20000};

const int8_t pieceSquareTables[12][64] =

{
    // white pawn
    {   0,    0,    0,    0,    0,    0,    0,    0,
        5,   10,   10,  -20,  -20,   10,   10,    5,
        5,   -5,  -10,    0,    0,  -10,   -5,    5,
        0,    0,    0,   20,   20,    0,    0,    0,
        5,    5,   10,   25,   25,   10,    5,    5,
        10,   10,   20,   30,   30,   20,   10,   10,
        50,   50,   50,   50,   50,   50,   50,   50,
    0,    0,    0,    0,    0,    0,    0,    0  },

    // white rook
    {   0,    0,    0,    5,    5,    0,    0,    0,
        -5,    0,    0,    0,    0,    0,    0,   -5,
        -5,    0,    0,    0,    0,    0,    0,   -5,
        -5,    0,    0,    0,    0,    0,    0,   -5,
        -5,    0,    0,    0,    0,    0,    0,   -5,
        -5,    0,    0,    0,    0,    0,    0,   -5,
        5,   10,   10,   10,   10,   10,   10,    5,
    0,    0,    0,    0,    0,    0,    0,    0  },

    // white knight
    { -50,  -40,  -30,  -30,  -30,  -30,  -40,  -50,
        -40,  -20,    0,    5,    5,    0,  -20,  -40,
        -30,    5,   10,   15,   15,   10,    5,  -30,
        -30,    0,   15,   20,   20,   15,    0,  -30,
        -30,    5,   15,   20,   20,   15,    5,  -30,
        -30,    0,   10,   15,   15,   10,    0,  -30,
        -40,  -20,    0,    0,    0,    0,  -20,  -40,
    -50,  -40,  -30,  -30,  -30,  -30,  -40,  -50  },

    // white bishop
    { -20,  -10,  -10,  -10,  -10,  -10,  -10,  -20,
        -10,    5,    0,    0,    0,    0,    5,  -10,
        -10,   10,   10,   10,   10,   10,   10,  -10,
        -10,    0,   10,   10,   10,   10,    0,  -10,
        -10,    5,    5,   10,   10,    5,    5,  -10,
        -10,    0,    5,   10,   10,    5,    0,  -10,
        -10,    0,    0,    0,    0,    0,    0,  -10,
    -20,  -10,  -10,  -10,  -10,  -10,  -10,  -20  },

    // white queen
    { -20,  -10,  -10,   -5,   -5,  -10,  -10,  -20,
        -10,    0,    5,    0,    0,    0,    0,  -10,
        -10,    5,    5,    5,    5,    5,    0,  -10,
        0,    0,    5,    5,    5,    5,    0,   -5,
        -5,    0,    5,    5,    5,    5,    0,   -5,
        -10,    0,    5,    5,    5,    5,    0,  -10,
        -10,    0,    0,    0,    0,    0,    0,  -10,
    -20,  -10,  -10,   -5,   -5,  -10,  -10,  -20  },

    // white king
    {  20,   30,   10,    0,    0,   10,   30,   20,
        20,   20,    0,    0,    0,    0,   20,   20,
        -10,  -20,  -20,  -20,  -20,  -20,  -20,  -10,
        -20,  -30,  -30,  -40,  -40,  -30,  -30,  -20,
        -30,  -40,  -40,  -50,  -50,  -40,  -40,  -30,
        -30,  -40,  -40,  -50,  -50,  -40,  -40,  -30,
        -30,  -40,  -40,  -50,  -50,  -40,  -40,  -30,
    -30,  -40,  -40,  -50,  -50,  -40,  -40,  -30  },

    // black pawn
    { 0,   0,   0,    0,    0,    0,    0,    0,
        -50,  -50,   -50,   -50,   -50,   -50,   -50,   -50,
        -10,  -10,  -20,   -30,   -30,   -20,   -10,   -10,
        -5,   -5,  -10,   -25,   -25,   -10,    -5,    -5,
        0,   0,    0,   -20,   -20,    0,    0,    0,
        -5,    5,  10,    0,    0,  10,   5,    -5,
        -5,  -10,   -10,  20,  20,   -10,   -10,    -5,
    0,   0,    0,    0,    0,    0,    0,    0  },

    // black rook
    {  0,    0,    0,    0,    0,    0,    0,    0,
        -5,   -10,   -10,   -10,   -10,   -10,   -10,    -5,
        +-5,    0,    0,    0,    0,    0,    0,   +5,
        +-5,    0,    0,    0,    0,    0,    0,   +5,
        +-5,    0,    0,    0,    0,    0,    0,   +5,
        +-5,    0,    0,    0,    0,    0,    0,   +5,
        +-5,    0,    0,    0,    0,    0,    0,   +5,
    0,    0,    0,    -5,    -5,    0,    0,    0  },

    // black knight
    { +50,  +40,  +30,  +30,  +30,  +30,  +40,  +50,
        +40,  +20,    0,    0,    0,    0,  +20,  +40,
        +30,    0,   -10,   -15,   -15,   -10,    0,  +30,
        +30,    -5,   -15,   -20,   -20,   -15,    5,  +30,
        +30,    0,   -15,   -20,   -20,   -15,    0,  +30,
        +30,    -5,   -10,   -15,   -15,   -10,    5,  +30,
        +40,  +20,    0,    -5,    -5,    0,  +20,  +40,
    +50,  +40,  +30,  +30,  +30,  +30,  +40,  +50  },

    //black bishop
    { +20,  +10,  +10,  +10,  +10,  +10,  +10,  +20,
        +10,    0,    0,    0,    0,    0,    0,  +10,
        +10,    0,    -5,   -10,   -10,    -5,    0,  +10,
        +10,    -5,    -5,   -10,   -10,    -5,    -5,  +10,
        +10,    0,   -10,   -10,   -10,   -10,    0,  +10,
        +10,   -10,   -10,   -10,   -10,   -10,   -10,  +10,
        +10,    -5,    0,    0,    0,    0,    -5,  +10,
    +20,  +10,  +10,  +10,  +10,  +10,  +10,  +20  },

    // black queen
    { +20,  +10,  +10,   +5,   +5,  +10,  +10,  +20,
        +10,    0,    0,    0,    0,    0,    0,  +10,
        +10,    0,    -5,    -5,    -5,    -5,    0,  +10,
        +5,    0,    -5,    -5,    -5,    -5,    0,   +5,
        0,    0,    -5,    -5,    -5,    -5,    0,   +5,
        +10,    -5,    -5,    -5,    -5,    -5,    0,  +10,
        +10,    0,    -5,    0,    0,    0,    0,  +10,
    +20,  +10,  +10,   +5,   +5,  +10,  +10,  +20  },

    // black king
    { +30,  +40,  +40,  +50,  +50,  +40,  +40,  +30,
        +30,  +40,  +40,  +50,  +50,  +40,  +40,  +30,
        +30,  +40,  +40,  +50,  +50,  +40,  +40,  +30,
        +30,  +40,  +40,  +50,  +50,  +40,  +40,  +30,
        +20,  +30,  +30,  +40,  +40,  +30,  +30,  +20,
        +10,  +20,  +20,  +20,  +20,  +20,  +20,  +10,
        -20,   -20,    0,    0,    0,    0,   -20,   -20,
    -20,   -30,   -10,    0,    0,   -10,   -30,   -20  }

};

// evaluation functions


int16_t board::evaluate_material() {
    // a rudimentary evaluation function based on material
    // white is positive, black is negative

    int16_t ret=0;
    for (int i=0; i<12;i++) {
        ret += count_bits_set(pieceBoards[i]) * pieceValues[i];
    }
    return ret;
}



int16_t board::evaluate_pieceSquareTables() {
    int i,j;
    bitboard tmp;
    int16_t ret = 0;

    for (i=0; i<12; i++) {
        tmp = pieceBoards[i];
        while ( tmp ) {
            ret += pieceSquareTables[i][first_set_bit( tmp )];
            tmp &= ( tmp - 1ULL );
        }
    }
    return ret;
}


int32_t board::evaluate() {
    return evaluate_material() + evaluate_pieceSquareTables();
}

int32_t board::getValue() {
    return value;
}
