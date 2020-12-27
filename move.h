/* Copyright 2020 Freddy Pringle */
#ifndef SRC_CORE_MOVE_H_
#define SRC_CORE_MOVE_H_

#include <cstdint>
#include <iostream>
#include <string>

#include "typedefs.h"
#include "twiddle.h"


namespace chessCore {
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

/**
 *
 *  Construct a move_t object from the relevant parameters.
 *
 *  \param from         The index of the square to move from.
 *  \param to           The index of the square to move to.
 *  \param promotion    Boolean indicating promotion.
 *  \param capture      Boolean indicating capture.
 *  \param spec1        Special flag for promotion type,
 *                      double pawn push etc.
 *  \param spec0        Special flag for promotion type,
 *                      double pawn push etc.
 *  \return             A move_t object representing the move.
 */
move_t make_move(uint8_t from, uint8_t to, bool promotion,
       bool capture, bool spec1, bool spec0);
/**
 *  Get the from square of the move.
 *
 *  \param move         move_t object representing the move.
 *  \return             8-bit unsigned integer representing the from square.
 */
uint8_t from_sq(move_t move);

/**
 *  Get the from square of the move.
 *
 *  \param move         move_t object representing the move.
 *  \return             8-bit unsigned integer representing the from square.
 */
uint8_t to_sq(move_t move);

/**
 *  Get the flag data of the square.
 *
 *  \param move         move_t object representing the move.
 *  \return             8-bit unsigned integer representing the move flags.
 */
uint8_t flags(move_t move);

/**
 *  Whether or not the move is "quiet", in other words not a capture,
 *  a promotion, castling or a double pawn push.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is quiet.
 */
bool is_quiet(move_t move);

/**
 *  Whether or not the move is a promotion.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is a prommotion.
 */
bool is_promotion(move_t move);

/**
 *  If the move is a promotion, get the promoting piece.
 *
 *  \param move         move_t object representing the move.
 *  \return             A \ref piece object indicating which piece the
 *                      pawn is being promoted to.
 */
piece which_promotion(move_t move);

/**
 *  Alter the promotion type of the move.
 *
 *  \param move         move_t object representing the move.
 *  \param pc           A \ref piece object to promote to.
 *  \return             The move with the promotion piece set.
 */
move_t set_promotion(move_t move, piece pc);

/**
 *  Whether or not the move is a capture.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is a capture.
 */
bool is_capture(move_t move);

/**
 *  Whether or not the move is an en-passant capture.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is an
 *                      en-passant capture.
 */
bool is_ep_capture(move_t move);

/**
 *  Whether or not the move is a double pawn push.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is a
 *                      double pawn push.
 */
bool is_doublePP(move_t move);

/**
 *  Whether or not the move is a king-side castle.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is a
 *                      king-side castle.
 */
bool is_kingCastle(move_t move);

/**
 *  Whether or not the move is a queen-side castle.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is a
 *                      queen-side castle.
 */
bool is_queenCastle(move_t move);

/**
 *  Whether or not the move is a castling move.
 *
 *  \param move         move_t object representing the move.
 *  \return             Boolean indicating whether the move is a
 *                      castling move.
 */
bool is_castle(move_t move);

/**
 *  Convert a move_t object to a string representing the move
 *  in the form FileRankFileRank, e.g. e2e4.
 *
 *  \param move         The move to print.
 *  \return             A string representing move.
 */
std::string mtos(move_t move);

/**
 *  Convert a string to a move_t object given an array of possible moves.
 *
 *  \param moves            An \ref MoveList of possible moves.
 *  \param s                The string to convert.
 *  \return                 A move_t object corresponding to the move string.
 */
move_t stom(MoveList moves, std::string s);


/**
 *  Calculate the pseudo legal pawn pushes from a given square.
 *
 *  \param sq               The square to push from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible pushes.
 */
bitboard pawnPushNaive(int sq, bitboard blockers, colour movingColour);
/**
 *  Calculate the naive pawn attacks from a given square (not taking into
 *  account the other pieces).
 *
 *  \param sq               The square to attack from.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible attacks.
 */
bitboard pawnAttackNaive(int sq, colour movingColour);
/**
 *  Calculate the pseudo-legal pawn attacks from a given square.
 *
 *  \param sq               The square to attack from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible attacks.
 */
bitboard pawnAttacks(int sq, bitboard _white, bitboard _black,
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
bitboard pawnTargets(int sq, bitboard _white, bitboard _black,
                     colour movingColour);

/**
 *  Calculate the naive knight moves from a given square (not taking into
 *  account the other pieces).
 *
 *  \param sq               The square to move from.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard knightPushNaive(int sq);
/**
 *  Calculate all pseudo-legal knight moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard knightTargets(int sq, bitboard _white, bitboard _black,
                       colour movingColour);

/**
 *  Calculate the naive king moves from a given square (not taking into
 *  account the other pieces).
 *
 *  \param sq               The square to move from.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard kingPushNaive(int sq);
/**
 *  Calculate all pseudo-legal king moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard kingTargets(int sq, bitboard _white, bitboard _black,
                     colour movingColour);

/**
 *  Calculate the naive bishop moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard bishopPushNaive(int sq, bitboard blockers);
/**
 *  Calculate all pseudo-legal bishop moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard bishopTargets(int sq, bitboard _white, bitboard _black,
                       colour movingColour);

/**
 *  Calculate the naive rook moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard rookPushNaive(int sq, bitboard blockers);
/**
 *  Calculate all pseudo-legal rook moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard rookTargets(int sq, bitboard _white, bitboard _black,
                     colour movingColour);

/**
 *  Calculate the naive queen moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param blockers         A bitboard representing all pieces on the board.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard queenPushNaive(int sq, bitboard blockers);
/**
 *  Calculate all pseudo-legal queen moves from a given square.
 *
 *  \param sq               The square to move from.
 *  \param _white           Bitboard representing white pieces on the board.
 *  \param _black           Bitboard representing black pieces on the board.
 *  \param movingColour     The side to move.
 *  \return                 The bitboard representing the possible moves.
 */
bitboard queenTargets(int sq, bitboard _white, bitboard _black,
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
bitboard pieceTargets(int sq, bitboard _white, bitboard _black,
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
bitboard allTargets(colour side, bitboard* pieces);



}   // namespace chessCore


#endif  // SRC_CORE_MOVE_H_
