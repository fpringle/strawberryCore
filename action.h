#ifndef __ACTION_H
#define __ACTION_H

#include "board.h"
#include "move.h"

namespace chessCore {

/**
 *  Perform a move and return the resultant board object.
 *
 *  \param startBoard       The board before the move is made.
 *  \param move             The move to be made.
 *  \return                 The board after the move is made.
 */
board doMove (board startBoard, move_t move);

/**
 *  Perform a move and return a pointer to the resultant board object.
 *
 *  \param startBoard       A pointer to the board before the move is made.
 *  \param move             The move to be made.
 *  \return                 A pointer to the board after the move is made.
 */
board* doMove (board* startBoard, move_t move);

}

#endif
