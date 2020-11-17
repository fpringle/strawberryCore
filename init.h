#ifndef __INIT_H
#define __INIT_H

#include <cstdint>

namespace chessCore {

/**
 *  Initialise some global constants.
 *
 *  Initialise a the 2-D array of bitboards rays, and the hash keys.
 *  The rays are generated by init_rays(), and the hash keys by init_keys().
 *  See \ref move.h and \ref hash.h.
 */
void init();

/**
 *  Initialise some global constants.
 *
 *  Initialise a the 2-D array of bitboards rays, and the hash keys.
 *  The rays are generated by init_rays, and the hash keys by init_keys().
 *  The only difference to \ref init() is that the seed for the hash keys
 *  can be specified, for debugging.
 *  See \ref move.h and \ref hash.h.
 *
 *  \param seed     The seed to be used for the hash keys.
 */
void init(uint64_t seed);


} // end of chessCore namespace

#endif
