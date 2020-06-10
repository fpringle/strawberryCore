#include "board.h"
//#include "twiddle.h"
#include "exceptions.h"

#include <assert.h>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>

// initialise constants declared in board.h
const char symbols[12]  = {'p','r','n','b','q','k',
                           'P','R','N','B','Q','K'};
const char colours[2]   = {'w','b'};

// define starting configuration

bitboard whitePawnStart   = 0x000000000000ff00;
bitboard whiteRookStart   = 0x0000000000000081;
bitboard whiteKnightStart = 0x0000000000000042;
bitboard whiteBishopStart = 0x0000000000000024;
bitboard whiteQueenStart  = 0x0000000000000008;
bitboard whiteKingStart   = 0x0000000000000010;

bitboard blackPawnStart   = 0x00ff000000000000;
bitboard blackRookStart   = 0x8100000000000000;
bitboard blackKnightStart = 0x4200000000000000;
bitboard blackBishopStart = 0x2400000000000000;
bitboard blackQueenStart  = 0x0800000000000000;
bitboard blackKingStart   = 0x1000000000000000;


// define class constructors

board::board() {
  // default constructor
  
  // initialise an array of pointers to the piece bitboards
  pieceBoards[0]  = whitePawnStart;
  pieceBoards[1]  = whiteRookStart;
  pieceBoards[2]  = whiteKnightStart;
  pieceBoards[3]  = whiteBishopStart;
  pieceBoards[4]  = whiteQueenStart;
  pieceBoards[5]  = whiteKingStart;
  pieceBoards[6]  = blackPawnStart;
  pieceBoards[7]  = blackRookStart;
  pieceBoards[8]  = blackKnightStart;
  pieceBoards[9]  = blackBishopStart;
  pieceBoards[10] = blackQueenStart;
  pieceBoards[11] = blackKingStart;
  
  // both sides start with king- and queen-side castling rights
  castleWhiteKingSide  = true;
  castleWhiteQueenSide = true;
  castleBlackKingSide  = true;
  castleBlackQueenSide = true;
  
  // keeps track of the number of reversible moves in order
  // to enforce the 50-move rule
  // because that always comes into effect.
  halfMoveClock = 0;
  
  // keep track of whether the previous move was a double
  // pawn push, for en passant pawn capture
  lastMoveDoublePawnPush = false;
//  int dPPFile;

  // side to move
  sideToMove = white;

  // value starts at 0
  value=0;
}

board::board(bitboard * startPositions, bool * castling, bool ep, int dpp, uint8_t clock, colour side, int32_t val) {
  // parameterised constructor

  // initialise an array of pointers to the piece bitboards
  bitboard * p = startPositions;
  bitboard * q = pieceBoards;

  while (p < startPositions + 12) *q++ = *p++;

  // castling rights
  castleWhiteKingSide  = castling[0];
  castleWhiteQueenSide = castling[1];
  castleBlackKingSide  = castling[2];
  castleBlackQueenSide = castling[3];
  
  // 50-move rule
  halfMoveClock = clock;
  
  // en passant pawn capture
  lastMoveDoublePawnPush = ep;
  dPPFile = dpp;

  // side to move
  sideToMove = side;

  // value
  value = val;
}

board::board(board & b1) {
  // copy constructor

  // initialise an array of pointers to the piece bitboards
  bitboard * p = b1.pieceBoards;
  bitboard * q = pieceBoards;

  while (p < b1.pieceBoards + 12) *q++ = *p++;

  // castling rights
  castleWhiteKingSide    = b1.castleWhiteKingSide;
  castleWhiteQueenSide   = b1.castleWhiteQueenSide;
  castleBlackKingSide    = b1.castleBlackKingSide;
  castleBlackQueenSide   = b1.castleBlackQueenSide;

  // 50-move rule
  halfMoveClock          = b1.halfMoveClock;

  // en passant pawn capture
  lastMoveDoublePawnPush = b1.lastMoveDoublePawnPush;
  
  // double pawn push file
  
  dPPFile = b1.dPPFile;

  // side to move
  sideToMove = b1.sideToMove;

  // value
  value = b1.value;
}


// operator overloading
bool board::operator==( const board& other) {
    int i;
    
    for ( i=0; i<12; i++ ) {
        if ( pieceBoards[i] != other.pieceBoards[i] ) {
            std::cout << "pieceBoard[" << i << "] is wrong\n";
            return false;
        }
    }
    
    if ( castleWhiteKingSide != other.castleWhiteKingSide ||
         castleWhiteQueenSide != other.castleWhiteQueenSide ||
         castleBlackKingSide != other.castleBlackKingSide ||
         castleBlackQueenSide != other.castleBlackQueenSide ) {
        std::cout << "castling rights wrong\n";
        return false;
    }
    
    if ( halfMoveClock != other.halfMoveClock ) {
        std::cout << "clock wrong\n";
        return false;
    }
    
    if ( lastMoveDoublePawnPush != other.lastMoveDoublePawnPush ) {
        std::cout << "ep wrong\n";
        return false;
    }
    
    if ( (lastMoveDoublePawnPush) && (dPPFile != other.dPPFile) ) {
        std::cout << "dPPFile wrong\n";
        return false;
    }
    
    if ( sideToMove != other.sideToMove ) {
        std::cout << "side wrong\n";
        return false;
    }
    
    if ( value != other.value ) {
        std::cout << "value wrong\n";
        return false;
    }
    
    return true;
}

bool board::operator!=( const board&other ) {
    return ( ! ( this->operator==(other) ) );
}

// get data
void board::getBitboards(bitboard * dest) {
  bitboard * p = pieceBoards;

  while (p < pieceBoards + 12) *dest++ = *p++;
}

void board::getCastlingRights(bool * dest) {
  dest[0] = castleWhiteKingSide;
  dest[1] = castleWhiteQueenSide;
  dest[2] = castleBlackKingSide;
  dest[3] = castleBlackQueenSide;
}

void board::getEP(bool * dest) {
  *dest = lastMoveDoublePawnPush;
}

void board::getdPPFile(int * dest) {
  *dest = dPPFile;
}

void board::getClock(int * dest) {
  *dest = halfMoveClock;
}

void board::getSide(colour * dest) {
  *dest = sideToMove;
}

void print_bb(bitboard bb, char c, std::ostream& cout) {//  = 'x') {
  int i,j;
  char to_print[64];
  for (i=0;i<64;i++) {
    if (bb&1ULL) to_print[i]=c;
    else to_print[i]='.';
    bb >>= 1;
  }
  for (i=7;i>=0;i--) {
    for (j=0;j<8;j++) {
      cout << to_print[i*8+j] << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

// display the current state of the board
void board::print_board(std::ostream& cout) {
  // uppercase = black, lowercase = white
  int i,j;
  bitboard tmp;
  char to_print[64];
  for (i=0;i<64;i++) to_print[i]='.';
  
  for (i=0;i<12;i++) {
    tmp = pieceBoards[i];//pieces[i];
    for (j=0;j<64;j++) {
      if (tmp & 1ULL) {
        to_print[j] = symbols[i];
      }
      tmp >>= 1;
      //if (~tmp) break;
    }
  }
  
  cout << "   A B C D E F G H\n\n";
  for (i=7;i>=0;i--) {
    cout << i+1 << " ";
    for (j=0;j<8;j++) {
      cout << " " << to_print[i*8+j];
    }
    cout << "  " << i+1 << endl;
  }
  cout << "\n   A B C D E F G H\n";
}

void board::print_all(std::ostream& cout) {
  print_board(cout);
  cout << "\nSide to move:\n";
  if ( sideToMove==white) cout << "  White\n";
  else cout << "  Black\n";
  cout << "\nCastling rights:\n";
  cout << "  White can";
  if ( !castleWhiteKingSide ) cout << "not";
  cout << " castle kingside\n";
  cout << "  White can";
  if ( !castleWhiteQueenSide ) cout << "not";
  cout << " castle queenside\n";
  cout << "  Black can";
  if ( !castleBlackKingSide ) cout << "not";
  cout << " castle kingside\n";
  cout << "  Black can";
  if ( !castleBlackQueenSide ) cout << "not";
  cout << " castle queenside\n";

  cout << "\nEn passant:\n";
  if (lastMoveDoublePawnPush) cout << "  last move was a double pawn push\n";
  else cout << "  last move was not a double pawn push\n";

  cout << "\nHalfmove Clock:\n";
  cout << "  " << halfMoveClock << "\n";
}

std::string board::FEN() {
  int count1;
  std::stringstream ss;
  int i,j,k;
  bitboard tmp;
  char to_print[64];
  for (i=0;i<64;i++) to_print[i]='.';

  for (i=0;i<12;i++) {
    tmp = pieceBoards[i];
    for (j=0;j<8;j++) {
      for (k=0;k<8;k++) {
        if (tmp & 1ULL) {
          to_print[((7-j)*8)+k] = symbols[(i+6)%12];
        }
        tmp >>= 1;
      }
    }
  }

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      if (to_print[i*8+j]=='.') {
        count1 = j;
        while (to_print[i*8+count1]=='.' && count1<8) count1++;
        ss << '0'+(count1-j);
        j=count1-1;
      }
      else {
        ss << to_print[i*8+j];
      }
    }
    if (i!=7) {
      ss << '/';
    }
  }
  
  
  if (sideToMove==white) ss << " w "; else ss << " b ";

  
  if ( !( castleWhiteKingSide | castleWhiteQueenSide | castleBlackKingSide | castleBlackQueenSide ) ) {
    ss << "-";
  }

  else {
    if ( castleWhiteKingSide )  ss << "K";
    if ( castleWhiteQueenSide ) ss << "Q";
    if ( castleBlackKingSide )  ss << "k";
    if ( castleBlackQueenSide ) ss << "q";
  }

  if ( lastMoveDoublePawnPush ) {
    ss << " ";
    ss << "a"+dPPFile;
    ss << (sideToMove==white) ? "6" : "3";
  }

  else {
    ss << " -";
  }

  ss << " " << halfMoveClock;

  return ss.str();
}

void board::FEN( std::ostream& ss ) {
  int count1;
  int i,j,k;
  bitboard tmp;
  char to_print[64];
  for (i=0;i<64;i++) to_print[i]='.';

  for (i=0;i<12;i++) {
    tmp = pieceBoards[i];
    for (j=0;j<8;j++) {
      for (k=0;k<8;k++) {
        if (tmp & 1ULL) {
          to_print[((7-j)*8)+k] = symbols[(i+6)%12];
        }
        tmp >>= 1;
      }
    }
  }

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      if (to_print[i*8+j]=='.') {
        count1 = j;
        while (to_print[i*8+count1]=='.' && count1<8) count1++;
        ss << char('0'+(count1-j));
        j=count1-1;
      }
      else {
        ss << to_print[i*8+j];
      }
    }
    if (i!=7) {
      ss << "/";
    }
  }
  
  if (sideToMove==white) ss << " w "; else ss << " b ";

  
  if ( !( castleWhiteKingSide | castleWhiteQueenSide | castleBlackKingSide | castleBlackQueenSide ) ) {
    ss << "-";
  }

  else {
    if ( castleWhiteKingSide )  ss << "K";
    if ( castleWhiteQueenSide ) ss << "Q";
    if ( castleBlackKingSide )  ss << "k";
    if ( castleBlackQueenSide ) ss << "q";
  }

  if ( lastMoveDoublePawnPush ) {
    ss << " ";
    ss << "a"+dPPFile;
    ss << (sideToMove==white) ? "6" : "3";
  }

  else {
    ss << " -";
  }

  ss << " " << halfMoveClock;
}


void board::set_piece(colourPiece cP,int pos) {
  pieceBoards[cP] |= (1ULL << pos);
}

void board::set_side(colour side) {
  sideToMove = side;
}

void board::clear_square( int ind ) {
    bitboard b = ! ( 1ULL << ind );
    for ( int i=0; i<12; i++ ) pieceBoards[i] &= b;
}

bitboard board::whiteSquares() {
  int i;
  bitboard ret = 0;

  for (i=0;i<6;i++) ret |= pieceBoards[i];
  return ret;
}
bitboard board::blackSquares() {
  int i;
  bitboard ret = 0;

  for (i=6;i<12;i++) ret |= pieceBoards[i];
  return ret;
}
bitboard board::takenSquares() {
  return (whiteSquares() | blackSquares());
}
bitboard board::emptySquares() {
  return ( ~ takenSquares());
}
