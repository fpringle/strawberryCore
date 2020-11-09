#ifndef __SEARCH_H
#define __SEARCH_H

#include "board.h"
#include "move.h"
#include "typedefs.h"


class Searcher {
public:
    static value_t algorithm(board b, int depth, colour side);
    static value_t algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta);
    static move_t search(board b, int depth);
};


class Minimax : Searcher {
public:
    static value_t algorithm(board b, int depth, colour side);
    static move_t search(board b, int depth);
};

class Negamax : Searcher {
public:
    static value_t algorithm(board b, int depth, colour side);
    static move_t search(board b, int depth);
};

class NegamaxAB : Searcher {
public:
    static value_t algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta);
    static move_t search(board b, int depth);
};

class NegaScoutOriginal : Searcher {
public:
    static value_t algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta);
    static move_t search(board b, int depth);
};

class NegaScoutAlternative : Searcher {
public:
    static value_t algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta);
    static move_t search(board b, int depth);
};

class PVS : Searcher {
public:
    static value_t algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta);
    static move_t search(board b, int depth);
};



////////////////////////////////////////

value_t quiesce(board b, colour side, value_t alpha, value_t beta);

////////////////////////////////////////


void reorder_moves(move_t * moves, int num_moves, move_t best_move);

#endif
