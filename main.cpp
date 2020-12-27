/* Copyright 2020 Freddy Pringle */
#include <cstdlib>
#include <limits>

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
    chessCore::Player* player = new chessCore::Player(chessCore::white);
    player->play();
    return 0;
}
