#include "board.h"
#include "move.h"
#include "init.h"
#include "hash.h"

#include <cstdint>

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
