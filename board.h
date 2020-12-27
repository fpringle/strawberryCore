/* Copyright 2020 Freddy Pringle */
#ifndef SRC_CORE_BOARD_H_
#define SRC_CORE_BOARD_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "move.h"
#include "typedefs.h"


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
 *  \return                 white if side is black, else black.
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

    // methods

    /**
     *  Get the number of pieces of a particular type that remain on the board.
     *
     *  \param cp       The colour/piece combination to count. See \ref colourPiece.
     *  \return         The number of pieces cp that remain on the board.
     */
    int num_pieces_left(colourPiece cp) const;
    /**
     *  Get the total number of pieces that remain on the board.
     *
     *  \return         The number of pieces hat remain on the board.
     */
    int num_pieces_left() const;

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
    value_t evaluate_material(int phase) const;

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
    value_t evaluate_pieceSquareTables(int phase) const;

    /**
     *  Calculate the opening value of the board.
     *
     *  \return         The opening evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluateOpening() const;

    /**
     *  Calculate the endgame value of the board.
     *
     *  \return         The endgame evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluateEndgame() const;

    /**
     *  Calculate the phase of the game based on the number of pieces
     *  left on the board.
     *
     *  \return         An integer between 0 and 256 representing the phase
                        of the game. 0 corresponds to the opening and 256
                        corresponds to the endgame.
     */
    int getPhase() const;

    /**
     *  Get a bitboard representing the locations of the white pieces.
     *
     *  \return         A bitboard representing the locations of the white pieces.
     */
    bitboard whiteSquares() const;

    /**
     *  Get a bitboard representing the locations of the black pieces.
     *
     *  \return         A bitboard representing the locations of the black pieces.
     */
    bitboard blackSquares() const;
    /**
     *  Get a bitboard representing the locations of all pieces.
     *
     *  \return         A bitboard representing the locations of all the pieces.
     */
    bitboard takenSquares() const;
    /**
     *  Get a bitboard representing the locations of all empty squares.
     *
     *  \return         A bitboard representing the locations of all empty squares.
     */
    bitboard emptySquares() const;

    /**
     *  Add a move to a MoveList object, with the option to check if the move is legal.
     *  Used by \ref board::gen_moves and \ref board::gen_legal_moves.
     *
     *  \param dest             A pointer to the MoveList object.
     *  \param move             The move to add to the MoveList object.
     *  \param legal_check      Whether or not to check if the move is legal.
     */
    void add_moves(MoveList* dest, move_t move, bool legal_check) const;

    /**
     *  Generate all moves that get the side to move out of check.
     *
     *  \param side             The side in check.
     *  \param checkingPiece    The colour/piece combination checking the king.
     *  \param checkingInd      The square index of the checking piece.
     *  \param kingInd          The square index of the king under attack.
     *  \param double_check     Whether the king is under double check.
     *  \return                 A \ref MoveList object containing all the moves
     *                          that get side out of check.
     */
    MoveList get_out_of_check(colour side, piece checkingPiece, int checkingInd,
                              int kingInd, bool double_check) const;

    /**
     *  Determine whether a given side can get out of check.
     *  Equivalent to (get_out_of_check(...) > 0)
     *
     *  \param side             The side in check.
     *  \param checkingPiece    The colour/piece combination checking the king.
     *  \param checkingInd      The square index of the checking piece.
     *  \param kingInd          The square index of the king under attack.
     *  \param double_check     Whether the king is under double check.
     *  \return                 True if side can get out of check, false otherwise.
     */
    bool can_get_out_of_check(colour side, piece checkingPiece,
                         int checkingInd, int kingInd, bool double_check) const;

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
    board(const board& other);

    /**
     *  \brief FEN constructor for board.
     *
     *  Construct a board from a string in Forsyth-Edwards Notation (FEN),
     *  a standard way of representing the state of the board.
     *
     *  \param fen      The string representing the required board state
     *                  in FEN format.
     */
    explicit board(std::string fen);

    /**
     *  Equality comparison operator for board.
     *
     *  \param other        RHS board object to compare.
     *  \return             True if the moves are equal, false otherwise.
     */
    bool operator==(const board& other) const;

    /**
     *  Inequality comparison operator for board.
     *
     *  \param other        RHS board object to compare.
     *  \return             False if the moves are equal, true otherwise.
     */
    bool operator!=(const board& other) const;

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
    void getCastlingRights(bool* dest) const;

    /**
     *  Get a copy of the en-passant boolean.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getEP(bool* dest) const;

    /**
     *  Get a copy of the double pawn push file.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getdPPFile(int* dest) const;

    /**
     *  Get a copy of the half-move clock.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getClock(uint8_t* dest) const;

    /**
     *  Get a copy of the full-move clock.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getFullClock(uint8_t* dest) const;

    /**
     *  Get a copy of the side to move.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getSide(colour* dest) const;

    /**
     *  Get a copy of the Zobrist hash value.
     *
     *  \param[out] dest    The location to copy to.
     */
    void getHash(uint64_t* dest) const;

    /**
     *  Get the Zobrist hash of a child node without having to
     *  make the move.
     *
     *  \param move         The move corresponding to the child node.
     *  \return             The hash value of the child node.
     */
    uint64_t childHash(move_t move) const;

    // evaluation
    // defined in eval.cpp

    /**
     *  Get the interpolated phase-dependent value of the board.
     *
     *  \return         The phase-dependent evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t getValue() const;

    /**
     *  Get the opening value of the board.
     *
     *  \return         The opening evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t getOpeningValue() const;

    /**
     *  Get the endgame value of the board.
     *
     *  \return         The endgame evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t getEndgameValue() const;

    /**
     *  Calculate the interpolated phase-dependent value of the board.
     *
     *  \return         The phase-dependent evaluation of the game state.
     *                  A positive value means that white is winning,
     *                  a negative value means that black is winning.
     *                  See \ref value_t.
     */
    value_t evaluate() const;

    /**
     *  Print the state of the pieces on the board, white side down.
     *
     *  \param cout     The output stream to print to. Defaults to the
     *                  standard ouput stream std::cout.
     */
    void print_board(std::ostream& cout = std::cout) const;

    /**
     *  Print the state of the pieces on the board, black side down.
     *
     *  \param cout     The output stream to print to. Defaults to the
     *                  standard ouput stream std::cout.
     */
    void print_board_flipped(std::ostream& cout = std::cout) const;

    /**
     *  Print the state of the pieces on the board, with added information
     *  e.g castling rights, move clocks etc.
     *
     *  \param cout     The output stream to print to. Defaults to the
     *                  standard ouput stream std::cout.
     */
    void print_all(std::ostream& cout = std::cout) const;

    /**
     *  Represent the board state as a string in Forsyth-Edwards Notation (FEN).
     *
     *  \return         A string representing the board in FEN format.
     */
    std::string FEN() const;

    /**
     *  Represent a move to be played as a string in Standard Algebraic
     *  Notation (SAN).
     *
     *  \param move     The move to represented
     *  \return         A string representing the move in SAN format.
     */
    std::string SAN_pre_move(move_t move) const;

    /**
     *  Represent a move that has just been played as a string in Standard
     *  Algebraic Notation (SAN).
     *
     *  \param move     The move to represented
     *  \return         A string representing the move in SAN format.
     */
    std::string SAN_post_move(move_t move) const;

    /**
     *  Parse a string representing a move in Standard Algebraic Notation
     *  (SAN) and return a move_t object representing the move.
     *
     *  \param san      The string in SAN format.
     *  \return         A move_t object representing the move.
     */
    move_t move_from_SAN(std::string san);

    /**
     *  Set the side to move.
     *
     *  \param side         The colour to set.
     */
    void set_side(colour side);                     // delete

    /**
     *  Re-calculate the opening and endgame values.
     */
    void update_value();                            // private / delete

    /**
     *  Re-calculate the Zobrist hash.
     */
    void update_hash();                             // private / delete

    /**
     *  Generate all pseudo-legal moves from a given position.
     *
     *  \return                A \ref MoveList of generated moves.
     */
    MoveList gen_moves() const;

    /**
     *  Check if a particular move is legal in the current board state.
     *
     *  \param move             A move to check.
     *  \return                 True if the move is legal, false otherwise.
     */
    bool is_legal(move_t move) const;

    /**
     *  Generate all captures from a given position.
     *
     *  \return                 An integer corresponding to the number of captures
     *                          generated.
     */
    MoveList gen_captures() const;

    /**
     *  Generate all legal moves from a given position.
     *
     *  \return                 An integer corresponding to the number of moves
     *                          generated.
     */
    MoveList gen_legal_moves() const;

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
    uint64_t zobrist_hash() const;          // private

    /**
     *  Test if a given side is in check.
     *
     *  \param side             The colour to test.
     *  \return                 True if side is in check, false otherwise.
     */
    bool is_check(colour side) const;

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
                  int* checkingInd, bool* doubleCheck) const;       // private

    /**
     *  Test if the side to move is in checkmate.
     *
     *  \return                 True if side is in checkmate, false otherwise.
     */
    bool is_checkmate() const;

    /**
     *  Test if the game is a stalemate.
     *
     *  \return                 True if stalemate, false otherwise.
     */
    bool is_stalemate() const;

    /**
     *  Test if the game is over, whether by checkmate or stalemate.
     *
     *  \return                 True if game over, false otherwise.
     */
    bool gameover() const;

    /**
     *  Test if the last played move was checking.
     *
     *  \param lastmove         The last move played.
     *  \return                 True if move was checking, false otherwise.
     */
    bool was_lastmove_check(move_t lastmove) const;         // private

    /**
     *  Test if a move will be checking.
     *
     *  \param move             A move to be played.
     *  \return                 True if move is checking, false otherwise.
     */
    bool is_checking_move(move_t move) const;               // private
};


}   // namespace chessCore


#endif  // SRC_CORE_BOARD_H_
