#ifndef __MOVE_H
#define __MOVE_H

#include "board.h"
#include "typedefs.h"

#include <cstdint>
#include <string>
#include <iostream>


// some constants
// constants for sliding pieces which moves in straight lines
// defined in move.cpp
extern const int N;
extern const int NE;
extern const int E;
extern const int SE;
extern const int S;
extern const int SW;
extern const int W;
extern const int NW;

// constants for knights
// defined in move.cpp
extern const int NNE;
extern const int ENE;
extern const int ESE;
extern const int SSE;
extern const int SSW;
extern const int WSW;
extern const int WNW;
extern const int NNW;

// functions for printing
// defined in move.cpp
void itos(int, char*);
std::string itos(int);
void itos(int, std::ostream&);
int _stoi(std::string);

// enumerate directions for ray generation
enum direction {dirN, dirNE, dirE, dirSE, dirS, dirSW, dirW, dirNW};
// defined in move.cpp
extern bitboard rays[8][64];
void init_rays();

// 2-byte container to store the information of a move.
struct move_t {
    // bits 0-5:  from square
    // bits 6-11: to square
    // bits 12:   capture
    // bit 13:    special 0
    // bit 14:    special 1
    // bit 15:    promotion
private:
    uint16_t data;
public:
    move_t();
    move_t(uint8_t, uint8_t, bool, bool, bool, bool);
    move_t(uint16_t);
    uint16_t from_sq();
    uint16_t to_sq();
    uint16_t flags();
    bool is_quiet();
    bool is_promotion();
    piece which_promotion();
    void set_promotion(piece);
    bool is_capture();
    bool is_ep_capture();
    bool is_doublePP();
    bool is_kingCastle();
    bool is_queenCastle();
    bool is_castle();
    uint16_t give();
    friend std::ostream& operator<<(std::ostream &out, const move_t &move);
    friend bool operator==(const move_t &self, const move_t &other);
    friend bool operator!=(const move_t &self, const move_t &other);
};

// defined in move.cpp
void print_move( struct move_t, std::ostream& cout = std::cout );
move_t stom( move_t*, int, std::string);

// naive move generation
// all defined in move.cpp
// pawns
bitboard pawnPushNaive        (int, bitboard, colour);
bitboard pawnAttackNaive      (int, colour);
bitboard pawnAttacks          (int, bitboard, bitboard, colour);
bitboard pawnTargets          (int, bitboard, bitboard, colour);

// knights
bitboard knightPushNaive      (int);
bitboard knightTargets        (int, bitboard, bitboard, colour);

// kings
bitboard kingPushNaive        (int);
bitboard kingTargets          (int, bitboard, bitboard, colour);

// sliding piece generation
// bishops
bitboard bishopPushNaive      (int, bitboard);
bitboard bishopTargets        (int, bitboard, bitboard, colour);

// rooks
bitboard rookPushNaive        (int, bitboard);
bitboard rookTargets          (int, bitboard, bitboard, colour);

// queens
bitboard queenPushNaive       (int, bitboard);
bitboard queenTargets         (int, bitboard, bitboard, colour);

// general piece
bitboard pieceTargets         (int, bitboard, bitboard, colourPiece);

#endif
