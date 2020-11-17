
/*
 * File:   twiddletestclass.cpp
 * Author: freddy
 *
 * Created on 09-Jun-2020, 05:45:02
 */

#include "twiddletestclass.h"
#include "board.h"
#include "twiddle.h"


using namespace chessCore;

CPPUNIT_TEST_SUITE_REGISTRATION(twiddletestclass);

twiddletestclass::twiddletestclass() { }

twiddletestclass::~twiddletestclass() { }

void twiddletestclass::setUp() { }

void twiddletestclass::tearDown() { }

void twiddletestclass::testCount_bits_set() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0xffffffffffffffff;
    bitboard _bitboard3 = 0x0000000000000000;
    CPPUNIT_ASSERT(count_bits_set(_bitboard1) == 23);
    CPPUNIT_ASSERT(count_bits_set(_bitboard2) == 64);
    CPPUNIT_ASSERT(count_bits_set(_bitboard3) == 0);
}

void twiddletestclass::testFirst_set_bit() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0xfffffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    CPPUNIT_ASSERT(first_set_bit(_bitboard1) == 0);
    CPPUNIT_ASSERT(first_set_bit(_bitboard2) == 4);
    CPPUNIT_ASSERT(first_set_bit(_bitboard3) == 32);
}

void twiddletestclass::testLast_set_bit() {
    bitboard bb[3] = {0x178e92000181fa89,
        0x0000effffffffff0,
        0x0000000100000000};


    int real_ans[3] = {60, 47, 32};
    int ans;

    std::stringstream ss;

    for (int i = 0; i < 3; i++) {
        ans = last_set_bit(bb[i]);
        ss << "last bit set wrong at " << i << "\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << ans;
        CPPUNIT_ASSERT_MESSAGE(ss.str(), real_ans[i] == ans);
        ss.str("");
    }

}

void twiddletestclass::testIs_bit_set() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    CPPUNIT_ASSERT_MESSAGE("1", !is_bit_set(_bitboard1, 35));
    CPPUNIT_ASSERT_MESSAGE("2", is_bit_set(_bitboard2, 60));
    CPPUNIT_ASSERT_MESSAGE("3", is_bit_set(_bitboard3, 32));
    CPPUNIT_ASSERT_MESSAGE("4", !is_bit_set(_bitboard3, 31));
}

void twiddletestclass::testOneN() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    CPPUNIT_ASSERT(oneN(_bitboard1) == 0x8e92000181fa8900);
    CPPUNIT_ASSERT(oneN(_bitboard2) == 0xfffffffffffff000);
    CPPUNIT_ASSERT(oneN(_bitboard3) == 0x0000010000000000);
}

void twiddletestclass::testOneE() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    CPPUNIT_ASSERT(oneE(_bitboard1) == 0x2e1c24000202f412);
    CPPUNIT_ASSERT(oneE(_bitboard2) == 0x20fefefefefefee0);
    CPPUNIT_ASSERT(oneE(_bitboard3) == 0x0000000200000000);
}

void twiddletestclass::testOneS() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    CPPUNIT_ASSERT(oneS(_bitboard1) == 0x00178e92000181fa);
    CPPUNIT_ASSERT(oneS(_bitboard2) == 0x0010ffffffffffff);
    CPPUNIT_ASSERT(oneS(_bitboard3) == 0x0000000001000000);
}

void twiddletestclass::testOneW() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;
    CPPUNIT_ASSERT(oneW(_bitboard1) == 0xb47490000407d44);
    CPPUNIT_ASSERT(oneW(_bitboard2) == 0x87f7f7f7f7f7f78);
    CPPUNIT_ASSERT(oneW(_bitboard3) == 0x0000000000000000);
}

void twiddletestclass::testOneNE() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;

    CPPUNIT_ASSERT(oneNE(_bitboard1) == 2027745740863050240ULL);
    CPPUNIT_ASSERT(oneNE(_bitboard2) == 18374403900871467008ULL);
    CPPUNIT_ASSERT(oneNE(_bitboard3) == 2199023255552ULL);
}

void twiddletestclass::testOneSE() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;

    CPPUNIT_ASSERT(oneSE(_bitboard1) == 12978789873222388ULL);
    CPPUNIT_ASSERT(oneSE(_bitboard2) == 9287570408013566ULL);
    CPPUNIT_ASSERT(oneSE(_bitboard3) == 33554432ULL);
}

void twiddletestclass::testOneSW() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;

    CPPUNIT_ASSERT(oneSW(_bitboard1) == 3174603602018429ULL);
    CPPUNIT_ASSERT(oneSW(_bitboard2) == 2391985390321535ULL);
    CPPUNIT_ASSERT(oneSW(_bitboard3) == 0ULL);
}

void twiddletestclass::testOneNW() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;

    CPPUNIT_ASSERT(oneNW(_bitboard1) == 5136636851074712576ULL);
    CPPUNIT_ASSERT(oneNW(_bitboard2) == 9187201950435735552ULL);
    CPPUNIT_ASSERT(oneNW(_bitboard3) == 0ULL);
}

void twiddletestclass::testTwoN() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;

    CPPUNIT_ASSERT(twoN(_bitboard1) == 10520410387303170048ULL);
    CPPUNIT_ASSERT(twoN(_bitboard2) == 18446744073708503040ULL);
    CPPUNIT_ASSERT(twoN(_bitboard3) == 281474976710656ULL);
}

void twiddletestclass::testTwoS() {
    bitboard _bitboard1 = 0x178e92000181fa89;
    bitboard _bitboard2 = 0x10fffffffffffff0;
    bitboard _bitboard3 = 0x0000000100000000;

    CPPUNIT_ASSERT(twoS(_bitboard1) == 25901102268801ULL);
    CPPUNIT_ASSERT(twoS(_bitboard2) == 18691697672191ULL);
    CPPUNIT_ASSERT(twoS(_bitboard3) == 65536ULL);
}
