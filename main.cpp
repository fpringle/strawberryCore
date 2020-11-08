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

    Player p("4kb1r/5ppp/1nr1p3/1N3b2/8/R7/5PPP/6K1 b k - 0 0");

    p.play(black, 5);

    return 0;
}
