#include "board.h"
#include "action.h"
#include "move.h"
#include "search.h"
#include "play.h"
#include "typedefs.h"

#include <limits>
#include <cstdint>
#include <time.h>

#define USE_TABLE

namespace chessCore {

value_t Minimax::algorithm(board b, int depth, colour side) {
    if (depth == 0) return b.getValue();

    value_t score;
    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    board child;

    if (side == white) {
        value_t max_score = std::numeric_limits<value_t>::max();
        if (num_moves == 0) {
            if (b.is_check(black)) {
                return max_score + 10;
            }
        }

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = Minimax::algorithm(child, depth - 1, black);
            if (score > max_score) max_score = score;
        }
        return max_score;
    }

    else {
        value_t min_score = std::numeric_limits<value_t>::max();
        if (num_moves == 0) {
            if (b.is_check(black)) {
                return min_score - 10;
            }
        }

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = algorithm(child, depth - 1, white);
            if (score < min_score) min_score = score;
        }
        return min_score;
    }
}

move_t Minimax::search(board b, int depth) {
    value_t score;
    board child;
    colour side;
    b.getSide(&side);
    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0 || depth == 0) return move_t(0, 0, 0, 0, 0, 0);
    struct move_t best_move;

    if (side == white) {
        value_t max_score = std::numeric_limits<value_t>::min();

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = Minimax::algorithm(child, depth - 1, black);
            if (score > max_score) {
                max_score = score;
                best_move = moves[i];
            }
        }
        return best_move;
    }

    else {
        value_t min_score = std::numeric_limits<value_t>::max();

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = Minimax::algorithm(child, depth - 1, white);
            if (score < min_score) {
                min_score = score;
                best_move = moves[i];
            }
        }
        return best_move;
    }
}

value_t Negamax::algorithm(board b, int depth, colour side) {
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    value_t score;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    value_t max_score = std::numeric_limits<value_t>::min();
    if (num_moves == 0) return (max_score + 10) * ((side == white) ? 1 : -1);

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -Negamax::algorithm(child, depth - 1, otherSide);
        if (score > max_score) max_score = score;
    }
    return max_score;
}

move_t Negamax::search(board b, int depth) {
    value_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    value_t max_score = std::numeric_limits<value_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -Negamax::algorithm(child, depth - 1, otherSide);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

value_t NegamaxAB::algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta) {
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    value_t score;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    value_t max_score = std::numeric_limits<value_t>::min();
    if (num_moves == 0) return (max_score + 10) * ((side == white) ? 1 : -1);
    //  value_t max_score = alpha;

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegamaxAB::algorithm(child, depth - 1, otherSide, -beta, -alpha);
        if (score > max_score) max_score = score;
        if (max_score > alpha) alpha = max_score;
        if (alpha >= beta) break;
    }
    return max_score;
}

move_t NegamaxAB::search(board b, int depth) {
    value_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    value_t alpha = std::numeric_limits<value_t>::min() + 1;
    value_t beta = std::numeric_limits<value_t>::max() - 1;

    value_t max_score = std::numeric_limits<value_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegamaxAB::algorithm(child, depth - 1, otherSide, - beta, - alpha);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

value_t NegaScoutOriginal::algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta) {
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    value_t score, a = alpha, _b = beta;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        value_t minusinf = (std::numeric_limits<value_t>::min() + 10);
        return minusinf * ((side == white) ? 1 : -1);
    }

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = - NegaScoutOriginal::algorithm(child, depth-1, otherSide, -_b, -a);
        if (score > a && score < _b && i > 0) {
            a = - NegaScoutOriginal::algorithm(child, depth-1, otherSide, -beta, -score);
        }
        a = std::max(a, score);
        if (a >= beta) {
            return a;
        }
        _b = a + 1;
    }
    return a;
}

move_t NegaScoutOriginal::search(board b, int depth) {
    value_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    value_t alpha = std::numeric_limits<value_t>::min() + 1;
    value_t beta = std::numeric_limits<value_t>::max() - 1;

    value_t max_score = std::numeric_limits<value_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegaScoutOriginal::algorithm(child, depth - 1, otherSide, - beta, - alpha);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

value_t NegaScoutAlternative::algorithm(board b, int depth, colour side,
                             value_t alpha, value_t beta) {
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    value_t _a, _b = beta;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        value_t minusinf = (std::numeric_limits<value_t>::min() + 10);
        return minusinf * ((side == white) ? 1 : -1);
    }

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        _a = - NegaScoutAlternative::algorithm(child, depth-1, otherSide, -_b, -alpha);
        if (_a > alpha && _a < _b && i > 0) {
            _a = - NegaScoutAlternative::algorithm(child, depth-1, otherSide, -beta, -alpha);
        }
        alpha = std::max(alpha, _a);
        if (alpha >= beta) {
            return alpha;
        }
        _b = alpha + 1;
    }
    return alpha;
}

move_t NegaScoutAlternative::search(board b, int depth) {
    value_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    value_t alpha = std::numeric_limits<value_t>::min() + 1;
    value_t beta = std::numeric_limits<value_t>::max() - 1;

    value_t max_score = std::numeric_limits<value_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegaScoutAlternative::algorithm(child, depth - 1, otherSide, - beta, - alpha);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

value_t PVS::algorithm(board b, int depth, colour side,
                       value_t alpha, value_t beta) {
    if (depth == 0) return quiesce(b, side, alpha, beta);

    colour otherSide = (side == white) ? black : white;
    bool bSearchPv = true;
    board child;
    struct move_t moves[256];
    value_t score;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        value_t ret = (std::numeric_limits<value_t>::min() + 10);
        ret *= ((side == white) ? 1 : -1);
        return ret;
    }

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        if (bSearchPv) {
            score = - PVS::algorithm(child, depth - 1, otherSide, -beta, -alpha);
        }
        else {
            score = - PVS::algorithm(child, depth - 1, otherSide, -alpha - 1, -alpha);
            if (score > alpha) {
                score = - PVS::algorithm(child, depth - 1, otherSide, -beta, -alpha);
            }
        }

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            bSearchPv = false;
        }
    }
    return alpha;
}

move_t PVS::search(board b, int depth) {
    value_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;
    value_t max_score = std::numeric_limits<value_t>::min();

    value_t alpha = std::numeric_limits<value_t>::min() + 1;
    value_t beta = std::numeric_limits<value_t>::max() - 1;

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -PVS::algorithm(child, depth - 1, otherSide, alpha, beta);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}


value_t quiesce(board b, colour side, value_t alpha, value_t beta) {
    value_t stand_pat = b.getValue() * ((side == white) ? 1 : -1);

//    std::cout << b << std::endl;
//    std::cout << b.FEN() << std::endl;
//    return stand_pat;
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;


    colour otherSide = (side == white) ? black : white;
    move_t captures[256];
    int num_captures = b.gen_captures(captures);
    board child;
    value_t score;

    for (int i=0; i<num_captures; i++) {
        child = doMove(b, captures[i]);
        score = - quiesce(child, otherSide, -beta, -alpha);
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}


void reorder_moves(move_t * moves, int num_moves, move_t best_move) {
    for (int i=0; i<num_moves; i++) {
        if (moves[i] == best_move) {
            moves[i] = moves[0];
            moves[0] = best_move;
            break;
        }
    }
}


value_t Player::principal_variation(board b, colour side, uint8_t depth,
                                 value_t alpha, value_t beta) {

    value_t ret;

#ifdef USE_TABLE
    uint64_t sig;
    uint32_t ind;
    move_t bestMove(0,0,0,0,0,0);
    value_t ibv;
    uint8_t age;
    b.getHash(&sig);
    ind = (uint32_t)sig;
    getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table.find(ind);
    if (it != trans_table.end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
            if (record.depth >= depth) {
                switch (record.IBV_score % 4) {
                    case -1:
                    case 3:
                        // upper bound
                        if (beta > (record.IBV_score + 1) / 4) {
                            beta = (record.IBV_score + 1) / 4;
                        }
                        break;
                    case 1:
                    case -3:
                        // lower bound
                        if (alpha < (record.IBV_score - 1) / 4) {
                            alpha = (record.IBV_score - 1) / 4;
                        }
                        break;
                    case 0:
                    case -4:
                        // exact
                        return record.IBV_score / 4;
                }
                if (alpha >= beta) {
                    return (record.IBV_score + 1) / 4;
                }
            }
        }
    }

#endif

    if (depth == 0) {
        ret = quiesce(b, side, alpha, beta);
        return ret;
    }

    colour otherSide = (side == white) ? black : white;
    bool bSearchPv = true;
    board child;
    struct move_t moves[256];
    value_t score;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10) * is_checkmate();
        ret *= ((side == white) ? 1 : -1);
#ifdef USE_TABLE
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table[ind] = record;
#endif
        return ret;
    }

#ifdef USE_TABLE
    if (bestMove.give()) {
        reorder_moves(moves, num_moves, bestMove);
    }
#endif

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        if (bSearchPv) {
            score = - principal_variation(child, otherSide, depth - 1, -beta, -alpha);
        }
        else {
            score = - principal_variation(child, otherSide, depth - 1, -alpha - 1, -alpha);
            if (score > alpha) {
                score = - principal_variation(child, otherSide, depth - 1, -beta, -alpha);
            }
        }

        if (score >= beta) {
            // lower bound
#ifdef USE_TABLE
            ibv = 4 * beta + 1;
            bestMove = moves[i];
            record = {sig, bestMove, depth, ibv, age};
            trans_table[ind] = record;
#endif
            return beta;
        }
        if (score > alpha) {
            // upper bound
            alpha = score;
            bSearchPv = false;
#ifdef USE_TABLE
            bestMove = moves[i];
#endif
        }
    }
#ifdef USE_TABLE
    if (bSearchPv) {
        // exact
        ibv = 4 * alpha - 1;
    }
    else {
        ibv = 4 * alpha;
        // upper bound
    }
    record = {sig, bestMove, depth, ibv, age};
    trans_table[ind] = record;
#endif
    return alpha;
}


value_t Player::negamax_alphabeta(board b, colour side, uint8_t depth,
                                 value_t alpha, value_t beta) {

    value_t ret;
    value_t alphaOrig = alpha;

#ifdef USE_TABLE
    uint64_t sig;
    uint32_t ind;
    move_t bestMove(0,0,0,0,0,0);
    value_t ibv;
    uint8_t age;
    b.getHash(&sig);
    ind = (uint32_t)sig;
    getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table.find(ind);
    if (it != trans_table.end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
            if (record.depth >= depth) {
                switch (record.IBV_score % 4) {
                    case -1:
                    case 3:
                        // upper bound
                        if (beta > (record.IBV_score + 1) / 4) {
                            beta = (record.IBV_score + 1) / 4;
                        }
                        break;
                    case 1:
                    case -3:
                        // lower bound
                        if (alpha < (record.IBV_score - 1) / 4) {
                            alpha = (record.IBV_score - 1) / 4;
                        }
                        break;
                    case 0:
                    case -4:
                        // exact
                        return record.IBV_score / 4;
                }
                if (alpha >= beta) {
                    return (record.IBV_score + 1) / 4;
                }
            }
        }
    }

#endif

    if (depth == 0) {
//        std::cout << b.FEN() << std::endl;
        ret = quiesce(b, side, alpha, beta);
        return ret;
    }

    colour otherSide = (side == white) ? black : white;
    board child;
    struct move_t moves[256];
    value_t score, value = std::numeric_limits<value_t>::min() + 10;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10) * is_checkmate();
        ret *= ((side == white) ? 1 : -1);
#ifdef USE_TABLE
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table[ind] = record;
#endif
        return ret;
    }

#ifdef USE_TABLE
    if (bestMove.give()) {
        reorder_moves(moves, num_moves, bestMove);
    }
#endif

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = - principal_variation(child, otherSide, depth - 1, -beta, -alpha);
        if (value < score) {
#ifdef USE_TABLE
            bestMove = moves[i];
#endif
            value = score;
        }
        if (alpha < value) {
            alpha = value;
        }
        if (alpha >= beta) break;

    }
#ifdef USE_TABLE
    if (value <= alphaOrig) {
        ibv = 4 * value - 1;
    }
    else if (value >= beta) {
        ibv = 4 * value + 1;
    }
    else {
        ibv = 4 * value;
    }
    record = {sig, bestMove, depth, ibv, age};
    trans_table[ind] = record;
#endif
    return value;
}

move_t Player::search_negamax_alphabeta(uint8_t depth) {

    value_t max_score = std::numeric_limits<value_t>::min();
    value_t alpha = std::numeric_limits<value_t>::min() / 10;
    value_t beta = std::numeric_limits<value_t>::max() / 10;

#ifdef USE_TABLE
    uint64_t sig;
    uint32_t ind;
    move_t bestMove(0,0,0,0,0,0);
    value_t ibv;
    uint8_t age;
    getHash(&sig);
    ind = (uint32_t)sig;
    getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table.find(ind);
    if (it != trans_table.end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
            if (record.depth >= depth) {
                switch (record.IBV_score % 4) {
                    case -1:
                    case 3:
                        // upper bound
                        if (beta > (record.IBV_score + 1) / 4) {
                            beta = (record.IBV_score + 1) / 4;
                        }
                        break;
                    case 1:
                    case -3:
                        // lower bound
                        if (alpha < (record.IBV_score - 1) / 4) {
                            alpha = (record.IBV_score - 1) / 4;
                        }
                        break;
                    case 0:
                    case -4:
                        // exact
                        //return record.IBV_score / 4;
                        return bestMove;
                }
                if (alpha >= beta) {
                    //return (record.IBV_score + 1) / 4;
                }
            }
        }
    }

#endif

    value_t score;
    board child;
    colour side;
    getSide(&side);
    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    struct move_t best_move;

#ifdef USE_TABLE
    if (bestMove.give()) {
        reorder_moves(moves, num_moves, bestMove);
    }
#endif

    for (int i = 0; i < num_moves; i++) {
        makeChild(&child, moves[i]);
        score = - negamax_alphabeta(child, otherSide, depth - 1, alpha, beta);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
        if (alpha < score) {
            alpha = score;
        }
        if (alpha >= beta) break;
    }
#ifdef USE_TABLE
    if (max_score <= alpha) {
        ibv = 4 * max_score - 1;
    }
    else if (max_score >= beta) {
        ibv = 4 * max_score + 1;
    }
    else {
        ibv = 4 * max_score;
    }
    record = {sig, best_move, depth, ibv, age};
    trans_table[ind] = record;
#endif
    return best_move;
}

move_t Player::search_negamax_alphabeta(uint8_t depth, move_t first_move, double time_remaining) {

    value_t max_score = std::numeric_limits<value_t>::min();
    value_t alpha = std::numeric_limits<value_t>::min() / 10;
    value_t beta = std::numeric_limits<value_t>::max() / 10;

    clock_t start_time = clock();

#ifdef USE_TABLE
    uint64_t sig;
    uint32_t ind;
    move_t bestMove(0,0,0,0,0,0);
    value_t ibv;
    uint8_t age;
    getHash(&sig);
    ind = (uint32_t)sig;
    getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table.find(ind);
    if (it != trans_table.end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
            if (record.depth >= depth) {
                switch (record.IBV_score % 4) {
                    case -1:
                    case 3:
                        // upper bound
                        if (beta > (record.IBV_score + 1) / 4) {
                            beta = (record.IBV_score + 1) / 4;
                        }
                        break;
                    case 1:
                    case -3:
                        // lower bound
                        if (alpha < (record.IBV_score - 1) / 4) {
                            alpha = (record.IBV_score - 1) / 4;
                        }
                        break;
                    case 0:
                    case -4:
                        // exact
                        //return record.IBV_score / 4;
                        return bestMove;
                }
                if (alpha >= beta) {
                    //return (record.IBV_score + 1) / 4;
                }
            }
        }
    }

#endif

    value_t score;
    board child;
    colour side;
    getSide(&side);
    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    struct move_t best_move;

    if (first_move.give()) {
        reorder_moves(moves, num_moves, first_move);
    }

#ifdef USE_TABLE
    if (bestMove.give()) {
        reorder_moves(moves, num_moves, bestMove);
    }
#endif

    for (int i = 0; i < num_moves; i++) {
        makeChild(&child, moves[i]);
        if (child.is_checkmate(otherSide)) {
            return moves[i];
        }
        if (double(clock() - start_time) / double(CLOCKS_PER_SEC) > time_remaining) {
            return {0,0,0,0,0,0};
        }
        score = - negamax_alphabeta(child, otherSide, depth - 1, alpha, beta);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
        if (alpha < score) {
            alpha = score;
        }
        if (alpha >= beta) break;
    }
#ifdef USE_TABLE
    if (max_score <= alpha) {
        ibv = 4 * max_score - 1;
    }
    else if (max_score >= beta) {
        ibv = 4 * max_score + 1;
    }
    else {
        ibv = 4 * max_score;
    }
    record = {sig, best_move, depth, ibv, age};
    trans_table[ind] = record;
#endif
    return best_move;
}

move_t Player::search_principal_variation(uint8_t depth) {

    value_t max_score = std::numeric_limits<value_t>::min();
    value_t alpha = std::numeric_limits<value_t>::min() / 10;
    value_t beta = std::numeric_limits<value_t>::max() / 10;

#ifdef USE_TABLE
    uint64_t sig;
    uint32_t ind;
    move_t bestMove(0,0,0,0,0,0);
    value_t ibv;
    uint8_t age;
    getHash(&sig);
    ind = (uint32_t)sig;
    getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table.find(ind);
    if (it != trans_table.end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
            if (record.depth >= depth) {
                switch (record.IBV_score % 4) {
                    case -1:
                    case 3:
                        // upper bound
                        if (beta > (record.IBV_score + 1) / 4) {
                            beta = (record.IBV_score + 1) / 4;
                        }
                        break;
                    case 1:
                    case -3:
                        // lower bound
                        if (alpha < (record.IBV_score - 1) / 4) {
                            alpha = (record.IBV_score - 1) / 4;
                        }
                        break;
                    case 0:
                    case -4:
                        // exact
                        //return record.IBV_score / 4;
                        return bestMove;
                }
                if (alpha >= beta) {
                    //return (record.IBV_score + 1) / 4;
                }
            }
        }
    }

#endif

    value_t score;
    board child;
    colour side;
    getSide(&side);
    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    struct move_t best_move;

#ifdef USE_TABLE
    if (bestMove.give()) {
        reorder_moves(moves, num_moves, bestMove);
    }
#endif

    for (int i = 0; i < num_moves; i++) {
        makeChild(&child, moves[i]);
        score = - negamax_alphabeta(child, otherSide, depth - 1, alpha, beta);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
        if (alpha < score) {
            alpha = score;
        }
        if (alpha >= beta) break;
    }
#ifdef USE_TABLE
    if (max_score <= alpha) {
        ibv = 4 * max_score - 1;
    }
    else if (max_score >= beta) {
        ibv = 4 * max_score + 1;
    }
    else {
        ibv = 4 * max_score;
    }
    record = {sig, best_move, depth, ibv, age};
    trans_table[ind] = record;
#endif
    return best_move;
}

move_t Player::iterative_deepening(int timeout, uint8_t max_depth) {
    clock_t start_time = clock();
    double time_taken = 0.0;
    uint8_t depth = 1;
    move_t best_move(0,0,0,0,0,0);
    move_t new_move;

    while (time_taken < timeout && depth <= max_depth) {
        new_move = search_negamax_alphabeta(depth, best_move, timeout - time_taken);
        depth++;
        if (new_move.give()) best_move = new_move;
        time_taken = double(clock() - start_time) / double(CLOCKS_PER_SEC);
        std::cout << "Depth searched: " << depth - 1 << "   ("
                  << time_taken
                  << " seconds total)"
                  << "  (best move so far: "
                  << best_move << ")" << std::endl;
    }

    return best_move;
}

move_t Player::search(uint8_t depth) {
    return iterative_deepening(1);
//    return search_negamax_alphabeta(depth);
}


} // end of chessCore namespace
