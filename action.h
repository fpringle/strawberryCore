#ifndef __ACTION_H
#define __ACTION_H

#include "board.h"
#include "move.h"

namespace chessCore {

/**
 *  Perform a move.
 *
 *  \param startBoard       The board before the move is made.
 *  \param move             The move to be made.
 *  \return                 The board after the move is made.
 */
board doMove (board startBoard, move_t move);

board* doMove (board* startBoard, move_t move);

}

#endif
