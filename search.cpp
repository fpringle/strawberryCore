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

Searcher::~Searcher() {
    delete trans_table;
}

void Searcher::set_timeout(int time) {
    search_start_time = clock();
    search_end_time = search_start_time + time * CLOCKS_PER_SEC;
}

void Searcher::kill_search() {
    search_end_time = clock();
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

class CompMoves {
 private:
    board* b;
    colour side;
    TransTable* trans_table;

    value_t get_value(move_t move) {
        uint64_t child_hash = b->childHash(move);
        uint32_t child_index = static_cast<uint32_t>(child_hash);
        record_t rec;
        if (table_lookup(child_hash, child_index, trans_table, &rec)) {
            return rec.score;
        } else {
            board* child = doMove(b, move);
            return child->getValue() * (side == white ? 1 : -1);
        }
    }

 public:
    CompMoves(board* curboard, TransTable* tt) {
        b = curboard;
        b->getSide(&side);
        trans_table = tt;
    }

    bool operator()(move_t move1, move_t move2) {
//        if (is_capture(move1) && !is_capture(move2)) {
//            return true;
//        } else if (!is_capture(move1) && is_capture(move2)) {
//            return false;
//        } else {
            return get_value(move1) > get_value(move2);
//        }
    }
};

/**
 *  Given an array of moves and a preferred "best move" to search first,
 *  reorder the array so that the move is searched first.
 *
 *  \param moves        A pointer to an array of moves to search.
 *  \param b            A pointer to the board we're searching from.
 *  \param tt           A pointer to the transposition table to use.
 *  \param first_move   If given, the move we want to search first.
 *  \param second_move  If given, the move we want to search second.
 */
void reorder_moves(MoveList* moves, board* b, TransTable*tt,
                   move_t first_move = 0, move_t second_move = 0) {
    MoveList::iterator it;
    int start = 0;
    if (first_move) {
        it = std::find(moves->begin(), moves->end(), first_move);
        if (it != moves->end()) {
            std::iter_swap(it, moves->begin());
            start++;
        }
    }
    if ((second_move) && (second_move != first_move)) {
        it = std::find(moves->begin(), moves->end(), second_move);
        if (it != moves->end()) {
            std::iter_swap(it, moves->begin()+start);
            start++;
        }
    }
    std::sort(moves->begin()+start, moves->end(), CompMoves(b, tt));
}

double time_diff(clock_t start_time) {
    return static_cast<double>(clock() - start_time) /
                static_cast<double>(CLOCKS_PER_SEC);
}

double time_diff(clock_t start_time, clock_t end_time) {
    return static_cast<double>(end_time - start_time) /
                static_cast<double>(CLOCKS_PER_SEC);
}

}   // namespace


value_t Searcher::principal_variation(board* b, uint8_t depth,
                                    value_t alpha, value_t beta,
                                    move_t first_move) {
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

    if (clock() > search_end_time || depth <= 0) {
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

    reorder_moves(&moves, b, trans_table, first_move, bestMove);

    for (move_t move : moves) {
        child = doMove(b, move);
        if (clock() > search_end_time) break;

        if (bSearchPv) {
            score = - principal_variation(child, depth-1, -beta, -alpha);
        } else {
            score = - principal_variation(child, depth - 1, -alpha - 1, -alpha);
            if (score > alpha) {
                score = - principal_variation(child, depth - 1, -beta, -alpha);
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
                                    move_t first_move) {
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

    if (clock() > search_end_time || depth <= 0) {
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

    reorder_moves(&moves, b, trans_table, first_move, bestMove);

    value_t score, value = -VAL_INFINITY;

    for (move_t move : moves) {
        child = doMove(b, move);
        if (clock() > search_end_time) break;
        score = - negamax_alphabeta(child, depth - 1, -beta, -alpha);
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
    set_timeout(timeout);
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

    while (clock() < search_end_time && depth < 100) {
        negamax_alphabeta(b, depth, alpha, beta, best_move);
        new_move = trans_table->operator[](ind).best_move;
        depth++;

        if (new_move) {
            best_move = new_move;
            moves.push_back(best_move);
#if DEBUG
            std::cerr << "Depth searched: " << depth - 1 << "   ("
                      << time_diff(search_start_time)
                      << " seconds total)"
                      << "  (best move so far: "
                      << b->SAN_pre_move(best_move) << ")" << std::endl;
#endif
        }
        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 &&
            time_diff(search_start_time) > 20.0))
            && moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}

move_t Searcher::iterative_deepening_pv(board* b,
                                        int timeout,
                                        bool cutoff) {
    set_timeout(timeout);
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

    while (clock() < search_end_time && depth < 100) {
        principal_variation(b, depth, alpha, beta, best_move);
        new_move = trans_table->operator[](ind).best_move;
        depth++;

        if (new_move) {
            best_move = new_move;
            moves.push_back(best_move);
#if DEBUG
            std::cerr << "Depth searched: " << depth - 1 << "   ("
                      << time_diff(search_start_time)
                      << " seconds total)"
                      << "  (best move so far: "
                      << b->SAN_pre_move(best_move) << ")" << std::endl;
#endif
        }
        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 &&
            time_diff(search_start_time) > 20.0)) &&
            moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}

move_t Searcher::search(board* b, int timeout, bool cutoff) {
    return iterative_deepening_negamax(b, timeout, cutoff);
}

}   // namespace chessCore
