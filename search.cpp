#include "board.h"
#include "action.h"
#include "move.h"
#include "hash.h"
#include "search.h"
#include "init.h"
#include <limits>
#include <cstdint>

int32_t Minimax::algorithm(board b, int depth, colour side) {
    if (depth == 0) return b.getValue();

    int32_t score;
    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) return b.getValue();
    board child;

    if (side == white) {
        int32_t max_score = std::numeric_limits<int32_t>::min();

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = Minimax::algorithm(child, depth - 1, black);
            if (score > max_score) max_score = score;
        }
        return max_score;
    }

    else {
        int32_t min_score = std::numeric_limits<int32_t>::max();

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = algorithm(child, depth - 1, white);
            if (score < min_score) min_score = score;
        }
        return min_score;
    }
}

move_t Minimax::search(board b, int depth, colour side) {
    int32_t score;
    board child;
    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0 || depth == 0) return move_t(0, 0, 0, 0, 0, 0);
    struct move_t best_move;

    if (side == white) {
        int32_t max_score = std::numeric_limits<int32_t>::min();

        for (int i = 0; i < num_moves; i++) {
            child = doMove(b, moves[i]);
            score = Minimax::algorithm(child, depth - 1, black);
            if (score > max_score) {
                max_score = score;
                best_move = moves[i];
            }
        }
        std::cout << "Top score: " << max_score << "\n";
        return best_move;
    }

    else {
        int32_t min_score = std::numeric_limits<int32_t>::max();

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

int32_t Negamax::algorithm(board b, int depth, colour side) {
    if (depth == 0) return b.evaluate() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    int32_t score;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);

    int32_t max_score = std::numeric_limits<int32_t>::min();

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -Negamax::algorithm(child, depth - 1, otherSide);
        if (score > max_score) max_score = score;
    }
    return max_score;
}

move_t Negamax::search(board b, int depth, colour side) {
    int32_t score;
    board child;

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    int32_t max_score = std::numeric_limits<int32_t>::min();
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

int32_t NegamaxAB::algorithm(board b, int depth, colour side,
        int32_t alpha, int32_t beta) {
    if (depth == 0) return b.evaluate() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    int32_t score;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);

    int32_t max_score = std::numeric_limits<int32_t>::min();
    //  int32_t max_score = alpha;

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegamaxAB::algorithm(child, depth - 1, otherSide, -beta, -alpha);
        if (score > max_score) max_score = score;
        if (max_score > alpha) alpha = max_score;
        if (alpha >= beta) break;
    }
    return max_score;
}

move_t NegamaxAB::search(board b, int depth, colour side) {
    int32_t score;
    board child;

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    int32_t alpha = std::numeric_limits<int32_t>::min();
    int32_t beta = std::numeric_limits<int32_t>::max();

    int32_t max_score = std::numeric_limits<int32_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegamaxAB::algorithm(child, depth - 1, otherSide, alpha, beta);
        //        print_move( moves[i] );
        //        std::cout << ": " << score << std::endl;
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}
