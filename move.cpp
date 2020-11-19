#include "move.h"

#include <stdlib.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include "action.h"
#include "board.h"
#include "twiddle.h"
#include "typedefs.h"


namespace chessCore {

// initialise constants declared in move.h
const int N = +8;
const int NE = +9;
const int E = +1;
const int SE = -7;
const int S = -8;
const int SW = -9;
const int W = -1;
const int NW = +7;
const int NNE = 17;
const int ENE = 10;
const int ESE = -6;
const int SSE = -15;
const int SSW = -17;
const int WSW = -10;
const int WNW = 6;
const int NNW = 15;

// utility

void itos(int i, char * dest) {
    *dest = 'a' + (i % 8);
    *(dest + 1) = '1' + (i / 8);
}

std::string itos(int i) {
    std::string ret = "  ";
    ret[0] = 'a' + (i % 8);
    ret[1] = '1' + (i / 8);
    return ret;
}

void itos(int i, std::ostream& cout) {
    cout << char('a' + (i % 8)) << char('1' + (i / 8));
}

int _stoi(std::string s) {
    char file = s[0];
    char rank = s[1];

    return int(rank - '1')*8 + int(file - 'a');
}

std::string board::SAN_pre_move( move_t move ) const {
    std::stringstream san;
    int i;
    colour otherSide = flipColour(sideToMove);

    if      ( move.is_queenCastle() ) san << "O-O-O";
    else if  ( move.is_kingCastle() ) san << "O-O";

    else {
        int from_sq = move.from_sq();
        int to_sq   = move.to_sq();
        piece movingPiece = piece((6 * sideToMove) + 6);

        for ( i = 6 * sideToMove; i < (6 * sideToMove) + 6; i++ ) {
            if ( is_bit_set(pieceBoards[i], from_sq) ) {
                movingPiece = piece(i % 6);
                break;
            }
        }

        if (movingPiece == piece((6 * sideToMove) + 6)) {
            return "";
        }


        char piece_sym = symbols[int(movingPiece)+6];
        std::string from_sq_str =  itos( from_sq );
        std::string to_sq_str   =  itos( to_sq );


        if ( movingPiece != 0 ) {
            san << piece_sym;

    //         ambiguity tests
            bitboard alternates = pieceTargets( to_sq, whiteSquares(), blackSquares(), colourPiece((movingPiece+(6*sideToMove)+6)%12) );
            alternates &= pieceBoards[movingPiece + (6 * sideToMove)];
            alternates &= (~ (1ULL << from_sq));

            int from_file = from_sq % 8;
            int from_rank = from_sq / 8;
            bool unique_file = true;
            bool unique_rank = true;
            int tmp;


            if ( alternates ) {
                ITER_BITBOARD(tmp, alternates) {
                    if (int(tmp / 8) == from_rank) unique_rank = false;
                    if (int(tmp % 8) == from_file) unique_file = false;
                }

                if      ( unique_file ) san << from_sq_str[0];
                else if ( unique_rank ) san << from_sq_str[1];
                else                    san << from_sq_str;
            }

        }

        if (move.is_capture()) {
            if ( movingPiece == 0 ) {
                san << char('a' + (from_sq % 8));
            }

            san << "x";
        }

        san << to_sq_str;

        if ( move.is_promotion() ) {
            san << "=";
            san << symbols[int(move.which_promotion()) + 6];
        }
    }


    // need to fix bug in is_checking_move
    board child = *this;
    child.doMoveInPlace(move);

    if ( child.is_check( otherSide ) ) {
        if ( child.is_checkmate( otherSide ) ) san << "#";
        else                             san << "+";
    }

    return san.str();
}


std::string board::SAN_post_move( move_t move ) const {
    std::stringstream san;
    int i;
    colour side_to_move = flipColour(sideToMove);
    colour otherSide = sideToMove;

    if      ( move.is_queenCastle() ) san << "O-O-O";
    else if  ( move.is_kingCastle() ) san << "O-O";

    else {
        int from_sq = move.from_sq();
        int to_sq   = move.to_sq();
        piece movingPiece = piece((6 * side_to_move) + 6);

        if (move.is_promotion()) {
            movingPiece = pawn;
        }
        else {

            for ( i = 6 * side_to_move; i < (6 * side_to_move) + 6; i++ ) {
                if ( is_bit_set(pieceBoards[i], to_sq) ) {
                    movingPiece = piece(i % 6);
                    break;
                }
            }

            if (movingPiece == piece((6 * side_to_move) + 6)) {
                return "";
            }
        }

        char piece_sym = symbols[int(movingPiece)+6];
        std::string from_sq_str =  itos( from_sq );
        std::string to_sq_str   =  itos( to_sq );



        if ( movingPiece != 0 ) {
            san << piece_sym;

            // ambiguity tests
            bitboard _white = whiteSquares();
            bitboard _black = blackSquares();
            if (side_to_move == white) {
                _white |= (1ULL << from_sq);
            }
            else {
                _black |= (1ULL << from_sq);
            }

            bitboard alternates = pieceTargets( to_sq, _white, _black, colourPiece((movingPiece+(6*otherSide))%12) );
            alternates &= pieceBoards[movingPiece + (6 * side_to_move)];
            alternates &= (~ (1ULL << from_sq));

            int from_file = from_sq % 8;
            int from_rank = from_sq / 8;
            bool unique_file = true;
            bool unique_rank = true;
            int tmp;

            if ( alternates ) {
                ITER_BITBOARD(tmp, alternates) {
                    if (int(tmp / 8) == from_rank) unique_rank = false;
                    if (int(tmp % 8) == from_file) unique_file = false;
                }

                if      ( unique_file ) san << from_sq_str[0];
                else if ( unique_rank ) san << from_sq_str[1];
                else                    san << from_sq_str;
            }

        }

        if (move.is_capture()) {
            if ( movingPiece == 0) {
                san << char('a' + (from_sq % 8));
            }

            san << "x";
        }

        san << to_sq_str;

        if ( move.is_promotion() ) {
            san << "=";
            san << symbols[int(move.which_promotion()) + 6];
        }
    }

    if ( is_check(otherSide) ) {
        if ( is_checkmate( otherSide ) ) san << "#";
        else                             san << "+";
    }

    return san.str();
}



//
//move_t board::move_from_SAN( std::string san ) {
//    // castling
//    if ( san == "O-O" ) {
//        if ( sideToMove == white ) {
//            return move_t (  4,  6, 0, 0, 1, 0 );
//        }
//        else {
//            return move_t ( 60, 62, 0, 0, 1, 0 );
//        }
//    }
//    else if ( san == "O-O-O" ) {
//        if ( sideToMove == white ) {
//            return move_t (  4,  1, 0, 0, 1, 1 );
//        }
//        else {
//            return move_t ( 60, 58, 0, 0, 1, 1 );
//        }
//    }
//
//    int len = san.size();
//    int to_sq;
//    bool prom = false;
//    bool s0 = 0;
//    bool s1 = 0;
//
//    if ( san[len-1] == '#' || san[len-1] == '+' ) {
//        san = san.substr( 0, len-1 );
//        len--;
//    }
//
//    if ( san[len-2] == '=' ) {
//        prom = true;
//        switch ( san[len-1] ) {
//            case 'R':
//                s0 = 1;
//                s1 = 0;
//                break;
//            case 'N':
//                s0 = 0;
//                s1 = 0;
//                break;
//            case 'B':
//                s0 = 0;
//                s1 = 1;
//                break;
//            case 'Q':
//                s0 = 1;
//                s1 = 1;
//                break;
//        }
//    }
//
//    to_sq = _stoi( san.substr( len-2, 2 ) );
//    san = san.substr( 0, len-2 );
//    len -= 2;
//    bool cap = false;
//    if ( san[len-1] == 'x' ) {
//        cap = true;
//        san = san.substr( 0, len-1 );
//        l--;
//    }
//
//    if ( san == "" ) {
//        // pawn move
//        return move_t ( to_sq + ( ( sideToMove == white ) ? S+S : N+N ), to_sq, prom, false, s0, s1 );
//    }
//
//    int from_file;
//    int to_file;
//    piece mP;
//
//    switch ( san[0] ) {
//        case 'R':
//            mp = 1;
//            break;
//        case 'N':
//            mp = 2;
//            break;
//        case 'B':
//            mp = 3;
//            break;
//        case 'Q':
//            mp = 4;
//            break;
//        case 'K':
//            mp = 5;
//            break;
//
//        case '1':
//            mp = 0;
//            break;
//        case '2':
//            mp = 0;
//            break;
//        case '3':
//            mp = 0;
//            break;
//        case '4':
//            mp = 0;
//            break;
//        case '5':
//            mp = 0;
//            break;
//        case '6':
//            mp = 0;
//            break;
//        case '7':
//            mp = 0;
//            break;
//        case '8':
//            mp = 0;
//            break;
//
//        case 'a':
//        case 'b':
//        case 'c':
//        case 'd':
//        case 'e':
//        case 'f':
//        case 'g':
//        case 'h':
//    }
//}

// move_t constructors
// default constructor

move_t::move_t() { }

// parameterised constructor

move_t::move_t(uint8_t from, uint8_t to, bool promotion,
               bool capture, bool spec1, bool spec0) {
    data = (from) | (to << 6) | (promotion << 15) | (capture << 12) |
           (spec1 << 14) | (spec0 << 13);
}

move_t::move_t(uint16_t cons_data) {
    data = cons_data;
}

// bits 0-5:  from square
// bits 6-11: to square
// bits 12:   capture
// bit 13:    special 0
// bit 14:    special 1
// bit 15:    promotion

uint8_t move_t::from_sq() {
    return (data & 63);
}

uint8_t move_t::to_sq() {
    return (data >> 6) & 63;
}

uint8_t move_t::flags() {
    return (data >> 12) & 15;
}

bool move_t::is_quiet() {
    return !flags();
}

bool move_t::is_promotion() {
    return (flags() & 8);
}

void move_t::set_promotion(piece pc) {
    switch (pc) {
    case queen:
        data = unset_bit(data, 13);
        data = unset_bit(data, 14);
        break;
    case rook:
        data = set_bit(data, 13);
        data = unset_bit(data, 14);
        break;
    case bishop:
        data = unset_bit(data, 13);
        data = set_bit(data, 14);
        break;
    case knight:
        data = set_bit(data, 13);
        data = set_bit(data, 14);
        break;
    default:
        break;
    }
}

bool move_t::is_capture() {
    return (flags() & 1);
}

bool move_t::is_ep_capture() {
    return (flags() == 3);
}

bool move_t::is_doublePP() {
    return (flags() == 2);
}

bool move_t::is_kingCastle() {
    return ( flags() == 4);
}

bool move_t::is_queenCastle() {
    return ( flags() == 6);
}

bool move_t::is_castle() {
    return ( is_kingCastle() | is_queenCastle());
}

uint16_t move_t::give() {
    return data;
}

piece move_t::which_promotion() {
    switch (flags() & 6) {
    case 0:
        return queen;
        break;
    case 2:
        return rook;
        break;
    case 4:
        return bishop;
        break;
    case 6:
        return knight;
        break;
    default:
        return queen;
        break;
    }
}

std::ostream& operator<<(std::ostream &out, const move_t &move) {

    move_t _move = move;

    if (_move.give() == 0) {
        out << "NULL";
        return out;
    }

    int fromSq = _move.from_sq();
    int toSq = _move.to_sq();

    itos(fromSq, out);
    itos(toSq, out);

    if (_move.is_promotion()) {
        switch (_move.flags() & 6) {
        case 0:
            out << "Q";
            break;
        case 2:
            out << "R";
            break;
        case 4:
            out << "B";
            break;
        case 6:
            out << "N";
            break;
        }
    }

    return out;
}

bool operator==(const move_t &self, const move_t &other) {
    return self.data == other.data;
}

bool operator!=(const move_t &self, const move_t &other) {
    return self.data != other.data;
}

move_t stom(move_t* moves, int n_moves, std::string s) {
    std::string from = s.substr(0, 2);
    std::string to = s.substr(2, 2);
    int from_ind = _stoi(from);
    int to_ind = _stoi(to);
    move_t cand;

    for (int i = 0; i < n_moves; i++) {
        cand = moves[i];
        if ((cand.from_sq() == from_ind) && (cand.to_sq() == to_ind)) {
            if (cand.is_promotion()) {
                if (s.size() != 5) return move_t(0, 0, 0, 0, 0, 0);
                switch (s[4]) {
                case 'q':
                case 'Q':
                    cand.set_promotion(queen);
                    break;
                case 'r':
                case 'R':
                    cand.set_promotion(rook);
                    break;
                case 'b':
                case 'B':
                    cand.set_promotion(bishop);
                    break;
                case 'n':
                case 'N':
                    cand.set_promotion(knight);
                    break;
                default:
                    return move_t(0, 0, 0, 0, 0, 0);
                }
            }
            return cand;
        }
    }

    return move_t(0, 0, 0, 0, 0, 0);
}

// calculate ray tables for move generation
// bitboards indexed by direction then square
//->     64         *      8       *     64   =   32768 bits = 4K

bitboard rays[8][64] = {0};

void init_rays() {
    int sq;
    int dir;
    bitboard tmp;

    for (dir = 0; dir < 8; dir++) {
        for (sq = 0; sq < 64; sq++) {
            tmp = (1ULL << sq);
            while (tmp) {
                tmp = oneGeneral8(tmp, dir);
                rays[dir][sq] |= tmp;
            }
        }
    }
}


// naive move generation

// pawns

bitboard pawnPushNaive(int sq, bitboard blockers, colour movingColour) {
    bitboard pB = (1ULL << sq);
    if (movingColour == white) return ( (oneN(pB) | twoN(pB & rankTwo)) &
                                            (~blockers));
    else return ( (oneS(pB) | twoS(pB & rankSeven)) & (~blockers));
}

bitboard pawnAttackNaive(int sq, colour movingColour) {
    bitboard pB = (1ULL << sq);
    if (movingColour == white) return ( (oneNE(pB) | oneNW(pB)));
    else return ( (oneSE(pB) | oneSW(pB)));
}

bitboard pawnAttacks(int sq, bitboard _white, bitboard _black,
                     colour movingColour) {
    if (movingColour == white) return ( pawnAttackNaive(sq, movingColour) & _black);
    else return ( pawnAttackNaive(sq, movingColour) & _white);
}

bitboard pawnTargets(int sq, bitboard _white, bitboard _black,
                     colour movingColour) {
    bitboard pushes = pawnPushNaive(sq, _white | _black, movingColour);
    bitboard attacks = pawnAttackNaive(sq, movingColour);
    if (movingColour == white) return ( (attacks & _black) | pushes);
    else return ( (attacks & _white) | pushes);
}


// knights

bitboard knightPushNaive(int sq) {
    bitboard n = (1ULL << sq);
    return ( oneNNE(n) | oneENE(n) | oneESE(n) | oneSSE(n)
             | oneSSW(n) | oneWSW(n) | oneWNW(n) | oneNNW(n));
}

bitboard knightTargets(int sq, bitboard _white, bitboard _black,
                       colour movingColour) {
    bitboard moves = knightPushNaive(sq);
    if (movingColour == white) return ( moves & (~_white));
    else return ( moves & (~_black));
}

// kings

bitboard kingPushNaive(int sq) {
    bitboard k = (1ULL << sq);
    return ( oneN(k) | oneE(k) | oneS(k) | oneW(k) | oneNE(k) | oneSE(k) | oneSW(
                 k) | oneNW(k));
}

bitboard kingTargets(int sq, bitboard _white, bitboard _black,
                     colour movingColour) {
    bitboard moves = kingPushNaive(sq);
    if (movingColour == white) return ( moves & (~_white));
    else return ( moves & (~_black));
}

// sliding piece generation
// bishops

bitboard bishopPushNaive(int sq, bitboard blockers) {
    bitboard tmp;
    bitboard diagRays = 0ULL;

    // NE
    diagRays |= rays[dirNE][sq];
    tmp = blockers & rays[dirNE][sq];
    if (tmp) {
        diagRays &= ~rays[dirNE][first_set_bit(tmp)];
    }

    // SE
    diagRays |= rays[dirSE][sq];
    tmp = blockers & rays[dirSE][sq];
    if (tmp) {
        diagRays &= ~rays[dirSE][last_set_bit(tmp)];
    }

    // SW
    diagRays |= rays[dirSW][sq];
    tmp = blockers & rays[dirSW][sq];
    if (tmp) {
        diagRays &= ~rays[dirSW][last_set_bit(tmp)];
    }

    // NW
    diagRays |= rays[dirNW][sq];
    tmp = blockers & rays[dirNW][sq];
    if (tmp) {
        diagRays &= ~rays[dirNW][first_set_bit(tmp)];
    }

    return diagRays;
}

bitboard bishopTargets(int sq, bitboard _white, bitboard _black,
                       colour movingColour) {
    bitboard moves = bishopPushNaive(sq, _white | _black);
    if (movingColour == white) return ( moves & (~_white));
    else return ( moves & (~_black));
}

// rooks

bitboard rookPushNaive(int sq, bitboard blockers) {
    bitboard tmp;
    bitboard compRays = 0ULL;

    // N
    compRays |= rays[dirN][sq];
    tmp = blockers & rays[dirN][sq];
    if (tmp) {
        compRays &= ~rays[dirN][first_set_bit(tmp)];
    }

    // E
    compRays |= rays[dirE][sq];
    tmp = blockers & rays[dirE][sq];
    if (tmp) {
        compRays &= ~rays[dirE][first_set_bit(tmp)];
    }

    // S
    compRays |= rays[dirS][sq];
    tmp = blockers & rays[dirS][sq];
    if (tmp) {
        compRays &= ~rays[dirS][last_set_bit(tmp)];
    }

    // W
    compRays |= rays[dirW][sq];
    tmp = blockers & rays[dirW][sq];
    if (tmp) {
        compRays &= ~rays[dirW][last_set_bit(tmp)];
    }

    return compRays;
}

bitboard rookTargets(int sq, bitboard _white, bitboard _black,
                     colour movingColour) {
    bitboard moves = rookPushNaive(sq, _white | _black);
    if (movingColour == white) return ( moves & (~_white));
    else return ( moves & (~_black));
}

// queens

bitboard queenPushNaive(int sq, bitboard blockers) {
    return bishopPushNaive(sq, blockers) | rookPushNaive(sq, blockers);
}

bitboard queenTargets(int sq, bitboard _white, bitboard _black,
                      colour movingColour) {
    bitboard moves = queenPushNaive(sq, _white | _black);
    if (movingColour == white) return ( moves & (~_white));
    else return ( moves & (~_black));
}


// piece targets

bitboard pieceTargets(int sq, bitboard _white, bitboard _black,
                      colourPiece cP) {
    colour col = colour(cP / 6);
    int piece = cP % 6;

    switch (piece) {
    case 0:
        return pawnTargets(sq, _white, _black, col);
        break;
    case 1:
        return rookTargets(sq, _white, _black, col);
        break;
    case 2:
        return knightTargets(sq, _white, _black, col);
        break;
    case 3:
        return bishopTargets(sq, _white, _black, col);
        break;
    case 4:
        return queenTargets(sq, _white, _black, col);
        break;
    case 5:
        return kingTargets(sq, _white, _black, col);
        break;
    default:
        return bitboard();
    }
}

bitboard allTargets(colour side, bitboard* pieces) {
    bitboard attacked = 0ULL;
    int sq;
    bitboard tmp;

    bitboard allPieces = 0ULL;
    for (sq=0; sq<12; sq++) {
        allPieces |= pieces[sq];
    }

    // pawns
    tmp = pieces[side * 6];
    ITER_BITBOARD(sq, tmp) {
        attacked |= pawnAttackNaive(sq, side);
    }

    // rooks
    tmp = pieces[(side * 6) + 1];
    ITER_BITBOARD(sq, tmp) {
        attacked |= rookPushNaive(sq, allPieces);
    }

    // knights
    tmp = pieces[(side * 6) + 2];
    ITER_BITBOARD(sq, tmp) {
        attacked |= knightPushNaive(sq);
    }

    // bishops
    tmp = pieces[(side * 6) + 3];
    ITER_BITBOARD(sq, tmp) {
        attacked |= bishopPushNaive(sq, allPieces);
    }

    // queens
    tmp = pieces[(side * 6) + 4];
    ITER_BITBOARD(sq, tmp) {
        attacked |= queenPushNaive(sq, allPieces);
    }

    // kings
    tmp = pieces[(side * 6) + 5];
    ITER_BITBOARD(sq, tmp) {
        attacked |= kingPushNaive(sq);
    }

    for (sq=(side * 6); sq < (side * 6) + 6; sq++) {
        attacked &= (~ pieces[sq]);
    }

    return attacked;
}




bool board::add_moves(move_t ** dest, move_t move, bool check_legal) const {
    if (check_legal) {
        if (is_legal(move)) {
            **dest = move;
            (*dest)++;
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        **dest = move;
        (*dest)++;
        return 1;
    }
}

// generate pseudo-legal moves (without checking for checks)
// returns the number of moves

int board::gen_moves(move_t * moves) const {
    int piece;
    int from_sq;
    int to_sq;
    int count = 0;
    bitboard targets;
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();

    for (piece = sideToMove * 6; piece < (1 + sideToMove)*6; piece++) {
        for (from_sq = 0; from_sq < 64; from_sq++) {
            if (pieceBoards[piece] & (1ULL << from_sq)) {
                targets = pieceTargets(from_sq, _white, _black, colourPiece(piece));

                ITER_BITBOARD(to_sq, targets) {
                    // found a move!
                    if (((1ULL << to_sq) & _black) | ((1ULL << to_sq) & _white)) {
                        // capture
                        if (piece % 6 == 0 && ((rankOne | rankEight)&(1ULL << to_sq))) {
                            // promotion
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 0, 0), false);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 0, 1), false);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 1, 0), false);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 1, 1), false);
                        }
                        else {
                            count += add_moves(&moves, move_t(from_sq, to_sq, 0, 1, 0, 0), false);
                        }
                    }
                    else {
                        // non-capture
                        if ((piece % 6 == 0) & (abs(from_sq - to_sq) == 16)) {
                            // double pawn push
                            if (!((1ULL << ((from_sq + to_sq) / 2)) & (_black | _white))) {
                                count += add_moves(&moves, move_t(from_sq, to_sq, 0, 0, 0, 1), false);
                            }
                            else {
                            }
                        }
                        else {
                            // quiet move
                            if (piece % 6 == 0 && ((rankOne | rankEight)&(1ULL << to_sq))) {
                                // promotion
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 0, 0), false);
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 0, 1), false);
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 1, 0), false);
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 1, 1), false);
                            }
                            else {
                                count += add_moves(&moves, move_t(from_sq, to_sq, 0, 0, 0, 0), false);
                            }
                        }
                    }
                }
            }
        }
    }
    // fringe cases

    // ep capture
    if (lastMoveDoublePawnPush) {
        // if white to move: potential squares are ( 32 + dPPFile +-1 )
        // if black to move: potential squares are ( 24 + dPPFile +-1 )
        int oppPawnSquare = 32 - (8 * sideToMove) + dPPFile;
        bitboard oppPawn = (1ULL << oppPawnSquare);
        bitboard left = oneW(oppPawn);
        bitboard right = oneE(oppPawn);
        int captureSquare = 40 - (24 * sideToMove) + dPPFile;
        if (left & (pieceBoards[ 6 * sideToMove ])) {
            int leftSquare = last_set_bit(left);
            count += add_moves(&moves, move_t(leftSquare, captureSquare, 0, 1, 0, 1),
                               false);
        }
        if (right & (pieceBoards[ 6 * sideToMove ])) {
            int rightSquare = last_set_bit(right);
            count += add_moves(&moves, move_t(rightSquare, captureSquare, 0, 1, 0, 1),
                               false);
        }
    }

    // castling
    bitboard pb[12];
    getBitboards(pb);
    bitboard attacked_squares = allTargets(flipColour(sideToMove), pb);
    if (sideToMove == white && !is_check(white)) {
        if (castleWhiteKingSide && (!((_white | _black) & 0x0000000000000060))) {
            // can't castle through check
            if (! (attacked_squares & 0x0000000000000060)) {
                count += add_moves(&moves, move_t(4, 6, 0, 0, 1, 0), false);
            }
        }
        if (castleWhiteQueenSide && (!((_white | _black) & 0x000000000000000e))) {
            if (! (attacked_squares & 0x000000000000000c)) {
                count += add_moves(&moves, move_t(4, 2, 0, 0, 1, 1), false);
            }
        }
    }
    else if (sideToMove == black && !is_check(black)) {
        if (castleBlackKingSide && (!((_white | _black) & 0x6000000000000000))) {
            if (! (attacked_squares & 0x6000000000000000)) {
                count += add_moves(&moves, move_t(60, 62, 0, 0, 1, 0), false);
            }
        }
        if (castleBlackQueenSide && (!((_white | _black) & 0x0e00000000000000))) {
            if (! (attacked_squares & 0x0c00000000000000)) {
                count += add_moves(&moves, move_t(60, 58, 0, 0, 1, 1), false);
            }
        }
    }

    return count;
}

int board::get_out_of_check(colour side, move_t * moves, piece checkingPiece,
                            int checkingInd, int kingInd, bool double_check) const {
    int count = 0;
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    bitboard _other = (side == white) ? _black : _white;


    // king moves out of check
    int king_ind = last_set_bit(pieceBoards[(side * 6) + 5]);
    bitboard targets = pieceTargets(king_ind, _white, _black,
                                    colourPiece((side * 6) + 5));
    int to_ind;

    ITER_BITBOARD(to_ind, targets) {
        if (_other & (1ULL << to_ind)) {
            count += add_moves(&moves, move_t(king_ind, to_ind, 0, 1, 0, 0), true);
        }
        else {
            count += add_moves(&moves, move_t(king_ind, to_ind, 0, 0, 0, 0), true);
        }
    }

    if (double_check) {
        return count;
    }

    // take the checking piece
    int from_sq;
    bitboard defender;
    for (int i = (6 * side); i < (6 * side) + 5; i++) {
        defender = pieceTargets(checkingInd, _white, _black,
                                colourPiece((i + 6) % 12)) & pieceBoards[i];

        ITER_BITBOARD(from_sq, defender) {
            if (i == (6 * side) && (checkingInd / 8 == 0 || checkingInd / 8 == 7)) {
                move_t prom_queen(from_sq, checkingInd, 1, 1, 0, 0);
                if (!is_legal(prom_queen)) continue;
                count += add_moves(&moves, prom_queen, false);
                count += add_moves(&moves, move_t(from_sq, checkingInd, 1, 1, 0, 1), false);
                count += add_moves(&moves, move_t(from_sq, checkingInd, 1, 1, 1, 0), false);
                count += add_moves(&moves, move_t(from_sq, checkingInd, 1, 1, 1, 1), false);
            }
            else {
                count += add_moves(&moves, move_t(from_sq, checkingInd, 0, 1, 0, 0), true);
            }
        }
    }
    bitboard left = oneW(1ULL << checkingInd);
    bitboard right = oneE(1ULL << checkingInd);
    if (lastMoveDoublePawnPush && (checkingPiece == 0)) {
        to_ind = checkingInd + ((side == white) ? N : S);
        if (pieceBoards[side * 6] & right) {
            count += add_moves(&moves, move_t(checkingInd + 1, to_ind, 0, 1, 0, 1), true);
        }
        if (pieceBoards[side * 6] & left) {
            count += add_moves(&moves, move_t(checkingInd - 1, to_ind, 0, 1, 0, 1), true);
        }
    }
    if (is_bit_set(kingTargets(kingInd, _white, _black, side), checkingInd)) {
        return count;
    }
    if (checkingPiece % 6 == 0 || checkingPiece % 6 == 2 ||
            checkingPiece % 6 == 5) {
        return count;
    }



    // moving into the way
    int blockingInd;
    int blockingPiece;
    int ind_diff = checkingInd - kingInd;
    bitboard blockers;
    int defenderInd;
    int _dir;

    if (ind_diff > 0) {
        if (ind_diff == 63 || ind_diff % 9 == 0) _dir = NE;
        else if (checkingInd / 8 == kingInd / 8) _dir = E;
        else if (ind_diff % 8 == 0) _dir = N;
        else if (ind_diff % 7 == 0) _dir = NW;
        else return count;
    }
    else {
        if (ind_diff == -63 || (-ind_diff) % 9 == 0) _dir = SW;
        else if (checkingInd / 8 == kingInd / 8) _dir = W;
        else if ((-ind_diff) % 8 == 0) _dir = S;
        else if ((-ind_diff) % 7 == 0) _dir = SE;
        else return count;
    }

    for (blockingInd = kingInd + _dir; blockingInd != checkingInd;
            blockingInd += _dir) {
        // pawns
        blockers = pieceBoards[side * 6];

        ITER_BITBOARD(defenderInd, blockers) {
            if (is_bit_set(pieceTargets(defenderInd, _white, _black,
                                        colourPiece(side * 6)), blockingInd)) {
                switch (abs(defenderInd - blockingInd)) {
                case 8:
                    count += add_moves(&moves, move_t(defenderInd, blockingInd, 0, 0, 0, 0), true);
                    break;
                case 16:
                    if ((_black | _white) & (1ULL << ((defenderInd + blockingInd) / 2))) break;
                    count += add_moves(&moves, move_t(defenderInd, blockingInd, 0, 0, 0, 1), true);
                    break;
                }
            }
        }


        // rooks, bishops, knights, queens
        for (blockingPiece = (side * 6) + 1; blockingPiece < (side * 6) + 5;
                blockingPiece++) {
            blockers = pieceBoards[blockingPiece] & pieceTargets(blockingInd, _white,
                       _black, colourPiece((blockingPiece + 6) % 12));
            ITER_BITBOARD(defenderInd, blockers) {
                count += add_moves(&moves, move_t(defenderInd, blockingInd, 0, 0, 0, 0), true);
            }
        }
    }

    return count;
}

bool board::can_get_out_of_check(colour side, piece checkingPiece,
                            int checkingInd, int kingInd, bool double_check) const {
    int count = 0;
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    bitboard _other = (side == white) ? _black : _white;


    // king moves out of check
    int king_ind = last_set_bit(pieceBoards[(side * 6) + 5]);
    bitboard targets = pieceTargets(king_ind, _white, _black,
                                    colourPiece((side * 6) + 5));
    int to_ind;

    ITER_BITBOARD(to_ind, targets) {
        if (_other & (1ULL << to_ind)) {
            if (is_legal(move_t(king_ind, to_ind, 0, 1, 0, 0))) return true;
        }
        else {
            if (is_legal(move_t(king_ind, to_ind, 0, 0, 0, 0))) return true;
        }
    }

    if (double_check) {
        return false;
    }

    // take the checking piece
    int from_sq;
    bitboard defender;
    for (int i = (6 * side); i < (6 * side) + 5; i++) {
        defender = pieceTargets(checkingInd, _white, _black,
                                colourPiece((i + 6) % 12)) & pieceBoards[i];

        ITER_BITBOARD(from_sq, defender) {
            if (i == (6 * side) && (checkingInd / 8 == 0 || checkingInd / 8 == 7)) {
                if (is_legal(move_t(from_sq, checkingInd, 1, 1, 0, 0))) return true;
            }
            else {
                if (is_legal(move_t(from_sq, checkingInd, 0, 1, 0, 0))) return true;
            }
        }
    }
    bitboard left = oneW(1ULL << checkingInd);
    bitboard right = oneE(1ULL << checkingInd);
    if (lastMoveDoublePawnPush && (checkingPiece == 0)) {
        to_ind = checkingInd + ((side == white) ? N : S);
        if (pieceBoards[side * 6] & right) {
            if (is_legal(move_t(checkingInd + 1, to_ind, 0, 1, 0, 1))) return true;
        }
        if (pieceBoards[side * 6] & left) {
            if (is_legal(move_t(checkingInd - 1, to_ind, 0, 1, 0, 1))) return true;
        }
    }
    if (is_bit_set(kingTargets(kingInd, _white, _black, side), checkingInd)) {
        return false;
    }
    if (checkingPiece % 6 == 0 || checkingPiece % 6 == 2 ||
            checkingPiece % 6 == 5) {
        return false;
    }



    // moving into the way
    int blockingInd;
    int blockingPiece;
    int ind_diff = checkingInd - kingInd;
    bitboard blockers;
    int defenderInd;
    int _dir;

    if (ind_diff > 0) {
        if (ind_diff == 63 || ind_diff % 9 == 0) _dir = NE;
        else if (checkingInd / 8 == kingInd / 8) _dir = E;
        else if (ind_diff % 8 == 0) _dir = N;
        else if (ind_diff % 7 == 0) _dir = NW;
        else return false;
    }
    else {
        if (ind_diff == -63 || (-ind_diff) % 9 == 0) _dir = SW;
        else if (checkingInd / 8 == kingInd / 8) _dir = W;
        else if ((-ind_diff) % 8 == 0) _dir = S;
        else if ((-ind_diff) % 7 == 0) _dir = SE;
        else return false;
    }

    for (blockingInd = kingInd + _dir; blockingInd != checkingInd;
            blockingInd += _dir) {
        // pawns
        blockers = pieceBoards[side * 6];

        ITER_BITBOARD(defenderInd, blockers) {
            if (is_bit_set(pieceTargets(defenderInd, _white, _black,
                                        colourPiece(side * 6)), blockingInd)) {
                switch (abs(defenderInd - blockingInd)) {
                case 8:
                    if(is_legal(move_t(defenderInd, blockingInd, 0, 0, 0, 0))) return true;
                    break;
                case 16:
                    if ((_black | _white) & (1ULL << ((defenderInd + blockingInd) / 2))) break;
                    if (is_legal(move_t(defenderInd, blockingInd, 0, 0, 0, 1))) return true;
                    break;
                }
            }
        }


        // rooks, bishops, knights, queens
        for (blockingPiece = (side * 6) + 1; blockingPiece < (side * 6) + 5;
                blockingPiece++) {
            blockers = pieceBoards[blockingPiece] & pieceTargets(blockingInd, _white,
                       _black, colourPiece((blockingPiece + 6) % 12));
            ITER_BITBOARD(defenderInd, blockers) {
                if (is_legal(move_t(defenderInd, blockingInd, 0, 0, 0, 0))) return true;
            }
        }
    }

    return false;
}

// generate captures
// returns the number of captures
int board::gen_captures(move_t * moves) const {
    int _piece;
    int from_sq;
    int to_sq;
    int count = 0;
    bitboard targets;
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    bitboard _other = ((sideToMove == white) ? _black : _white);
    for (_piece = sideToMove * 6; _piece < (1 + sideToMove)*6; _piece++) {
        for (from_sq = 0; from_sq < 64; from_sq++) {
            if (pieceBoards[_piece] & (1ULL << from_sq)) {
                targets = pieceTargets(from_sq, _white, _black, colourPiece(_piece));

                ITER_BITBOARD(to_sq, targets) {
                    if ((1ULL << to_sq) & _other) {
                        // capture
                        if (_piece % 6 == 0 && ((rankOne | rankEight)&(1ULL << to_sq))) {
                            // promotion
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 0, 0), true);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 0, 1), true);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 1, 0), true);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 1, 1), true);
                        }
                        else {
                            count += add_moves(&moves, move_t(from_sq, to_sq, 0, 1, 0, 0), true);
                        }
                    }
                }
            }
        }
    }

    // ep capture
    if (lastMoveDoublePawnPush) {
        // if white to move: potential squares are ( 32 + dPPFile +-1 )
        // if black to move: potential squares are ( 24 + dPPFile +-1 )
        int oppPawnSquare = 32 - (8 * sideToMove) + dPPFile;
        bitboard oppPawn = (1ULL << oppPawnSquare);
        bitboard left = oneW(oppPawn);
        bitboard right = oneE(oppPawn);
        int captureSquare = 40 - (24 * sideToMove) + dPPFile;
        if (left & (pieceBoards[ 6 * sideToMove ])) {
            int leftSquare = last_set_bit(left);
            count += add_moves(&moves, move_t(leftSquare, captureSquare, 0, 1, 0, 1), true);
        }
        if (right & (pieceBoards[ 6 * sideToMove ])) {
            int rightSquare = last_set_bit(right);
            count += add_moves(&moves, move_t(rightSquare, captureSquare, 0, 1, 0, 1),
                               true);
        }
    }
    return count;
}



// generate pseudo-legal moves (without checking for checks)
// returns the number of moves

int board::gen_legal_moves(move_t * moves) const {
    int _piece;
    int from_sq;
    int to_sq;
    int count = 0;
    bitboard targets;
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    bitboard _other = ((sideToMove == white) ? _black : _white);

    // if we're in check we can limit the search
    piece checkingPiece;
    int checkingInd;
    bool double_check = false;
    if (is_check(sideToMove, &checkingPiece, &checkingInd, &double_check)) {
        return get_out_of_check(sideToMove, moves, checkingPiece, checkingInd,
                                last_set_bit(pieceBoards[(6 * sideToMove) + 5]), double_check);
    }

    for (_piece = sideToMove * 6; _piece < (1 + sideToMove)*6; _piece++) {
        for (from_sq = 0; from_sq < 64; from_sq++) {
            if (pieceBoards[_piece] & (1ULL << from_sq)) {
                targets = pieceTargets(from_sq, _white, _black, colourPiece(_piece));

                ITER_BITBOARD(to_sq, targets) {
                    // found a move!
                    if ((1ULL << to_sq) & _other) {
                        // capture
                        if (_piece % 6 == 0 && ((rankOne | rankEight)&(1ULL << to_sq))) {
                            // promotion
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 0, 0), true);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 0, 1), true);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 1, 0), true);
                            count += add_moves(&moves, move_t(from_sq, to_sq, 1, 1, 1, 1), true);
                        }
                        else {
                            count += add_moves(&moves, move_t(from_sq, to_sq, 0, 1, 0, 0), true);
                        }
                    }
                    else {
                        // non-capture
                        if ((_piece % 6 == 0) & (abs(from_sq - to_sq) == 16)) {
                            // double pawn push
                            if (!((1ULL << ((from_sq + to_sq) / 2)) & (_black | _white))) {
                                count += add_moves(&moves, move_t(from_sq, to_sq, 0, 0, 0, 1), true);
                            }
                            else {
                            }
                        }
                        else {
                            // quiet move
                            if (_piece % 6 == 0 && ((rankOne | rankEight)&(1ULL << to_sq))) {
                                // promotion
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 0, 0), true);
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 0, 1), true);
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 1, 0), true);
                                count += add_moves(&moves, move_t(from_sq, to_sq, 1, 0, 1, 1), true);
                            }
                            else {
                                count += add_moves(&moves, move_t(from_sq, to_sq, 0, 0, 0, 0), true);
                            }
                        }
                    }
                }
            }
        }
    }
    // fringe cases

    // ep capture
    if (lastMoveDoublePawnPush) {
        // if white to move: potential squares are ( 32 + dPPFile +-1 )
        // if black to move: potential squares are ( 24 + dPPFile +-1 )
        int oppPawnSquare = 32 - (8 * sideToMove) + dPPFile;
        bitboard oppPawn = (1ULL << oppPawnSquare);
        bitboard left = oneW(oppPawn);
        bitboard right = oneE(oppPawn);
        int captureSquare = 40 - (24 * sideToMove) + dPPFile;
        if (left & (pieceBoards[ 6 * sideToMove ])) {
            int leftSquare = last_set_bit(left);
            count += add_moves(&moves, move_t(leftSquare, captureSquare, 0, 1, 0, 1), true);
        }
        if (right & (pieceBoards[ 6 * sideToMove ])) {
            int rightSquare = last_set_bit(right);
            count += add_moves(&moves, move_t(rightSquare, captureSquare, 0, 1, 0, 1),
                               true);
        }
    }

    // castling
    bitboard pb[12];
    getBitboards(pb);
    bitboard attacked_squares = allTargets(flipColour(sideToMove), pb);
    if (sideToMove == white && !is_check(white)) {
        if (castleWhiteKingSide && (!((_white | _black) & 0x0000000000000060))) {
            // can't castle through check
            if (! (attacked_squares & 0x0000000000000060)) {
                count += add_moves(&moves, move_t(4, 6, 0, 0, 1, 0), true);
            }
        }
        if (castleWhiteQueenSide && (!((_white | _black) & 0x000000000000000e))) {
            if (! (attacked_squares & 0x000000000000000c)) {
                count += add_moves(&moves, move_t(4, 2, 0, 0, 1, 1), true);
            }
        }
    }
    else if (sideToMove == black && !is_check(black)) {
        if (castleBlackKingSide && (!((_white | _black) & 0x6000000000000000))) {
            if (! (attacked_squares & 0x6000000000000000)) {
                count += add_moves(&moves, move_t(60, 62, 0, 0, 1, 0), true);
            }
        }
        if (castleBlackQueenSide && (!((_white | _black) & 0x0e00000000000000))) {
            if (! (attacked_squares & 0x0c00000000000000)) {
                count += add_moves(&moves, move_t(60, 58, 0, 0, 1, 1), true);
            }
        }
    }

    return count;
}

bool board::is_legal(struct move_t move) const {
    colourPiece movingPiece;
    int from_ind = move.from_sq();
    int to_ind = move.to_sq();
    bitboard from_square = (1ULL << from_ind);
    bool foundMovingPiece = false;

    for (int i = sideToMove * 6; i < (1 + sideToMove)*6; i++) {
        if (pieceBoards[i] & from_square) {
            movingPiece = colourPiece(i);
            foundMovingPiece = true;
            break;
        }
    }

    if (! foundMovingPiece) {
        return false;
    }

    colour otherSide = flipColour(sideToMove);
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();


    // king can't move into check
    if (movingPiece % 6 == 5) {
        if (pawnAttackNaive(to_ind,
                            sideToMove) & pieceBoards[6 * otherSide]) return false;
        if (rookTargets(to_ind, _white & (~from_square), _black & (~from_square),
                        sideToMove) & pieceBoards[(6 * otherSide) + 1]) return false;
        if (knightTargets(to_ind, _white & (~from_square), _black & (~from_square),
                          sideToMove) & pieceBoards[(6 * otherSide) + 2]) return false;
        if (bishopTargets(to_ind, _white & (~from_square), _black & (~from_square),
                          sideToMove) & pieceBoards[(6 * otherSide) + 3]) return false;
        if (queenTargets(to_ind, _white & (~from_square), _black & (~from_square),
                         sideToMove) & pieceBoards[(6 * otherSide) + 4]) return false;
        if (kingTargets(to_ind, _white & (~from_square), _black & (~from_square),
                        sideToMove) & pieceBoards[(6 * otherSide) + 5]) return false;
        return true;
    }

    // can't move pinned pieces
    bitboard _ray;
    bitboard _ray2;
    bitboard blockers = takenSquares();
    bitboard tmp;
    bitboard attacker;

    for (int i = 0; i < 8; i++) {
        _ray2 = rays[i][from_ind] & blockers;
        if ((i + 1) % 8 < 4) _ray2 = (1ULL << first_set_bit(_ray2));
        else _ray2 = (1ULL << last_set_bit(_ray2));
        if (_ray2 & pieceBoards[(6 * sideToMove) + 5]) {
            int j = (i + 4) % 8;
            if (i % 2) {
                // bishops and queens
                _ray = rays[j][from_ind];
                tmp = blockers & _ray;
                if (tmp) {
                    if ((j + 1) % 8 < 4) attacker = (1ULL << first_set_bit(tmp));
                    else attacker = (1ULL << last_set_bit(tmp));
                    if ((attacker & pieceBoards[(6 * otherSide) + 3]) || (attacker & pieceBoards[(6 * otherSide) + 4])) {
//                        return is_bit_set(_ray, to_ind) | is_bit_set(rays[i][from_ind], to_ind);
                        if(!(is_bit_set(_ray, to_ind) | is_bit_set(rays[i][from_ind], to_ind))) return false;
                    }
                }
            }
            else {
                // rooks and queens
                _ray = rays[j][from_ind];
                tmp = blockers & _ray;
                if (tmp) {
                    if ((j + 1) % 8 < 4) attacker = (1ULL << first_set_bit(tmp));
                    else attacker = (1ULL << last_set_bit(tmp));
                    if ((attacker & pieceBoards[(6 * otherSide) + 1]) || (attacker & pieceBoards[(6 * otherSide) + 4])) {
                        if (!(is_bit_set(_ray, to_ind) | is_bit_set(rays[i][from_ind], to_ind))) return false;
                    }
                }
            }
        }
    }

    int other_pawn_ind;

    if (move.is_ep_capture()) {
        other_pawn_ind = to_ind + ((sideToMove == white) ? S : N);
        bitboard left_ray = rays[6][from_ind] & rays[6][other_pawn_ind] & blockers;
        bitboard right_ray = rays[2][from_ind] & rays[2][other_pawn_ind] & blockers;
        bitboard attacker_left = (left_ray) ? (1ULL << last_set_bit(left_ray)) : 0;
        bitboard attacker_right = (right_ray) ? (1ULL << first_set_bit(right_ray)) : 0;

        if ((attacker_left & pieceBoards[(sideToMove * 6) + 5]) &&
                (attacker_right & (pieceBoards[(otherSide * 6) + 1] |
                                   pieceBoards[(otherSide * 6) + 4]))) {
            return false;
        }
        if ((attacker_right & pieceBoards[(sideToMove * 6) + 5]) &&
                (attacker_left & (pieceBoards[(otherSide * 6) + 1] |
                                  pieceBoards[(otherSide * 6) + 4]))) {
            return false;
        }

    }

    // if we're in check already

    piece checkingPiece;
    int checkingInd;
    bool double_check = false;

    if (is_check(sideToMove, &checkingPiece, &checkingInd, &double_check)) {
        if (double_check) return false;

        if (to_ind == checkingInd) {
            return true;
        }

        if (move.is_ep_capture()) {
            if (other_pawn_ind == checkingInd) return true;
        }

        int kingInd = last_set_bit(pieceBoards[(sideToMove * 6) + 5]);
        int blockingInd;
        int ind_diff = checkingInd - kingInd;
        int _dir;

        if (ind_diff > 0) {
            if (ind_diff == 63 || ind_diff % 9 == 0) _dir = NE;
            else if (checkingInd / 8 == kingInd / 8) _dir = E;
            else if (ind_diff % 8 == 0) _dir = N;
            else if (ind_diff % 7 == 0) _dir = NW;
            else return false;
        }
        else {
            if (ind_diff == -63 || (-ind_diff) % 9 == 0) _dir = SW;
            else if (checkingInd / 8 == kingInd / 8) _dir = W;
            else if ((-ind_diff) % 8 == 0) _dir = S;
            else if ((-ind_diff) % 7 == 0) _dir = SE;
            else return false;
        }

        for (blockingInd = kingInd + _dir; blockingInd != checkingInd;
                blockingInd += _dir) {
            if (blockingInd == to_ind) return true;
        }
        return false;
    }

    return true;
}


} // end of chessCore namespace
