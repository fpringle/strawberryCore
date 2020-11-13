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


using namespace chessCore;

int main() {
    init();

    bitboard bb = 1152921504606846976ULL;

    print_bb(bb);

    return 0;
}


