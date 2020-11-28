#include "search.h"

#include <time.h>

#include <cstdint>
#include <vector>

#include "action.h"
#include "board.h"
#include "move.h"
#include "play.h"
#include "typedefs.h"

#define DEBUG 1

namespace chessCore {

std::ostream& operator<<(std::ostream &out, const record_t &rec) {
    out << "Best/refutation move:     " << mtos(rec.best_ref_move) << std::endl
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


value_t Searcher::quiesce(board* b, value_t alpha, value_t beta) {
    colour side;
    b->getSide(&side);
    value_t stand_pat = b->getValue() * ((side == white) ? 1 : -1);
//    return stand_pat;
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
        for (MoveList::iterator it = moves->begin(); it != moves->end(); it++) {
            if (*it == best_move) {
                moves->erase(it);
                moves->insert(moves->begin(), best_move);
                break;
            }
        }
    }
/*
    void reorder_moves(board* b, move_t * moves, int num_moves) {
        move_t 
        for (int i=0; i<num_moves; i++) {
            if (moves[i] == best_move) {
                moves[i] = moves[0];
                moves[0] = best_move;
                break;
            }
        }
    }
*/
}

value_t Searcher::principal_variation(board* b, uint8_t depth,
                                 value_t alpha, value_t beta) {

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
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
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
    }

    if (depth == 0) {
        ret = quiesce(b, alpha, beta);
        return ret;
    }

    bool bSearchPv = true;
    board* child;
    value_t score, value = -VAL_INFINITY;

    MoveList moves = b->gen_legal_moves();
    if (moves.empty()) {
        if (b->is_checkmate()) {
            ret = value * ((side == white) ? 1 : -1);
        }
        else {
            ret = 0;
        }
        record = {sig, bestMove, depth, ret, age, EXACT};
        trans_table->operator[](ind) = record;
        return ret;
    }

    if (bestMove) {
        reorder_moves(&moves, bestMove);
    }

    for (move_t move : moves) {
        child = doMove(b, move);
        if (bSearchPv) {
            score = - principal_variation(child, depth - 1, -beta, -alpha);
        }
        else {
            score = - principal_variation(child, depth - 1, -alpha - 1, -alpha);
            if (score > alpha) {
                score = - principal_variation(child, depth - 1, -beta, -alpha);
            }
        }

        if (value < score) {
            bestMove = move;
            value = score;
        }
        if (alpha < value) {
            alpha = value;
            bSearchPv = false;
            bestMove = move;
        }
        if (alpha >= beta) {
            // lower bound
            bestMove = move;
            record = {sig, bestMove, depth, beta, age, LOWER};
            trans_table->operator[](ind) = record;
            return beta;
        }
/*
        if (score >= beta) {
            // lower bound
            ibv = 4 * beta + 1;
            bestMove = move;
            record = {sig, bestMove, depth, ibv, age};
            trans_table->operator[](ind) = record;
            return beta;
        }
        if (score > alpha) {
            // upper bound
            alpha = score;
            bSearchPv = false;
            bestMove = move;
        }
*/
    }
    if (bSearchPv) {
        // exact
        record = {sig, bestMove, depth, value, age, EXACT};
    }
    else {
        // upper bound
        record = {sig, bestMove, depth, value, age, UPPER};
    }
    trans_table->operator[](ind) = record;
    return alpha;
}


value_t Searcher::principal_variation(board* b, uint8_t depth,
                                      value_t alpha, value_t beta,
                                      double time_remaining) {

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
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
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
    }

    if (double(clock() - start_time) / double(CLOCKS_PER_SEC) > time_remaining ||
                depth <= 0) {
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
        }
        else {
            ret = 0;
        }
        record = {sig, bestMove, depth, ret, age, EXACT};
        trans_table->operator[](ind) = record;
        return ret;
    }

    if (bestMove) {
        reorder_moves(&moves, bestMove);
    }

    for (move_t move : moves) {
        child = doMove(b, move);
        time_remaining -= double(clock() - start_time) / double(CLOCKS_PER_SEC);
        if (time_remaining <= 0) break;

        if (bSearchPv) {
            score = - principal_variation(child, depth - 1, -beta, -alpha, time_remaining);
        }
        else {
            score = - principal_variation(child, depth - 1, -alpha - 1, -alpha, time_remaining);
            if (score > alpha) {
                score = - principal_variation(child, depth - 1, -beta, -alpha, time_remaining);
            }
        }

        if (score >= beta) {
            // lower bound
            bestMove = move;
            record = {sig, bestMove, depth, beta, age, LOWER};
            trans_table->operator[](ind) = record;
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
        record = {sig, bestMove, depth, alpha, age, EXACT};
    }
    else {
        // upper bound
        record = {sig, bestMove, depth, alpha, age, UPPER};
    }
    trans_table->operator[](ind) = record;
    return alpha;
}


value_t Searcher::negamax_alphabeta(board* b, uint8_t depth,
                                    value_t alpha, value_t beta) {

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
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
            bestMove = record.best_ref_move;
            if (record.depth >= depth) {
                switch (record.flag) {
                    case UPPER:
                        // upper bound
                        beta = std::min(beta, record.score);
                        break;
                    case LOWER:
                        // lower bound
                        alpha = std::min(alpha, record.score);
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
    }

    if (depth == 0) {
        ret = quiesce(b, alpha, beta);
        return ret;
    }

    board* child;
    value_t score, value = -VAL_INFINITY;

    MoveList moves = b->gen_legal_moves();
    if (moves.empty()) {
        if (b->is_checkmate()) {
            ret = value * ((side == white) ? 1 : -1);
        }
        else {
            ret = 0;
        }
        record = {sig, bestMove, depth, ret, age, EXACT};
        trans_table->operator[](ind) = record;
        return ret;
    }

    if (bestMove) {
        reorder_moves(&moves, bestMove);
    }

    for (move_t move : moves) {
        child = doMove(b, move);
        score = - negamax_alphabeta(child, depth - 1, -beta, -alpha);
        if (value < score) {
            bestMove = move;
            value = score;
        }
        if (alpha < value) {
            alpha = value;
        }
        if (alpha >= beta) break;

    }
    if (value <= alphaOrig) {
        record = {sig, bestMove, depth, value, age, UPPER};
    }
    else if (value >= beta) {
        record = {sig, bestMove, depth, value, age, LOWER};
    }
    else {
        record = {sig, bestMove, depth, value, age, EXACT};
    }
    trans_table->operator[](ind) = record;
    return value;
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
    std::map<uint32_t, record_t>::iterator it = trans_table->find(ind);
    if (it != trans_table->end()) {
        record = it->second;
        if (record.signature == sig) {
            // match
//            if (record.depth > 1) {
                bestMove = record.best_ref_move;
//            }
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
                }
                if (alpha >= beta) {
                    return record.score;
                }
            }
        }
    }

    if (double(clock() - start_time) / double(CLOCKS_PER_SEC) > time_remaining ||
        depth <= 0) {
        ret = quiesce(b, alpha, beta);      // check this
        return ret;
    }

    board* child;

    MoveList moves = b->gen_legal_moves();
    if (moves.empty()) {
        if (b->is_checkmate()) {
            ret = VAL_INFINITY * ((side == white) ? -1 : 1);
        }
        else {
            ret = 0;
        }
        record = {sig, bestMove, depth, ret, age, EXACT};
        trans_table->operator[](ind) = record;
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
        child = doMove(b, move);
        time_remaining -= double(clock() - start_time) / double(CLOCKS_PER_SEC);
        if (time_remaining <= 0) break;
        score = - negamax_alphabeta(child, depth - 1, -beta, -alpha, time_remaining);
        if (value < score) {
            bestMove = move;
            value = score;
        }
        if (alpha < value) {
            alpha = value;
        }
        if (alpha >= beta) break;

    }
    if (value <= alphaOrig) {
        record = {sig, bestMove, depth, value, age, UPPER};
    }
    else if (value >= beta) {
        record = {sig, bestMove, depth, value, age, LOWER};
    }
    else {
        record = {sig, bestMove, depth, value, age, EXACT};
    }
    trans_table->operator[](ind) = record;
    return value;
}

namespace {
    move_t getBest(board* b, TransTable* tt) {
        colour side;
        b->getSide(&side);
        board* child;
        MoveList moves = b->gen_legal_moves();
        value_t best_value = VAL_INFINITY * (side==white ? -1 : 1);
        value_t value;
        move_t best_move = 0;
        uint64_t hash;
        uint32_t ind;
        TransTable::iterator it;
        for (move_t move : moves) {
            child = doMove(b, move);
            child->getHash(&hash);
            ind = (uint32_t)hash;
            it = tt->find(ind);
            if (it == tt->end()) continue;
            value = it->second.score;
            if (side == white &&  value >= best_value) {
                best_value = value;
                best_move = move;
            }
            else if (side == black && value <= best_value) {
                best_value = value;
                best_move = move;
            }
        }
        return best_move;
    }
}


move_t Searcher::iterative_deepening_negamax(board* b, int timeout, bool cutoff) {
    clock_t start_time = clock();
    double time_taken = 0.0;
    uint8_t depth = 1;
    move_t best_move = 0;
    move_t new_move;
    MoveList moves;
    int sz;
    colour side;
    b->getSide(&side);
//    uint64_t hash;
//    b->getHash(&hash);
//    uint32_t ind = (uint32_t)hash;
    value_t alpha = -VAL_INFINITY;
    value_t beta = VAL_INFINITY;

    while (time_taken < timeout) {
        negamax_alphabeta(b, depth, alpha, beta, timeout - time_taken, best_move);
        new_move = getBest(b, trans_table);
//        new_move = trans_table->operator[](ind).best_ref_move;
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
                  << b->SAN_pre_move(best_move) << ")" << std::endl;
#endif
        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 && time_taken > 20.0)) && moves[sz - 1] == moves[sz - 2]) {
            break;
        }
    }

    return best_move;
}

move_t Searcher::iterative_deepening_pv(board* b, int timeout, bool cutoff) {
    clock_t start_time = clock();
    double time_taken = 0.0;
    uint8_t depth = 1;
    move_t best_move = 0;
    move_t new_move;
    std::vector<move_t> moves;
    int sz;
    colour side;
    b->getSide(&side);
    uint64_t hash;
    b->getHash(&hash);
    uint32_t ind = (uint32_t)hash;
    value_t alpha = -VAL_INFINITY;
    value_t beta = VAL_INFINITY;

    while (time_taken < timeout) {
        principal_variation(b, depth, alpha, beta, timeout - time_taken);
        new_move = trans_table->operator[](ind).best_ref_move;
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
                  << b->SAN_pre_move(moves.empty() ? 0 : moves.back()) << ")" << std::endl;
#endif
        sz = moves.size();
        if (cutoff && ((sz >= 8) || (sz >= 5 && time_taken > 20.0)) && moves[sz - 1] == moves[sz - 2]) {
            break;
        }

    }

    return moves.back();
}

move_t Searcher::search(board* b, int timeout, bool cutoff) {
    return iterative_deepening_negamax(b, timeout, cutoff);
}

} // end of chessCore namespace
