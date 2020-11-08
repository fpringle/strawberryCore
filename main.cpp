#include "action.h"
#include "board.h"
#include "eval.h"
#include "hash.h"
#include "init.h"
#include "move.h"
#include "play.h"
#include "search.h"
#include "twiddle.h"

#include <cstdlib>


int main() {
    init();

    Player p;

    p.play(black, 5);

    return 0;
}
