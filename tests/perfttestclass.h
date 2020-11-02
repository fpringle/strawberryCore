/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   perfttestclass.h
 * Author: pi
 *
 * Created on Jun 11, 2020, 9:26:44 AM
 */

#ifndef PERFTTESTCLASS_H
#define PERFTTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class perfttestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(perfttestclass);
    
    CPPUNIT_TEST(testStartBoard);
    CPPUNIT_TEST(testPosition2);
    CPPUNIT_TEST(testPosition3);
    CPPUNIT_TEST(testPosition4);
    CPPUNIT_TEST(testPosition5);
    CPPUNIT_TEST(testPosition6);
//    CPPUNIT_TEST(divideStartboard);
//    CPPUNIT_TEST(dividePos2);
//    CPPUNIT_TEST(dividePos3);
//    CPPUNIT_TEST(dividePos4);
//    CPPUNIT_TEST(pos3b4b1f4f3e2e3f3g2b1h1);

    CPPUNIT_TEST_SUITE_END();

public:
    perfttestclass();
    virtual ~perfttestclass();
    void setUp();
    void tearDown();

private:
    void testStartBoard();
    void testPosition2();
    void testPosition3();   
    void testPosition4();
    void testPosition5();   
    void testPosition6();
    void divideStartboard();
    void dividePos2();
    void dividePos3();
    void dividePos4();
    void pos3b4b1f4f3e2e3f3g2b1h1();
};

#endif /* PERFTTESTCLASS_H */

