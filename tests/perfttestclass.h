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
};

#endif /* PERFTTESTCLASS_H */

