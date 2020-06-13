#include "board.h"
#include "move.h"
#include "action.h"
#include <math.h>
#include <iostream>


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

  for (int i=0; i<num_moves; i++) {
    child = doMove(cpy,moves[i]);
    if ( ! child.is_check(side) ) return false;
  }
  
//  for ( int i=0; i<num_moves; i++ ) {
//    print_move( moves[i] );
//    std::cout << ": is a check\n";
//  }
  
//  move_t c8b8 = moves[ num_moves - 2 ];
//  print_move( c8b8 );
//  std::cout << std::endl;
//  std::cout << c8b8.give();
//  std::cout << std::endl;
//  child = doMove( cpy, c8b8 );
//  
//  child.print_board();
//  
//  std::cout << bool( child == cpy ) << std::endl;
//  
//  bitboard pb[12];
//  
//  child.getBitboards( pb );
//  
//  std::cout << pb[11] << std::endl;
  
  return true;
}