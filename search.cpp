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


namespace chessCore {

std::ostream& operator<<(std::ostream &out, const record_t &rec) {
    value_t val;

    prettyMove pm = {rec.best_ref_move};

    out << "Best/refutation move:     " << pm << std::endl
        << "Depth searched:           " << +rec.depth << std::endl
        << "Clock when last searched: " << +rec.age << std::endl
        << "Bound flag:               ";

    switch (rec.IBV_score % 4) {
        case -1:
        case 3:
            out << "Upper" << std::endl;
            val = (rec.IBV_score + 1) / 4;
            break;
        case 1:
        case -3:
            out << "Lower" << std::endl;
            val = (rec.IBV_score - 1) / 4;
            break;
        case 0:
        case -4:
        default:
            out << "Exact" << std::endl;
            val = rec.IBV_score / 4;
            break;
    }

    out << "Value:                    " << val << std::endl;
    return out;
}


Searcher::Searcher() {
    trans_table = new TransTable;
}

Searcher::Searcher(TransTable* tt) {
    trans_table = tt;
}


value_t Searcher::quiesce(board b, value_t alpha, value_t beta) {
    colour side;
    b.getSide(&side);
    value_t stand_pat = b.getValue() * ((side == white) ? 1 : -1);
//    return stand_pat;
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;


    move_t captures[256];
    int num_captures = b.gen_captures(captures);
    board child;
    value_t score;

    for (int i=0; i<num_captures; i++) {
        child = doMove(b, captures[i]);
        score = - quiesce(child, -beta, -alpha);
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}

namespace {
    /**
     *  Given an array of moves and a preferred "best move" to search first,
     *  reorder the array so that the move is searched first.
     *
     *  \param moves        An array of moves to search.
     *  \param num_moves    The length of the array moves.
     *  \param best_move    The move we want to search first.
     */
    void reorder_moves(move_t * moves, int num_moves, move_t best_move) {
        for (int i=0; i<num_moves; i++) {
            if (moves[i] == best_move) {
                moves[i] = moves[0];
                moves[0] = best_move;
                break;
            }
        }
    }
}

value_t Searcher::principal_variation(board b, uint8_t depth,
                                 value_t alpha, value_t beta) {

    colour side;
    b.getSide(&side);
    value_t ret;
    uint64_t sig;
    uint32_t ind;
    move_t bestMove = 0;
    value_t ibv;
    uint8_t age;
    b.getHash(&sig);
    ind = (uint32_t)sig;
    b.getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
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


    if (depth == 0) {
        ret = quiesce(b, alpha, beta);
        return ret;
    }

    bool bSearchPv = true;
    board child;
    move_t moves[256];
    value_t score;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10);
        if (b.is_checkmate()) ret *= ((side == white) ? 1 : -1);
        else ret = 0;
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table->operator[](ind) = record;
        return ret;
    }

    if (bestMove) {
        reorder_moves(moves, num_moves, bestMove);
    }

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        if (bSearchPv) {
            score = - principal_variation(child, depth - 1, -beta, -alpha);
        }
        else {
            score = - principal_variation(child, depth - 1, -alpha - 1, -alpha);
            if (score > alpha) {
                score = - principal_variation(child, depth - 1, -beta, -alpha);
            }
        }

        if (score >= beta) {
            // lower bound
            ibv = 4 * beta + 1;
            bestMove = moves[i];
            record = {sig, bestMove, depth, ibv, age};
            trans_table->operator[](ind) = record;
            return beta;
        }
        if (score > alpha) {
            // upper bound
            alpha = score;
            bSearchPv = false;
            bestMove = moves[i];
        }
    }
    if (bSearchPv) {
        // exact
        ibv = 4 * alpha - 1;
    }
    else {
        ibv = 4 * alpha;
        // upper bound
    }
    record = {sig, bestMove, depth, ibv, age};
    trans_table->operator[](ind) = record;
    return alpha;
}


value_t Searcher::negamax_alphabeta(board b, uint8_t depth,
                                    value_t alpha, value_t beta) {

    colour side;
    b.getSide(&side);
    value_t ret;
    value_t alphaOrig = alpha;
    uint64_t sig;
    uint32_t ind;
    move_t bestMove = 0;
    value_t ibv;
    uint8_t age;
    b.getHash(&sig);
    ind = (uint32_t)sig;
    b.getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
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


    if (depth == 0) {
//        std::cout << b.FEN() << std::endl;
        ret = quiesce(b, alpha, beta);
        return ret;
    }

    board child;
    move_t moves[256];
    value_t score, value = std::numeric_limits<value_t>::min() + 10;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10);
        if (b.is_checkmate()) ret *= ((side == white) ? 1 : -1);
        else ret = 0;
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table->operator[](ind) = record;
        return ret;
    }

    if (bestMove) {
        reorder_moves(moves, num_moves, bestMove);
    }

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = - negamax_alphabeta(child, depth - 1, -beta, -alpha);
        if (value < score) {
            bestMove = moves[i];
            value = score;
        }
        if (alpha < value) {
            alpha = value;
        }
        if (alpha >= beta) break;

    }
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
    trans_table->operator[](ind) = record;
    return value;
}

value_t Searcher::negamax_alphabeta(board b, uint8_t depth,
                                    value_t alpha, value_t beta,
                                    double time_remaining) {
    clock_t start_time = clock();
    colour side;
    b.getSide(&side);
    value_t ret;
    value_t alphaOrig = alpha;
    uint64_t sig;
    uint32_t ind;
    move_t bestMove = 0;
    value_t ibv;
    uint8_t age;
    b.getHash(&sig);
    ind = (uint32_t)sig;
    b.getFullClock(&age);
    record_t record;

    // lookup
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
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


    if (double(clock() - start_time) / double(CLOCKS_PER_SEC) > time_remaining ||
        depth <= 0) {
//        std::cout << b.FEN() << std::endl;
        ret = quiesce(b, alpha, beta);
        return ret;
    }

    board child;
    move_t moves[256];
    value_t score, value = std::numeric_limits<value_t>::min() + 10;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        ret = (std::numeric_limits<value_t>::min() / 10);
        if (b.is_checkmate()) ret *= ((side == white) ? 1 : -1);
        else ret = 0;
        ibv = 4 * ret;
        record = {sig, bestMove, depth, ibv, age};
        trans_table->operator[](ind) = record;
        return ret;
    }

    if (bestMove) {
        reorder_moves(moves, num_moves, bestMove);
    }

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        time_remaining -= double(clock() - start_time) / double(CLOCKS_PER_SEC);
        if (time_remaining <= 0) break;
        score = - negamax_alphabeta(child, depth - 1, -beta, -alpha, time_remaining);
        if (value < score) {
            bestMove = moves[i];
            value = score;
        }
        if (alpha < value) {
            alpha = value;
        }
        if (alpha >= beta) break;

    }
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
    trans_table->operator[](ind) = record;
    return value;
}

move_t Searcher::iterative_deepening(board b, int timeout, bool cutoff) {
    clock_t start_time = clock();
    double time_taken = 0.0;
    uint8_t depth = 1;
    move_t best_move = 0;
    move_t new_move;
    std::vector<move_t> moves;
    int sz;
    colour side;
    b.getSide(&side);
    uint64_t hash;
    b.getHash(&hash);
    uint32_t ind = (uint32_t)hash;
    value_t alpha = std::numeric_limits<value_t>::min();
    value_t beta = std::numeric_limits<value_t>::max();
    prettyMove pm;

    while (time_taken < timeout) {
        negamax_alphabeta(b, depth, alpha, beta, timeout - time_taken);
        new_move = trans_table->operator[](ind).best_ref_move;
        depth++;
        if (new_move) {
            best_move = new_move;
            moves.push_back(best_move);
        }
        time_taken = double(clock() - start_time) / double(CLOCKS_PER_SEC);
        pm.data = best_move;
        std::cout << "Depth searched: " << depth - 1 << "   ("
                  << time_taken
                  << " seconds total)"
                  << "  (best move so far: "
                  << pm << ")" << std::endl;

        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 && time_taken > 20.0)) && moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}


} // end of chessCore namespace
