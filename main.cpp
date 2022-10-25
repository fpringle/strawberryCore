/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
#include <cstdlib>
#include <limits>
#include <string>

#include "action.h"
#include "board.h"
#include "eval.h"
#include "hash.h"
#include "init.h"
#include "move.h"
#include "play.h"
#include "search.h"
#include "twiddle.h"


int main() {
    chessCore::init();
    std::string str = "1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -";
    chessCore::Player* player = new chessCore::Player(str);
    player->play();
    return 0;
}
