#include "action.h"
#include "twiddle.h"
#include "eval.h"
#include <cstdint>

board doMove(board startBoard, move_t move) {
  int i;
  uint16_t fromSquare = move.from_sq();
  uint16_t toSquare   = move.to_sq();
  bool castling[4];
  bool ep;
  int dpp;
  int clk;
  int full_clk;
  int32_t value = startBoard.getValue();
  colour movingColour;
  startBoard.getSide( & movingColour );
  int plus_minus = ( movingColour == white ) ? 1 : -1;
  colour otherColour = ( movingColour == white ) ? black : white;
  colourPiece movingPiece;

  bitboard startingPos[12];

  startBoard.getBitboards(startingPos);

  for (i=0;i<12;i++) {
    if ( is_bit_set(startingPos[i], fromSquare) ) {
      movingPiece = colourPiece(i);
      startingPos[i] = (startingPos[i] & ~( 1ULL << fromSquare) ) | ( 1ULL << toSquare );
      value -= pieceSquareTables[i][fromSquare];
      value += pieceSquareTables[i][toSquare];
      break;
    }
  }


  if ( move.is_capture() ) {
    if ( ! move.is_ep_capture() ) {
      for (i=(1-movingColour)*6;i<12;i++) {
        if (is_bit_set(startingPos[i], toSquare) ) {
          startingPos[i] = (startingPos[i] & ~( 1ULL << toSquare) );
          value -= pieceSquareTables[i][toSquare];
          value -= pieceValues[i];
          break;
        }
      }
    }

    else {
      if ( movingColour == white ) {
        for (i=(1-movingColour)*6;i<12;i++) {
          if (is_bit_set(startingPos[i], toSquare+S) ) {
            startingPos[i] = (startingPos[i] & ~( 1ULL << (toSquare+S) ) );
            value -= pieceSquareTables[i][toSquare+S];
            value -= pieceValues[i];
            break;
          }
        }
      }
      else {
        for (i=(1-movingColour)*6;i<12;i++) {
          if (is_bit_set(startingPos[i], toSquare+N) ) {
            startingPos[i] = (startingPos[i] & ~( 1ULL << (toSquare+N) ) );
            value -= pieceSquareTables[i][toSquare+N];
            value -= pieceValues[i];
            break;
          }
        }
      }
    }
  }

  if ( move.is_kingCastle() ) {
    startingPos[1+(6*movingColour)] = (startingPos[1+(6*movingColour)] & ~( 1ULL << (fromSquare+3)) )
                                  | ( 1ULL << (toSquare-1) );
    value -= pieceSquareTables[1+(6*movingColour)][fromSquare+3];
    value += pieceSquareTables[1+(6*movingColour)][toSquare-1];
  }
  else if ( move.is_queenCastle() ) {
    startingPos[1+(6*movingColour)] = (startingPos[1+(6*movingColour)] & ~( 1ULL << (fromSquare-4)) )
                                  | ( 1ULL << (toSquare+1) );
    value -= pieceSquareTables[1+(6*movingColour)][fromSquare-4];
    value += pieceSquareTables[1+(6*movingColour)][toSquare+1];
  }
  
  // promotion
  if ( move.is_promotion() ) {
    startingPos[6*movingColour] &= ( ~ ( 1ULL << toSquare ) );
    
    switch ( move.flags() & 6 ) {
        case 0:
            startingPos[(6*movingColour)+4] |= ( 1ULL << toSquare );
            break;
        case 2:
            startingPos[(6*movingColour)+1] |= ( 1ULL << toSquare );
            break;
        case 4:
            startingPos[(6*movingColour)+3] |= ( 1ULL << toSquare );
            break;
        case 6:
            startingPos[(6*movingColour)+2] |= ( 1ULL << toSquare );
            break;
    }
  }

  startBoard.getCastlingRights(castling);
  startBoard.getClock(&clk);
  startBoard.getFullClock(&full_clk);

  // check for double pawn push
  if (move.is_doublePP()) {
    ep=true;
    dpp= fromSquare%8;
  }
  else ep=false;

  // check for changes to castling rights
  if (movingPiece % 6 == 1) {
    switch (fromSquare) {
      case 0:
        castling[1] = false;
        break;
      case 7:
        castling[0] = false;
        break;
      case 56:
        castling[3] = false;
        break;
      case 63:
        castling[2] = false;
        break;
    }
  }
  else if (movingPiece % 6 == 5) {
    switch (movingColour) {
      case white:
        castling[0] = false;
        castling[1] = false;
        break;
      case black:
        castling[2] = false;
        castling[3] = false;
        break;
    }
  }
  
  switch ( toSquare ) {
    case 0:
      castling[1] = false;
      break;
    case 7:
      castling[0] = false;
      break;
    case 56:
      castling[3] = false;
      break;
    case 63:
      castling[2] = false;
      break;
  
  }

  // increment halfMoveClock
  if ( move.is_capture() | ( movingPiece % 6 == 0 ) ) clk = 0;
  else clk ++;

  // increment fullMoveClock
  if ( movingColour == black ) full_clk++;

  board endBoard (startingPos, castling, ep, dpp, clk, full_clk, otherColour, value );

  return endBoard;
}
