#ifndef __SEARCH_H
#define __SEARCH_H

#include "board.h"
#include "move.h"
#include <cstdint>



// search algorithms
// defined in search.cpp
int32_t minimax(board,int,colour);
struct move_t search_minimax(board,int,colour);

int32_t negamax(board,int,colour);
struct move_t search_negamax(board,int,colour);

int32_t negamaxAB(board,int,colour,int32_t,int32_t);
int32_t negamaxAB(board,int,colour);

struct move_t search_negamaxAB(board,int,colour);

int16_t F2(board,struct node*,uint8_t,colour,int16_t,int16_t);
struct move_t search_F2(board,uint8_t depth,colour);
#endif
