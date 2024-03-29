/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
#ifndef SRC_CORE_SEARCH_H_
#define SRC_CORE_SEARCH_H_

#include <limits>

#include "board.h"
#include "move.h"
#include "typedefs.h"


namespace chessCore {

/**
 *  \enum valueType
 *
 *  An Enum to specify the bound flag in the transposition table.
 */
enum valueType {
    EXACT,
    LOWER,
    UPPER
};

/**
 *  \struct record_t
 *
 *  \brief A struct for recording search details, used in the transposition table.
 */
struct record_t {
    /** The hash value of the corresponding board state. */
    uint64_t signature;
    /** If available, the best move for this node. Leave = 0 if not. */
    move_t best_move;
    /** The depth to which this node has been searched. */
    uint8_t depth;
    /** The bound or value for this node. */
    value_t score;
    /** The full move clock of the game when this node was searched. */
    uint8_t age;
    /**
     *  A flag to specify whether the score is exact, a lower bound,
     *  or an upper bound. See \ref valueType.
     */
    valueType flag;
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
 *  \class Searcher
 *  \brief A class to do all of the searching for the chess engine.
 */
class Searcher {
 private:
    /**
     *  The transposition table for the Searcher object.
     */
    TransTable* trans_table;

    /** The time when we started the current search. */
    clock_t search_start_time;

    /** The time when the current search will end. */
    clock_t search_end_time;

    /**
     *  Set the search timeout.
     *
     *  \param time     The number of seconds to search for.
     */
    void set_timeout(int time);

    /** End the search, i.e. set the timeout to 0. Not yet thread-safe. */
    void kill_search();

    /**
     *  Prune all entries from the transposition table that are older
     *  than a given value.
     *
     *  \param age  Discard any entries older than age.
     */
    void prune_table(uint8_t age);

    /**
     *  \brief Perform a quiescence search on a position.
     *
     *  Quiescence search is a technique to mitigate the horizon effect,
     *  where we recursively search through all the upcoming capture tradeoffs
     *  to get a better heuristic value for a given position.
     *
     *  \param b            The board to analyse.
     *  \param alpha        The current value of alpha in negamax search.
     *  \param beta         The current value of beta in negamax search.
     *  \return             The heuristic value for the node.
     */
    value_t quiesce(Board* b, value_t alpha, value_t beta);

    /**
     *  Search using Principal Variation search, to
     *  estimate the value of the given node.
     *
     *  \param b                The board state of the node to be searched.
     *  \param depth            The depth to search to.
     *  \param alpha            The current value of alpha.
     *  \param beta             The current value of beta.
     *  \param first_move       If given, search this move first.
     *  \return                 The estimated value of node b.
     */
    value_t principal_variation(Board* b, uint8_t depth,
                              value_t alpha, value_t beta,
                              move_t first_move = 0);

    /**
     *  Search using the Negamax algorithm with alpha-beta pruning, to
     *  estimate the value of the given node.
     *
     *  \param b                The board state of the node to be searched.
     *  \param depth            The depth to search to.
     *  \param alpha            The current value of alpha.
     *  \param beta             The current value of beta.
     *  \param first_move       If given, search this move first.
     *  \return                 The estimated value of node b.
     */
    value_t negamax_alphabeta(Board* b, uint8_t depth,
                              value_t alpha, value_t beta,
                              move_t first_move = 0);

    /**
     *  Search using the Negamax algorithm to increasing depth, to
     *  choose the best move from the current node.
     *
     *  \param b            The board state of the node to be searched.
     *  \param timeout      The maximum time to spend searching.
     *  \param cutoff       A boolean indicating whether or not to
     *                      end the search early if we find the same
     *                      best move several times in a row.
     *  \return             The best move to play from the current node.
     */
    move_t iterative_deepening_negamax(Board* b,
                                       int timeout,
                                       bool cutoff = false);

    /**
     *  Search using the Prinicpal Variation algorithm to increasing depth, to
     *  choose the best move from the current node.
     *
     *  \param b            The board state of the node to be searched.
     *  \param timeout      The maximum time to spend searching.
     *  \param cutoff       A boolean indicating whether or not to
     *                      end the search early if we find the same
     *                      best move several times in a row.
     *  \return             The best move to play from the current node.
     */
    move_t iterative_deepening_pv(Board* b,
                                  int timeout,
                                  bool cutoff = false);

 public:
    /**
     *  Default constructor for Searcher.
     */
    Searcher();

    /**
     *  Constructor for Searcher. Supply an existing transposition table.
     *
     *  \param tt           A pointer to the transposition table to use.
     */
    explicit Searcher(TransTable* tt);

    /** Destructor for Searcher. */
    virtual ~Searcher();

    /**
     *  Search from a node for the best move to play.
     *
     *  \param b            The board state of the node to be searched.
     *  \param timeout      The maximum time to spend searching.
     *  \param cutoff       A boolean indicating whether or not to
     *                      end the search early if we find the same
     *                      best move several times in a row.
     *  \return             The best move to play from the current node.
     */
    move_t search(Board* b, int timeout, bool cutoff = false);
};


}   // namespace chessCore


#endif  // SRC_CORE_SEARCH_H_
