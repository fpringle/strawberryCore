/*
 * A header file to define a "board" class to represent
 * the current state of the chessboard.
 *
 * We are using an unsigned 64-bit integer to represent
 * the positions of each type of piece. e.g. if the 10th
 * bit of whiteKnight is set to 1, there is a white knight
 * at position 10.
 *
 * For the square-mapping we use Little-Endian Rank-File,
 * so 0 = A1, 7 = H1, 56 = A8, 63 = H8
 *
 */

#ifndef __BOARD_H
#define __BOARD_H

#include "typedefs.h"
#include "move.h"

#include <cstdint>
#include <iostream>
#include <string>


/**
 *  \namespace chessCore
 *  \brief The main namespace for the core chess engine.
 *
 *  The namespace chessCore contains all the classes, functions, constants etc
 *  used by the core chess engine.
 */
namespace chessCore {

/**
 *
 *  Convenience function for getting the opposing side.
 *
 *  \param side             The colour to invert.
 *  \return                 \ref white if side is \ref black, else \black.
 */
colour flipColour(colour side);

// declare starting bitboards
// defined in board.cpp
/**@{*/
/** Bitboards representing the starting positions. */
extern bitboard whitePawnStart;
extern bitboard whiteRookStart;
extern bitboard whiteKnightStart;
extern bitboard whiteBishopStart;
extern bitboard whiteQueenStart;
extern bitboard whiteKingStart;
extern bitboard blackPawnStart;
extern bitboard blackRookStart;
extern bitboard blackKnightStart;
extern bitboard blackBishopStart;
extern bitboard blackQueenStart;
extern bitboard blackKingStart;
/**@}*/

// define symbols for string representation
// defined in board.cpp

/** Latin letters representing the twelve piece types. */
extern const char symbols[12];
/** Latin letters representing the two colours. */
extern const char colours[2];


// defined in board.cpp
/**
 *  \brief A utility/debug function for printing bitboards.
 *  \param bb the bitboard to print.
 *  \param c the character to print. Defaults to 'x'.
 *  \param cout the output stream to print to.
 */
void print_bb(bitboard bb, char c = 'x', std::ostream& cout = std::cout);


/**
 *  \class board
 *  \brief Represents the state of the chessboard.
 *
 *  The board class completely represents the state of the chessboard at any given time.
 *  Contains all information such as piece positions, castling rights, half- and full-
 *  move clock, whether the last move was a double pawn push and if so on which file,
 *  the side to move, a heuristic of the value at this state, and the board hash.
 *  Provides access and modification of information, as well as move generation,
 *  position evaluation, hashing, various forms of output, and getting check and
 *  checkmate information.
 */
class board {

    /**
     *  \brief An array of 12 bitboards representing the positions of each piece.
     *
     *  The positions of each of the 12 colour/piece combinations (e.g. black queen)
     *  is represented by a 64-bit integer (see \ref bitboard).
     */
    bitboard pieceBoards[12];

//    /** \name Castling Rights */
    /**@{*/
    /** The castling rights. */
    bool castleWhiteKingSide;
    bool castleWhiteQueenSide;
    bool castleBlackKingSide;
    bool castleBlackQueenSide;
    /**@}*/

    /**@{*/
    /** The half- and full- move clocks.*/
    uint8_t halfMoveClock;
    uint8_t fullMoveClock;
    /**@}*/

    /**@{*/
    /**
     *  \brief The double pawn push information for en-passant capture
     *
     *  lastMoveDoublePawnPush is a boolean indicating whether or not the last
     *  move was a double pawn push. If true, then dPPFile represents the
     *  file of the pawn that was double-pushed.
     */
    bool lastMoveDoublePawnPush;
    int dPPFile;
    /**@}*/

    /** \brief The side whose turn it is.
     *
     *  See \ref colour.
     */
    colour sideToMove;

    /**@{*/
    /**
     *  \brief Running integers to keep track of the board value.
     *
     *  We use an incremental evaluation strategy. Two values are kept,
     *  corresponding to the opening and endgame. The real value is then
     *  calculated by interpolating between them using a measure of the
     *  stage of the game, based on the number of pieces still on the board.
     *  See \ref value_t and \ref eval.h.
     */
    value_t opening_value;
    value_t endgame_value;
    /**@}*/

    /**
     *  \brief A running integer to keep track of the Zobrist hash value.
     *
     *  See \ref hash.h.
     */
    uint64_t hash_value;

public:
    // constructors
    // defined in board.cpp
    /**
     *  \brief The default constructor for board.
     *
     *  Construct the default board with starting bitboards, all
     *  castling rights, value 0 and generate the hash.
     */
    board();

    /**
     *  \brief Parameterised constructor for board.
     *
     *  \param startPositions   An array of 12 bitboards representing the
     *                          positions of the pieces. See \ref pieceBoards.
     *  \param castling         An array of 4 booleans representing the
     *                          castling rights for each side. See
     *                          \ref castleWhiteKingSide,
     *                          \ref castleWhiteQueenSide,
     *                          \ref castleBlackKingSide and
     *                          \ref castleBlackQueenSide.
     *  \param ep               A boolean indicating whether the last move
     *                          was a double pawn push. See
     *                          \ref lastMoveDoublePawnPush.
     *  \param dpp              An integer represents the file of the pawn
     *                          that was double-pushed (if relevant).
     *                          See \ref dPPFile.
     *  \param clock            The half-move clock. See \ref halfMoveClock.
     *  \param full_clock       The full-move clock. See \ref fullMoveClock.
     *  \param side             The side whose turn it is. See \ref sideToMove.
     *  \param open_val         Evaluation corresponding to the opening.
     *                          See \ref opening_value.
     *  \param end_val          Evaluation corresponding to the endgame.
     *                          See \ref endgame_value.
     *  \param hash             The Zobrist hash. See \ref hash_value.
     */
    board(bitboard * startPositions, bool * castling, bool ep, int dpp,
             uint8_t clock, uint8_t full_clock, colour side,
             value_t open_val, value_t end_val, uint64_t hash = 0);

    /**
     *  \brief Copy constructor for board.
     *
     *  \param other    The board object to be copied.
     */
    board(board& other);

    /**
     *  \brief FEN constructor for board.
     *
     *  Construct a board from a string in Forsyth-Edwards Notation (FEN),
     *  a standard way of representing the state of the board.
     *
     *  \param fen      The string representing the required board state
     *                  in FEN format.
     */
    board(std::string fen);

    /**
     *  Equality comparison operator for board.
     *
     *  \param other        RHS board object to compare.
     *  \return             True if the moves are equal, false otherwise.
     */
    bool operator==(const board& other);

    /**
     *  Inequality comparison operator for board.
     *
     *  \param other        RHS board object to compare.
     *  \return             False if the moves are equal, true otherwise.
     */
    bool operator!=(const board& other);

    /**
     *  Print the board to an output stream.
     *
     *  \param out          The output stream to print to. Defaults to
     *                      the standard output stream std::cout
     *  \param brd          The board to print.
     *  \return             The output stream.
     */
    friend std::ostream& operator<<(std::ostream& out, const board& brd);

    /**
     *  Assignment operator for board.
     *
     *  \param other        RHS board object to copy.
     *  \return             Self.
     */
    board& operator=(const board& other);

    /**
     *  Get a copy of the piece bitboards.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getBitboards(bitboard* dest) const;

    /**
     *  Get a copy of the castling rights.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getCastlingRights(bool* dest);

    /**
     *  Get a copy of the en-passant boolean.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getEP(bool* dest);

    /**
     *  Get a copy of the double pawn push file.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getdPPFile(int* dest);

    /**
     *  Get a copy of the half-move clock.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getClock(uint8_t* dest);

    /**
     *  Get a copy of the full-move clock.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getFullClock(uint8_t* dest);

    /**
     *  Get a copy of the side to move.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getSide(colour* dest);

    /**
     *  Get a copy of the Zobrist hash value.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getHash(uint64_t* dest);

    /**
     *  Get the number of pieces of a particular type that remain on the board.
     *
     *  \param cp       The colour/piece combination to count. See \ref colourPiece.
     *  \return         The number of pieces cp that remain on the board.
     */
    int num_pieces_left(colourPiece cp);
    /**
     *  Get the total number of pieces that remain on the board.
     *
     *  \return         The number of pieces hat remain on the board.
     */
    int num_pieces_left();

    // evaluation
    // defined in eval.cpp
    /**
     *  Evaluate the board based on the number of pieces left on each side.
     *
     *  \param phase    The phase of the game. 0 corresponds to the opening,
     *                                         1 corresponds to the endgame.
     *  \return         The phase-dependent evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluate_material(int phase);

    /**
     *  Evaluate the board based on the placement of pieces left on each side.
     *
     *  \param phase    The phase of the game. 0 corresponds to the opening,
     *                                         1 corresponds to the endgame.
     *  \return         The phase-dependent evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluate_pieceSquareTables(int phase);

    /**
     *  Get the interpolated phase-dependent value of the board.
     *
     *  \return         The phase-dependent evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t getValue();

    /**
     *  Get the opening value of the board.
     *
     *  \return         The opening evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t getOpeningValue();

    /**
     *  Get the endgame value of the board.
     *
     *  \return         The endgame evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t getEndgameValue();

    /**
     *  Calculate the interpolated phase-dependent value of the board.
     *
     *  \return         The phase-dependent evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluate();

    /**
     *  Calculate the opening value of the board.
     *
     *  \return         The opening evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluateOpening();

    /**
     *  Calculate the endgame value of the board.
     *
     *  \return         The endgame evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluateEndgame();

    /**
     *  Calculate the phase of the game based on the number of pieces
     *  left on the board.
     *
     *  \return         An integer between 0 and 256 representing the phase
                        of the game. 0 corresponds to the opening and 256
                        corresponds to the endgame.
     */
    int getPhase();

    /**
     *  Print the state of the pieces on the board, white side down.
     *
     *  \param cout     The output stream to print to. Defaults to the
     *                  standard ouput stream std::cout.
     */
    void print_board(std::ostream& cout = std::cout);

    /**
     *  Print the state of the pieces on the board, black side down.
     *
     *  \param cout     The output stream to print to. Defaults to the
     *                  standard ouput stream std::cout.
     */
    void print_board_flipped(std::ostream& cout = std::cout);

    /**
     *  Print the state of the pieces on the board, with added information
     *  e.g castling rights, move clocks etc.
     *
     *  \param cout     The output stream to print to. Defaults to the
     *                  standard ouput stream std::cout.
     */
    void print_all(std::ostream& cout = std::cout);

    /**
     *  Represent the board state as a string in Forsyth-Edwards Notation (FEN).
     *
     *  \return         A string representing the board in FEN format.
     */
    std::string FEN();

    /**
     *  Represent a move to be played as a string in Standard Algebraic
     *  Notation (SAN).
     *
     *  \param move     The move to represented
     *  \return         A string representing the move in SAN format.
     */
    std::string SAN_pre_move(move_t move);

    /**
     *  Represent a move that has just been played as a string in Standard
     *  Algebraic Notation (SAN).
     *
     *  \param move     The move to represented
     *  \return         A string representing the move in SAN format.
     */
    std::string SAN_post_move(move_t move);
    //    move_t move_from_SAN(std::string);

    /**
     *  Set a given piece at a given square index.
     *
     *  \param cP           The colour/piece to set.
     *  \param pos          The square index to set.
     */
    void set_piece(colourPiece cP, int pos);

    /**
     *  Set the side to move.
     *
     *  \param side         The colour to set.
     */
    void set_side(colour side);

    /**
     *  Clear a given square index.
     *
     *  \param ind          The square index to clear.
     */
    void clear_square(int ind);

    /**
     *  Re-calculate the opening and endgame values.
     */
    void update_value();

    /**
     *  Re-calculate the Zobrist hash.
     */
    void update_hash();

    /**
     *  Set the Zobrist hash.
     *
     *  \param hash         The 64-bit hash to set.
     */
    void set_hash(uint64_t hash);

    /**
     *  Set the bitboards representing the piece positions.
     *
     *  \param bb           An array of bitboards.
     */
    void setBitboards(bitboard* bb);

    /**
     *  Set the castling rights.
     *
     *  \param castling     An array of booleans.
     */
    void setCastlingRights(bool* castling);

    /**
     *  Set the en-passant boolean.
     *
     *  \param ep           The boolean to set.
     */
    void setEP(bool ep);

    /**
     *  Set the double pawn push file.
     *
     *  \param file         An integer representing the file of the
     *                      double pawn push in the last move.
     */
    void setdPPFile(int file);

    /**
     *  Set the half-move clock.
     *
     *  \param clk          The clock value to set.
     */
    void setClock(uint8_t clk);

    /**
     *  Set the full-move clock.
     *
     *  \param clk          The clock value to set.
     */
    void setFullClock(uint8_t clk);

    /**
     *  Get a bitboard representing the locations of the white pieces.
     *
     *  \return         A bitboard representing the locations of the white pieces.
     */
    bitboard whiteSquares();

    /**
     *  Get a bitboard representing the locations of the black pieces.
     *
     *  \return         A bitboard representing the locations of the black pieces.
     */
    bitboard blackSquares();
    /**
     *  Get a bitboard representing the locations of all pieces.
     *
     *  \return         A bitboard representing the locations of all the pieces.
     */
    bitboard takenSquares();
    /**
     *  Get a bitboard representing the locations of all empty squares.
     *
     *  \return         A bitboard representing the locations of all empty squares.
     */
    bitboard emptySquares();

    /**
     *  Add a move to an array, with the option to check if the move is legal.
     *  Used by \ref board::gen_moves and \ref board::gen_legal_moves.
     *
     *  \param dest             A pointer to the array of moves.
     *  \param move             The move to add to the array.
     *  \param legal_check      Whether or not to check if the move is legal.
     *  \return                 A boolean indicating whether the move was added
     *                          successfully. Automatically returns true if
     *                          check_legal is false.
     */
    bool add_moves(move_t** dest, move_t move, bool legal_check);

    /**
     *  Generate all pseudo-legal moves from a given position.
     *
     *  \param moves            An empty array of moves.
     *  \return                 An integer corresponding to the number of moves
     *                          generated.
     */
    int gen_moves(move_t* moves);

    /**
     *  Check if a particular move is legal in the current board state.
     *
     *  \param move             A move to check.
     *  \return                 True if the move is legal, false otherwise.
     */
    bool is_legal(move_t move);

    /**
     *  Generate all captures from a given position.
     *
     *  \param moves            An empty array of moves.
     *  \return                 An integer corresponding to the number of captures
     *                          generated.
     */
    int gen_captures(move_t* moves);

    /**
     *  Generate all moves that get the side to move out of check.
     *
     *  \param moves            An empty array of moves.
     *  \param checkingPiece    The colour/piece combination checking the king.
     *  \param checkingInd      The square index of the checking piece.
     *  \param kingInd          The square index of the king under attack.
     *  \param double_check     Whether the king is under double check.
     *  \return                 An integer corresponding to the number of moves
     *                          generated.
     */
    int get_out_of_check(move_t* moves, piece checkingPiece, int checkingInd,
                         int kingInd, bool double_check);

    /**
     *  Generate all legal moves from a given position.
     *
     *  \param moves            An empty array of moves.
     *  \return                 An integer corresponding to the number of moves
     *                          generated.
     */
    int gen_legal_moves(move_t* moves);

    /**
     *  Do a move in-place.
     *
     *  \param move             The move to make.
     */
    void doMoveInPlace(move_t move);

    /**
     *  Calculate the Zobrist hash of the board state.
     *  See \ref hash.h.
     *
     *  \return                 An unsigned 64-bit integer representing the
                                Zobrist hash of the board.
     */
    uint64_t zobrist_hash();

    /**
     *  Test if a given side is in check.
     *
     *  \param side             The colour to test.
     *  \return                 True if side is in check, false otherwise.
     */
    bool is_check(colour side);

    /**
     *  Test if a given side is in check. If so, output the type and location
     *  of the checking piece, and whether the king is in double check.
     *
     *  \param[in] side                 The colour to test.
     *  \param[out] checkingPiece       Output the checking piece.
     *  \param[out] checkingInd         Output the square index of
     *                                  the checking piece.
     *  \param[out] doubleCheck         Output a boolean indicating whether
     *                                  the king is in double check.
     *  \return                         True if side is in check, false otherwise.
     */
    bool is_check(colour side, piece* checkingPiece,
                  int* checkingInd, bool* doubleCheck);

    /**
     *  Test if a given side is in checkmate.
     *
     *  \param side             The colour to test.
     *  \return                 True if side is in checkmate, false otherwise.
     */
    bool is_checkmate(colour side);

    /**
     *  Test if either side is in checkmate.
     *
     *  \param side             The colour to test.
     *  \return                 1  if white is in checkmate,\n
     *                          -1 if black is in checkmate,\n
     *                          0  otherwise.
     */
    int is_checkmate();

    /**
     *  Test if the game is a stalemate.
     *
     *  \return                 True if stalemate, false otherwise.
     */
    bool is_stalemate();

    /**
     *  Test if the game is over, whether by checkmate or stalemate.
     *
     *  \return                 True if game over, false otherwise.
     */
    bool gameover();

    /**
     *  Test if the last played move was checking.
     *
     *  \param lastmove         The last move played.
     *  \return                 True if move was checking, false otherwise.
     */
    bool was_lastmove_check(move_t lastmove);

    /**
     *  Test if a move will be checking.
     *
     *  \param move             A move to be played.
     *  \return                 True if move is checking, false otherwise.
     */
    bool is_checking_move(move_t move);
};


} // end of chessCore namespace


#endif
