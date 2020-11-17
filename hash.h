#ifndef __HASH_H
#define __HASH_H

#include <cstdint>

namespace chessCore {

// n=312, w=64

/**@{*/
/** Some variables used for Zobrist hash. */
extern const int w, m, r, u, s, t, l;
extern const uint64_t n, a, d, b, c, f;
extern uint64_t MT[312];
extern uint64_t check_index;
extern uint64_t lower_mask;
extern uint64_t upper_mask;
extern uint64_t zobristKeys[781];
/**@}*/

/**
 *  Initialise the pseudo-random number generator (PRNG).
 *
 *  \param seed     The seed for the PRNG
 */
void init_gen(uint64_t seed);

/**
 *  Get a number from the PRNG.
 *  \return         A pseudo-random unsigned 64-bit integer.
 */
uint64_t extract_number();

/** Mix up the array. */
void twist();

/** Generate the Zobrist hash keys, seeded with the current time. */
void init_keys();

/**
 *  Generate the Zobrist hash keys with a custom seed (for debugging).
 *
 *  \param seed     The seed to use for the Zobrist hash.
 */
void init_keys(uint64_t seed);


} // end of chessCore namespace

#endif
