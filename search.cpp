/* Copyright 2020 Freddy Pringle */
#include "search.h"

#include <time.h>

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

#include "action.h"
#include "board.h"
#include "move.h"
#include "play.h"
#include "typedefs.h"

#define DEBUG 1

namespace chessCore {

std::ostream& operator<<(std::ostream &out, const record_t &rec) {
    out << "Best/refutation move:     " << mtos(rec.best_move) << std::endl
        << "Depth searched:           " << +rec.depth << std::endl
        << "Clock when last searched: " << +rec.age << std::endl
        << "Bound flag:               " << (rec.flag == UPPER ? "Upper\n" :
                                            rec.flag == LOWER ? "Lower\n" :
                                                                "Exact\n")

        << "Value:                    " << rec.score << std::endl;
    return out;
}


Searcher::Searcher() {
    trans_table = new TransTable;
}

Searcher::Searcher(TransTable* tt) {
    trans_table = tt;
}

void Searcher::prune_table(uint8_t age) {
    TransTable::const_iterator it;
    for (it=trans_table->cbegin(); it != trans_table->cend();) {
        if (it->second.age < age) {
            trans_table->erase(it++);
        } else {
            ++it;
        }
    }
}

value_t Searcher::quiesce(board* b, value_t alpha, value_t beta) {
    colour side;
    b->getSide(&side);
    value_t stand_pat = b->getValue() * ((side == white) ? 1 : -1);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;


    MoveList captures = b->gen_captures();
    board* child;
    value_t score;

    for (move_t capture : captures) {
        child = doMove(b, capture);
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
    void reorder_moves(MoveList* moves, move_t best_move) {
        MoveList::iterator it;
        it = std::find(moves->begin(), moves->end(), best_move);
        if (it == moves->end()) return;
        std::iter_swap(it, moves->begin());
    }

    bool table_lookup(uint64_t sig,
                      uint32_t ind,
                      TransTable* tt,
                      record_t* rec) {
        TransTable::iterator it = tt->find(ind);
        if (it == tt->end()) return false;
        if (it->second.signature != sig) return false;
        *rec = it->second;
        return true;
    }

    void table_save(uint64_t sig,
                    uint32_t ind,
                    move_t best_move,
                    uint8_t depth,
                    value_t score,
                    uint8_t age,
                    valueType flag,
                    TransTable* tt) {
        record_t rec({sig, best_move, depth, score, age, flag});
        tt->operator[](ind) = rec;
    }

    double time_diff(clock_t start_time) {
        return static_cast<double>(clock() - start_time) /
                    static_cast<double>(CLOCKS_PER_SEC);
    }

}   // namespace


value_t Searcher::principal_variation(board* b, uint8_t depth,
                                    value_t alpha, value_t beta,
                                    double time_remaining,
                                    move_t first_move) {
    clock_t start_time = clock();
    colour side;
    b->getSide(&side);
    value_t ret;
    uint64_t sig;
    uint32_t ind;
    move_t bestMove = 0;
    uint8_t age;
    b->getHash(&sig);
    ind = (uint32_t)sig;
    b->getFullClock(&age);
    record_t record;

    // lookup
    if (table_lookup(sig, ind, trans_table, &record)) {
        bestMove = record.best_move;
        if (record.depth >= depth) {
            switch (record.flag) {
                case UPPER:
                    // upper bound
                    beta = std::min(beta, record.score);
                    break;
                case LOWER:
                    // lower bound
                    alpha = std::max(alpha, record.score);
                    break;
                case EXACT:
                    // exact
                    return record.score;
            }
            if (alpha >= beta) {
                return record.score;
            }
        }
    }

    if (time_diff(start_time) > time_remaining || depth <= 0) {
        ret = quiesce(b, alpha, beta);
        return ret;
    }

    bool bSearchPv = true;
    board* child;
    value_t score = -VAL_INFINITY;

    MoveList moves = b->gen_legal_moves();
    if (moves.empty()) {
        if (b->is_checkmate()) {
            ret = score * ((side == white) ? 1 : -1);
        } else {
            ret = 0;
        }
        table_save(sig, ind, bestMove, depth, ret, age, EXACT, trans_table);
        return ret;
    }

    if (bestMove) {
        reorder_moves(&moves, bestMove);
    }

    if (first_move) {
        reorder_moves(&moves, first_move);
    }

    for (move_t move : moves) {
        child = doMove(b, move);
        time_remaining -= time_diff(start_time);
        if (time_remaining <= 0) break;

        if (bSearchPv) {
            score = - principal_variation(child, depth-1, -beta,
                                          -alpha, time_remaining);
        } else {
            score = - principal_variation(child, depth - 1, -alpha - 1,
                                          -alpha, time_remaining);
            if (score > alpha) {
                score = - principal_variation(child, depth - 1, -beta,
                                              -alpha, time_remaining);
            }
        }

        if (score >= beta) {
            // lower bound
            table_save(sig, ind, move, depth, beta, age, LOWER, trans_table);
            return beta;
        }
        if (score > alpha) {
            // upper bound
            alpha = score;
            bSearchPv = false;
            bestMove = move;
        }
    }
    if (bSearchPv) {
        // exact
        table_save(sig, ind, bestMove, depth, alpha, age, EXACT, trans_table);
    } else {
        // upper bound
        table_save(sig, ind, bestMove, depth, alpha, age, UPPER, trans_table);
    }
    return alpha;
}

value_t Searcher::negamax_alphabeta(board* b, uint8_t depth,
                                    value_t alpha, value_t beta,
                                    double time_remaining,
                                    move_t first_move) {
    clock_t start_time = clock();
    colour side;
    b->getSide(&side);
    value_t ret;
    value_t alphaOrig = alpha;
    uint64_t sig;
    uint32_t ind;
    move_t bestMove = 0;
    uint8_t age;
    b->getHash(&sig);
    ind = (uint32_t)sig;
    b->getFullClock(&age);
    record_t record;

    // lookup
    if (table_lookup(sig, ind, trans_table, &record)) {
        bestMove = record.best_move;
        if (record.depth >= depth) {
            switch (record.flag) {
                case EXACT:
                    return record.score;
                    break;
                case LOWER:
                    alpha = std::max(alpha, record.score);
                    break;
                case UPPER:
                    beta = std::min(beta, record.score);
                    break;
                default:
                    break;
            }
            if (alpha >= beta) {
                table_save(sig, ind, bestMove, depth, record.score,
                           age, LOWER, trans_table);
                return record.score;
            }
        }
    }

    if (time_diff(start_time) > time_remaining
            || depth <= 0) {
        ret = quiesce(b, alpha, beta);      // check this
        table_save(sig, ind, bestMove, depth, record.score,
                   age, LOWER, trans_table);
        return ret;
    }

    board* child;

    MoveList moves = b->gen_legal_moves();
    if (moves.empty()) {
        ret = (b->is_checkmate()) ? -VAL_INFINITY : 0;
        table_save(sig, ind, bestMove, depth, ret, age, EXACT, trans_table);
        return ret;
    }

    if (bestMove) {
        reorder_moves(&moves, bestMove);
    }

    if (first_move) {
        reorder_moves(&moves, first_move);
    }

    value_t score, value = -VAL_INFINITY;

    for (move_t move : moves) {
        time_remaining -= time_diff(start_time);
        if (time_remaining <= 0) break;
        child = doMove(b, move);
        score = - negamax_alphabeta(child, depth - 1, -beta,
                                    -alpha, time_remaining);
        if (score > value) {
            bestMove = move;
            value = score;
        }
        if (alpha < value) {
            alpha = value;
        }
        if (alpha >= beta) break;
    }
    if (value <= alphaOrig) {
        table_save(sig, ind, bestMove, depth, value, age, UPPER, trans_table);
    } else if (value >= beta) {
        table_save(sig, ind, bestMove, depth, value, age, LOWER, trans_table);
    } else {
        table_save(sig, ind, bestMove, depth, value, age, EXACT, trans_table);
    }
    return value;
}


move_t Searcher::iterative_deepening_negamax(board* b,
                                             int timeout,
                                             bool cutoff) {
    clock_t start_time = clock();
    uint8_t age;
    b->getFullClock(&age);
#if DEBUG
    std::cerr << "Size of transposition table: "
              << trans_table->size() << std::endl;
#endif
    if (age > 3) {
        prune_table(age - 3);
#if DEBUG
    std::cerr << "Size of transposition table after pruning: "
              << trans_table->size() << std::endl;
#endif
    }
    uint8_t depth = 1;
    move_t best_move = 0;
    move_t new_move;
    MoveList moves;
    int sz;
    colour side;
    uint64_t hsh;
    uint32_t ind;
    b->getSide(&side);
    b->getHash(&hsh);
    ind = (uint32_t)hsh;
    value_t alpha = -VAL_INFINITY;
    value_t beta = VAL_INFINITY;
    double time_taken = time_diff(start_time);

    while (time_taken < timeout && depth < 100) {
        negamax_alphabeta(b, depth, alpha, beta,
                          timeout - time_taken, best_move);
        new_move = trans_table->operator[](ind).best_move;
        depth++;
        time_taken = time_diff(start_time);

        if (new_move) {
            best_move = new_move;
            moves.push_back(best_move);
#if DEBUG
            std::cerr << "Depth searched: " << depth - 1 << "   ("
                      << time_taken
                      << " seconds total)"
                      << "  (best move so far: "
                      << b->SAN_pre_move(best_move) << ")" << std::endl;
#endif
        }
        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 && time_taken > 20.0))
            && moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}

move_t Searcher::iterative_deepening_pv(board* b,
                                        int timeout,
                                        bool cutoff) {
    clock_t start_time = clock();
    uint8_t age;
    b->getFullClock(&age);
#if DEBUG
    std::cerr << "Size of transposition table: "
              << trans_table->size() << std::endl;
#endif
    if (age > 3) {
        prune_table(age - 3);
#if DEBUG
    std::cerr << "Size of transposition table after pruning: "
              << trans_table->size() << std::endl;
#endif
    }
    double time_taken = 0.0;
    uint8_t depth = 1;
    move_t best_move = 0;
    move_t new_move;
    MoveList moves;
    int sz;
    colour side;
    uint64_t hsh;
    uint32_t ind;
    b->getSide(&side);
    b->getHash(&hsh);
    ind = (uint32_t)hsh;
    value_t alpha = -VAL_INFINITY;
    value_t beta = VAL_INFINITY;

    while (time_taken < timeout && depth < 100) {
        negamax_alphabeta(b, depth, alpha, beta,
                          timeout - time_taken, best_move);
        new_move = trans_table->operator[](ind).best_move;
        depth++;
        time_taken = time_diff(start_time);

        if (new_move) {
            best_move = new_move;
            moves.push_back(best_move);
#if DEBUG
            std::cerr << "Depth searched: " << depth - 1 << "   ("
                      << time_taken
                      << " seconds total)"
                      << "  (best move so far: "
                      << b->SAN_pre_move(best_move) << ")" << std::endl;
#endif
        }
        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 && time_taken > 20.0))
            && moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}

move_t Searcher::search(board* b, int timeout, bool cutoff) {
    return iterative_deepening_negamax(b, timeout, cutoff);
}

}   // namespace chessCore
