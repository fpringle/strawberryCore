#ifndef __SEARCH_H
#define __SEARCH_H

#include "board.h"
#include "move.h"
#include <cstdint>


// tree structure
struct node {
  uint64_t data;         // the hash value of the node
  struct node * first;   // pointer to first child
  struct node * next;    // pointer to next sibling
};
//void add_child(struct node*, uint64_t);
struct node * newNode(uint64_t);
struct node * lastChild(struct node*);
void append_sibling(struct node*, uint64_t);
void append_child(struct node*, uint64_t);

// search algorithms
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
