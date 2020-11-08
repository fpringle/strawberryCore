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

    board b("qqqqqqqK/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqk w - - 0 0");

    b.print_board();

    std::cout << "Opening: " << b.getOpeningValue() << std::endl;
    std::cout << "Opening: " << b.evaluateOpening() << std::endl;
    std::cout << "Endgame: " << b.getEndgameValue() << std::endl;
    std::cout << "Endgame: " << b.evaluateEndgame() << std::endl;
    std::cout << "Total:   " << b.evaluate() << std::endl;
    std::cout << "Total:   " << b.getValue() << std::endl;

    return 0;
}
