
/*
 * File:   evaltestclass.h
 * Author: freddy
 *
 * Created on 08-Jun-2020, 20:21:02
 */

#ifndef EVALTESTCLASS_H
#define EVALTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class evaltestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(evaltestclass);

    CPPUNIT_TEST(testEvaluate_material);
    CPPUNIT_TEST(testEvaluate_pieceSquareTables);
    CPPUNIT_TEST(testEvaluate);

    CPPUNIT_TEST_SUITE_END();

public:
    evaltestclass();
    virtual ~evaltestclass();
    void setUp();
    void tearDown();

private:
    void testEvaluate_material();
    void testEvaluate_pieceSquareTables();
    void testEvaluate();

};

#endif /* EVALTESTCLASS_H */

