#ifndef __SEARCH_H
#define __SEARCH_H

#include "board.h"
#include "move.h"
#include "typedefs.h"


namespace chessCore {


/**
 *  \brief Perform a quiescence search on a board.
 *
 *  Quiescence search is a technique to mitigate the horizon effect,
 *  where we recursively search through all the upcoming capture tradeoffs
 *  to get a better heuristic value for a given position.
 *
 *  \param b            The board to analyse.
 *  \param side         The side whose turn it is to move.
 *  \param alpha        The current value of alpha in negamax search.
 *  \param beta         The current value of beta in negamax search.
 *  \return             The heuristic value for the node.
 */
value_t quiesce(board b, colour side, value_t alpha, value_t beta);

/**
 *  Given an array of moves and a preferred "best move" to search first,
 *  reorder the array so that the move is searched first.
 *
 *  \param moves        An array of moves to search.
 *  \param num_moves    The length of the array moves.
 *  \param best_move    The move we want to search first.
 */
void reorder_moves(move_t * moves, int num_moves, move_t best_move);


} // end of chessCore namespace
#endif
