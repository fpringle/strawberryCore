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

int stoi( std::string s, std::string s2 ) {
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
piece move_t::which_promotion() {
    switch ( flags() & 6 ) {
        case 0:
            return queen;
            break;
        case 2:
            return rook;
            break;
        case 4:
            return bishop;
            break;
        case 6:
            return knight;
            break;
        default:
            return queen;
            break;
    }
}


void print_move(struct move_t move, std::ostream& cout) {
  //char   to_c[2];
  //char from_c[2];
  int fromSq = move.from_sq();
  int toSq   = move.to_sq();
  
  //itos( fromSq, from_c );
  //itos(   toSq,   to_c );
  
  itos( fromSq, cout );
  // cout << " to ";
  itos( toSq, cout );
  
  if ( move.is_promotion() ) {
    switch ( move.flags() & 6 ) {
        case 0:
            cout << "Q";
            break;
        case 2:
            cout << "R";
            break;
        case 4:
            cout << "B";
            break;
        case 6:
            cout << "N";
            break;
    }
  }

  //cout << from_c[0] << from_c[1] << " to " << to_c[0] << to_c[1];
}

move_t stom( move_t* moves, int n_moves, std::string s ) {
    std::string from = s.substr( 0, 2 );
    std::string to = s.substr( 2, 2 );
    int from_ind = stoi( from, " " );
    int to_ind   = stoi( to, " " );
    
    for ( int i=0; i<n_moves; i++ ) {
        if ( ( moves[i].from_sq() == from_ind ) &&
             (  moves[i].to_sq()  ==  to_ind  ) ) {
                return moves[i];
        }
    }
    
    return move_t ( 0, 0, 0, 0, 0, 0 );
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
      while ( tmp ) {
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
  if ( tmp ) {
    diagRays &= ~rays[dirNE][first_set_bit( tmp )];
  }
  //print_bb(diagRays,'1');
  
  // SE
  diagRays |= rays[dirSE][sq];
  tmp = blockers & rays[dirSE][sq];
  if ( tmp ) {
    diagRays &= ~rays[dirSE][last_set_bit( tmp )];
  }
  //print_bb(diagRays,'1');
  
  // SW
  diagRays |= rays[dirSW][sq];
  tmp = blockers & rays[dirSW][sq];
  if ( tmp ) {
    diagRays &= ~rays[dirSW][last_set_bit( tmp )];
  }
  //print_bb(diagRays,'1');
  
  // NW
  diagRays |= rays[dirNW][sq];
  tmp = blockers & rays[dirNW][sq];
  if ( tmp ) {
    diagRays &= ~rays[dirNW][first_set_bit( tmp )];
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
  if ( tmp ) {
    compRays &= ~rays[dirN][first_set_bit( tmp )];
  }
  //print_bb(compRays,'1');
  
  // E
  compRays |= rays[dirE][sq];
  tmp = blockers & rays[dirE][sq];
  if ( tmp ) {
    compRays &= ~rays[dirE][first_set_bit( tmp )];
  }
  //print_bb(compRays,'1');
  
  // S
  compRays |= rays[dirS][sq];
  tmp = blockers & rays[dirS][sq];
  if ( tmp ) {
    compRays &= ~rays[dirS][last_set_bit( tmp )];
  }
  //print_bb(compRays,'1');
  
  // W
  compRays |= rays[dirW][sq];
  tmp = blockers & rays[dirW][sq];
  if ( tmp ) {
    compRays &= ~rays[dirW][last_set_bit( tmp )];
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


bool board::add_moves( move_t ** dest, move_t move, bool check_legal ) {
    if ( check_legal ) {
        if ( is_legal( move ) ) {
            **dest = move;
            (*dest)++;
            return 1;
        }
        else {
            return 0;
        }
    }
    else{
        **dest = move;
        (*dest)++;
        return 1;
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
              if ( piece%6==0 && ((rankOne|rankEight)&(1ULL<<to_sq)) ) {
                  // promotion
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,0,0), false );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,0,1), false );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,1,0), false );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,1,1), false );
              }
              else {
                  count += add_moves( &moves, move_t(from_sq,to_sq,0,1,0,0), false );
              }
            }
            else {
              // non-capture
              if ( ( piece % 6 == 0 ) & ( abs(from_sq-to_sq) == 16 ) ) {
                // double pawn push
                if ( !( ( 1ULL << ((from_sq+to_sq)/2) ) & ( _black | _white ) ) ) {
                  count += add_moves( &moves, move_t(from_sq,to_sq,0,0,0,1), false );
                }
                else {
//                  cout << "Falsely identified as blocked double pawn push: ";
//                  cout << from_sq << " " << to_sq << " " << ((from_sq+to_sq)/2) << endl;
                }
              }
              else {
                // quiet move
                if ( piece%6==0 && ((rankOne|rankEight)&(1ULL<<to_sq)) ) {
                  // promotion
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,0,0), false );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,0,0,1), false );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,0,1,0), false );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,0,1,1), false );
                }
                else {
                    count += add_moves( &moves, move_t(from_sq,to_sq,0,0,0,0), false );
                }
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
    // if white to move: potential squares are ( 32 + dPPFile +-1 )
    // if black to move: potential squares are ( 24 + dPPFile +-1 )
    int oppPawnSquare = 32 - (  8 * sideToMove ) + dPPFile;
    bitboard oppPawn = ( 1ULL << oppPawnSquare );
    bitboard left    = oneW( oppPawn );
    bitboard right   = oneE( oppPawn );
    int captureSquare = 40 - ( 24 * sideToMove ) + dPPFile;
    if ( left & ( pieceBoards[ 6 * sideToMove ] ) ) {
      int leftSquare = last_set_bit( left );
      count += add_moves( &moves, move_t(leftSquare,captureSquare,0,1,0,1), false );
    }
    if ( right & ( pieceBoards[ 6 * sideToMove ] ) ) {
      int rightSquare = last_set_bit( right );
      count += add_moves( &moves, move_t(rightSquare,captureSquare,0,1,0,1), false );
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
        count += add_moves( &moves, move_t(4,6,0,0,1,0), false );
      }
      //copy.print_board(std::cout);
    }
    if ( castleWhiteQueenSide && ( ! ( ( _white | _black ) & 0x000000000000000e ) ) ) {
      board copy = *this;
      copy.set_piece(whiteKing,3);
      copy.clear_square(4);
      if ( !copy.is_check(white) ) {
        count += add_moves( &moves, move_t(4,2,0,0,1,1), false );
      }
      //copy.print_board(std::cout);
    }
  }
  else if ( sideToMove == black && ! is_check(black) ){
    if ( castleBlackKingSide &&  ( ! ( ( _white | _black ) & 0x6000000000000000 ) ) ) {
      board copy = *this;
      copy.set_piece(blackKing,61);
      copy.clear_square(60);
      if ( !copy.is_check(black) ) {
        count += add_moves( &moves, move_t(60,62,0,0,1,0), false );
      }
      //copy.print_board(std::cout);
    }
    if ( castleBlackQueenSide && ( ! ( ( _white | _black ) & 0x0e00000000000000 ) ) ) {
      board copy = *this;
      copy.set_piece(blackKing,59);
      copy.clear_square(60);
      if ( !copy.is_check(black) ) {
        count += add_moves( &moves, move_t(60,58,0,0,1,1), false );
      }
      //copy.print_board(std::cout);
    }
  }

  return count;
}


// generate pseudo-legal moves (without checking for checks)
// returns the number of moves
int board::gen_legal_moves ( move_t * moves) {
  int _piece;
  int from_sq;
  int to_sq;
  int count=0;
  bitboard targets;
  bitboard _white = whiteSquares();
  bitboard _black = blackSquares();
  bitboard _other = ( ( sideToMove == white ) ? _black : _white );
  colour otherSide = ( ( sideToMove == white ) ? black : white );
  bitboard defender;
  
  // if we're in check we can limit the search
  piece checkingPiece;
  int checkingInd;
  if ( is_check( sideToMove, &checkingPiece, &checkingInd ) ) {
      std::cout << "we're in check\n";
      // king moves out of check
      int king_ind = last_set_bit( pieceBoards[(sideToMove*6)+5] );
      bitboard targets = pieceTargets( king_ind, _white, _black, colourPiece((sideToMove*6)+5) );
      int to_ind = first_set_bit( targets );
      targets >>= to_ind;
      
      while ( targets ) {
          if ( targets & ( 1ULL ) ) {
              if ( _other & ( 1ULL << to_ind ) ) {
                  count += add_moves( &moves, move_t(king_ind,to_ind,0,1,0,0), true );
              }
              else {
                  count += add_moves( &moves, move_t(king_ind,to_ind,0,0,0,0), true );
              }
          }
          to_ind++;
          targets >>= 1;
      }
      
      // take the checking piece
      from_sq = 0;
      std::cout << "Checking index: " << checkingInd << std::endl;
      for ( int i=(6*sideToMove); i<(6*sideToMove)+6; i++ ) {
          defender = pieceTargets( checkingInd, _white, _black, colourPiece((6*otherSide)+i) ) & pieceBoards[i];
          while ( defender ) {
              if ( defender & 1ULL ) {
                  count += add_moves( &moves, move_t( from_sq, checkingInd, 0, 1, 0, 0 ), true );
              }
              defender >>= 1;
              from_sq++;
          }
      }
      bitboard left  = oneW(1ULL << checkingInd);
      bitboard right = oneE(1ULL << checkingInd);
      if ( lastMoveDoublePawnPush && ( checkingPiece==0 ) ) {
          to_ind = checkingInd + ( ( sideToMove == white ) ? N : S );
          if ( pieceBoards[sideToMove*6] & right ) count += add_moves( &moves, move_t( checkingInd+1, to_ind, 0, 1, 0, 1 ), true );
          if ( pieceBoards[sideToMove*6] & left )  count += add_moves( &moves, move_t( checkingInd-1, to_ind, 0, 1, 0, 1 ), true );
      }
      
      // moving into the way
      
      
      return count;
  }

  for (_piece=sideToMove*6;_piece<(1+sideToMove)*6;_piece++) {
    for ( from_sq=0; from_sq<64; from_sq++) {
      if ( pieceBoards[_piece] & ( 1ULL << from_sq ) ) {
//        cout << "Piece " << _piece << " at square " << from_sq << ":\n";
        targets = pieceTargets(from_sq,_white,_black,colourPiece(_piece));
//        print_bb(targets,'x');

        for ( to_sq=0; to_sq<64; to_sq++ ) {
          if (targets & 1ULL) {
            // found a move!
//            cout << "Found a move!\n";
            if ( ( ( 1ULL << to_sq ) & _black ) | ( ( 1ULL << to_sq ) & _white ) ) {
              // capture
              if ( _piece%6==0 && ((rankOne|rankEight)&(1ULL<<to_sq)) ) {
                  // promotion
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,0,0), true );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,0,1), true );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,1,0), true );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,1,1), true );
              }
              else {
                  count += add_moves( &moves, move_t(from_sq,to_sq,0,1,0,0), true );
              }
            }
            else {
              // non-capture
              if ( ( _piece % 6 == 0 ) & ( abs(from_sq-to_sq) == 16 ) ) {
                // double pawn push
                if ( !( ( 1ULL << ((from_sq+to_sq)/2) ) & ( _black | _white ) ) ) {
                  count += add_moves( &moves, move_t(from_sq,to_sq,0,0,0,1), true );
                }
                else {
//                  cout << "Falsely identified as blocked double pawn push: ";
//                  cout << from_sq << " " << to_sq << " " << ((from_sq+to_sq)/2) << endl;
                }
              }
              else {
                // quiet move
                if ( _piece%6==0 && ((rankOne|rankEight)&(1ULL<<to_sq)) ) {
                  // promotion
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,1,0,0), true );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,0,0,1), true );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,0,1,0), true );
                  count += add_moves( &moves, move_t(from_sq,to_sq,1,0,1,1), true );
                }
                else {
                    count += add_moves( &moves, move_t(from_sq,to_sq,0,0,0,0), true );
                }
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
    // if white to move: potential squares are ( 32 + dPPFile +-1 )
    // if black to move: potential squares are ( 24 + dPPFile +-1 )
    int oppPawnSquare = 32 - (  8 * sideToMove ) + dPPFile;
    bitboard oppPawn = ( 1ULL << oppPawnSquare );
    bitboard left    = oneW( oppPawn );
    bitboard right   = oneE( oppPawn );
    int captureSquare = 40 - ( 24 * sideToMove ) + dPPFile;
    if ( left & ( pieceBoards[ 6 * sideToMove ] ) ) {
      int leftSquare = last_set_bit( left );
      count += add_moves( &moves, move_t(leftSquare,captureSquare,0,1,0,1), true );
    }
    if ( right & ( pieceBoards[ 6 * sideToMove ] ) ) {
      int rightSquare = last_set_bit( right );
      count += add_moves( &moves, move_t(rightSquare,captureSquare,0,1,0,1), true );
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
        count += add_moves( &moves, move_t(4,6,0,0,1,0), true );
      }
      //copy.print_board(std::cout);
    }
    if ( castleWhiteQueenSide && ( ! ( ( _white | _black ) & 0x000000000000000e ) ) ) {
      board copy = *this;
      copy.set_piece(whiteKing,3);
      copy.clear_square(4);
      if ( !copy.is_check(white) ) {
        count += add_moves( &moves, move_t(4,2,0,0,1,1), true );
      }
      //copy.print_board(std::cout);
    }
  }
  else if ( sideToMove == black && ! is_check(black) ){
    if ( castleBlackKingSide &&  ( ! ( ( _white | _black ) & 0x6000000000000000 ) ) ) {
      board copy = *this;
      copy.set_piece(blackKing,61);
      copy.clear_square(60);
      if ( !copy.is_check(black) ) {
        count += add_moves( &moves, move_t(60,62,0,0,1,0), true );
      }
      //copy.print_board(std::cout);
    }
    if ( castleBlackQueenSide && ( ! ( ( _white | _black ) & 0x0e00000000000000 ) ) ) {
      board copy = *this;
      copy.set_piece(blackKing,59);
      copy.clear_square(60);
      if ( !copy.is_check(black) ) {
        count += add_moves( &moves, move_t(60,58,0,0,1,1), true );
      }
      //copy.print_board(std::cout);
    }
  }

  return count;
}

bool board::is_legal( struct move_t move ) {
    colourPiece movingPiece;
    int from_ind = move.from_sq();
    int to_ind = move.to_sq();
    bitboard from_square = ( 1ULL << from_ind );
    for ( int i=sideToMove*6; i<(1+sideToMove)*6; i++ ) {
        if ( pieceBoards[i] & from_square ) {
            movingPiece = colourPiece(i);
            break;
        }
    }
    
    colour otherSide = ( sideToMove == white ) ? black : white;
    bitboard _white = whiteSquares();
    bitboard _black = blackSquares();
    
    // king can't move into check
    if ( movingPiece%6 == 5 ) {
        if ( pawnAttackNaive( to_ind, sideToMove ) & pieceBoards[6*otherSide] ) return false;
        if ( rookTargets( to_ind, _white, _black, sideToMove ) & pieceBoards[(6*otherSide)+1] ) return false;
        if ( knightTargets( to_ind, _white, _black, sideToMove ) & pieceBoards[(6*otherSide)+2] ) return false;
        if ( bishopTargets( to_ind, _white, _black, sideToMove ) & pieceBoards[(6*otherSide)+3] ) return false;
        if ( queenTargets( to_ind, _white, _black, sideToMove ) & pieceBoards[(6*otherSide)+4] ) return false;
        if ( kingTargets( to_ind, _white, _black, sideToMove ) & pieceBoards[(6*otherSide)+5] ) return false;
        return true;
    }
    
    // can't move pinned pieces
    bitboard _ray;
    bitboard blockers = takenSquares();
    bitboard tmp;
    bitboard attacker;
    
    for ( int i=0; i<8; i++ ) {
        if ( rays[i][from_ind] & pieceBoards[(6*sideToMove)+5] ) {
            int j = (i+4)%8;
            if ( i%2 ) {
                // bishops and queens
                _ray = rays[j][from_ind];
                tmp = blockers & _ray;
                if ( tmp ) {
                    if ( (j+2)%6 < 4 ) attacker = ( 1ULL << first_set_bit( tmp ) );
                    else               attacker = ( 1ULL <<  last_set_bit( tmp ) );
                    if ( attacker & pieceBoards[(6*otherSide)+3] ) return false;
                    if ( attacker & pieceBoards[(6*otherSide)+5] ) return false;
                }
            }
            else {
                // rooks and queens
                _ray = rays[j][from_ind];
                tmp = blockers & _ray;
                if ( tmp ) {
                    if ( (j+2)%6 < 4 ) attacker = ( 1ULL << first_set_bit( tmp ) );
                    else               attacker = ( 1ULL <<  last_set_bit( tmp ) );
                    if ( attacker & pieceBoards[(6*otherSide)+1] ) return false;
                    if ( attacker & pieceBoards[(6*otherSide)+5] ) return false;
                }
            }
        }
    }
    
    if ( move.is_ep_capture() ) {
        int other_pawn_ind = to_ind + ( ( sideToMove==white ) ? S : N );
        bitboard left_ray  = rays[6][from_ind] & rays[6][other_pawn_ind] & blockers;
        bitboard right_ray = rays[2][from_ind] & rays[2][other_pawn_ind] & blockers;
        bitboard attacker_left  = ( 1ULL << last_set_bit(left_ray) );
        bitboard attacker_right = ( 1ULL << first_set_bit(right_ray) );
        
        if ( ( attacker_left  & pieceBoards[(sideToMove*6)+5] ) &&
             ( attacker_right & ( pieceBoards[(otherSide*6) + 1] |
                                  pieceBoards[(otherSide*6) + 4] ) ) ) return false;
        if ( ( attacker_right & pieceBoards[(sideToMove*6)+5] ) &&
             ( attacker_left  & ( pieceBoards[(otherSide*6) + 1] |
                                  pieceBoards[(otherSide*6) + 4] ) ) ) return false;
        
    }
    
    return true;
}