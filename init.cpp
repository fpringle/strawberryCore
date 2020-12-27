/* Copyright 2020 Freddy Pringle */
#include "init.h"

#include <cstdint>

#include "board.h"
#include "hash.h"
#include "move.h"


namespace chessCore {


void init() {
    // initialise rays for sliding piece move generation
    init_rays();

    // initialise zobrist keys
    init_keys();
}

void init(uint64_t seed) {
    // initialise rays for sliding piece move generation
    init_rays();

    // initialise zobrist keys
    init_keys(seed);
}


}   // namespace chessCore
