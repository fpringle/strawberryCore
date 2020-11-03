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
    board b("1nb1k1nr/p1pp1p1p/8/1pbp4/P4P2/2PN3N/1P1PP1PP/R2QK2R w KQk - 0 1");

        b.print_board();

        std::cout << "material value:    " << b.evaluate_material()
                  << "\npositional value:  " << b.evaluate_pieceSquareTables()
                  << "\nphase:             " << b.getPhase()
                  << "\nvalue:             " << b.getValue()
                  << std::endl;

    return 0;
}


int main2() {
    Player p("5k2/1P6/8/6B1/1R6/Q7/8/4K2R w K - 0 1");

    std::cout << p;

    p.print_history();

    return 0;
}
