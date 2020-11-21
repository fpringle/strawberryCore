#ifndef __MOVE_H
#define __MOVE_H

#include <cstdint>
#include <iostream>
#include <string>

#include "typedefs.h"


namespace chessCore {


/**@{*/
/** Constants for sliding pieces which moves in straight lines. */
extern const int N;
extern const int NE;
extern const int E;
extern const int SE;
extern const int S;
extern const int SW;
extern const int W;
extern const int NW;
/**@}*/

/**@{*/
/** Constants for knights. */
extern const int NNE;
extern const int ENE;
extern const int ESE;
extern const int SSE;
extern const int SSW;
extern const int WSW;
extern const int WNW;
extern const int NNW;
/**@}*/

/**
 *  Convert a square index to a 2-char array.
 *
 *  \param i        The square index to convert.
 *  \param dest     A pointer to a char array.
 */
void itos(int i, char* dest);

/**
 *  Convert a square index to a std::string.
 *
 *  \param i        The square index to convert.
 *  \return         A string representing the index.
 */
std::string itos(int i);

/**
 *  Print the 2-char representation of a square index to an output stream.
 *
 *  \param i        The square index to convert.
 *  \param cout     The output stream to print to.
 */
void itos(int i, std::ostream& cout);

/**
 *  Convert a 2-character string to a square index.
 *
 *  \param s        The string to convert.
 *  \return         An integer representing the index of the square.
 */
int _stoi(std::string s);


/**
 *  A pre-generated array of bitboard representing the moves that sliding
 *  pieces can make from each square of the board.
 */
extern bitboard rays[8][64];

/** Generate the ray bitboards */
void init_rays();

// 2-byte container to store the information of a move.

/**
 *  \struct move_t
 *
 *  \brief A struct representing one move on the board.
 */
struct move_t {
    // bits 0-5:  from square
    // bits 6-11: to square
    // bits 12:   capture
    // bit 13:    special 0
    // bit 14:    special 1
    // bit 15:    promotion
private:
    /**
     *  A 16-bit integer containing all the necessary information.
     *
     *  Bits 0-5        From square index.\n
     *  Bits 6-11       From square index.\n
     *  Bit 12          Capture flag.\n
     *  Bit 13          Special bit 0.\n
     *  Bit 14          Special bit 1.\n
     *  Bit 15          Promotion flag.\n
     *
     *  See <a href="https://www.chessprogramming.org/Encoding_Moves">
     *  Chess Programming Wiki</a>.
     */
    uint16_t data;
public:
    /** Default constructor for move_t */
    move_t();

    /**
     *
     *  Parameterised constructor for move_t.
     *
     *  \param from         The index of the square to move from.
     *  \param to           The index of the square to move to.
     *  \param promotion    Boolean indicating promotion.
     *  \param capture      Boolean indicating capture.
     *  \param spec1        Special flag for promotion type,
     *                      double pawn push etc.
     *  \param spec2        Special flag for promotion type,
     *                      double pawn push etc.
     */
    move_t(uint8_t from, uint8_t to, bool promotion,
           bool capture, bool spec1, bool spec2);

    /**
     *  Construct the move explicitly from \ref data.
     *
     *  \param cons_data    16-bit unsigned integer representing the move data.
     */
    move_t(uint16_t cons_data);

    /**
     *  Get the from square of the move.
     *
     *  \return             8-bit unsigned integer representing the from square.
     */
    uint8_t from_sq() const;
    /**
     *  Get the from square of the move.
     *
     *  \return             8-bit unsigned integer representing the from square.
     */
    uint8_t to_sq() const;
    /**
     *  Get the flag data of the square.
     *
     *  \return             8-bit unsigned integer representing the move flags.
     */
    uint8_t flags() const;

    /**
     *  Whether or not the move is "quiet", in other words not a capture,
     *  a promotion, castling or a double pawn push.
     *
     *  \return             Boolean indicating whether the move is quiet.
     */
    bool is_quiet() const;

    /**
     *  Whether or not the move is a promotion.
     *
     *  \return             Boolean indicating whether the move is a prommotion.
     */
    bool is_promotion() const;

    /**
     *  If the move is a promotion, get the promoting piece.
     *
     *  \return             A \ref piece object indicating which piece the
     *                      pawn is being promoted to.
     */
    piece which_promotion() const;

    /**
     *  Alter the promotion type of the move.
     *
     *  \param pc           A \ref piece object to promote to.
     */
    void set_promotion(piece);

    /**
     *  Whether or not the move is a capture.
     *
     *  \return             Boolean indicating whether the move is a capture.
     */
    bool is_capture() const;

    /**
     *  Whether or not the move is an en-passant capture.
     *
     *  \return             Boolean indicating whether the move is an
     *                      en-passant capture.
     */
    bool is_ep_capture() const;

    /**
     *  Whether or not the move is a double pawn push.
     *
     *  \return             Boolean indicating whether the move is a
     *                      double pawn push.
     */
    bool is_doublePP() const;

    /**
     *  Whether or not the move is a king-side castle.
     *
     *  \return             Boolean indicating whether the move is a
     *                      king-side castle.
     */
    bool is_kingCastle() const;

    /**
     *  Whether or not the move is a queen-side castle.
     *
     *  \return             Boolean indicating whether the move is a
     *                      queen-side castle.
     */
    bool is_queenCastle() const;

    /**
     *  Whether or not the move is a castling move.
     *
     *  \return             Boolean indicating whether the move is a
     *                      castling move.
     */
    bool is_castle() const;

    /**
     *  Get the 16-bit integer representing the move data.
     *
     *  \return             16-bit integer representing the move data.
     */
    uint16_t give() const;

    /**
     *  Print the move to an output stream in the form FileRankFileRank,
     *  e.g. e2e4.
     *
     *  \param out          The output stream to print to. Defaults to
     *                      the standard output stream std::cout
     *  \param move         The move to print.
     *  \return             The output stream.
     */
    friend std::ostream& operator<<(std::ostream& out, const move_t& move);

    /**
     *  Equality comparison operator for move_t.
     *
     *  \param self         LHS move_t object to compare.
     *  \param other        RHS move_t object to compare.
     *  \return             True if the moves are equal, false otherwise.
     */
    friend bool operator==(const move_t& self, const move_t& other);

    /**
     *  Inequality comparison operator for move_t.
     *
     *  \param self         LHS move_t object to compare.
     *  \param other        RHS move_t object to compare.
     *  \return             False if the moves are equal, true otherwise.
     */
    friend bool operator!=(const move_t& self, const move_t& other);
};


/**
 *  Convert a string to a move_t object given an array of possible moves.
 *
 *  \param moves            An array of possible moves.
 *  \param n_moves          The length of the move array.
 *  \param s                The string to convert.
 *  \return                 A move_t object corresponding to the move string.
 */
move_t stom(move_t* moves, int n_moves, std::string s);


/**
 *  Calculate the pseudo legal pawn pushes from a given square.
 *
 *  \param sq               The square to push from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible pushes.
 */
bitboard pawnPushNaive        (int sq, bitboard blockers, colour movingColour);
/**
 *  Calculate the naive pawn attacks from a given square (not taking into
 *  account the other pieces).
 *
 *  \param sq               The square to attack from.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible attacks.
 */
bitboard pawnAttackNaive      (int sq, colour movingColour);
/**
 *  Calculate the pseudo-legal pawn attacks from a given square.
 *
 *  \param sq               The square to attack from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible attacks.
 */
bitboard pawnAttacks          (int sq, bitboard _white, bitboard _black,
                               colour movingColour);
/**
 *  Calculate all pseudo-legal pawn moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard pawnTargets          (int sq, bitboard _white, bitboard _black,
                               colour movingColour);

/**
 *  Calculate the naive knight moves from a given square (not taking into
 *  account the other pieces).
 *
 *  \param sq               The square to move from.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard knightPushNaive      (int sq);
/**
 *  Calculate all pseudo-legal knight moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard knightTargets        (int sq, bitboard _white, bitboard _black,
                               colour movingColour);

/**
 *  Calculate the naive king moves from a given square (not taking into
 *  account the other pieces).
 *
 *  \param sq               The square to move from.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard kingPushNaive        (int sq);
/**
 *  Calculate all pseudo-legal king moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard kingTargets          (int sq, bitboard _white, bitboard _black,
                               colour movingColour);

/**
 *  Calculate the naive bishop moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard bishopPushNaive      (int sq, bitboard blockers);
/**
 *  Calculate all pseudo-legal bishop moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard bishopTargets        (int sq, bitboard _white, bitboard _black,
                               colour movingColour);

/**
 *  Calculate the naive rook moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard rookPushNaive        (int sq, bitboard blockers);
/**
 *  Calculate all pseudo-legal rook moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard rookTargets          (int sq, bitboard _white, bitboard _black,
                               colour movingColour);

/**
 *  Calculate the naive queen moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard queenPushNaive       (int sq, bitboard blockers);
/**
 *  Calculate all pseudo-legal queen moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard queenTargets         (int sq, bitboard _white, bitboard _black,
                               colour movingColour);

/**
 *  Calculate all pseudo-legal moves from a given square for a given piece.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param cP               The colour/piece combination to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard pieceTargets         (int sq, bitboard _white, bitboard _black,
                               colourPiece cP);

/**
 *  Calculate all squares under attack by a particular side.
 *  Note: legality of attacks is not taken into consideration.
 *
 *  \param side             The attacking side.
 *  \param pieces           A 12-bitboard array of piece positions.
 *  \return                 A bitboard representing all the squares under
 *                          attack by side.
 */
bitboard allTargets           (colour side, bitboard* pieces);



} // end of chessCore namespace


#endif
