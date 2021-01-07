/* Copyright 2020 Freddy Pringle */
#ifndef SRC_CORE_ACTION_H_
#define SRC_CORE_ACTION_H_

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
Board doMove(Board startBoard, move_t move);

/**
 *  Perform a move and return a pointer to the resultant board object.
 *
 *  \param startBoard       A pointer to the board before the move is made.
 *  \param move             The move to be made.
 *  \return                 A pointer to the board after the move is made.
 */
Board* doMove(Board* startBoard, move_t move);

}  // namespace chessCore

#endif  // SRC_CORE_ACTION_H_
