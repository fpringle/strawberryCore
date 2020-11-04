#include "board.h"
#include "action.h"
#include "move.h"
#include "search.h"
#include "play.h"

#include <limits>
#include <cstdint>



int32_t Minimax::algorithm(board b, int depth, colour side) {
    if (depth == 0) return b.getValue();

    int32_t score;
    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    board child;

    if (side == white) {
        int32_t max_score = std::numeric_limits<int32_t>::max();
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
        int32_t min_score = std::numeric_limits<int32_t>::max();
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
    int32_t score;
    board child;
    colour side;
    b.getSide(&side);
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
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    int32_t score;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    int32_t max_score = std::numeric_limits<int32_t>::min();
    if (num_moves == 0) return (max_score + 10) * ((side == white) ? 1 : -1);

    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -Negamax::algorithm(child, depth - 1, otherSide);
        if (score > max_score) max_score = score;
    }
    return max_score;
}

move_t Negamax::search(board b, int depth) {
    int32_t score;
    board child;
    colour side;
    b.getSide(&side);

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
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    int32_t score;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    int32_t max_score = std::numeric_limits<int32_t>::min();
    if (num_moves == 0) return (max_score + 10) * ((side == white) ? 1 : -1);
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

move_t NegamaxAB::search(board b, int depth) {
    int32_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    int32_t alpha = std::numeric_limits<int32_t>::min() + 1;
    int32_t beta = std::numeric_limits<int32_t>::max() - 1;

    int32_t max_score = std::numeric_limits<int32_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegamaxAB::algorithm(child, depth - 1, otherSide, - beta, - alpha);
        //        print_move( moves[i] );
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

int32_t NegaScoutOriginal::algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta) {
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    int32_t score, a = alpha, _b = beta;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        int32_t minusinf = (std::numeric_limits<int32_t>::min() + 10);
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
    int32_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    int32_t alpha = std::numeric_limits<int32_t>::min() + 1;
    int32_t beta = std::numeric_limits<int32_t>::max() - 1;

    int32_t max_score = std::numeric_limits<int32_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegaScoutOriginal::algorithm(child, depth - 1, otherSide, - beta, - alpha);
        //        print_move( moves[i] );
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

int32_t NegaScoutAlternative::algorithm(board b, int depth, colour side,
                             int32_t alpha, int32_t beta) {
    if (depth == 0) return b.getValue() * ((side == white) ? 1 : -1);

    colour otherSide = (side == white) ? black : white;

    int32_t _a, _b = beta;
    board child;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        int32_t minusinf = (std::numeric_limits<int32_t>::min() + 10);
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
    int32_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;

    int32_t alpha = std::numeric_limits<int32_t>::min() + 1;
    int32_t beta = std::numeric_limits<int32_t>::max() - 1;

    int32_t max_score = std::numeric_limits<int32_t>::min();
    for (int i = 0; i < num_moves; i++) {
        child = doMove(b, moves[i]);
        score = -NegaScoutAlternative::algorithm(child, depth - 1, otherSide, - beta, - alpha);
        //        print_move( moves[i] );
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

int32_t PVS::algorithm(board b, int depth, colour side,
                       int32_t alpha, int32_t beta) {
    if (depth == 0) return quiesce(b, side, alpha, beta);

    colour otherSide = (side == white) ? black : white;
    bool bSearchPv = true;
    board child;
    struct move_t moves[256];
    int32_t score;

    int num_moves = b.gen_legal_moves(moves);
    if (num_moves == 0) {
        int32_t minusinf = (std::numeric_limits<int32_t>::min() + 10);
        return minusinf * ((side == white) ? 1 : -1);
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
    int32_t score;
    board child;
    colour side;
    b.getSide(&side);

    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    struct move_t best_move;
    int32_t max_score = std::numeric_limits<int32_t>::min();

    int32_t alpha = std::numeric_limits<int32_t>::min() + 1;
    int32_t beta = std::numeric_limits<int32_t>::max() - 1;

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


int32_t quiesce(board b, colour side, int32_t alpha, int32_t beta) {
    int32_t stand_pat = b.getValue() * ((side == white) ? 1 : -1);

    return stand_pat;
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;


    colour otherSide = (side == white) ? black : white;
    move_t moves[256];
    int num_moves = b.gen_legal_moves(moves);
    board child;
    int32_t score;

    for (int i=0; i<num_moves; i++) {
        if (moves[i].is_capture()) {
            child = doMove(b, moves[i]);
            score = - quiesce(child, otherSide, -beta, -alpha);
            if (score >= beta) return beta;
            if (score > alpha) alpha = score;
        }
    }

    return alpha;
}




int32_t Player::search_algorithm(board b, colour side, int depth,
                                 int32_t alpha, int32_t beta) {
    return PVS::algorithm(b, depth, side, alpha, beta);
}

move_t Player::search(int depth) {
    int32_t score;
    board child;
    colour side;
    getSide(&side);
    colour otherSide = (side == white) ? black : white;

    struct move_t moves[256];
    int num_moves = gen_legal_moves(moves);
    struct move_t best_move;

    int32_t max_score = std::numeric_limits<int32_t>::min();
    int32_t alpha = std::numeric_limits<int32_t>::min() + 1;
    int32_t beta = std::numeric_limits<int32_t>::max() - 1;

    for (int i = 0; i < num_moves; i++) {
        makeChild(&child, moves[i]);
        score = - search_algorithm(child, otherSide, depth - 1, alpha, beta);
        if (score > max_score) {
            max_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

