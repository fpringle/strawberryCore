// bit twiddling functions

#ifndef __TWIDDLE_H
#define __TWIDDLE_H

#include "typedefs.h"


namespace chessCore {

// constant masks
// defined in twiddle.cpp
/**@{*/
/** Some constant bitboard mask, corresponding to useful files and rows. */
extern const bitboard fileA;
extern const bitboard fileB;
extern const bitboard fileG;
extern const bitboard fileH;
extern const bitboard rankOne;
extern const bitboard rankTwo;
extern const bitboard rankSeven;
extern const bitboard rankEight;
extern const bitboard notFileA;
extern const bitboard notFileB;
extern const bitboard notFileG;
extern const bitboard notFileH;
extern const bitboard notRankOne;
extern const bitboard notRankEight;
/**@{*/

/**
 *  Count the number of bits set in a bitboard.
 *
 *  \param v        The bitboard to count.
 *  \return         The number of bits set in v.
 */
int count_bits_set(bitboard v);

/**
 *  Find the index of the first bit set in a bitboard.
 *
 *  \param v        The bitboard.
 *  \return         The index of the first bit set in v.
 */
int first_set_bit(bitboard v);

/**
 *  Find the index of the last bit set in a bitboard.
 *
 *  \param v        The bitboard.
 *  \return         The index of the last bit set in v.
 */
int last_set_bit(bitboard v);

/**
 *  Determine whether a given bit is set in a bitboard.
 *
 *  \param v        The bitboard.
 *  \param i        The index of the bit to check.
 *  \return         A boolean corresponding to whether the i-th
 *                  bit of v is set.
 */
bool is_bit_set(bitboard v, int i);

/**
 *  Set a particular bit in a bitboard.
 *
 *  \param v        The bitboard.
 *  \param i        The index of the bit to set.
 *  \return         A bitboard identical to v but with the i-th index set.
 */
bitboard set_bit(bitboard v, int i);

/**
 *  Unset a particular bit in a bitboard.
 *
 *  \param v        The bitboard.
 *  \param i        The index of the bit to unset.
 *  \return         A bitboard identical to v but with the i-th index unset.
 */
bitboard unset_bit(bitboard v, int i);

/**
 *  Translate an entire bitboard one square north.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square north.
 */
bitboard oneN(bitboard b);

/**
 *  Translate an entire bitboard one square south.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square south.
 */
bitboard oneS(bitboard b);

/**
 *  Translate an entire bitboard one square east.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square east.
 */
bitboard oneE(bitboard b);

/**
 *  Translate an entire bitboard one square west.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square west.
 */
bitboard oneW(bitboard b);

/**
 *  Translate an entire bitboard one square north-east.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square north-east.
 */
bitboard oneNE(bitboard b);

/**
 *  Translate an entire bitboard one square south-east.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square south-east.
 */
bitboard oneSE(bitboard b);

/**
 *  Translate an entire bitboard one square south-west.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square south-west.
 */
bitboard oneSW(bitboard b);

/**
 *  Translate an entire bitboard one square north-west.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, one square north-west.
 */
bitboard oneNW(bitboard b);

/**
 *  Translate an entire bitboard in a given direction.
 *
 *  \param v    The bitboard to translate.
 *  \param d    The direction to translate.
 *              0   ->   North
 *              1   ->   North-East
 *              2   ->   East
 *              3   ->   South-East
 *              4   ->   South
 *              5   ->   South-West
 *              6   ->   West
 *              7   ->   North-West
 *  \return     The translation of b, one square in the direction of d.
 */
bitboard oneGeneral8(bitboard v, int d);

/**
 *  Translate an entire bitboard two squares north.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, two squares north.
 */
bitboard twoN(bitboard b);

/**
 *  Translate an entire bitboard two squares south.
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b, two squares south.
 */
bitboard twoS(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two north, one east).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneNNE(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two east, one north).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneENE(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two east, one south).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneESE(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two south, one east).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneSSE(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two south, one west).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneSSW(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two west, one south).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneWSW(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two west, one north).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneWNW(bitboard b);

/**
 *  Translate an entire bitboard as a knight moves (two north, one west).
 *
 *  \param b    The bitboard to translate.
 *  \return     The translation of b.
 */
bitboard oneNNW(bitboard b);


} // end of chessCore namespace

#endif
