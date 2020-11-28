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


using namespace chessCore;


int main() {
    init();

    board b("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -");
//    Player p;

    std::cout << b;

    TransTable *tt = new TransTable;

    Searcher searcher(tt);
    move_t move = searcher.search(&b, 120, false);

    std::cout << b.SAN_pre_move(move) << std::endl;

    board child;
    MoveList moves = b.gen_legal_moves();
    value_t best_value = VAL_INFINITY;
    value_t value;
    move_t best_move = 0;
    uint64_t hash;
    uint32_t ind;
    TransTable::iterator it;
    for (move_t move : moves) {
        child = doMove(b, move);
        child.getHash(&hash);
        ind = (uint32_t)hash;
        it = tt->find(ind);
        if (it == tt->end()) continue;
        value = it->second.score;
        if (value <= best_value) {
            best_value = value;
            best_move = move;
        }
    }
    std::cout << b.SAN_pre_move(best_move) << std::endl;;

    b.getHash(&hash);
    ind = (uint32_t)hash;
    it = tt->find(ind);
    if (it != tt->end()) {
        std::cout << b.SAN_pre_move(it->second.best_ref_move) << std::endl;
    }

    return 0;
}


