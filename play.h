/* Copyright 2020 Freddy Pringle */
#ifndef SRC_CORE_PLAY_H_
#define SRC_CORE_PLAY_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "move.h"
#include "search.h"
#include "typedefs.h"


namespace chessCore {


/**
 *  \class Player
 *  \brief Represents the computer player in a game of chess.
 *
 *  The Player class wraps around the \ref board class to provide an interface
 *  for the user to play against. It will compute the engine's move, keep track
 *  of the move history, and maintain a transposition table for lookup to speed
 *  up search.
 *
 *
 */
class Player : public board {
 private:
    /**
     *  A table storing the nodes that have been visited in previous searches.
     *  See \ref record_t.
     */
    TransTable trans_table;

    /** A pointer to the Searcher object for the engine to use. */
    Searcher* searcher;

    /**
     *  A record of all the previous moves.
     */
    std::vector<move_t> move_history;

    /**
     *  A record of all the previoius moves in SAN format.
     */
    std::vector<std::string> move_history_san;

    /**
     *  The side that the user is playing as.
     */
    colour user_colour;

    /**
     *  The maximum time the computer can use to search for each move.
     */
    int iterative_deepening_timeout;

 public:
    // constructors
    // defined in play.cpp
    /**
     *  \brief The default constructor for Player.
     *
     *  Construct the default board with starting bitboards, all
     *  castling rights, value 0, empty move history and transposition
     *  table, and generate the hash.
     */
    Player();

    /**
     *  \brief The default constructor for Player. Specify the user colour.
     *
     *  Construct the default board with starting bitboards, all
     *  castling rights, value 0, empty move history and transposition
     *  table, and generate the hash.
     *
     *  \param userColour           The colour the user will play as.
     */
    explicit Player(colour userColour);

    /**
     *  \brief Parameterised constructor for Player.
     *
     *  \param startPositions   An array of 12 bitboards representing the
     *                          positions of the pieces.
     *                          See \ref board::pieceBoards.
     *  \param castling         An array of 4 booleans representing the
     *                          castling rights for each side. See
     *                          \ref board::castleWhiteKingSide,
     *                          \ref board::castleWhiteQueenSide,
     *                          \ref board::castleBlackKingSide and
     *                          \ref board::castleBlackQueenSide.
     *  \param ep               A boolean indicating whether the last move
     *                          was a double pawn push. See
     *                          \ref board::lastMoveDoublePawnPush.
     *  \param dpp              An integer represents the file of the pawn
     *                          that was double-pushed (if relevant).
     *                          See \ref board::dPPFile.
     *  \param clock            The half-move clock.
     *                          See \ref board::halfMoveClock.
     *  \param full_clock       The full-move clock.
     *                          See \ref board::fullMoveClock.
     *  \param side             The side whose turn it is.
 *                              See \ref board::sideToMove.
     *  \param open_val         Evaluation corresponding to the opening.
     *                          See \ref board::opening_value.
     *  \param end_val          Evaluation corresponding to the endgame.
     *                          See \ref board::endgame_value.
     *  \param hash             The Zobrist hash.
     *                          See \ref board::hash_value.
     */
    Player(bitboard * startPositions, bool * castling, bool ep, int dpp,
             uint8_t clock, uint8_t full_clock, colour side,
             value_t open_val, value_t end_val, uint64_t hash = 0);

    /**
     *  \brief Copy constructor for Player.
     *
     *  \param other    The Player object to be copied.
     */
    Player(const Player& other);

    /**
     *  \brief FEN constructor for Player.
     *
     *  Construct the board from a string in Forsyth-Edwards Notation (FEN),
     *  a standard way of representing the state of the board.
     *
     *  \param fen      The string representing the required board state
     *                  in FEN format.
     */
    explicit Player(std::string fen);

    /**
     *  Set the search timeout.
     *
     *  \param timeout      The timeout in seconds.
     */
    void setTimeout(int timeout);

    /**
     *  Get the search timeout.
     *
     *  \return             The timeout in seconds.
     */
    int getTimeout();

    /**
     *  Read a config file.
     *
     *  \param filename         The path of the TOML config file.
     */
    void read_config(std::string filename);

    /**
     *  Access the move history.
     *
     *  \return                 A vector of the history in move_t formmat.
     */
    std::vector<move_t> getHistory() const;

    /**
     *  Access the move history in SAN format.
     *
     *  \return                 A vector of the history in SAN formmat.
     */
    std::vector<std::string> getHistorySAN() const;

    /**
     *  Access the transposition table.
     *
     *  \return                 A copy of the transposition table.
     */
    TransTable getTable() const;

    /**@{*/
    /**
     *  Pretty-print the private data.
     *
     *  \param cout             The output stream to print to.
     */
    void print_history(std::ostream& cout = std::cout) const;
    void print_history_san(std::ostream& cout = std::cout) const;
    void print_table(std::ostream& cout = std::cout);
    /**@}*/

    /**
     *  Print the board state to an output stream with the orientation
     *  depending on which side the user is playing.
     *
     *  \param cout             The output stream to print to.
     */
    void print_board(std::ostream& cout = std::cout) const;

    /**
     *  Save the transposition table to file.
     *
     *  \param filename         The name of the file to save to.
     */
    void save_table(std::string filename);              // private

    /**
     *  Load a transposition table from file.
     *
     *  \param filename         The name of the file to load from.
     */
    void load_table(std::string filename);              // private

    /**
     *  Do a move in-place.
     *
     *  \param move             The move to make.
     */
    void doMoveInPlace(move_t move);

    /**
     *  Make the result of a move in a given arbitrary.
     *
     *  \param child            A pointer to the board to transform.
     *  \param move             The move to make.
     */
    void makeChild(board* child, move_t move) const;    // private?

    /**
     *  Prompt the user for a move.
     *
     *  \return             The move the player has entered.
     */
    move_t input_move() const;

    /**
     *  Prompt the user for a move in SAN format.
     *
     *  \return             The move the player has entered.
     */
    move_t input_move_SAN();

    /**
     *  Search from the current node to get the best move.
     *
     *  \return             The best move to play from this position.
     */
    move_t search();

    /**
     *  Search from the current node to get the best move, overriding the
     *  default timeout.
     *
     *  \param timeout      The maximum time to spend searhcing.
     *  \return             The best move to play from this position.
     */
    move_t search(int timeout);

    /**
     *  Start a game between the user and the computer. Uses the class members
     *  \ref user_colour and \ref iterative_deepening_timeout.
     *
     */
    void play();

    /**
     *  Start a game between the user and the computer. The parameters
     *  playerSide and timeout override the class members \ref user_colour
     *  and \ref iterative_deepening_timeout, respectively.
     *
     *  \param playerSide   The user colour.
     *  \param timeout      The cutoff time for computer search per move.
     */
    void play(colour playerSide, int timeout);
};


/** Play out a game with both sides controlled by the user. */
void two_players();

/** Play out a game with both sides controlled by the engine. */
void two_computers();

}   // namespace chessCore

#endif  // SRC_CORE_PLAY_H_
