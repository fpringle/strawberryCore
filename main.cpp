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

    board b("6R1/8/8/8/Q6b/K7/8/7k w - - 0 1");

    move_t move(24,31,0,1,0,0);

    std::cout << b.SAN_pre_move(move) << std::endl;

    b.doMoveInPlace(move);

    std::cout << b.SAN_post_move(move) << std::endl;

    if (b.is_checkmate(black)) std::cout << "Black is in checkmate\n";

    return 0;
}


