#ifndef __PLAY_H
#define __PLAY_H

#include "board.h"
#include "move.h"
#include "tree.h"

#include <string>
#include <map>
#include <cstdint>
#include <vector>

// defined in play.cpp
move_t input_move( board );
void play_white(int);
void play_black(int);
void play_white(int, board);
void play_black(int, board);
void play(int, colour);
void play(int, board, colour);

// transposition table
// "signature" = zobrist hash (64 bits)
// "index"     = singature % table size (32 bits)
// best/refutation move (16 bits)
// depth (8 bits)
// score (integrated bound and values (16 bits?)
// age (8 bits)
// total 128 bits?

struct record_t {
    uint64_t signature;
    struct move_t best_ref_move;
    uint8_t depth;
    int16_t IBV_score;
    uint8_t age;
};

class Player : public board {
private:
    std::map<uint32_t, record_t> trans_table;
    std::vector<move_t> move_history;
public:
    // constructors
    // defined in play.cpp
    Player();
    Player(bitboard*, bool*, bool, int, uint8_t, uint8_t,
           colour, int32_t, uint64_t);
    Player(Player&);
    Player(std::string);

    // get data
    // defined in play.cpp
    bool lookup(uint64_t pos_hash, record_t * dest);
    std::vector<move_t> getHistory();
    std::map<uint32_t, record_t> getTable();
    void print_history();

    // make move
    // defined in action.cpp
    void doMoveInPlace(move_t);
    void makeChild(board*, move_t);

    // searching
    // defined in search.cpp
    int32_t search_algorithm(board b, colour side, int depth);
    int32_t search_algorithm(board b, colour side, int depth,
                             int32_t alpha, int32_t beta);
    move_t search(int depth);

    // play
    // defined in play.cpp
    move_t input_move();
    void play(colour side, int plies);
};


#endif
