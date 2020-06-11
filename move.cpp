#include "board.h"
#include "move.h"
#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include "twiddle.h"
#include <bitset>
#include <string>

// initialise constants declared in move.h
const int N   =  +8;
const int NE  =  +9;
const int E   =  +1;
const int SE  =  -7;
const int S   =  -8;
const int SW  =  -9;
const int W   =  -1;
const int NW  =  +7;
const int NNE =  17;
const int ENE =  10;
const int ESE =  -6;
const int SSE = -15;
const int SSW = -17;
const int WSW = -10;
const int WNW =   6;
const int NNW =  15;

// utility
void itos (int i, char * dest) {
  *dest     = 'a' + (i%8);
  *(dest+1) = '1' + (i/8);
}

std::string itos (int i) {
    std::string ret = "  ";
    ret[0] = 'a' + (i%8);
    ret[1] = '1' + (i/8);
    return ret;
}

void itos(int i, std::ostream& cout) {
    cout << char('a'+(i%8)) << char('1'+(i/8));
}

int stoi(char * s) {
  char file = s[0];
  char rank = s[1];

  return int(rank-'1')*8 + int(file-'a');
}

int stoi( std::string s ) {
    return int(s[0]-'1')*8 + int(s[1]-'a');
}

// move_t constructors
// default constructor
move_t::move_t() {}

// parameterised constructor
move_t::move_t(uint8_t from, uint8_t to, bool promotion,
               bool capture, bool spec1, bool spec0) {
  data = (from) | (to<<6) | (promotion<<15) | (capture<<12) | (spec1<<14) | (spec0<<13);
}

// bits 0-5:  from square
// bits 6-11: to square
// bits 12:   capture
// bit 13:    special 0
// bit 14:    special 1
// bit 15:    promotion

uint16_t move_t::from_sq()  { return (data & 63); }
uint16_t move_t::to_sq()    { return (data >> 6) & 63; }
uint16_t move_t::flags()    { return (data >> 12); }
bool move_t::is_quiet()     { return !flags(); }
bool move_t::is_promotion() { return (flags() & 8); }
bool move_t::is_capture()   { return (flags() & 1); }
bool move_t::is_ep_capture(){ return (flags() == 3); }
//bool move_t::special1()     { return (flags() & 4); }
//bool move_t::special0()     { return (flags() & 2); }
bool move_t::is_doublePP()  { return (flags() == 2); }
bool move_t::is_kingCastle(){ return ( flags() == 4 ); }
bool move_t::is_queenCastle(){ return ( flags() == 6 ); }
bool move_t::is_castle()    { return ( is_kingCastle() | is_queenCastle() ); }
uint16_t move_t::give()     { return data;}


void print_move(struct move_t move, std::ostream& cout) {
  //char   to_c[2];
  //char from_c[2];
  int fromSq = move.from_sq();
  int toSq   = move.to_sq();
  
  //itos( fromSq, from_c );
  //itos(   toSq,   to_c );
  
  itos( fromSq, cout );
  cout << " to ";
  itos( toSq, cout );

  //cout << from_c[0] << from_c[1] << " to " << to_c[0] << to_c[1];
}

// calculate ray tables for move generation
// bitboards indexed by direction then square
//->     64         *      8       *     64   =   32768 bits = 4K

bitboard rays[8][64] = {0};

void init_rays() {
  int sq;
  int dir;
  bitboard ray_bb,tmp;

  for (dir=0;dir<8;dir++) {
    for (sq=0;sq<64;sq++) {
      ray_bb = 0;
      tmp = (1ULL << sq);
      while (tmp) {
        tmp = oneGeneral8(tmp,dir);
        ray_bb |= tmp;
      }
      rays[dir][sq] = ray_bb;
    }
  }
}


// naive move generation

// pawns
bitboard pawnPushNaive (int sq, bitboard blockers, colour movingColour) {
  bitboard pB = ( 1ULL << sq );
  if ( movingColour == white ) return ( ( oneN(pB) | twoN(pB&rankTwo) ) & ( ~ blockers ) );
  else                         return ( ( oneS(pB) | twoS(pB&rankSeven) ) & ( ~ blockers ) );
}
bitboard pawnAttackNaive (int sq, colour movingColour) {
  bitboard pB = ( 1ULL << sq );
  if ( movingColour == white ) return ( ( oneNE(pB) | oneNW(pB) ) );
  else                         return ( ( oneSE(pB) | oneSW(pB) ) );
}
bitboard pawnAttacks (int sq, bitboard _white, bitboard _black, colour movingColour) {
  if ( movingColour == white ) return ( pawnAttackNaive(sq,movingColour) & _black );
  else                         return ( pawnAttackNaive(sq,movingColour) & _white );
}
bitboard pawnTargets (int sq, bitboard _white, bitboard _black, colour movingColour) {
  bitboard pushes  = pawnPushNaive(sq,_white|_black,movingColour);
  bitboard attacks = pawnAttackNaive(sq,movingColour);
  if ( movingColour == white ) return ( ( attacks & _black ) | pushes );
  else                         return ( ( attacks & _white ) | pushes );
}


// knights
bitboard knightPushNaive (int sq) {
  bitboard n = ( 1ULL << sq );
  return ( oneNNE(n) | oneENE(n) | oneESE(n) | oneSSE(n)
         | oneSSW(n) | oneWSW(n) | oneWNW(n) | oneNNW(n) );
}
bitboard knightTargets (int sq, bitboard _white, bitboard _black, colour movingColour) {
  bitboard moves = knightPushNaive( sq );
  if ( movingColour == white) return ( moves & ( ~ _white ) );
  else return ( moves & ( ~ _black ) );
}

// kings
bitboard kingPushNaive (int sq) {
  bitboard k = ( 1ULL << sq );
  return ( oneN(k) | oneE(k) | oneS(k) | oneW(k) | oneNE(k) | oneSE(k) | oneSW(k) | oneNW(k) );
}
bitboard kingTargets (int sq, bitboard _white, bitboard _black, colour movingColour) {
  bitboard moves = kingPushNaive( sq );
  if ( movingColour == white) return ( moves & ( ~ _white ) );
  else return ( moves & ( ~ _black ) );
}

// sliding piece generation
// bishops
bitboard bishopPushNaive(int sq, bitboard blockers) {
  bitboard tmp;
  bitboard diagRays = 0ULL;
  
  // NE
  diagRays |= rays[dirNE][sq];
  tmp = blockers & rays[dirNE][sq];
  if (tmp) {
    diagRays &= ~rays[dirNE][first_set_bit(tmp)];
  }
  //print_bb(diagRays,'1');
  
  // SE
  diagRays |= rays[dirSE][sq];
  tmp = blockers & rays[dirSE][sq];
  if (tmp) {
    diagRays &= ~rays[dirSE][last_set_bit(tmp)];
  }
  //print_bb(diagRays,'1');
  
  // SW
  diagRays |= rays[dirSW][sq];
  tmp = blockers & rays[dirSW][sq];
  if (tmp) {
    diagRays &= ~rays[dirSW][last_set_bit(tmp)];
  }
  //print_bb(diagRays,'1');
  
  // NW
  diagRays |= rays[dirNW][sq];
  tmp = blockers & rays[dirNW][sq];
  if (tmp) {
    diagRays &= ~rays[dirNW][first_set_bit(tmp)];
  }
  //print_bb(diagRays,'1');
  
  return diagRays;
}

bitboard bishopTargets(int sq, bitboard _white, bitboard _black, colour movingColour) {
  bitboard moves = bishopPushNaive(sq,  _white | _black );
  if ( movingColour == white) return ( moves & ( ~ _white ) );
  else return ( moves & ( ~ _black ) );
}

// rooks
bitboard rookPushNaive(int sq, bitboard blockers) {
  bitboard tmp;
  bitboard compRays = 0ULL;
  
  // N
  compRays |= rays[dirN][sq];  
  tmp = blockers & rays[dirN][sq];
  if (tmp) {
    compRays &= ~rays[dirN][first_set_bit(tmp)];
  }
  //print_bb(compRays,'1');
  
  // E
  compRays |= rays[dirE][sq];
  tmp = blockers & rays[dirE][sq];
  if (tmp) {
    compRays &= ~rays[dirE][first_set_bit(tmp)];
  }
  //print_bb(compRays,'1');
  
  // S
  compRays |= rays[dirS][sq];
  tmp = blockers & rays[dirS][sq];
  if (tmp) {
    compRays &= ~rays[dirS][last_set_bit(tmp)];
  }
  //print_bb(compRays,'1');
  
  // W
  compRays |= rays[dirW][sq];
  tmp = blockers & rays[dirW][sq];
  if (tmp) {
    compRays &= ~rays[dirW][last_set_bit(tmp)];
  }
  //print_bb(compRays,'1');
  
  return compRays;
}

bitboard rookTargets(int sq, bitboard _white, bitboard _black, colour movingColour) {
  bitboard moves = rookPushNaive(sq, _white | _black );
  if ( movingColour == white) return ( moves & ( ~ _white ) );
  else return ( moves & ( ~ _black ) );
}

// queens
bitboard queenPushNaive(int sq, bitboard blockers) {
  return bishopPushNaive(sq,blockers) | rookPushNaive(sq,blockers);
}

bitboard queenTargets(int sq, bitboard _white, bitboard _black, colour movingColour) {
  bitboard moves = queenPushNaive(sq,  _white | _black );
  if ( movingColour == white) return ( moves & ( ~ _white ) );
  else return ( moves & ( ~ _black ) );
}


// piece targets

bitboard pieceTargets(int sq, bitboard _white, bitboard _black, colourPiece cP) {
  colour col = colour(cP/6);
  int piece  = cP%6;

  switch (piece) {
    case 0:
//      cout << "pawn\n";
      return pawnTargets(sq,_white,_black,col);
      break;
    case 1:
//      cout << "rook\n";
      return rookTargets(sq,_white,_black,col);
      break;
    case 2:
//      cout << "knight\n";
      return knightTargets(sq,_white,_black,col);
      break;
    case 3:
//      cout << "bishop\n";
      return bishopTargets(sq,_white,_black,col);
      break;
    case 4:
//      cout << "queen\n";
      return queenTargets(sq,_white,_black,col);
      break;
    case 5:
//      cout << "king\n";
      return kingTargets(sq,_white,_black,col);
      break;
    default:
      return bitboard();
  }
}


// generate pseudo-legal moves (without checking for checks)
// returns the number of moves
int board::gen_moves ( move_t * moves) {
  int piece;
  int from_sq;
  int to_sq;
  int count=0;
  bitboard targets;
  bitboard _white = whiteSquares();
  bitboard _black = blackSquares();

  for (piece=sideToMove*6;piece<(1+sideToMove)*6;piece++) {
    for ( from_sq=0; from_sq<64; from_sq++) {
      if ( pieceBoards[piece] & ( 1ULL << from_sq ) ) {
//        cout << "Piece " << piece << " at square " << from_sq << ":\n";
        targets = pieceTargets(from_sq,_white,_black,colourPiece(piece));
//        print_bb(targets,'x');

        for ( to_sq=0; to_sq<64; to_sq++ ) {
          if (targets & 1ULL) {
            // found a move!
//            cout << "Found a move!\n";
            if ( ( ( 1ULL << to_sq ) & _black ) | ( ( 1ULL << to_sq ) & _white ) ) {
              // capture
              *moves = move_t(from_sq,to_sq,
                              bool( (piece%6==0) && ((rankOne|rankEight)&(1ULL<<to_sq)) ),
                              1,0,0);
              moves++;
              count++;
            }
            else {
              // non-capture
              if ( ( piece % 6 == 0 ) & ( abs(from_sq-to_sq) == 16 ) ) {
                // double pawn push
                if ( !( ( 1ULL << ((from_sq+to_sq)/2) ) & ( _black | _white ) ) ) {
                  *moves = move_t(from_sq,to_sq,0,0,0,1);
                  moves++;
                  count++;
                }
                else {
//                  cout << "Falsely identified as blocked double pawn push: ";
//                  cout << from_sq << " " << to_sq << " " << ((from_sq+to_sq)/2) << endl;
                }
              }
              else {
                // quiet move
                *moves = move_t(from_sq,to_sq,
                                bool( (piece%6==0) && ((rankOne|rankEight)&(1ULL<<to_sq)) ),
                                0,0,0);
                moves++;
                count++;
              }
            }
//            moves++;
//            count++;
          }
          targets >>= 1;
        }
      }
    }
  }
  // fringe cases

  // ep capture
  if ( lastMoveDoublePawnPush ) {
    // if white to move: potential squares are ( 40 + dPPFile +-1 )
    // if black to move: potential squares are ( 32 + dPPFile +-1 )
    int leftSquare    = 40 - ( 8 * sideToMove ) + dPPFile - 1;
    int rightSquare   = 40 - ( 8 * sideToMove ) + dPPFile + 1;
    int captureSquare = 32 + ( 8 * sideToMove ) + dPPFile;
    if ( ( 1ULL << leftSquare ) & ( pieceBoards[ 6 * sideToMove ] ) ) {
      *moves = move_t(leftSquare,captureSquare,0,1,0,1);
      moves++;
      count++;
    }
    if ( ( 1ULL << rightSquare ) & ( pieceBoards[ 6 * sideToMove ] ) ) {
      *moves = move_t(rightSquare,captureSquare,0,1,0,1);
      moves++;
      count++;
    }
  }

  // castling
  if ( sideToMove == white && ! is_check(white) ) {
    if ( castleWhiteKingSide &&  ( ! ( ( _white | _black ) & 0x0000000000000060 ) ) ) {
      // can't castle through check
      board copy = *this;
      copy.set_piece(whiteKing,5);
      copy.clear_square(4);
      if ( !copy.is_check(white) ) {
        *moves = move_t(4,6,0,0,1,0);
        moves++;
        count++;
      }
      copy.print_board(std::cout);
    }
    if ( castleWhiteQueenSide && ( ! ( ( _white | _black ) & 0x000000000000000e ) ) ) {
      board copy = *this;
      copy.set_piece(whiteKing,3);
      copy.clear_square(4);
      if ( !copy.is_check(white) ) {
        *moves = move_t(4,2,0,0,1,1);
        moves++;
        count++;
      }
      copy.print_board(std::cout);
    }
  }
  else if ( sideToMove == black && ! is_check(black) ){
    if ( castleBlackKingSide &&  ( ! ( ( _white | _black ) & 0x6000000000000000 ) ) ) {
      board copy = *this;
      copy.set_piece(whiteKing,61);
      copy.clear_square(60);
      if ( !copy.is_check(black) ) {
        *moves = move_t(60,62,0,0,1,0);
        moves++;
        count++;
      }
      copy.print_board(std::cout);
    }
    if ( castleBlackQueenSide && ( ! ( ( _white | _black ) & 0x0e00000000000000 ) ) ) {
      board copy = *this;
      copy.set_piece(whiteKing,59);
      copy.clear_square(60);
      if ( !copy.is_check(black) ) {
        *moves = move_t(60,58,0,0,1,1);
        moves++;
        count++;
      }
      copy.print_board(std::cout);
    }
  }

  return count;
}



// from to prom cap 1 0

