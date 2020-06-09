
/*
 * File:   twiddletestclass.cpp
 * Author: freddy
 *
 * Created on 09-Jun-2020, 05:45:02
 */

#include "twiddletestclass.h"
#include "board.h"
#include "twiddle.h"


CPPUNIT_TEST_SUITE_REGISTRATION(twiddletestclass);

twiddletestclass::twiddletestclass() {
}

twiddletestclass::~twiddletestclass() {
}

void twiddletestclass::setUp() {
}

void twiddletestclass::tearDown() {
}


void testCount_bits_set() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0xffffffffffffffff;
    bitboard _bitboard3 = 0x0000000000000000;
    if ( count_bits_set( _bitboard1 ) != 23 ) CPPUNIT_ASSERT(false);
    if ( count_bits_set( _bitboard2 ) != 64 ) CPPUNIT_ASSERT(false);
    if ( count_bits_set( _bitboard3 ) !=  0 ) CPPUNIT_ASSERT(false);
}

void testFirst_set_bit() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0xfffffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( first_set_bit( _bitboard1 ) !=  0 ) CPPUNIT_ASSERT(false);
    if ( first_set_bit( _bitboard2 ) !=  4 ) CPPUNIT_ASSERT(false);
    if ( first_set_bit( _bitboard3 ) != 32 ) CPPUNIT_ASSERT(false);
}

void testLast_set_bit() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( last_set_bit( _bitboard1 ) !=  0 ) CPPUNIT_ASSERT(false);
    if ( last_set_bit( _bitboard2 ) != 61 ) CPPUNIT_ASSERT(false);
    if ( last_set_bit( _bitboard3 ) != 32 ) CPPUNIT_ASSERT(false);
}

void testIs_bit_set() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if (   is_bit_set( _bitboard1, 35 ) ) CPPUNIT_ASSERT(false);
    if ( ! is_bit_set( _bitboard2, 60 ) ) CPPUNIT_ASSERT(false);
    if ( ! is_bit_set( _bitboard3, 32 ) ) CPPUNIT_ASSERT(false);
    if (   is_bit_set( _bitboard3, 31 ) ) CPPUNIT_ASSERT(false);
}

void testOneN() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneN( _bitboard1 ) != 0x8e92000181fa8900 ) CPPUNIT_ASSERT(false);
    if ( oneN( _bitboard2 ) != 0xfffffffffffff000 ) CPPUNIT_ASSERT(false);
    if ( oneN( _bitboard3 ) != 0x0000010000000000 ) CPPUNIT_ASSERT(false);
}

void testOneE() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneE( _bitboard1 ) != 0x2f1d24000303f512 ) CPPUNIT_ASSERT(false);
    if ( oneE( _bitboard2 ) != 0x21ffffffffffffe0 ) CPPUNIT_ASSERT(false);
    if ( oneE( _bitboard3 ) != 0x0000000200000000 ) CPPUNIT_ASSERT(false);
}

void testOneS() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneS( _bitboard1 ) != 0x00178e92000181fa ) CPPUNIT_ASSERT(false);
    if ( oneS( _bitboard2 ) != 0x0010ffffffffffff ) CPPUNIT_ASSERT(false);
    if ( oneS( _bitboard3 ) != 0x0000000001000000 ) CPPUNIT_ASSERT(false);
}

void testOneW() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneW( _bitboard1 ) != 0x0bc7490000c0fd44 ) CPPUNIT_ASSERT(false);
    if ( oneW( _bitboard2 ) != 0x087ffffffffffff8 ) CPPUNIT_ASSERT(false);
    if ( oneW( _bitboard3 ) != 0x0000000080000000 ) CPPUNIT_ASSERT(false);
}

void testOneNE() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneNE( _bitboard1 ) != 0x1d24000303f51200 ) CPPUNIT_ASSERT(false);
    if ( oneNE( _bitboard2 ) != 0xffffffffffffe000 ) CPPUNIT_ASSERT(false);
    if ( oneNE( _bitboard3 ) != 0x0000020000000000 ) CPPUNIT_ASSERT(false);
}

void testOneSE() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneSE( _bitboard1 ) != 0x002f1d24000303f5 ) CPPUNIT_ASSERT(false);
    if ( oneSE( _bitboard2 ) != 0x0021ffffffffffff ) CPPUNIT_ASSERT(false);
    if ( oneSE( _bitboard3 ) != 0x0000000002000000 ) CPPUNIT_ASSERT(false);
}

void testOneSW() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneSW( _bitboard1 ) != 0x000bc7490000c0fd ) CPPUNIT_ASSERT(false);
    if ( oneSW( _bitboard2 ) != 0x00087fffffffffff ) CPPUNIT_ASSERT(false);
    if ( oneSW( _bitboard3 ) != 0x0000000000800000 ) CPPUNIT_ASSERT(false);
}

void testOneNW() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( oneNW( _bitboard1 ) != 0xc7490000c0fd4480 ) CPPUNIT_ASSERT(false);
    if ( oneNW( _bitboard2 ) != 0x7ffffffffffff800 ) CPPUNIT_ASSERT(false);
    if ( oneNW( _bitboard3 ) != 0x0000008000000000 ) CPPUNIT_ASSERT(false);
}

void testTwoN() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( twoS( _bitboard1 ) != 0x92000181fa890000 ) CPPUNIT_ASSERT(false);
    if ( twoS( _bitboard2 ) != 0xfffffffffff00000 ) CPPUNIT_ASSERT(false);
    if ( twoS( _bitboard3 ) != 0x0001000000000000 ) CPPUNIT_ASSERT(false);
}

void testTwoS() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    if ( twoS( _bitboard1 ) != 0x0000178e92000181 ) CPPUNIT_ASSERT(false);
    if ( twoS( _bitboard2 ) != 0x000010ffffffffff ) CPPUNIT_ASSERT(false);
    if ( twoS( _bitboard3 ) != 0x0000000000010000 ) CPPUNIT_ASSERT(false);
}

