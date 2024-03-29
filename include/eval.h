/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
#ifndef SRC_CORE_EVAL_H_
#define SRC_CORE_EVAL_H_

#include "typedefs.h"

namespace chessCore {

/**
 *  Constant values for material evaluation, indexed by phase and square index.
 */
constexpr value_t pieceValues[2][12] = {
    {100, 500, 320, 330, 900, 20000, -100, -500, -320, -330, -900, -20000},
    {100, 500, 320, 330, 900, 20000, -100, -500, -320, -330, -900, -20000}
};


/**
 *  Constant values for piece placement evaluation, indexed by phase,
 *  piece, and square index.
 */
constexpr value_t pieceSquareTables[2][12][64] = {
    {
        // white pawn - opening
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            5, 10, 10, -25, -25, 10, 10, 5,
            5, -5, -5, 0, 0, -5, -5, 5,
            0, 0, 0, 25, 25, 0, 0, 0,
            5, 5, 10, 25, 25, 10, 5, 5,
            10, 10, 20, 30, 30, 20, 10, 10,
            50, 50, 50, 50, 50, 50, 50, 50,
            0, 0, 0, 0, 0, 0, 0, 0
        },

        // white rook - opening
        {
            0, 0, 0, 5, 5, 0, 0, 0,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            5, 10, 10, 10, 10, 10, 10, 5,
            0, 0, 0, 0, 0, 0, 0, 0
        },

        // white knight - opening
        {
            -50, -40, -30, -30, -30, -30, -40, -50,
                -40, -20, 0, 5, 5, 0, -20, -40,
                -30, 5, 10, 15, 15, 10, 5, -30,
                -30, 0, 15, 20, 20, 15, 0, -30,
                -30, 5, 15, 20, 20, 15, 5, -30,
                -30, 0, 10, 15, 15, 10, 0, -30,
                -40, -20, 0, 0, 0, 0, -20, -40,
                -50, -40, -30, -30, -30, -30, -40, -50
            },

        // white bishop - opening
        {
            -20, -10, -10, -10, -10, -10, -10, -20,
                -10, 5, 0, 0, 0, 0, 5, -10,
                -10, 10, 10, 10, 10, 10, 10, -10,
                -10, 0, 10, 10, 10, 10, 0, -10,
                -10, 5, 5, 10, 10, 5, 5, -10,
                -10, 0, 5, 10, 10, 5, 0, -10,
                -10, 0, 0, 0, 0, 0, 0, -10,
                -20, -10, -10, -10, -10, -10, -10, -20
            },

        // white queen - opening
        {
            -20, -10, -10, -5, -5, -10, -10, -20,
                -10, 0, 5, 0, 0, 0, 0, -10,
                -10, 5, 5, 5, 5, 5, 0, -10,
                0, 0, 5, 5, 5, 5, 0, -5,
                -5, 0, 5, 5, 5, 5, 0, -5,
                -10, 0, 5, 5, 5, 5, 0, -10,
                -10, 0, 0, 0, 0, 0, 0, -10,
                -20, -10, -10, -5, -5, -10, -10, -20
            },

        // white king - opening
        {
            20, 30, 10, 0, 0, 10, 30, 20,
            20, 20, 0, 0, 0, 0, 20, 20,
            -10, -20, -20, -20, -20, -20, -20, -10,
            -20, -30, -30, -40, -40, -30, -30, -20,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30
        },

        // black pawn - opening
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -50, -50, -50, -50, -50, -50, -50, -50,
            -10, -10, -20, -30, -30, -20, -10, -10,
            -5, -5, -10, -25, -25, -10, -5, -5,
            0, 0, 0, -25, -25, 0, 0, 0,
            -5, 5, 5, 0, 0, 5, 5, -5,
            -5, -10, -10, 25, 25, -10, -10, -5,
            0, 0, 0, 0, 0, 0, 0, 0
        },

        // black rook - opening
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -5, -10, -10, -10, -10, -10, -10, -5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            0, 0, 0, -5, -5, 0, 0, 0
        },

        // black knight - opening
        {
            +50, +40, +30, +30, +30, +30, +40, +50,
                +40, +20, 0, 0, 0, 0, +20, +40,
                +30, 0, -10, -15, -15, -10, 0, +30,
                +30, -5, -15, -20, -20, -15, -5, +30,
                +30, 0, -15, -20, -20, -15, 0, +30,
                +30, -5, -10, -15, -15, -10, -5, +30,
                +40, +20, 0, -5, -5, 0, +20, +40,
                +50, +40, +30, +30, +30, +30, +40, +50
            },

        // black bishop - opening
        {
            +20, +10, +10, +10, +10, +10, +10, +20,
                +10, 0, 0, 0, 0, 0, 0, +10,
                +10, 0, -5, -10, -10, -5, 0, +10,
                +10, -5, -5, -10, -10, -5, -5, +10,
                +10, 0, -10, -10, -10, -10, 0, +10,
                +10, -10, -10, -10, -10, -10, -10, +10,
                +10, -5, 0, 0, 0, 0, -5, +10,
                +20, +10, +10, +10, +10, +10, +10, +20
            },

        // black queen - opening
        {
            +20, +10, +10, +5, +5, +10, +10, +20,
                +10, 0, 0, 0, 0, 0, 0, +10,
                +10, 0, -5, -5, -5, -5, 0, +10,
                +5, 0, -5, -5, -5, -5, 0, +5,
                0, 0, -5, -5, -5, -5, 0, +5,
                +10, -5, -5, -5, -5, -5, 0, +10,
                +10, 0, -5, 0, 0, 0, 0, +10,
                +20, +10, +10, +5, +5, +10, +10, +20
            },

        // black king - opening
        {
            +30, +40, +40, +50, +50, +40, +40, +30,
                +30, +40, +40, +50, +50, +40, +40, +30,
                +30, +40, +40, +50, +50, +40, +40, +30,
                +30, +40, +40, +50, +50, +40, +40, +30,
                +20, +30, +30, +40, +40, +30, +30, +20,
                +10, +20, +20, +20, +20, +20, +20, +10,
                -20, -20, 0, 0, 0, 0, -20, -20,
                -20, -30, -10, 0, 0, -10, -30, -20
            }
    },

    {
        // white pawn - endgame
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            5, 10, 10, -20, -20, 10, 10, 5,
            5, -5, -10, 0, 0, -10, -5, 5,
            0, 0, 0, 20, 20, 0, 0, 0,
            5, 5, 10, 25, 25, 10, 5, 5,
            10, 10, 20, 30, 30, 20, 10, 10,
            50, 50, 50, 50, 50, 50, 50, 50,
            0, 0, 0, 0, 0, 0, 0, 0
        },

        // white rook - endgame
        {
            0, 0, 0, 5, 5, 0, 0, 0,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            5, 10, 10, 10, 10, 10, 10, 5,
            0, 0, 0, 0, 0, 0, 0, 0
        },

        // white knight - endgame
        {
            -50, -40, -30, -30, -30, -30, -40, -50,
                -40, -20, 0, 5, 5, 0, -20, -40,
                -30, 5, 10, 15, 15, 10, 5, -30,
                -30, 0, 15, 20, 20, 15, 0, -30,
                -30, 5, 15, 20, 20, 15, 5, -30,
                -30, 0, 10, 15, 15, 10, 0, -30,
                -40, -20, 0, 0, 0, 0, -20, -40,
                -50, -40, -30, -30, -30, -30, -40, -50
            },

        // white bishop - endgame
        {
            -20, -10, -10, -10, -10, -10, -10, -20,
                -10, 5, 0, 0, 0, 0, 5, -10,
                -10, 10, 10, 10, 10, 10, 10, -10,
                -10, 0, 10, 10, 10, 10, 0, -10,
                -10, 5, 5, 10, 10, 5, 5, -10,
                -10, 0, 5, 10, 10, 5, 0, -10,
                -10, 0, 0, 0, 0, 0, 0, -10,
                -20, -10, -10, -10, -10, -10, -10, -20
            },

        // white queen - endgame
        {
            -20, -10, -10, -5, -5, -10, -10, -20,
                -10, 0, 5, 0, 0, 0, 0, -10,
                -10, 5, 5, 5, 5, 5, 0, -10,
                0, 0, 5, 5, 5, 5, 0, -5,
                -5, 0, 5, 5, 5, 5, 0, -5,
                -10, 0, 5, 5, 5, 5, 0, -10,
                -10, 0, 0, 0, 0, 0, 0, -10,
                -20, -10, -10, -5, -5, -10, -10, -20
            },

        // white king   - endgame
        {
            -50, -30, -30, -30, -30, -30, -30, -50,
            -30, -30, 0, 0, 0, 0, -30, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -20, -10, 0, 0, -10, -20, -30,
            -50, -40, -30, -20, -20, -30, -40, -50
        },

        // black pawn - endgame
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -50, -50, -50, -50, -50, -50, -50, -50,
            -10, -10, -20, -30, -30, -20, -10, -10,
            -5, -5, -10, -25, -25, -10, -5, -5,
            0, 0, 0, -20, -20, 0, 0, 0,
            -5, 5, 10, 0, 0, 10, 5, -5,
            -5, -10, -10, 20, 20, -10, -10, -5,
            0, 0, 0, 0, 0, 0, 0, 0
        },

        // black rook - endgame
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -5, -10, -10, -10, -10, -10, -10, -5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            +5, 0, 0, 0, 0, 0, 0, +5,
            0, 0, 0, -5, -5, 0, 0, 0
        },

        // black knight - endgame
        {
            +50, +40, +30, +30, +30, +30, +40, +50,
                +40, +20, 0, 0, 0, 0, +20, +40,
                +30, 0, -10, -15, -15, -10, 0, +30,
                +30, -5, -15, -20, -20, -15, -5, +30,
                +30, 0, -15, -20, -20, -15, 0, +30,
                +30, -5, -10, -15, -15, -10, -5, +30,
                +40, +20, 0, -5, -5, 0, +20, +40,
                +50, +40, +30, +30, +30, +30, +40, +50
            },

        // black bishop - endgame
        {
            +20, +10, +10, +10, +10, +10, +10, +20,
                +10, 0, 0, 0, 0, 0, 0, +10,
                +10, 0, -5, -10, -10, -5, 0, +10,
                +10, -5, -5, -10, -10, -5, -5, +10,
                +10, 0, -10, -10, -10, -10, 0, +10,
                +10, -10, -10, -10, -10, -10, -10, +10,
                +10, -5, 0, 0, 0, 0, -5, +10,
                +20, +10, +10, +10, +10, +10, +10, +20
            },

        // black queen - endgame
        {
            +20, +10, +10, +5, +5, +10, +10, +20,
                +10, 0, 0, 0, 0, 0, 0, +10,
                +10, 0, -5, -5, -5, -5, 0, +10,
                +5, 0, -5, -5, -5, -5, 0, +5,
                0, 0, -5, -5, -5, -5, 0, +5,
                +10, -5, -5, -5, -5, -5, 0, +10,
                +10, 0, -5, 0, 0, 0, 0, +10,
                +20, +10, +10, +5, +5, +10, +10, +20
            },

        // black king - endgame
        {
            50, 40, 30, 20, 20, 30, 40, 50,
            30, 20, 10, 0, 0, 10, 20, 30,
            30, 10, -20, -30, -30, -20, 10, 30,
            30, 10, -30, -40, -40, -30, 10, 30,
            30, 10, -30, -40, -40, -30, 10, 30,
            30, 10, -20, -30, -30, -20, 10, 30,
            30, 30, 0, 0, 0, 0, 30, 30,
            50, 30, 30, 30, 30, 30, 30, 50
        }
    }
};



}   // namespace chessCore

#endif  // SRC_CORE_EVAL_H_
