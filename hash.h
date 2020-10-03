#ifndef __HASH_H
#define __HASH_H

#include <cstdint>
#include "move.h"

// n=312, w=64

extern uint64_t MT[312];
extern uint64_t check_index;
extern uint64_t lower_mask;
extern uint64_t upper_mask;


// initialise the PRNG
// defined in hash.cpp
void init_gen(uint64_t);

// get a number from the PRNG
// defined in hash.cpp
uint64_t extract_number();

// defined in hash.cpp
void twist();

// generate the zobrist keys

// 0-767:   12 pieces * 64 squares
// 768:     white kingside castle right
// 769:     white queenside castle right
// 770:     black kingside castle right
// 771:     black queenside castle right
// 772-779: en passant square on file a-h
// 780:     if it's black to move
extern uint64_t zobristKeys[781];

// do all of that
// defined in hash.cpp
void init_keys();
// custom seed for reproducability
// defined in hash.cpp
void init_keys(uint64_t);

// transposition table
// "signature" = zobrist hash (64 bits)
// "index"     = singature % table size (probably 8 bits)
// best/refutation move (16 bits)
// depth (8 bits)
// score (integrated bound and values (16 bits?)
// age (8 bits)
// total 128 bits?

struct record_t {
    uint64_t signature;
    struct move_t best_ref_move;
    uint8_t depth;
    int16_t IBV_score;
    uint8_t age;
};

// 1.3e8 seems to be the max
extern struct record_t table[130000000];


#endif
