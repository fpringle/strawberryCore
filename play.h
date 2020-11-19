#ifndef __PLAY_H
#define __PLAY_H

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "board.h"
#include "move.h"
#include "tree.h"
#include "typedefs.h"


namespace chessCore {

// transposition table
// "signature" = zobrist hash (64 bits)
// best/refutation move (16 bits)
// depth (8 bits)
// score (integrated bound and values (16 bits)
// age (8 bits)
// total 112 bits
// -> 16 bits left over if we want to add something


/**
 *  \struct record_t
 *
 *  A struct for recording search details, used in the transposition table.
 */
struct record_t {
    /** The hash value of the corresponding board state. */
    uint64_t signature;
    /** If available, the best move for this node. Leave = 0 if not. */
    move_t best_ref_move;
    /** The depth to which this node has been searched. */
    uint8_t depth;
    /**
     *  The integrated bound and value for this node.
     *  See <a href="https://www.chessprogramming.org/Integrated_Bounds_and_Values">
     *  Chess Programming Wiki</a>.
     */
    value_t IBV_score;
    /** The full move clock of the game when this node was searched. */
    uint8_t age;
};

/**
 *  Pretty-print a record_t object to an output stream.
 *
 *  \param out      The output stream to print to.
 *  \param rec      The record_t object to print.
 *  \return         The output stream.
 */
std::ostream& operator<<(std::ostream& out, const record_t& rec);


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
     *  See \ref record_t
     */
    std::map<uint32_t, record_t> trans_table;

    /**
     *  A record of all the previous moves.
     */
    std::vector<move_t> move_history;

    /**
     *  A record of all the previoius moves in SAN format.
     */
    std::vector<std::string> move_history_san;
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
    Player(Player& other);

    /**
     *  \brief FEN constructor for Player.
     *
     *  Construct the board from a string in Forsyth-Edwards Notation (FEN),
     *  a standard way of representing the state of the board.
     *
     *  \param fen      The string representing the required board state
     *                  in FEN format.
     */
    Player(std::string fen);

    /**
     *  Lookup a node in the transposition table.
     *
     *  \param pos_hash         The hash value of the node to look up.
     *  \param dest             A pointer to the destination record_t.
     *  \return                 True if a record was found, false otherwise.
     */
    bool lookup(uint64_t pos_hash, record_t * dest);

    /**
     *  Access the move history.
     *
     *  \return                 A vector of the history in move_t formmat.
     */
    std::vector<move_t> getHistory();

    /**
     *  Access the move history in SAN format.
     *
     *  \return                 A vector of the history in SAN formmat.
     */
    std::vector<std::string> getHistorySAN();

    /**
     *  Access the transposition table.
     *
     *  \return                 A copy of the transposition table.
     */
    std::map<uint32_t, record_t> getTable();

    /**@{*/
    /**
     *  Pretty-print the private data.
     *
     *  \param cout             The output stream to print to.
     */
    void print_history(std::ostream& cout = std::cout);
    void print_history_san(std::ostream& cout = std::cout);
    void print_table(std::ostream& cout = std::cout);
    /**@}*/

    /**
     *  Save the transposition table to file.
     *
     *  \param filename         The name of the file to save to.
     */
    void save_state(std::string filename);

    /**
     *  Load a transposition table from file.
     *
     *  \param filename         The name of the file to load from.
     */
    void load_state(std::string filename);

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
    void makeChild(board* child, move_t move);

    /**
     *  Search using Principal Variation search, to
     *  estimate the value of the given node.
     *
     *  \param b            The board state of the node to be searched.
     *  \param side         The side to move.
     *  \param depth        The depth to search to.
     *  \param alpha        The current value of alpha.
     *  \param beta         The current value of beta.
     *  \return             The estimated value of node b.
     */
    value_t principal_variation(board b, colour side, uint8_t depth,
                                value_t alpha, value_t beta);

    /**
     *  Search using the Negamax algorithm with alpha-beta pruning, to
     *  estimate the value of the given node.
     *
     *  \param b            The board state of the node to be searched.
     *  \param side         The side to move.
     *  \param depth        The depth to search to.
     *  \param alpha        The current value of alpha.
     *  \param beta         The current value of beta.
     *  \return             The estimated value of node b.
     */
    value_t negamax_alphabeta(board b, colour side, uint8_t depth,
                              value_t alpha, value_t beta);

    /**
     *  Search using the Negamax algorithm to increasing depth, to
     *  choose the best move from the current node.
     *
     *  \param timeout      The maximum time to spend searching.
     *  \param max_depth    The maximum depth to search to.
     *  \return             The best move to play from the current node.
     */
    move_t iterative_deepening(int timeout, uint8_t max_depth = 100);

    /**
     *  Use Player::negamax_alphabeta to choose the best move to make
     *  from the current node.
     *
     *  \param depth        The depth to search to.
     *  \return             The best move to play from the current node.
     */
    move_t search_negamax_alphabeta(uint8_t depth);

    /**
     *  Use Player::negamax_alphabeta to choose the best move to make
     *  from the current node. Specify the first move to search and set
     *  a limit on the time available.
     *
     *  \param depth            The depth to search to.
     *  \param first_move       The first move to search.
     *  \param time_remaining   The amount of time available.
     *  \return                 The best move to play from the current node.
     */
    move_t search_negamax_alphabeta(uint8_t depth, move_t first_move, double time_remaining);

    /**
     *  Use Player::prinicpal_variation to choose the best move to make
     *  from the current node.
     *
     *  \param depth        The depth to search to.
     *  \return             The best move to play from the current node.
     */
    move_t search_principal_variation(uint8_t depth);

    /**
     *  Choose the best move to make from the current node.
     *
     *  \param depth        The depth to search to.
     *  \return             The best move to play from the current node.
     */
    move_t search(uint8_t depth);

    /**
     *  Prompt the user for a move.
     *
     *  \return             The move the player has entered.
     */
    move_t input_move();

    /**
     *  Start a game between the user and the computer.
     *
     *  \param side         The user colour.
     *  \param plies        The depth to search to.
     */
    void play(colour side, int plies);
};


} // end of chessCore namespace

#endif
