/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
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
