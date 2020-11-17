#ifndef __EVAL_H
#define __EVAL_H

#include "typedefs.h"

namespace chessCore {

/**
 *  Constant values for material evaluation, indexed by phase and square index.
 *  See \ref eval.h.
 */
extern const value_t pieceValues[2][12];


/**
 *  Constant values for piece placement evaluation, indexed by phase,
 *  piece, and square index.
 *  See \ref eval.h.
 */
extern const value_t pieceSquareTables[2][12][64];


} // end of chessCore namespace

#endif
