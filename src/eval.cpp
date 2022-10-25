/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
#include "eval.h"

#include "board.h"
#include "twiddle.h"
#include "typedefs.h"


namespace chessCore {

// evaluation functions

value_t Board::evaluate_material(int phase) const {
    // a rudimentary evaluation function based on material
    // white is positive, black is negative

    value_t ret = 0;
    for (int i = 0; i < 12; i++) {
        ret += count_bits_set(pieceBoards[i]) * pieceValues[phase][i];
    }
    return ret;
}

value_t Board::evaluate_pieceSquareTables(int phase) const {
    int i;
    bitboard tmp;
    value_t ret = 0;
    int ind;

    for (i = 0; i < 12; i++) {
        tmp = pieceBoards[i];
        ITER_BITBOARD(ind, tmp) {
            ret += pieceSquareTables[phase][i][ind];
        }
    }
    return ret;
}

value_t Board::getValue() const {
    int phase = getPhase();
    return (opening_value * (256 - phase) + endgame_value * phase) / 256;
}

value_t Board::getOpeningValue() const {
    return opening_value;
}

value_t Board::getEndgameValue() const {
    return endgame_value;
}

value_t Board::evaluate() const {
    int phase = getPhase();
    value_t open = evaluateOpening();
    value_t end = evaluateEndgame();
    return (open * (256 - phase) + end * phase) / 256;
//    return evaluate_material() + evaluate_pieceSquareTables();
}

value_t Board::evaluateOpening() const {
    return evaluate_material(0) + evaluate_pieceSquareTables(0);
}

value_t Board::evaluateEndgame() const {
    return evaluate_material(1) + evaluate_pieceSquareTables(1);
}

int Board::getPhase() const {
    int pawnPhase = 0;
    int knightPhase = 1;
    int bishopPhase = 1;
    int rookPhase = 2;
    int queenPhase = 4;
    int totalPhase = pawnPhase*16 + knightPhase*4 + bishopPhase*4
                   + rookPhase*4 + queenPhase*2;

    int phase = totalPhase;

    phase -= pawnPhase * num_pieces_left(whitePawn);
    phase -= knightPhase * num_pieces_left(whiteKnight);
    phase -= bishopPhase * num_pieces_left(whiteBishop);
    phase -= rookPhase * num_pieces_left(whiteRook);
    phase -= queenPhase * num_pieces_left(whiteQueen);

    phase -= pawnPhase * num_pieces_left(blackPawn);
    phase -= knightPhase * num_pieces_left(blackKnight);
    phase -= bishopPhase * num_pieces_left(blackBishop);
    phase -= rookPhase * num_pieces_left(blackRook);
    phase -= queenPhase * num_pieces_left(blackQueen);

    phase = (phase * 256 + (totalPhase / 2)) / totalPhase;

    return phase;
}



}   // namespace chessCore
