#include "board.h"
#include "action.h"
#include "move.h"
#include "hash.h"
#include "search.h"
#include "init.h"
#include <limits>
#include <cstdint>

// tree structure
struct node * newNode(uint64_t data) {
  struct node * new_node = new struct node();
  new_node->data = data;
  new_node->first = NULL;
  new_node->next  = NULL;
  return new_node;
}

struct node * lastChild(struct node * parent) {
  if ( parent->first == NULL ) return NULL;
  struct node * p = parent->first;
  while ( p->next != NULL ) p = p->next;
  return p;
}

void append_sibling(struct node * sib1, uint64_t data) {
  struct node * sib2 = newNode(data);
  struct node * p = sib1;
  while ( p->next != NULL ) p = p->next;
  p->next = sib2;
}

void append_child(struct node * parent, uint64_t data) {
  struct node * child = newNode(data);
  struct node * p = lastChild(parent);
  if ( p == NULL ) parent->first = child;
  else p->next = child;
}

// search algorithms

// simple minimax
int32_t minimax(board b, int depth, colour side) {
  if ( depth == 0 ) return b.evaluate();
  int32_t score;
  board child;
  struct move_t moves[256];
  int num_moves = b.gen_moves( moves );

  if ( side == white ) {
    int32_t max_score = std::numeric_limits<int32_t>::min();

    for (int i=0; i<num_moves; i++) {
      child = doMove( b, moves[i] );
      score = minimax( child, depth-1, black );
      if ( score > max_score ) max_score = score;
    }
    return max_score;
  }

  else {
    int32_t min_score = std::numeric_limits<int32_t>::max();

    for (int i=0; i<num_moves; i++) {
      child = doMove( b, moves[i] );
      score = minimax( child, depth-1, white );
      if ( score < min_score ) min_score = score;
    }
    return min_score;
  }
}

struct move_t search_minimax(board b, int depth, colour side) {
  int32_t score;
  board child;
  struct move_t moves[256];
  int num_moves = b.gen_moves( moves );
  struct move_t best_move;

  if ( side == white ) {
    int32_t max_score = std::numeric_limits<int32_t>::min();

    for (int i=0; i<num_moves; i++) {
      child = doMove( b, moves[i] );
      score = minimax( child, depth-1, black );
      if ( score > max_score ) {
        max_score = score;
        best_move = moves[i];
      }
    }
    return best_move;
  }

  else {
    int32_t min_score = std::numeric_limits<int32_t>::max();

    for (int i=0; i<num_moves; i++) {
      child = doMove( b, moves[i] );
      score = minimax( child, depth-1, white );
      if ( score < min_score ) {
        min_score = score;
        best_move = moves[i];
      }
    }
    return best_move;
  }
}


// negamax
int32_t negamax(board b, int depth, colour side) {
  if ( depth == 0 ) return b.evaluate() * ( ( side == white ) ? 1 : -1 );

  colour otherSide = ( side == white ) ? black : white;

  int32_t score;
  board child;

  struct move_t moves[256];
  int num_moves = b.gen_moves( moves );

  int32_t max_score = std::numeric_limits<int32_t>::min();

  for (int i=0; i<num_moves; i++) {
    child = doMove( b, moves[i] );
    score = - negamax( child, depth-1, otherSide );
    if ( score > max_score ) max_score = score;
  }
  return max_score;
}

struct move_t search_negamax(board b, int depth, colour side) {
  int32_t score;
  board child;

  colour otherSide = ( side == white ) ? black : white;

  struct move_t moves[256];
  int num_moves = b.gen_moves( moves );
  struct move_t best_move;

  int32_t max_score = std::numeric_limits<int32_t>::min();
  for (int i=0; i<num_moves; i++) {
    child = doMove( b, moves[i] );
    score = - negamax( child, depth-1, otherSide );
    if ( score > max_score ) {
      max_score = score;
      best_move = moves[i];
    }
  }
  return best_move;
}


// negamax with alpha-beta pruning
int32_t negamaxAB(board b, int depth, colour side, int32_t alpha, int32_t beta) {
  if ( depth == 0 ) return b.evaluate() * ( ( side == white ) ? 1 : -1 );

  colour otherSide = ( side == white ) ? black : white;

  int32_t score;
  board child;

  struct move_t moves[256];
  int num_moves = b.gen_moves( moves );

//  int32_t max_score = std::numeric_limits<int32_t>::min();
  int32_t max_score = alpha;

  for (int i=0; i<num_moves; i++) {
    child = doMove( b, moves[i] );
    score = - negamaxAB( child, depth-1, otherSide, -beta, -alpha );
    if ( score > max_score ) max_score = score;
    if ( max_score > alpha ) alpha = max_score;
    if ( alpha >= beta ) break;
  }
  return max_score;
}

// overloading to set alpha = -inf, beta = +inf
int32_t negamaxAB(board b, int depth, colour side) {
  int32_t alpha = std::numeric_limits<int32_t>::min();
  int32_t beta  = std::numeric_limits<int32_t>::max();
  return negamaxAB( b, depth, side, alpha, beta );
}

struct move_t search_negamaxAB(board b, int depth, colour side) {
  int32_t score;
  board child;

  colour otherSide = ( side == white ) ? black : white;

  struct move_t moves[256];
  int num_moves = b.gen_moves( moves );
  struct move_t best_move;

  int32_t max_score = std::numeric_limits<int32_t>::min();
  for (int i=0; i<num_moves; i++) {
    child = doMove( b, moves[i] );
    score = - negamaxAB( child, depth-1, otherSide );
    if ( score > max_score ) {
      max_score = score;
      best_move = moves[i];
    }
  }
  return best_move;
}

