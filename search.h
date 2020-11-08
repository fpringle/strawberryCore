#ifndef __SEARCH_H
#define __SEARCH_H

#include "board.h"
#include "move.h"
#include <cstdint>


class Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side);
    static int32_t algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta);
    static move_t search(board b, int depth);
};


class Minimax : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side);
    static move_t search(board b, int depth);
};

class Negamax : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side);
    static move_t search(board b, int depth);
};

class NegamaxAB : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta);
    static move_t search(board b, int depth);
};

class NegaScoutOriginal : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta);
    static move_t search(board b, int depth);
};

class NegaScoutAlternative : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta);
    static move_t search(board b, int depth);
};

class PVS : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta);
    static move_t search(board b, int depth);
};



////////////////////////////////////////

int32_t quiesce(board b, colour side, int32_t alpha, int32_t beta);

////////////////////////////////////////


void reorder_moves(move_t * moves, int num_moves, move_t best_move);

#endif
