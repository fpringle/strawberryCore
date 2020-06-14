#include "board.h"
#include "move.h"
#include "hash.h"
#include <assert.h>
#include <time.h>
#include "twiddle.h"


// transposition table
struct record_t table[130000000];

// Mersenne_Twister pseudorandom number generator

const int w = 64, m = 156, r = 31;
const uint64_t n = 312;
const uint64_t a = 0xB5026F5AA96619E9;
const int u = 29;
const uint64_t d = 0x5555555555555555;
const int s=17;
const uint64_t b = 0x71D67FFFEDA60000;
const int t = 37;
const uint64_t c = 0xFFF7EEE000000000;
const int l = 43;
const uint64_t f = 6364136223846793005;



uint64_t check_index = n+1;
uint64_t lower_mask = ( 1ULL << r ) - 1;
uint64_t upper_mask = ~ lower_mask;
uint64_t MT[n];
uint64_t zobristKeys[781];

void init_gen(uint64_t seed) {
  check_index = n;
  MT[0] = seed;
  for (int i=1; i<n; i++) {
    MT[i] = ( ( ( ( MT[i-1] >> (w-1) ) ^ MT[i-1] ) + i) * f ) + i;
  }
}

uint64_t extract_number() {
  if (check_index >= n) {
    assert (check_index == n);
    twist();
  }

  uint64_t y = MT[check_index];

  y = y ^ ( ( y >> u) & d );
  y = y ^ ( ( y << s) & b );
  y = y ^ ( ( y >> t) & c );
  y = y ^ ( y >> l);

  check_index++;
  return y;
}

void twist() {
  for (int i=0; i<n; i++) {
    uint64_t x  = ( MT[i] & upper_mask ) + ( MT[(i+1)%n] & lower_mask);
    uint64_t xA = x >> 1;
    if ( x & 1ULL ) xA ^= a;
    MT[i] = ( MT[(i+m)%n] ^ xA );
  }
  check_index=0;
}

// initialise zobrist keys
void init_keys() {
  init_gen(time(NULL));

  for (int i=0; i<781; i++) {
    zobristKeys[i] = extract_number();
  }
}

void init_keys(uint64_t seed) {
  init_gen(seed);

  for (int i=0; i<781; i++) {
    zobristKeys[i] = extract_number();
  }
}

// zobrist hash

uint64_t board::zobrist_hash() {
  uint64_t ret = 0;
  int cP;
  bitboard tmp;
  int i;

  // pieces
  for (cP=0; cP<12; cP++) {
    tmp = pieceBoards[cP];
    while ( tmp ) {
        i = first_set_bit( tmp );
        ret ^= zobristKeys[cP*64 + i];
        tmp &= ( tmp - 1ULL );
    }
  }

  // castling
  if ( castleWhiteKingSide ) ret ^= zobristKeys[768];
  if ( castleWhiteQueenSide ) ret ^= zobristKeys[769];
  if ( castleBlackKingSide ) ret ^= zobristKeys[770];
  if ( castleBlackQueenSide ) ret ^= zobristKeys[771];

  // side to move
  if ( sideToMove == black) ret ^= zobristKeys[780];

  // en passant file
  if ( lastMoveDoublePawnPush ) ret ^= zobristKeys[772 + dPPFile];

  return ret;
}


