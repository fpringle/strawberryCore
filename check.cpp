#include "board.h"
#include "move.h"
#include "action.h"
#include <math.h>


bool board::is_check(colour side) {
  // on-the-fly check detection
  int kingpos = log2( pieceBoards[ ( 6 * side ) + 5 ] );
  bitboard _white = whiteSquares();
  bitboard _black = blackSquares();
  colour otherSide = ( side == white ) ? black : white;

  // pawns
  if ( pawnAttackNaive   (kingpos, side) & pieceBoards[(6*otherSide)+0] ) return true;
  // rooks
  if ( rookTargets   (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+1] ) return true;
  // knights
  if ( knightTargets (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+2] ) return true;
  // bishops
  if ( bishopTargets (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+3] ) return true;
  // queens
  if ( queenTargets  (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+4] ) return true;

  return false;
}


bool board::is_checkmate(colour side) {
  if ( ! is_check(side) ) return false;
  board child;
  board cpy = * this;
  cpy.set_side(side);
  struct move_t moves[256];
  int num_moves = cpy.gen_moves( moves );

//  for (int i=0; i<num_moves; i++) {
//    child = doMove(cpy,moves[i]);
//    if ( ! child.is_check(side) ) return false;
//  }
  return ( num_moves == 0 );
}
