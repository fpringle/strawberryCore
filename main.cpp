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

//    Player p("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -");
//    Player p;

//    p.read_config("config.conf");

//    p.play();

    board b;
    uint64_t hash;
    b.getHash(&hash);
    uint32_t ind = uint32_t(hash);
    TransTable tt;
    std::cout << b << ind << std::endl;

    Searcher searcher(&tt);

    searcher.negamax_alphabeta(b, 5, std::numeric_limits<value_t>::min(),
                                     std::numeric_limits<value_t>::max());

    move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    board child;

    TransTable::iterator it;
    prettyMove pm;
    for (int i=0; i<num_moves; i++) {
        child = doMove(b, moves[i]);
        pm.data = moves[i];
        child.getHash(&hash);
        ind = (uint32_t)hash;
        it = tt.find(ind);
        std::cout << pm << std::endl;
        std::cout << it->first << ":" << std::endl
             << it->second << std:: endl;
    }


    return 0;
}


