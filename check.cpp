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


bool board::is_check(colour side, piece * checkingPiece, int * checkingInd ) {
  // on-the-fly check detection
  int kingpos = log2( pieceBoards[ ( 6 * side ) + 5 ] );
  bitboard _white = whiteSquares();
  bitboard _black = blackSquares();
  colour otherSide = ( side == white ) ? black : white;
  bitboard _check;

  // pawns
  _check = pawnAttackNaive   (kingpos, side) & pieceBoards[(6*otherSide)+0];
  if ( _check ) { *checkingPiece = piece(0); *checkingInd = last_set_bit( _check ); return true; }
  // rooks
  _check = rookTargets   (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+1];
  if ( _check ) { *checkingPiece = piece(0); *checkingInd = last_set_bit( _check ); return true; }
  // knights
  _check = knightTargets   (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+2];
  if ( _check ) { *checkingPiece = piece(0); *checkingInd = last_set_bit( _check ); return true; }
  // bishops
  _check = bishopTargets   (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+3];
  if ( _check ) { *checkingPiece = piece(0); *checkingInd = last_set_bit( _check ); return true; }
  // queens
  _check = queenTargets   (kingpos, _white, _black, side) & pieceBoards[(6*otherSide)+4];
  if ( _check ) { *checkingPiece = piece(0); *checkingInd = last_set_bit( _check ); return true; }

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


bool board::was_lastmove_check( move_t lastmove ) {
    int i,j;
    colourPiece movingPiece;
    int from_ind = lastmove.from_sq();
    int to_ind = lastmove.to_sq();
    bitboard from_square = ( 1ULL << from_ind );
    bitboard to_square   = ( 1ULL <<   to_ind );
    bitboard kingBoard   = pieceBoards[(sideToMove*6)+5];
    int king_ind         = log2( kingBoard );
    
    for ( i=sideToMove*6; i<(1+sideToMove)*6; i++ ) {
        if ( pieceBoards[i] & to_square ) {
            movingPiece = colourPiece(i);
            break;
        }
    }
    
    colour otherSide = ( sideToMove == white ) ? black : white;  // side that just moved
    bitboard _white   = whiteSquares();
    bitboard _black   = blackSquares();
    bitboard blockers = takenSquares();
    
    
    // direct check
    switch ( movingPiece % 6 ) {
        case 0:
            if ( kingBoard & pawnAttacks( to_ind, _white, _black, otherSide ) ) return true;
            break;
        case 5:
            break;
        default:
            if ( kingBoard & pieceTargets( to_ind, _white, _black, movingPiece ) ) return true;
            break;
    }
    
    // discovered check
    bitboard _ray;
    bitboard tmp;
    bitboard attacker;
    // rooks and queens
    for ( i=0; i<8; i+=2 ) {
        if ( kingBoard & rays[i][from_ind] ) {
            j = (i+4)%8;
            _ray = rays[j][king_ind];
            tmp = blockers & _ray;
            if ( tmp ) {
                if ( (j+2)%6 < 4 ) attacker = ( 1ULL << first_set_bit( tmp ) );
                else               attacker = ( 1ULL <<  last_set_bit( tmp ) );
                if ( attacker & pieceBoards[(6*otherSide)+1] ) return true;
                if ( attacker & pieceBoards[(6*otherSide)+5] ) return true;
            }
        }
    }
    // bishops and queens
    for ( i=1; i<8; i+=2 ) {
        if ( kingBoard & rays[i][from_ind] ) {
            j = (i+4)%8;
            _ray = rays[j][king_ind];
            tmp = blockers & _ray;
            if ( tmp ) {
                if ( (j+2)%6 < 4 ) attacker = ( 1ULL << first_set_bit( tmp ) );
                else               attacker = ( 1ULL <<  last_set_bit( tmp ) );
                if ( attacker & pieceBoards[(6*otherSide)+3] ) return true;
                if ( attacker & pieceBoards[(6*otherSide)+5] ) return true;
            }
        }
    }
    
    
    // en passant
    if ( lastmove.is_ep_capture() ) {
        int other_pawn_ind = to_ind + ( ( sideToMove==black ) ? S : N );
        bitboard left_ray  = rays[6][from_ind] & rays[6][other_pawn_ind] & blockers;
        bitboard right_ray = rays[2][from_ind] & rays[2][other_pawn_ind] & blockers;
        bitboard attacker_left  = ( 1ULL << last_set_bit(left_ray) );
        bitboard attacker_right = ( 1ULL << first_set_bit(right_ray) );
        
        if ( ( attacker_left  & pieceBoards[(otherSide*6)+5] ) &&
             ( attacker_right & ( pieceBoards[(sideToMove*6) + 1] |
                                  pieceBoards[(sideToMove*6) + 4] ) ) ) {
            std::cout << "king left, attacker right\n";
            return true;
        }
        if ( ( attacker_right & pieceBoards[(otherSide*6)+5] ) &&
             ( attacker_left  & ( pieceBoards[(sideToMove*6) + 1] |
                                  pieceBoards[(sideToMove*6) + 4] ) ) ) {
            std::cout << "king right, attacker left\n";
            return true;
        }
    }
    // castling rook
    if ( lastmove.is_kingCastle() ) {
        int rook_ind = to_ind - 1;
        if ( kingBoard & rookTargets( rook_ind, _white, _black, otherSide ) ) return true;
    }
    else if ( lastmove.is_queenCastle() ) {
        int rook_ind = to_ind + 1;
        if ( kingBoard & rookTargets( rook_ind, _white, _black, otherSide ) ) return true;
    }
    
    // promotion
    if ( lastmove.is_promotion() ) {
        colourPiece prom_piece = colourPiece( (6*otherSide) + lastmove.which_promotion() );
        if ( kingBoard & pieceTargets( to_ind, _white, _black, prom_piece) ) return true;
    }
    
    return false;
}