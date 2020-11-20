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

    Player p;

    p.read_config("config.toml");

    p.play();

    return 0;
}


