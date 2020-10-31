#include "board.h"
#include "twiddle.h"
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
    fullMoveClock = 0;

    // keep track of whether the previous move was a double
    // pawn push, for en passant pawn capture
    lastMoveDoublePawnPush = false;
    //  int dPPFile;

    // side to move
    sideToMove = white;

    // value starts at 0
    value=0;

    // initial hash value
    hash_value = zobrist_hash();
}

board::board(bitboard * startPositions, bool * castling, bool ep, int dpp, uint8_t clock, uint8_t full_clock, colour side, int32_t val, uint64_t hash) {
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
    fullMoveClock = full_clock;

    // en passant pawn capture
    lastMoveDoublePawnPush = ep;
    dPPFile = dpp;

    // side to move
    sideToMove = side;

    // value
    value = val;

    // hash
    hash_value = hash;
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
    fullMoveClock          = b1.fullMoveClock;

    // en passant pawn capture
    lastMoveDoublePawnPush = b1.lastMoveDoublePawnPush;

    // double pawn push file

    dPPFile = b1.dPPFile;

    // side to move
    sideToMove = b1.sideToMove;

    // value
    value = b1.value;

    // hash
    hash_value = b1.hash_value;
}

board::board( std::string fen ) {
    int j,i=0;
    for ( j=0;j<12;j++ ) pieceBoards[j] = 0;
    int cp;

    for ( int count_slash=0; count_slash<8; count_slash++ ) {
        j = 0;
        while ( fen[i] != '/' && fen[i] != ' ' ) {
            //            std::cout << fen[i];
            switch ( fen[i] ) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                    j += int(fen[i]-'0');
                    //                    std::cout << "number " << fen[i] << std::endl;
                    i++;
                    cp = -1;
                    break;
                case 'P':
                    cp = 0;
                    break;
                case 'R':
                    cp = 1;
                    break;
                case 'N':
                    cp = 2;
                    break;
                case 'B':
                    cp = 3;
                    break;
                case 'Q':
                    cp = 4;
                    break;
                case 'K':
                    cp = 5;
                    break;
                case 'p':
                    cp = 6;
                    break;
                case 'r':
                    cp = 7;
                    break;
                case 'n':
                    cp = 8;
                    break;
                case 'b':
                    cp = 9;
                    break;
                case 'q':
                    cp = 10;
                    break;
                case 'k':
                    cp = 11;
                    break;
            }
            if ( cp != -1) {
                pieceBoards[cp] |= ( 1ULL << ( (7-count_slash)*8 + j ) );
                j++;
                i++;
            }

        }
        i++;
    }

    sideToMove = ( fen[i] == 'w' ) ? white : black;
    i += 2;

    castleWhiteKingSide  = false;
    castleWhiteQueenSide = false;
    castleBlackKingSide  = false;
    castleBlackQueenSide = false;

    if ( fen[i] == '-' ) {
        i += 2;
    }
    else {
        while ( fen[i] != ' ' ) {
            switch ( fen[i] ) {
                case 'K':
                    castleWhiteKingSide  = true;
                    break;
                case 'Q':
                    castleWhiteQueenSide = true;
                    break;
                case 'k':
                    castleBlackKingSide = true;
                    break;
                case 'q':
                    castleBlackQueenSide = true;
                    break;
            }
            i++;
        }
        i++;
    }

    if ( fen[i] == '-' ) {
        lastMoveDoublePawnPush = false;
        i += 2;
    }
    else {
        lastMoveDoublePawnPush = true;
        dPPFile = int( fen[i] - 'a' );
        i += 3;
    }

    std::stringstream clock;

    while ( fen[i] != ' ' ) {
        clock << fen[i];
        i++;
    }
    clock >> halfMoveClock;
    clock.str("");
    i++;

    //come back to this
    while ( fen[i] != '\0' ) {
        clock << fen[i];
        i++;
    }
    clock >> fullMoveClock;


    // value starts at 0
    value = evaluate();

    // hash
    hash_value = zobrist_hash();
}


// operator overloading - do this with hashes instead?
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
        std::cout << "half move clock wrong\n";
        return false;
    }

    if ( fullMoveClock != other.fullMoveClock ) {
        std::cout << "full move clock clock wrong\n";
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

bool board::operator!=( const board& other ) {
    return ! ( *this == other ) ;
}

// get data
void board::getBitboards(bitboard * dest) const {
    const bitboard * p = pieceBoards;

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

void board::getFullClock(int * dest) {
    *dest = fullMoveClock;
}

void board::getSide(colour * dest) {
    *dest = sideToMove;
}

void board::getHash(uint64_t * dest) {
    *dest = hash_value;
}

int board::num_pieces_left() {
    int ret = 0;
    for ( int i=0; i<12; i++ ) {
        ret += count_bits_set( pieceBoards[i] );
    }
    return ret;
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

void board::print_board_indent( std::ostream& cout, int indent ) {
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

    for (i=0;i<indent;i++) cout << " ";

    cout << "   A B C D E F G H\n\n";
    for (i=7;i>=0;i--) {
        for (j=0;j<indent;j++) cout << " ";
        cout << i+1 << " ";
        for (j=0;j<8;j++) {
            cout << " " << to_print[i*8+j];
        }
        cout << "  " << i+1 << endl;
    }
    cout << std::endl;
    for (i=0;i<indent;i++) cout << " ";
    cout << "   A B C D E F G H\n";
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
    cout << "  " << halfMoveClock;
    cout << "\nFullmove Clock:\n";
    cout << "  " << fullMoveClock << "\n";
}

std::ostream& operator<<(std::ostream &out, const board &brd) {
    // uppercase = black, lowercase = white
    int i,j;
    bitboard pieceBoards[12];
    brd.getBitboards(pieceBoards);
    char to_print[64];
    for (i=0;i<64;i++) to_print[i]='.';

    for (i=0;i<12;i++) {
        //tmp = pieceBoards[i];//pieces[i];
        for (j=0;j<64;j++) {
            if (pieceBoards[i] & 1ULL) {
                to_print[j] = symbols[i];
            }
            pieceBoards[i] >>= 1;
            //if (~tmp) break;
        }
    }

    out << "   A B C D E F G H\n\n";
    for (i=7;i>=0;i--) {
        out << i+1 << " ";
        for (j=0;j<8;j++) {
            out << " " << to_print[i*8+j];
        }
        out << "  " << i+1 << endl;
    }
    out << "\n   A B C D E F G H\n";
    
    return out;
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

    ss << " " << fullMoveClock;

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
    ss << " " << fullMoveClock;
}


void board::set_piece(colourPiece cP,int pos) {
    pieceBoards[cP] |= (1ULL << pos);
}

void board::set_side(colour side) {
    sideToMove = side;
}

void board::clear_square( int ind ) {
    bitboard b = ~ ( 1ULL << ind );
    for ( int i=0; i<12; i++ ) pieceBoards[i] &= b;
}

void board::update_value() {
    value = evaluate();
}

void board::set_value( int32_t _value ) {
    value = _value;
}

void board::update_hash() {
    hash_value = zobrist_hash();
}

void board::set_hash( uint64_t _hash ) {
    hash_value = _hash;
}

void board::setBitboards(bitboard * bb) {
    for (int i=0; i<12; i++) pieceBoards[i] = bb[i];
}

void board::setCastlingRights(bool * castling) {
    castleWhiteKingSide  = castling[0];
    castleWhiteQueenSide = castling[1];
    castleBlackKingSide  = castling[2];
    castleBlackQueenSide = castling[3];
}

void board::setEP(bool ep) {
    lastMoveDoublePawnPush = ep;
}

void board::setdPPFile(int file) {
    dPPFile = file;
}

void board::setClock(int clk) {
    halfMoveClock = clk;
}

void board::setFullClock(int clk) {
    fullMoveClock = clk;
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
