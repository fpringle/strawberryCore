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
    static move_t search(board b, int depth, colour side);
};


class Minimax : Searcher {
public:
    static int32_t algorithm(board b, int depth, colour side);
    static move_t search(board b, int depth, colour side);
};

class Negamax : Searcher {;
public:
    static int32_t algorithm(board b, int depth, colour side);
    static move_t search(board b, int depth, colour side);
};

class NegamaxAB : Searcher {;
public:
    static int32_t algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta);
    static move_t search(board b, int depth, colour side);
};

#endif
