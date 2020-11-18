#include "board.h"
#include "twiddle.h"
#include "typedefs.h"

#include <cstdint>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>


namespace chessCore {

colour flipColour(colour side) {
    if (side == white) return black;
    else return white;
}

// initialise constants declared in board.h
const char symbols[12] = {'p', 'r', 'n', 'b', 'q', 'k',
                          'P', 'R', 'N', 'B', 'Q', 'K'
                         };
//const char symbols[12] = {'\u2659', '\u2656', '\u2658', '\u2657', '\u2655', '\u2654',
//                          '\u265F', '\u265C', '\u265E', '\u265D', '\u265B', '\u265A'
//                         };
//const char symbols[12] = {'♙','♘','♗','♖','♕','♔','♟','♞','♝','♜','♛','♚'};
const char colours[2] = {'w', 'b'};

// define starting configuration

bitboard whitePawnStart = 0x000000000000ff00;
bitboard whiteRookStart = 0x0000000000000081;
bitboard whiteKnightStart = 0x0000000000000042;
bitboard whiteBishopStart = 0x0000000000000024;
bitboard whiteQueenStart = 0x0000000000000008;
bitboard whiteKingStart = 0x0000000000000010;

bitboard blackPawnStart = 0x00ff000000000000;
bitboard blackRookStart = 0x8100000000000000;
bitboard blackKnightStart = 0x4200000000000000;
bitboard blackBishopStart = 0x2400000000000000;
bitboard blackQueenStart = 0x0800000000000000;
bitboard blackKingStart = 0x1000000000000000;


// define class constructors

board::board() {
    // default constructor

    // initialise an array of pointers to the piece bitboards
    pieceBoards[0] = whitePawnStart;
    pieceBoards[1] = whiteRookStart;
    pieceBoards[2] = whiteKnightStart;
    pieceBoards[3] = whiteBishopStart;
    pieceBoards[4] = whiteQueenStart;
    pieceBoards[5] = whiteKingStart;
    pieceBoards[6] = blackPawnStart;
    pieceBoards[7] = blackRookStart;
    pieceBoards[8] = blackKnightStart;
    pieceBoards[9] = blackBishopStart;
    pieceBoards[10] = blackQueenStart;
    pieceBoards[11] = blackKingStart;

    // both sides start with king- and queen-side castling rights
    castleWhiteKingSide = true;
    castleWhiteQueenSide = true;
    castleBlackKingSide = true;
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
    opening_value = 0;
    endgame_value = 0;

    // initial hash value
    hash_value = zobrist_hash();
}

board::board(bitboard * startPositions, bool * castling, bool ep, int dpp,
             uint8_t clock, uint8_t full_clock, colour side,
             value_t open_val, value_t end_val, uint64_t hash) {
    // parameterised constructor

    // initialise an array of pointers to the piece bitboards
    bitboard * p = startPositions;
    bitboard * q = pieceBoards;

    while (p < startPositions + 12) *q++ = *p++;

    // castling rights
    castleWhiteKingSide = castling[0];
    castleWhiteQueenSide = castling[1];
    castleBlackKingSide = castling[2];
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
    opening_value = open_val;
    endgame_value = end_val;

    // hash
    hash_value = hash;
}

board::board(board& other) {
    // copy constructor

    // initialise an array of pointers to the piece bitboards
    bitboard * p = other.pieceBoards;
    bitboard * q = pieceBoards;

    while (p < other.pieceBoards + 12) *q++ = *p++;

    // castling rights
    castleWhiteKingSide = other.castleWhiteKingSide;
    castleWhiteQueenSide = other.castleWhiteQueenSide;
    castleBlackKingSide = other.castleBlackKingSide;
    castleBlackQueenSide = other.castleBlackQueenSide;

    // 50-move rule
    halfMoveClock = other.halfMoveClock;
    fullMoveClock = other.fullMoveClock;

    // en passant pawn capture
    lastMoveDoublePawnPush = other.lastMoveDoublePawnPush;

    // double pawn push file

    dPPFile = other.dPPFile;

    // side to move
    sideToMove = other.sideToMove;

    // value
    opening_value = other.opening_value;
    endgame_value = other.endgame_value;

    // hash
    hash_value = other.hash_value;
}

board::board(std::string fen) {
    //std::cout << fen << std::endl;
    int j, i = 0;
    for (j = 0; j < 12; j++) pieceBoards[j] = 0;
    int cp = -1;
    int sz = fen.size();

    while (fen[i] == ' ') i++;

    for (int count_slash = 0; count_slash < 8; count_slash++) {
        j = 0;
        while (fen[i] != '/' && fen[i] != ' ') {
            switch (fen[i]) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                j += int(fen[i] - '0');
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
            if (cp != -1) {
                pieceBoards[cp] |= (1ULL << ((7 - count_slash)*8 + j));
                j++;
                i++;
            }

        }
        i++;
    }

    sideToMove = (fen[i] == 'w') ? white : black;
    do {
        i++;
    } while (fen[i] == ' ');

    castleWhiteKingSide = false;
    castleWhiteQueenSide = false;
    castleBlackKingSide = false;
    castleBlackQueenSide = false;

    if (fen[i] != '-') {
        while (fen[i] != ' ') {
            switch (fen[i]) {
            case 'K':
                castleWhiteKingSide = true;
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
    }

    do {
        i++;
    } while (fen[i] == ' ');

    if (fen[i] == '-') {
        lastMoveDoublePawnPush = false;
    }
    else {
        lastMoveDoublePawnPush = true;
        dPPFile = int( fen[i] - 'a');
        i++;
    }

    // value starts at 0
    opening_value = evaluateOpening();
    endgame_value = evaluateEndgame();

    // hash
    hash_value = zobrist_hash();

    do {
        i++;
    } while (fen[i] == ' ');

    halfMoveClock = 0;
    fullMoveClock = 0;

    if (i >= sz) {
        return;
    }

    std::stringstream clock;

    while (fen[i] != ' ') {
        clock << fen[i];
        i++;
    }
    halfMoveClock = stoi(clock.str());
    clock.clear();
    clock.str("");

    do {
        i++;
    } while (fen[i] == ' ');

    if (i >= sz) {
        fullMoveClock = 0;
        return;
    }

    fullMoveClock = std::stoi(fen.substr(i));
}


// operator overloading - do this with hashes instead?

bool board::operator==(const board& other) {
    int i;

    for (i = 0; i < 12; i++) {
        if (pieceBoards[i] != other.pieceBoards[i]) {
            return false;
        }
    }

    if (castleWhiteKingSide != other.castleWhiteKingSide ||
            castleWhiteQueenSide != other.castleWhiteQueenSide ||
            castleBlackKingSide != other.castleBlackKingSide ||
            castleBlackQueenSide != other.castleBlackQueenSide) {
        return false;
    }

    if (halfMoveClock != other.halfMoveClock) {
        return false;
    }

    if (fullMoveClock != other.fullMoveClock) {
        return false;
    }

    if (lastMoveDoublePawnPush != other.lastMoveDoublePawnPush) {
        return false;
    }

    if ((lastMoveDoublePawnPush) && (dPPFile != other.dPPFile)) {
        return false;
    }

    if (sideToMove != other.sideToMove) {
        return false;
    }

    if (opening_value != other.opening_value) {
        return false;
    }

    if (endgame_value != other.endgame_value) {
        return false;
    }

    return true;
}

bool board::operator!=(const board& other) {
    return !(*this == other);
}

board& board::operator=(const board& other) {
//    bitboard * p = other.pieceBoards;
//    bitboard * q = pieceBoards;

//    while (p < other.pieceBoards + 12) *q++ = *p++;


    other.getBitboards(pieceBoards);

    // castling rights
    castleWhiteKingSide = other.castleWhiteKingSide;
    castleWhiteQueenSide = other.castleWhiteQueenSide;
    castleBlackKingSide = other.castleBlackKingSide;
    castleBlackQueenSide = other.castleBlackQueenSide;

    // 50-move rule
    halfMoveClock = other.halfMoveClock;
    fullMoveClock = other.fullMoveClock;

    // en passant pawn capture
    lastMoveDoublePawnPush = other.lastMoveDoublePawnPush;

    // double pawn push file

    dPPFile = other.dPPFile;

    // side to move
    sideToMove = other.sideToMove;

    // value
    opening_value = other.opening_value;
    endgame_value = other.endgame_value;

    // hash
    hash_value = other.hash_value;

    return *this;
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

void board::getClock(uint8_t * dest) {
    *dest = halfMoveClock;
}

void board::getFullClock(uint8_t * dest) {
    *dest = fullMoveClock;
}

void board::getSide(colour * dest) {
    *dest = sideToMove;
}

void board::getHash(uint64_t * dest) {
    *dest = hash_value;
}

int board::num_pieces_left(colourPiece cp) {
    return count_bits_set(pieceBoards[cp]);
}

int board::num_pieces_left() {
    int ret = 0;
    for (int i = 0; i < 12; i++) {
        ret += num_pieces_left(colourPiece(i));
    }
    return ret;
}

void print_bb(bitboard bb, char c, std::ostream& cout) {
    int i, j;
    char to_print[64];
    for (i = 0; i < 64; i++) {
        if (bb & 1ULL) to_print[i] = c;
        else to_print[i] = '.';
        bb >>= 1;
    }
    for (i = 7; i >= 0; i--) {
        for (j = 0; j < 8; j++) {
            cout << to_print[i * 8 + j] << ' ';
        }
        cout << std::endl;
    }
    cout << std::endl;
}

// display the current state of the board

void board::print_board(std::ostream& cout) {
    // uppercase = black, lowercase = white
    int i, j;
    bitboard tmp;
    char to_print[64];
    for (i = 0; i < 64; i++) to_print[i] = '.';

    for (i = 0; i < 12; i++) {
        tmp = pieceBoards[i]; //pieces[i];
        for (j = 0; j < 64; j++) {
            if (tmp & 1ULL) {
                to_print[j] = symbols[i];
            }
            tmp >>= 1;
            //if (~tmp) break;
        }
    }

    cout << "   A B C D E F G H\n\n";
    for (i = 7; i >= 0; i--) {
        cout << i + 1 << " ";
        for (j = 0; j < 8; j++) {
            cout << " " << to_print[i * 8 + j];
        }
        cout << "  " << i + 1 << std::endl;
    }
    cout << "\n   A B C D E F G H\n";
}

void board::print_board_flipped(std::ostream& cout) {
    // uppercase = black, lowercase = white
    int i, j;
    bitboard tmp;
    char to_print[64];
    for (i = 0; i < 64; i++) to_print[i] = '.';

    for (i = 0; i < 12; i++) {
        tmp = pieceBoards[i]; //pieces[i];
        for (j = 0; j < 64; j++) {
            if (tmp & 1ULL) {
                to_print[j] = symbols[i];
            }
            tmp >>= 1;
            //if (~tmp) break;
        }
    }

    cout << "   H G F E D C B A\n\n";
    for (i = 0; i < 8; i++) {
        cout << i + 1 << " ";
        for (j = 7; j >= 0; j--) {
            cout << " " << to_print[i * 8 + j];
        }
        cout << "  " << i + 1 << std::endl;
    }
    cout << "\n   H G F E D C B A\n";
}

void board::print_all(std::ostream& cout) {
    print_board(cout);

    cout << "\nSide to move:\n";
    if (sideToMove == white) cout << "  White\n";
    else cout << "  Black\n";

    cout << "\nCastling rights:\n";
    cout << "  White can";
    if (!castleWhiteKingSide) cout << "not";
    cout << " castle kingside\n";
    cout << "  White can";
    if (!castleWhiteQueenSide) cout << "not";
    cout << " castle queenside\n";
    cout << "  Black can";
    if (!castleBlackKingSide) cout << "not";
    cout << " castle kingside\n";
    cout << "  Black can";
    if (!castleBlackQueenSide) cout << "not";
    cout << " castle queenside\n";

    cout << "\nEn passant:\n";
    if (lastMoveDoublePawnPush) cout << "  last move was a double pawn push (file "
        << char('a' + dPPFile) << ")\n";
    else cout << "  last move was not a double pawn push\n";

    cout << "\nHalfmove Clock:\n";
    cout << "  " << + halfMoveClock;
    cout << "\nFullmove Clock:\n";
    cout << "  " << + fullMoveClock << "\n";
}

std::ostream& operator<<(std::ostream& out, const board& brd) {
    // uppercase = black, lowercase = white
    int i, j;
    bitboard pieceBoards[12];
    brd.getBitboards(pieceBoards);
    char to_print[64];
    for (i = 0; i < 64; i++) to_print[i] = '.';

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 64; j++) {
            if (pieceBoards[i] & 1ULL) {
                to_print[j] = symbols[i];
            }
            pieceBoards[i] >>= 1;
        }
    }

    out << "   A B C D E F G H\n\n";
    for (i = 7; i >= 0; i--) {
        out << i + 1 << " ";
        for (j = 0; j < 8; j++) {
            out << " " << to_print[i * 8 + j];
        }
        out << "  " << i + 1 << std::endl;
    }
    out << "\n   A B C D E F G H\n";

    return out;
}

std::string board::FEN() {
    int count;
    std::stringstream ss;
    int i, j, k;
    bitboard tmp;
    char to_print[64];
    for (i = 0; i < 64; i++) to_print[i] = '.';

    for (i = 0; i < 12; i++) {
        tmp = pieceBoards[i];
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 8; k++) {
                if (tmp & 1ULL) {
                    to_print[((7 - j)*8) + k] = symbols[(i + 6) % 12];
                }
                tmp >>= 1;
            }
        }
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (to_print[i * 8 + j] == '.') {
                count = j;
                while (to_print[i * 8 + count] == '.' && count < 8) count++;
                ss << char('0' + (count - j));
                j = count - 1;
            }
            else {
                ss << to_print[i * 8 + j];
            }
        }
        if (i != 7 ) {
            ss << '/';
        }
    }


    if (sideToMove == white) ss << " w ";
    else ss << " b ";


    if (!(castleWhiteKingSide | castleWhiteQueenSide | castleBlackKingSide |
            castleBlackQueenSide)) {
        ss << "-";
    }

    else {
        if (castleWhiteKingSide) ss << "K";
        if (castleWhiteQueenSide) ss << "Q";
        if (castleBlackKingSide) ss << "k";
        if (castleBlackQueenSide) ss << "q";
    }

    ss << " ";

    if (lastMoveDoublePawnPush) {
        ss << char('a' + dPPFile)
           << ((sideToMove == white) ? "6" : "3");
    }

    else {
        ss << "-";
    }

    ss << " " << + halfMoveClock
       << " " << + fullMoveClock;

    return ss.str();
}

void board::set_piece(colourPiece cP, int pos) {
    pieceBoards[cP] |= (1ULL << pos);
}

void board::set_side(colour side) {
    sideToMove = side;
}

void board::clear_square(int ind) {
    bitboard b = ~(1ULL << ind);
    for (int i = 0; i < 12; i++) pieceBoards[i] &= b;
}

void board::update_value() {
    opening_value = evaluateOpening();
    endgame_value = evaluateEndgame();
}

void board::update_hash() {
    hash_value = zobrist_hash();
}

void board::set_hash(uint64_t hash) {
    hash_value = hash;
}

void board::setBitboards(bitboard * bb) {
    for (int i = 0; i < 12; i++) pieceBoards[i] = bb[i];
}

void board::setCastlingRights(bool * castling) {
    castleWhiteKingSide = castling[0];
    castleWhiteQueenSide = castling[1];
    castleBlackKingSide = castling[2];
    castleBlackQueenSide = castling[3];
}

void board::setEP(bool ep) {
    lastMoveDoublePawnPush = ep;
}

void board::setdPPFile(int file) {
    dPPFile = file;
}

void board::setClock(uint8_t clk) {
    halfMoveClock = clk;
}

void board::setFullClock(uint8_t clk) {
    fullMoveClock = clk;
}

bitboard board::whiteSquares() {
    int i;
    bitboard ret = 0;

    for (i = 0; i < 6; i++) ret |= pieceBoards[i];
    return ret;
}

bitboard board::blackSquares() {
    int i;
    bitboard ret = 0;

    for (i = 6; i < 12; i++) ret |= pieceBoards[i];
    return ret;
}

bitboard board::takenSquares() {
    return (whiteSquares() | blackSquares());
}

bitboard board::emptySquares() {
    return ( ~takenSquares());
}


} // end of chessCore namespace
