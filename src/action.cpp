/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
#include "action.h"

#include <cstdint>
#include <string>

#include "eval.h"
#include "hash.h"
#include "play.h"
#include "twiddle.h"
#include "typedefs.h"


namespace chessCore {


Board doMove(Board startBoard, move_t move) {
    int i;
    uint16_t fromSquare = from_sq(move);
    uint16_t toSquare = to_sq(move);
    bool castling[4];
    bool ep;
    startBoard.getEP(&ep);
    int dpp;
    startBoard.getdPPFile(&dpp);
    uint8_t clk;
    uint8_t full_clk;
    uint64_t hash;
    startBoard.getHash(&hash);
    value_t opening_value = startBoard.getOpeningValue();
    value_t endgame_value = startBoard.getEndgameValue();
    colour movingColour;
    startBoard.getSide(& movingColour);
    colour otherColour = flipColour(movingColour);
    colourPiece movingPiece;
    bool rooktaken = false;
    bool foundMovingPiece = false;

    bitboard startingPos[12];
    startBoard.getBitboards(startingPos);

    for (i = (movingColour * 6); i < (1 + movingColour)*6; i++) {
        if (is_bit_set(startingPos[i], fromSquare)) {
            movingPiece = colourPiece(i);
            startingPos[i] = (startingPos[i] & ~(1ULL << fromSquare)) |
                             (1ULL << toSquare);
            opening_value -= pieceSquareTables[0][i][fromSquare];
            endgame_value -= pieceSquareTables[1][i][fromSquare];
            opening_value += pieceSquareTables[0][i][toSquare];
            endgame_value += pieceSquareTables[1][i][toSquare];
            hash ^= zobristKeys[i * 64 + fromSquare];
            hash ^= zobristKeys[i * 64 + toSquare];
            foundMovingPiece = true;
            break;
        }
    }

    if (!foundMovingPiece) {
        return startBoard;
    }

    if (is_capture(move)) {
        if (!is_ep_capture(move)) {
            for (i = (1 - movingColour)*6; i < (2 - movingColour)*6; i++) {
                if (is_bit_set(startingPos[i], toSquare)) {
                    if (i % 6 == 1) rooktaken = true;
                    startingPos[i] = (startingPos[i] & ~(1ULL << toSquare));
                    opening_value -= pieceSquareTables[0][i][toSquare];
                    endgame_value -= pieceSquareTables[1][i][toSquare];
                    opening_value -= pieceValues[0][i];
                    endgame_value -= pieceValues[1][i];
                    hash ^= zobristKeys[i * 64 + toSquare];
                    break;
                }
            }
        } else {
            int _dir = (movingColour == white) ? S : N;
            startingPos[(1 - movingColour)*6] &=
                ~(1ULL << (toSquare + _dir));
            opening_value -= pieceSquareTables[0]
                                [(1 - movingColour)*6][toSquare + _dir];
            endgame_value -= pieceSquareTables[1]
                                [(1 - movingColour)*6][toSquare + _dir];
            opening_value -= pieceValues[0][(1 - movingColour)*6];
            endgame_value -= pieceValues[1][(1 - movingColour)*6];
            hash ^= zobristKeys[(1 - movingColour)*6 * 64 + toSquare + _dir];
            hash ^= zobristKeys[(1 - movingColour)*6 * 64 + toSquare + _dir];
        }
    }

    if (is_kingCastle(move)) {
        startingPos[1 + (6 * movingColour)] =
                            (startingPos[1 + (6 * movingColour)] & ~
                            (1ULL << (fromSquare + 3))) |
                            (1ULL << (toSquare - 1));
        opening_value -= pieceSquareTables[0]
                            [1 + (6 * movingColour)][fromSquare + 3];
        endgame_value -= pieceSquareTables[1]
                            [1 + (6 * movingColour)][fromSquare + 3];
        opening_value += pieceSquareTables[0]
                            [1 + (6 * movingColour)][toSquare - 1];
        endgame_value += pieceSquareTables[1]
                            [1 + (6 * movingColour)][toSquare - 1];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + fromSquare + 3];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + toSquare - 1];
    } else if (is_queenCastle(move)) {
        startingPos[1 + (6 * movingColour)] =
                            (startingPos[1 + (6 * movingColour)] & ~
                            (1ULL << (fromSquare - 4))) |
                            (1ULL << (toSquare + 1));
        opening_value -= pieceSquareTables[0]
                            [1 + (6 * movingColour)][fromSquare - 4];
        endgame_value -= pieceSquareTables[1]
                            [1 + (6 * movingColour)][fromSquare - 4];
        opening_value += pieceSquareTables[0]
                            [1 + (6 * movingColour)][toSquare + 1];
        endgame_value += pieceSquareTables[1]
                            [1 + (6 * movingColour)][toSquare + 1];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + fromSquare - 4];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + toSquare + 1];
    }

    // promotion
    if (is_promotion(move)) {
        startingPos[6 * movingColour] &= (~(1ULL << toSquare));
        opening_value -= pieceSquareTables[0][6 * movingColour][toSquare];
        endgame_value -= pieceSquareTables[1][6 * movingColour][toSquare];
        opening_value -= pieceValues[0][6 * movingColour];
        endgame_value -= pieceValues[1][6 * movingColour];
        hash ^= zobristKeys[6 * movingColour * 64 + toSquare];

        colourPiece prom_piece = colourPiece((6 * movingColour) +
                                             which_promotion(move));

        startingPos[prom_piece] |= (1ULL << toSquare);
        opening_value += pieceSquareTables[0][prom_piece][toSquare];
        endgame_value += pieceSquareTables[1][prom_piece][toSquare];
        opening_value += pieceValues[0][prom_piece];
        endgame_value += pieceValues[1][prom_piece];
        hash ^= zobristKeys[prom_piece * 64 + toSquare];
    }

    startBoard.getCastlingRights(castling);
    startBoard.getClock(&clk);
    startBoard.getFullClock(&full_clk);

    // check for double pawn push
    if (ep) {
        hash ^= zobristKeys[772 + dpp];
    }
    if (is_doublePP(move)) {
        ep = true;
        dpp = fromSquare % 8;
        hash ^= zobristKeys[772 + dpp];
    } else {
        ep = false;
    }

    // check for changes to castling rights
    if (movingPiece % 6 == 1) {
        switch (fromSquare) {
        case 0:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
            }
            break;
        case 7:
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case 56:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
            }
            break;
        case 63:
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    } else if (movingPiece % 6 == 5) {
        switch (movingColour) {
        case white:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
            }
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case black:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
            }
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    }

    if (rooktaken) {
        switch (toSquare) {
        case 0:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
            }
            break;
        case 7:
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case 56:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
            }
            break;
        case 63:
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    }

    // increment halfMoveClock
    if (is_capture(move) | (movingPiece % 6 == 0)) {
        clk = 0;
    } else {
        clk++;
    }

    // increment fullMoveClock
    if (movingColour == black) full_clk++;

    // change hash for different side to move
    hash ^= zobristKeys[780];

    Board endBoard(startingPos, castling, ep, dpp, clk, full_clk, otherColour,
                   opening_value, endgame_value, hash);

    return endBoard;
}

Board* doMove(Board* startBoard, move_t move) {
    int i;
    uint16_t fromSquare = from_sq(move);
    uint16_t toSquare = to_sq(move);
    bool castling[4];
    bool ep;
    startBoard->getEP(&ep);
    int dpp;
    startBoard->getdPPFile(&dpp);
    uint8_t clk;
    uint8_t full_clk;
    uint64_t hash;
    startBoard->getHash(&hash);
    value_t opening_value = startBoard->getOpeningValue();
    value_t endgame_value = startBoard->getEndgameValue();
    colour movingColour;
    startBoard->getSide(& movingColour);
    colour otherColour = flipColour(movingColour);
    colourPiece movingPiece;
    bool rooktaken = false;
    bool foundMovingPiece = false;

    bitboard startingPos[12];
    startBoard->getBitboards(startingPos);

    for (i = (movingColour * 6); i < (1 + movingColour)*6; i++) {
        if (is_bit_set(startingPos[i], fromSquare)) {
            movingPiece = colourPiece(i);
            startingPos[i] = (startingPos[i] & ~(1ULL << fromSquare)) |
                             (1ULL << toSquare);
            opening_value -= pieceSquareTables[0][i][fromSquare];
            endgame_value -= pieceSquareTables[1][i][fromSquare];
            opening_value += pieceSquareTables[0][i][toSquare];
            endgame_value += pieceSquareTables[1][i][toSquare];
            hash ^= zobristKeys[i * 64 + fromSquare];
            hash ^= zobristKeys[i * 64 + toSquare];
            foundMovingPiece = true;
            break;
        }
    }

    if (!foundMovingPiece) {
        return startBoard;
    }

    if (is_capture(move)) {
        if (!is_ep_capture(move)) {
            for (i = (1 - movingColour)*6; i < (2 - movingColour)*6; i++) {
                if (is_bit_set(startingPos[i], toSquare)) {
                    if (i % 6 == 1) rooktaken = true;
                    startingPos[i] = (startingPos[i] & ~(1ULL << toSquare));
                    opening_value -= pieceSquareTables[0][i][toSquare];
                    endgame_value -= pieceSquareTables[1][i][toSquare];
                    opening_value -= pieceValues[0][i];
                    endgame_value -= pieceValues[1][i];
                    hash ^= zobristKeys[i * 64 + toSquare];
                    break;
                }
            }
        } else {
            int _dir = (movingColour == white) ? S : N;
            startingPos[(1 - movingColour)*6] &= ~(1ULL << (toSquare + _dir));
            opening_value -= pieceSquareTables[0]
                                [(1 - movingColour)*6][toSquare + _dir];
            endgame_value -= pieceSquareTables[1]
                                [(1 - movingColour)*6][toSquare + _dir];
            opening_value -= pieceValues[0][(1 - movingColour)*6];
            endgame_value -= pieceValues[1][(1 - movingColour)*6];
            hash ^= zobristKeys[(1 - movingColour)*6 * 64 + toSquare + _dir];
            hash ^= zobristKeys[(1 - movingColour)*6 * 64 + toSquare + _dir];
        }
    }

    if (is_kingCastle(move)) {
        startingPos[1 + (6 * movingColour)] =
                            (startingPos[1 + (6 * movingColour)] & ~
                            (1ULL << (fromSquare + 3))) |
                            (1ULL << (toSquare - 1));
        opening_value -= pieceSquareTables[0]
                            [1 + (6 * movingColour)][fromSquare + 3];
        endgame_value -= pieceSquareTables[1]
                            [1 + (6 * movingColour)][fromSquare + 3];
        opening_value += pieceSquareTables[0]
                            [1 + (6 * movingColour)][toSquare - 1];
        endgame_value += pieceSquareTables[1]
                            [1 + (6 * movingColour)][toSquare - 1];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + fromSquare + 3];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + toSquare - 1];
    } else if (is_queenCastle(move)) {
        startingPos[1 + (6 * movingColour)] =
                            (startingPos[1 + (6 * movingColour)] & ~
                            (1ULL << (fromSquare - 4))) |
                            (1ULL << (toSquare + 1));
        opening_value -= pieceSquareTables[0]
                            [1 + (6 * movingColour)][fromSquare - 4];
        endgame_value -= pieceSquareTables[1]
                            [1 + (6 * movingColour)][fromSquare - 4];
        opening_value += pieceSquareTables[0]
                            [1 + (6 * movingColour)][toSquare + 1];
        endgame_value += pieceSquareTables[1]
                            [1 + (6 * movingColour)][toSquare + 1];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + fromSquare - 4];
        hash ^= zobristKeys[(1 + (6 * movingColour))*64 + toSquare + 1];
    }

    // promotion
    if (is_promotion(move)) {
        startingPos[6 * movingColour] &= (~(1ULL << toSquare));
        opening_value -= pieceSquareTables[0][6 * movingColour][toSquare];
        endgame_value -= pieceSquareTables[1][6 * movingColour][toSquare];
        opening_value -= pieceValues[0][6 * movingColour];
        endgame_value -= pieceValues[1][6 * movingColour];
        hash ^= zobristKeys[6 * movingColour * 64 + toSquare];

        colourPiece prom_piece = colourPiece((6 * movingColour) +
                                             which_promotion(move));

        startingPos[prom_piece] |= (1ULL << toSquare);
        opening_value += pieceSquareTables[0][prom_piece][toSquare];
        endgame_value += pieceSquareTables[1][prom_piece][toSquare];
        opening_value += pieceValues[0][prom_piece];
        endgame_value += pieceValues[1][prom_piece];
        hash ^= zobristKeys[prom_piece * 64 + toSquare];
    }

    startBoard->getCastlingRights(castling);
    startBoard->getClock(&clk);
    startBoard->getFullClock(&full_clk);

    // check for double pawn push
    if (ep) {
        hash ^= zobristKeys[772 + dpp];
    }
    if (is_doublePP(move)) {
        ep = true;
        dpp = fromSquare % 8;
        hash ^= zobristKeys[772 + dpp];
    } else {
        ep = false;
    }

    // check for changes to castling rights
    if (movingPiece % 6 == 1) {
        switch (fromSquare) {
        case 0:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
            }
            break;
        case 7:
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case 56:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
            }
            break;
        case 63:
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    } else if (movingPiece % 6 == 5) {
        switch (movingColour) {
        case white:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
            }
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case black:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
            }
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    }

    if (rooktaken) {
        switch (toSquare) {
        case 0:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
            }
            break;
        case 7:
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case 56:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
            }
            break;
        case 63:
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    }

    // increment halfMoveClock
    if (is_capture(move) | (movingPiece % 6 == 0)) {
        clk = 0;
    } else {
        clk++;
    }

    // increment fullMoveClock
    if (movingColour == black) full_clk++;

    // change hash for different side to move
    hash ^= zobristKeys[780];

    return new Board(startingPos, castling, ep, dpp, clk, full_clk, otherColour,
                   opening_value, endgame_value, hash);
}

void Board::doMoveInPlace(move_t move) {
    int i;
    uint16_t fromSquare = from_sq(move);
    uint16_t toSquare = to_sq(move);
    colour otherColour = flipColour(sideToMove);
    colourPiece movingPiece;
    bool rooktaken = false;
    bool foundMovingPiece = false;

    for (i = (sideToMove * 6); i < (1 + sideToMove)*6; i++) {
        if (is_bit_set(pieceBoards[i], fromSquare)) {
            movingPiece = colourPiece(i);
            pieceBoards[i] = (pieceBoards[i] & ~(1ULL << fromSquare)) |
                             (1ULL << toSquare);
            opening_value -= pieceSquareTables[0][i][fromSquare];
            endgame_value -= pieceSquareTables[1][i][fromSquare];
            opening_value += pieceSquareTables[0][i][toSquare];
            endgame_value += pieceSquareTables[1][i][toSquare];
            hash_value ^= zobristKeys[i * 64 + fromSquare];
            hash_value ^= zobristKeys[i * 64 + toSquare];
            foundMovingPiece = true;
            break;
        }
    }

    if (!foundMovingPiece) {
        return;
    }

    if (is_capture(move)) {
        if (!is_ep_capture(move)) {
            for (i = (1 - sideToMove)*6; i < (2 - sideToMove)*6; i++) {
                if (is_bit_set(pieceBoards[i], toSquare)) {
                    if (i % 6 == 1) rooktaken = true;
                    pieceBoards[i] = (pieceBoards[i] & ~(1ULL << toSquare));
                    opening_value -= pieceSquareTables[0][i][toSquare];
                    endgame_value -= pieceSquareTables[1][i][toSquare];
                    opening_value -= pieceValues[0][i];
                    endgame_value -= pieceValues[1][i];
                    hash_value ^= zobristKeys[i * 64 + toSquare];
                    break;
                }
            }
        } else {
            int _dir = (sideToMove == white) ? S : N;
            pieceBoards[(1 - sideToMove)*6] &= ~(1ULL << (toSquare + _dir));
            opening_value -= pieceSquareTables[0]
                                [(1 - sideToMove)*6][toSquare + _dir];
            endgame_value -= pieceSquareTables[1]
                                [(1 - sideToMove)*6][toSquare + _dir];
            opening_value -= pieceValues[0][(1 - sideToMove)*6];
            endgame_value -= pieceValues[1][(1 - sideToMove)*6];
            hash_value ^= zobristKeys[(1 - sideToMove)*384 + toSquare + _dir];
        }
    }

    if (is_kingCastle(move)) {
        pieceBoards[1 + (6 * sideToMove)] =
                            (pieceBoards[1 + (6 * sideToMove)] & ~
                            (1ULL << (fromSquare + 3))) |
                            (1ULL << (toSquare - 1));
        opening_value -= pieceSquareTables[0]
                            [1 + (6 * sideToMove)][fromSquare + 3];
        endgame_value -= pieceSquareTables[1]
                            [1 + (6 * sideToMove)][fromSquare + 3];
        opening_value += pieceSquareTables[0]
                            [1 + (6 * sideToMove)][toSquare - 1];
        endgame_value += pieceSquareTables[1]
                            [1 + (6 * sideToMove)][toSquare - 1];
        hash_value ^= zobristKeys[(1 + (6 * sideToMove))*64 + fromSquare + 3];
        hash_value ^= zobristKeys[(1 + (6 * sideToMove))*64 + toSquare - 1];
    } else if (is_queenCastle(move)) {
        pieceBoards[1 + (6 * sideToMove)] =
                            (pieceBoards[1 + (6 * sideToMove)] & ~
                            (1ULL << (fromSquare - 4))) |
                            (1ULL << (toSquare + 1));
        opening_value -= pieceSquareTables[0]
                            [1 + (6 * sideToMove)][fromSquare - 4];
        endgame_value -= pieceSquareTables[1]
                            [1 + (6 * sideToMove)][fromSquare - 4];
        opening_value += pieceSquareTables[0]
                            [1 + (6 * sideToMove)][toSquare + 1];
        endgame_value += pieceSquareTables[1]
                            [1 + (6 * sideToMove)][toSquare + 1];
        hash_value ^= zobristKeys[(1 + (6 * sideToMove))*64 + fromSquare - 4];
        hash_value ^= zobristKeys[(1 + (6 * sideToMove))*64 + toSquare + 1];
    }

    // promotion
    if (is_promotion(move)) {
        pieceBoards[6 * sideToMove] &= (~(1ULL << toSquare));
        opening_value -= pieceSquareTables[0][6 * sideToMove][toSquare];
        endgame_value -= pieceSquareTables[1][6 * sideToMove][toSquare];
        opening_value -= pieceValues[0][6 * sideToMove];
        endgame_value -= pieceValues[1][6 * sideToMove];
        hash_value ^= zobristKeys[6 * sideToMove * 64 + toSquare];

        colourPiece prom_piece = colourPiece((6 * sideToMove) +
                                             which_promotion(move));

        pieceBoards[prom_piece] |= (1ULL << toSquare);
        opening_value += pieceSquareTables[0][prom_piece][toSquare];
        endgame_value += pieceSquareTables[1][prom_piece][toSquare];
        opening_value += pieceValues[0][prom_piece];
        endgame_value += pieceValues[1][prom_piece];
        hash_value ^= zobristKeys[prom_piece * 64 + toSquare];
    }


    // check for double pawn push
    if (lastMoveDoublePawnPush) {
        hash_value ^= zobristKeys[772 + dPPFile];
    }
    if (is_doublePP(move)) {
        lastMoveDoublePawnPush = true;
        dPPFile = fromSquare % 8;
        hash_value ^= zobristKeys[772 + dPPFile];
    } else {
        lastMoveDoublePawnPush = false;
    }

    // check for changes to castling rights
    if (movingPiece % 6 == 1) {
        switch (fromSquare) {
        case 0:
            if (castleWhiteQueenSide) {
                hash_value ^= zobristKeys[769];
                castleWhiteQueenSide = false;
            }
            break;
        case 7:
            if (castleWhiteKingSide) {
                hash_value ^= zobristKeys[768];
                castleWhiteKingSide = false;
            }
            break;
        case 56:
            if (castleBlackQueenSide) {
                hash_value ^= zobristKeys[771];
                castleBlackQueenSide = false;
            }
            break;
        case 63:
            if (castleBlackKingSide) {
                hash_value ^= zobristKeys[770];
                castleBlackKingSide = false;
            }
            break;
        }
    } else if (movingPiece % 6 == 5) {
        switch (sideToMove) {
        case white:
            if (castleWhiteQueenSide) {
                hash_value ^= zobristKeys[769];
                castleWhiteQueenSide = false;
            }
            if (castleWhiteKingSide) {
                hash_value ^= zobristKeys[768];
                castleWhiteKingSide = false;
            }
            break;
        case black:
            if (castleBlackQueenSide) {
                hash_value ^= zobristKeys[771];
                castleBlackQueenSide = false;
            }
            if (castleBlackKingSide) {
                hash_value ^= zobristKeys[770];
                castleBlackKingSide = false;
            }
            break;
        }
    }

    if (rooktaken) {
        switch (toSquare) {
        case 0:
            if (castleWhiteQueenSide) {
                hash_value ^= zobristKeys[769];
                castleWhiteQueenSide = false;
            }
            break;
        case 7:
            if (castleWhiteKingSide) {
                hash_value ^= zobristKeys[768];
                castleWhiteKingSide = false;
            }
            break;
        case 56:
            if (castleBlackQueenSide) {
                hash_value ^= zobristKeys[771];
                castleBlackQueenSide = false;
            }
            break;
        case 63:
            if (castleBlackKingSide) {
                hash_value ^= zobristKeys[770];
                castleBlackKingSide = false;
            }
            break;
        }
    }

    // increment halfMoveClock
    if (is_capture(move) | (movingPiece % 6 == 0)) {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }

    // increment fullMoveClock
    if (sideToMove == black) fullMoveClock++;

    // change hash for different side to move
    hash_value ^= zobristKeys[780];

    // change side to move
    sideToMove = otherColour;
}


void Player::doMoveInPlace(move_t move) {
    std::string san = SAN_pre_move(move);
    Board::doMoveInPlace(move);
    move_history.push_back(move);
    move_history_san.push_back(san);
}

void Player::makeChild(Board* child, move_t move) const {
    *child = *this;
    child->doMoveInPlace(move);
}

}   // namespace chessCore
