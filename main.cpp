#include <cstdlib>

#include "action.h"
#include "board.h"
#include "eval.h"
#include "hash.h"
#include "init.h"
#include "move.h"
#include "play.h"
#include "search.h"
#include "twiddle.h"


using namespace chessCore;


int main() {
    init();

    Player p("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -");

    p.read_config("config.conf");

    p.play();

    return 0;
}


