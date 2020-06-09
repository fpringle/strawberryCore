
/*
 * File:   twiddletestclass.h
 * Author: freddy
 *
 * Created on 09-Jun-2020, 05:45:02
 */

#ifndef TWIDDLETESTCLASS_H
#define TWIDDLETESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class twiddletestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(twiddletestclass);

    CPPUNIT_TEST(testCount_bits_set);
    CPPUNIT_TEST(testFirst_set_bit);
    CPPUNIT_TEST(testLast_set_bit);
    CPPUNIT_TEST(testIs_bit_set);
    
    CPPUNIT_TEST(testOneN);
    CPPUNIT_TEST(testOneE);
    CPPUNIT_TEST(testOneS);
    CPPUNIT_TEST(testOneW);
    CPPUNIT_TEST(testOneNE);
    CPPUNIT_TEST(testOneSE);
    CPPUNIT_TEST(testOneSW);
    CPPUNIT_TEST(testOneNW);
    CPPUNIT_TEST(testTwoN);
    CPPUNIT_TEST(testTwoS);

    CPPUNIT_TEST_SUITE_END();

public:
    twiddletestclass();
    virtual ~twiddletestclass();
    void setUp();
    void tearDown();

private:
    void testCount_bits_set();
    void testFirst_set_bit();
    void testLast_set_bit();
    void testIs_bit_set();
    
    void testOneN();
    void testOneS();
    void testOneE();
    void testOneW();
    void testOneNE();
    void testOneSE();
    void testOneSW();
    void testOneNW();
    void testTwoN();
    void testTwoS();
};

#endif /* EVALTESTCLASS_H */

