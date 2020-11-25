#include "search.h"

#include <time.h>

#include <cstdint>
#include <limits>
#include <vector>

#include "action.h"
#include "board.h"
#include "move.h"
#include "play.h"
#include "typedefs.h"

#define USE_TABLE
#define DEBUG 0

namespace chessCore {


value_t quiesce(board b, colour side, value_t alpha, value_t beta) {
    value_t stand_pat = b.getValue() * ((side == white) ? 1 : -1);

//    std::cout << b << std::endl;
//    std::cout << b.FEN() << std::endl;
//    return stand_pat;
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;


    colour otherSide = flipColour(side);
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
    move_t bestMove = 0;
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

    colour otherSide = flipColour(side);
    bool bSearchPv = true;
    board child;
    move_t moves[256];
    value_t score;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10);
        if (is_checkmate()) ret *= ((side == white) ? 1 : -1);
        else ret = 0;
#ifdef USE_TABLE
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table[ind] = record;
#endif
        return ret;
    }

#ifdef USE_TABLE
    if (bestMove) {
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

#ifdef USE_TABLE
    value_t alphaOrig = alpha;
    uint64_t sig;
    uint32_t ind;
    move_t bestMove = 0;
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

    colour otherSide = flipColour(side);
    board child;
    move_t moves[256];
    value_t score, value = std::numeric_limits<value_t>::min() + 10;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10);
        if (is_checkmate()) ret *= ((side == white) ? 1 : -1);
        else ret = 0;
#ifdef USE_TABLE
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table[ind] = record;
#endif
        return ret;
    }

#ifdef USE_TABLE
    if (bestMove) {
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
    move_t bestMove = 0;
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
    colour otherSide = flipColour(side);

    move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    move_t best_move = 0;

#ifdef USE_TABLE
    if (bestMove) {
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
    move_t bestMove = 0;
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
    colour otherSide = flipColour(side);

    move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    move_t best_move = 0;

    if (first_move) {
        reorder_moves(moves, num_moves, first_move);
    }

#ifdef USE_TABLE
    if (bestMove) {
        reorder_moves(moves, num_moves, bestMove);
    }
#endif

    for (int i = 0; i < num_moves; i++) {
        makeChild(&child, moves[i]);
        if (child.is_checkmate()) {
            return moves[i];
        }
        if (double(clock() - start_time) / double(CLOCKS_PER_SEC) > time_remaining) {
            return 0;
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
    move_t bestMove = 0;
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
    colour otherSide = flipColour(side);

    move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    move_t best_move = 0;

#ifdef USE_TABLE
    if (bestMove) {
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
    move_t best_move = 0;
    move_t new_move;
    std::vector<move_t> moves;
    int sz;

    while (time_taken < timeout && depth <= max_depth) {
        new_move = search_negamax_alphabeta(depth, best_move, timeout - time_taken);
        depth++;
        if (new_move) {
            best_move = new_move;
            moves.push_back(best_move);
        }
        time_taken = double(clock() - start_time) / double(CLOCKS_PER_SEC);
#if DEBUG
        std::cout << "Depth searched: " << depth - 1 << "   ("
                  << time_taken
                  << " seconds total)"
                  << "  (best move so far: "
                  << best_move << ")" << std::endl;
#endif

        sz = moves.size();
        if (((sz >= 8) || (sz >= 5 && time_taken > 10.0)) && moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}

move_t Player::iterative_deepening() {
    return iterative_deepening(iterative_deepening_timeout);
}


} // end of chessCore namespace
