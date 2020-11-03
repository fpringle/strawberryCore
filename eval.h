#ifndef __EVAL_H
#define __EVAL_H

#include <cstdint>

// piece values for material evaluation
// defined in eval.cpp
extern const int16_t pieceValues[2][12];


// piece square tables
// defined in eval.cpp
extern const int8_t pieceSquareTables[2][12][64];


#endif
