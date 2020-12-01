#include "hash.h"

#include <assert.h>
#include <time.h>

#include "board.h"
#include "move.h"
#include "twiddle.h"
#include "typedefs.h"

#include <cstdint>


namespace chessCore {


// Mersenne_Twister pseudorandom number generator

const int w = 64, m = 156, r = 31;
const uint64_t n = 312;
const uint64_t a = 0xB5026F5AA96619E9;
const int u = 29;
const uint64_t d = 0x5555555555555555;
const int s = 17;
const uint64_t b = 0x71D67FFFEDA60000;
const int t = 37;
const uint64_t c = 0xFFF7EEE000000000;
const int l = 43;
const uint64_t f = 6364136223846793005;



uint64_t check_index = n + 1;
uint64_t lower_mask = (1ULL << r) - 1;
uint64_t upper_mask = ~lower_mask;
uint64_t MT[n];
uint64_t zobristKeys[781];

void init_gen(uint64_t seed) {
    check_index = n;
    MT[0] = seed;
    for (uint64_t i = 1; i < n; i++) {
        MT[i] = ((((MT[i - 1] >> (w - 1)) ^ MT[i - 1]) + i) * f) + i;
    }
}

uint64_t extract_number() {
    if (check_index >= n) {
        assert(check_index == n);
        twist();
    }

    uint64_t y = MT[check_index];

    y = y ^ ((y >> u) & d);
    y = y ^ ((y << s) & b);
    y = y ^ ((y >> t) & c);
    y = y ^ (y >> l);

    check_index++;
    return y;
}

void twist() {
    for (uint64_t i = 0; i < n; i++) {
        uint64_t x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
        uint64_t xA = x >> 1;
        if (x & 1ULL) xA ^= a;
        MT[i] = (MT[(i + m) % n] ^ xA);
    }
    check_index = 0;
}

// initialise zobrist keys

void init_keys() {
    init_gen(time(NULL));

    for (int i = 0; i < 781; i++) {
        zobristKeys[i] = extract_number();
    }
}

void init_keys(uint64_t seed) {
    init_gen(seed);

    for (int i = 0; i < 781; i++) {
        zobristKeys[i] = extract_number();
    }
}

// zobrist hash

uint64_t board::zobrist_hash() const {
    uint64_t ret = 0;
    int cP;
    bitboard tmp;
    int i;

    // pieces
    for (cP = 0; cP < 12; cP++) {
        tmp = pieceBoards[cP];
        ITER_BITBOARD(i, tmp) {
            ret ^= zobristKeys[cP * 64 + i];
        }
    }

    // castling
    if (castleWhiteKingSide) ret ^= zobristKeys[768];
    if (castleWhiteQueenSide) ret ^= zobristKeys[769];
    if (castleBlackKingSide) ret ^= zobristKeys[770];
    if (castleBlackQueenSide) ret ^= zobristKeys[771];

    // side to move
    if (sideToMove == black) ret ^= zobristKeys[780];

    // en passant file
    if (lastMoveDoublePawnPush) ret ^= zobristKeys[772 + dPPFile];

    return ret;
}

uint64_t board::childHash(move_t move) const {
    int i;
    uint16_t fromSquare = from_sq(move);
    uint16_t toSquare = to_sq(move);
    colour otherColour = flipColour(sideToMove);
    colourPiece movingPiece;
    bool castling[4];
    getCastlingRights(castling);
    bool rooktaken = false;
    bool foundMovingPiece = false;
    uint64_t child_hash;
    getHash(&child_hash);

    for (i = (sideToMove * 6); i < (1 + sideToMove)*6; i++) {
        if (is_bit_set(pieceBoards[i], fromSquare)) {
            movingPiece = colourPiece(i);
            child_hash ^= zobristKeys[i * 64 + fromSquare];
            child_hash ^= zobristKeys[i * 64 + toSquare];
            foundMovingPiece = true;
            break;
        }
    }

    if (! foundMovingPiece) {
        return 0;
    }

    if (is_capture(move)) {
        if (!is_ep_capture(move)) {
            for (i = (1 - sideToMove)*6; i < (2 - sideToMove)*6; i++) {
                if (is_bit_set(pieceBoards[i], toSquare)) {
                    if (i % 6 == 1) rooktaken = true;
                    child_hash ^= zobristKeys[i * 64 + toSquare];
                    break;
                }
            }
        }

        else {
            int _dir = (sideToMove == white) ? S : N;
            child_hash ^= zobristKeys[(1 - sideToMove)*6 * 64 + toSquare + _dir];
        }
    }

    if (is_kingCastle(move)) {
        child_hash ^= zobristKeys[(1 + (6 * sideToMove))*64 + fromSquare + 3];
        child_hash ^= zobristKeys[(1 + (6 * sideToMove))*64 + toSquare - 1];
    }
    else if (is_queenCastle(move)) {
        child_hash ^= zobristKeys[(1 + (6 * sideToMove))*64 + fromSquare - 4];
        child_hash ^= zobristKeys[(1 + (6 * sideToMove))*64 + toSquare + 1];
    }

    // promotion
    if (is_promotion(move)) {
        child_hash ^= zobristKeys[6 * sideToMove * 64 + toSquare];
        colourPiece prom_piece = colourPiece((6 * sideToMove) + which_promotion(move));
        child_hash ^= zobristKeys[prom_piece * 64 + toSquare];
    }


    // check for double pawn push
    if (lastMoveDoublePawnPush) {
        child_hash ^= zobristKeys[772 + dPPFile];
    }
    if (is_doublePP(move)) {
        child_hash ^= zobristKeys[772 + (fromSquare % 8)];
    }

    // check for changes to castling rights
    if (movingPiece % 6 == 1) {
        switch (fromSquare) {
        case 0:
            if (castling[1]) {
                child_hash ^= zobristKeys[769];
                castling[1] = false;
            }
            break;
        case 7:
            if (castling[0]) {
                child_hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case 56:
            if (castling[3]) {
                child_hash ^= zobristKeys[771];
                castling[3] = false;
            }
            break;
        case 63:
            if (castling[2]) {
                child_hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    }
    else if (movingPiece % 6 == 5) {
        switch (sideToMove) {
        case white:
            if (castling[1]) {
                child_hash ^= zobristKeys[769];
                castling[1] = false;
            }
            if (castling[0]) {
                child_hash ^= zobristKeys[768];
                castling[0] = false;
            }
            break;
        case black:
            if (castling[3]) {
                child_hash ^= zobristKeys[771];
                castling[3] = false;
            }
            if (castling[2]) {
                child_hash ^= zobristKeys[770];
                castling[2] = false;
            }
            break;
        }
    }

    if (rooktaken) {
        switch (toSquare) {
        case 0:
            if (castling[1]) {
                child_hash ^= zobristKeys[769];
            }
            break;
        case 7:
            if (castling[0]) {
                child_hash ^= zobristKeys[768];
            }
            break;
        case 56:
            if (castling[3]) {
                child_hash ^= zobristKeys[771];
            }
            break;
        case 63:
            if (castling[2]) {
                child_hash ^= zobristKeys[770];
            }
            break;
        }
    }

    // change hash for different side to move
    child_hash ^= zobristKeys[780];

    return child_hash;
}



} // end of chessCore namespace
