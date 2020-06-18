
/*
 * File:   hashtestclass.h
 * Author: freddy
 *
 * Created on 09-Jun-2020, 07:52:00
 */

#ifndef HASHTESTCLASS_H
#define HASHTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class hashtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(hashtestclass);

//    CPPUNIT_TEST(testInit_gen);
//    CPPUNIT_TEST(testExtract_number);
//    CPPUNIT_TEST(testInit_keys);
//    
//    CPPUNIT_TEST(incrementalHashStartBoard);
//    CPPUNIT_TEST(incrementalHashPos2);
    CPPUNIT_TEST(incrementalHashPos3);
    CPPUNIT_TEST(incrementalHashPos4);
    CPPUNIT_TEST(incrementalHashPos5);
    CPPUNIT_TEST(incrementalHashPos6);
    
//    CPPUNIT_TEST(dividePos2);

    CPPUNIT_TEST_SUITE_END();

public:
    hashtestclass();
    virtual ~hashtestclass();
    void setUp();
    void tearDown();

private:
    void testInit_gen();
    void testExtract_number();
    void testInit_keys();
    
    void incrementalHashStartBoard();
    void incrementalHashPos2();
    void incrementalHashPos3();
    void incrementalHashPos4();
    void incrementalHashPos5();
    void incrementalHashPos6();
    void dividePos2();

};

#endif /* HASHTESTCLASS_H */

