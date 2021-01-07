/* Copyright 2020 Freddy Pringle */
#include "board.h"
#include "move.h"
#include "twiddle.h"
#include "typedefs.h"


namespace chessCore {

bool Board::is_check(colour side) const {
    // on-the-fly check detection
    int kingpos = last_set_bit(pieceBoards[ (6 * side) + 5 ]);
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    colour otherSide = flipColour(side);

    // pawns
    if (pawnAttackNaive(kingpos,
                        side) & pieceBoards[(6 * otherSide) + 0]) return true;
    // rooks
    if (rookTargets(kingpos, _white, _black,
                    side) & pieceBoards[(6 * otherSide) + 1]) return true;
    // knights
    if (knightTargets(kingpos, _white, _black,
                      side) & pieceBoards[(6 * otherSide) + 2]) return true;
    // bishops
    if (bishopTargets(kingpos, _white, _black,
                      side) & pieceBoards[(6 * otherSide) + 3]) return true;
    // queens
    if (queenTargets(kingpos, _white, _black,
                     side) & pieceBoards[(6 * otherSide) + 4]) return true;

    return false;
}

bool Board::is_check(colour side, piece * checkingPiece, int * checkingInd,
                     bool * doubleCheck) const {
    // on-the-fly check detection
    int kingpos = last_set_bit(pieceBoards[ (6 * side) + 5 ]);
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    colour otherSide = flipColour(side);
    bitboard _check;
    int count = 0;

    // pawns
    _check = pawnAttackNaive(kingpos, side) & pieceBoards[(6 * otherSide) + 0];
    if (_check) {
        *checkingPiece = piece(0);
        *checkingInd = last_set_bit(_check);
        count += count_bits_set(_check);
    }
    // rooks
    _check = rookTargets(kingpos, _white, _black,
                         side) & pieceBoards[(6 * otherSide) + 1];
    if (_check) {
        *checkingPiece = piece(1);
        *checkingInd = last_set_bit(_check);
        count += count_bits_set(_check);
        if (count == 2) {
            * doubleCheck = true;
            return true;
        }
    }
    // knights
    _check = knightTargets(kingpos, _white, _black,
                           side) & pieceBoards[(6 * otherSide) + 2];
    if (_check) {
        *checkingPiece = piece(2);
        *checkingInd = last_set_bit(_check);
        count += count_bits_set(_check);
        if (count == 2) {
            * doubleCheck = true;
            return true;
        }
    }
    // bishops
    _check = bishopTargets(kingpos, _white, _black,
                           side) & pieceBoards[(6 * otherSide) + 3];
    if (_check) {
        *checkingPiece = piece(3);
        *checkingInd = last_set_bit(_check);
        count += count_bits_set(_check);
        if (count == 2) {
            * doubleCheck = true;
            return true;
        }
    }
    // queens
    _check = queenTargets(kingpos, _white, _black,
                          side) & pieceBoards[(6 * otherSide) + 4];
    if (_check) {
        *checkingPiece = piece(4);
        *checkingInd = last_set_bit(_check);
        count += count_bits_set(_check);
        if (count == 2) {
            * doubleCheck = true;
            return true;
        }
    }

    return ( count > 0);
}

bool Board::is_checkmate() const {
    piece checkingPiece;
    int checkingInd;
    bool double_check = false;

    if (!is_check(sideToMove, &checkingPiece, &checkingInd, &double_check)) {
        return false;
    } else {
        return !can_get_out_of_check(sideToMove, checkingPiece, checkingInd,
                last_set_bit(pieceBoards[(6 * sideToMove) + 5]), double_check);
    }
}

bool Board::is_stalemate() const {
    if (halfMoveClock >= 50) return true;
    if (is_check(sideToMove)) return false;
    return gen_legal_moves().empty();
}

bool Board::gameover() const {
    return is_checkmate() || is_stalemate();
}

bool Board::was_lastmove_check(move_t lastmove) const {
    int i, j;
    colourPiece movingPiece;
    int from_ind = from_sq(lastmove);
    int to_ind = to_sq(lastmove);
    bitboard to_square = (1ULL << to_ind);
    bitboard kingBoard = pieceBoards[(sideToMove * 6) + 5];
    int king_ind = last_set_bit(kingBoard);

    colour otherSide = flipColour(sideToMove);
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    bitboard blockers = takenSquares();

    bool foundMovingPiece = false;

    for (i = (otherSide)*6; i < (1 + otherSide)*6; i++) {
        if (pieceBoards[i] & to_square) {
            movingPiece = colourPiece(i);
            foundMovingPiece = true;
            break;
        }
    }

    if (!foundMovingPiece) {
        return false;
    }

    // direct check
    switch (movingPiece % 6) {
    case 0:
        if (kingBoard & pawnAttacks(to_ind, _white, _black, otherSide)) {
            return true;
        }
        break;
    case 5:
        break;
    default:
        if (kingBoard & pieceTargets(to_ind, _white, _black, movingPiece))  {
            return true;
        }
        break;
    }

    // discovered check
    bitboard _ray;
    bitboard tmp;
    bitboard attacker;
    // rooks and queens
    for (i = 0; i < 8; i += 2) {
        if (kingBoard & rays[i][from_ind]) {
            j = (i + 4) % 8;
            _ray = rays[j][king_ind];
            tmp = blockers & _ray;
            if (tmp) {
                if (j == 0 || j == 2) {
                    attacker = (1ULL << first_set_bit(tmp));
                } else {
                    attacker = (1ULL << last_set_bit(tmp));
                }
                if (attacker & pieceBoards[(6 * otherSide) + 1]) return true;
                if (attacker & pieceBoards[(6 * otherSide) + 4]) return true;
            }
        }
    }
    // bishops and queens
    for (i = 1; i < 8; i += 2) {
        if (kingBoard & rays[i][from_ind]) {
            j = (i + 4) % 8;
            _ray = rays[j][king_ind];
            tmp = blockers & _ray;
            if (tmp) {
//                print_bb(tmp);
                if (j == 1 || j == 7) {
                    attacker = (1ULL << first_set_bit(tmp));
                } else {
                    attacker = (1ULL << last_set_bit(tmp));
                }
                if (attacker & pieceBoards[(6 * otherSide) + 3]) return true;
                if (attacker & pieceBoards[(6 * otherSide) + 4]) return true;
            }
        }
    }

    // en passant
    if (is_ep_capture(lastmove)) {
        int other_pawn_ind = to_ind + ((sideToMove == black) ? S : N);
        // rooks and queens
        for (i = 0; i < 8; i += 2) {
            if (kingBoard & rays[i][other_pawn_ind]) {
                j = (i + 4) % 8;
                _ray = rays[j][king_ind];
                tmp = blockers & _ray;
                if (tmp) {
    //                print_bb(tmp);
                    if (j == 0 || j == 2) {
                        attacker = (1ULL << first_set_bit(tmp));
                    } else {
                        attacker = (1ULL << last_set_bit(tmp));
                    }
    //                print_bb(attacker);
                    if (attacker & pieceBoards[(6 * otherSide) + 1]) {
                        return true;
                    }
                    if (attacker & pieceBoards[(6 * otherSide) + 4]) {
                        return true;
                    }
                }
            }
        }
        // bishops and queens
        for (i = 1; i < 8; i += 2) {
            if (kingBoard & rays[i][other_pawn_ind]) {
                j = (i + 4) % 8;
                _ray = rays[j][king_ind];
                tmp = blockers & _ray;
                if (tmp) {
    //                print_bb(tmp);
                    if (j == 1 || j == 7) {
                        attacker = (1ULL << first_set_bit(tmp));
                    } else {
                        attacker = (1ULL << last_set_bit(tmp));
                    }
                    if (attacker & pieceBoards[(6 * otherSide) + 3]) {
                        return true;
                    }
                    if (attacker & pieceBoards[(6 * otherSide) + 4]) {
                        return true;
                    }
                }
            }
        }
    }

    // castling rook
    if (is_kingCastle(lastmove)) {
        int rook_ind = to_ind - 1;
        if (kingBoard & rookTargets(rook_ind, _white, _black, otherSide)) {
            return true;
        }
    } else if (is_queenCastle(lastmove)) {
        int rook_ind = to_ind + 1;
        if (kingBoard & rookTargets(rook_ind, _white, _black, otherSide)) {
            return true;
        }
    }

    // promotion
    if (is_promotion(lastmove)) {
        colourPiece prom_piece = colourPiece((6 * otherSide) +
                                             which_promotion(lastmove));
        if (kingBoard & pieceTargets(to_ind, _white, _black, prom_piece)) {
            return true;
        }
    }

    return false;
}

bool Board::is_checking_move(move_t move) const {
    // assume the move is legal
    int i, j;
    colourPiece movingPiece;
    int from_ind = from_sq(move);
    int to_ind = to_sq(move);
    bitboard from_square = (1ULL << from_ind);
    bitboard kingBoard = pieceBoards[((1 - sideToMove)*6) + 5];
    int king_ind = last_set_bit(kingBoard);
    bool foundMovingPiece = false;

    for (i = sideToMove * 6; i < (1 + sideToMove)*6; i++) {
        if (pieceBoards[i] & from_square) {
            movingPiece = colourPiece(i);
            foundMovingPiece = true;
            break;
        }
    }

    if (!foundMovingPiece) {
        return false;
    }

//    colour otherSide = flipColour(sideToMove);
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    bitboard blockers = takenSquares();
    blockers &= ~from_square;
    blockers |= (1ULL << to_ind);
    int other_pawn_ind = to_ind + ((sideToMove == white) ? S : N);
    if (is_ep_capture(move)) {
        blockers &= ~(1ULL << other_pawn_ind);
    }

    // direct check
    if (pieceTargets(to_ind, _white, _black, movingPiece) & kingBoard) {
        return true;
    }

    // discover check
    bitboard _ray;
    bitboard tmp;
    bitboard attacker;
    // rooks and queens
    for (i = 0; i < 8; i += 2) {
        if (kingBoard & rays[i][from_ind]) {
            j = (i + 4) % 8;
            _ray = rays[j][king_ind];
            tmp = blockers & _ray;
            if (tmp) {
                if (j == 0 || j == 2) {
                    attacker = (1ULL << first_set_bit(tmp));
                } else {
                    attacker = (1ULL << last_set_bit(tmp));
                }
                if (attacker & pieceBoards[(6 * sideToMove) + 1]) return true;
                if (attacker & pieceBoards[(6 * sideToMove) + 4]) return true;
            }
        }
    }
    // bishops and queens
    for (i = 1; i < 8; i += 2) {
        if (kingBoard & rays[i][from_ind]) {
            j = (i + 4) % 8;
            _ray = rays[j][king_ind];
            tmp = blockers & _ray;
            if (tmp) {
                if (j == 1 || j == 7) {
                    attacker = (1ULL << first_set_bit(tmp));
                } else {
                    attacker = (1ULL << last_set_bit(tmp));
                }
                if (attacker & pieceBoards[(6 * sideToMove) + 3]) return true;
                if (attacker & pieceBoards[(6 * sideToMove) + 4]) return true;
            }
        }
    }


    // en passant
    if (is_ep_capture(move)) {
        // rooks and queens
        for (i = 0; i < 8; i += 2) {
            if (kingBoard & rays[i][other_pawn_ind]) {
                j = (i + 4) % 8;
                _ray = rays[j][king_ind];
                tmp = blockers & _ray;
                if (tmp) {
                    if (j == 0 || j == 2) {
                        attacker = (1ULL << first_set_bit(tmp));
                    } else {
                        attacker = (1ULL << last_set_bit(tmp));
                    }
                    if (attacker & pieceBoards[(6 * sideToMove) + 1]) {
//                        std::cerr << "EP discovered check by rook at "
//                                << last_set_bit(attacker) << "\n";
                        return true;
                    }
                    if (attacker & pieceBoards[(6 * sideToMove) + 4]) {
//                        std::cerr << "EP discovered check by queen at "
//                                << last_set_bit(attacker) << "\n";
                        return true;
                    }
                }
            }
        }
        // bishops and queens
        for (i = 1; i < 8; i += 2) {
            if (kingBoard & rays[i][other_pawn_ind]) {
                j = (i + 4) % 8;
                _ray = rays[j][king_ind];
                tmp = blockers & _ray;
                if (tmp) {
                    if (j == 1 || j == 7) {
                        attacker = (1ULL << first_set_bit(tmp));
                    } else {
                        attacker = (1ULL << last_set_bit(tmp));
                    }
                    if (attacker & pieceBoards[(6 * sideToMove) + 3]) {
//                        std::cerr << "EP discovered check by bishop at "
//                                << last_set_bit(attacker) << "\n";
                        return true;
                    }
                    if (attacker & pieceBoards[(6 * sideToMove) + 4]) {
//                        std::cerr << "EP discovered check by queen at "
//                                << last_set_bit(attacker) << "\n";
                        return true;
                    }
                }
            }
        }
    }


    // castling rook
    if (is_kingCastle(move)) {
        int rook_ind = to_ind - 1;
        if (kingBoard & rookTargets(rook_ind, _white, _black, sideToMove)) {
            return true;
        }
    } else if (is_queenCastle(move)) {
        int rook_ind = to_ind + 1;
        if (kingBoard & rookTargets(rook_ind, _white, _black, sideToMove)) {
            return true;
        }
    }

    // promotion
    if (is_promotion(move)) {
        colourPiece prom_piece = colourPiece((6 * sideToMove) +
                                             which_promotion(move));
        if (kingBoard & pieceTargets(to_ind, _white, _black, prom_piece)) {
            return true;
        }
    }

    return false;
}


}   // namespace chessCore
