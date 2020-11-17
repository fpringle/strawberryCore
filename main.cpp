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

    Player p;

    move_t g1f3 = move_t( 6, 21, 0, 0, 0, 0);
    move_t g8f6 = move_t(62, 45, 0, 0, 0, 0);
    move_t b1c3 = move_t( 1, 18, 0, 0, 0, 0);

//    p.print_board();
    std::cout << p.SAN_pre_move(g1f3) << std::endl;
    p.doMoveInPlace(g1f3);

//    p.print_board();
    std::cout << p.SAN_pre_move(g8f6) << std::endl;
    p.doMoveInPlace(g8f6);

//    p.print_board();
    std::cout << p.SAN_pre_move(b1c3) << std::endl;
    p.doMoveInPlace(b1c3);

    p.print_history_san();

    return 0;
}


