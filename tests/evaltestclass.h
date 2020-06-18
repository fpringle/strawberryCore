
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

    CPPUNIT_TEST(incrementalEvaluationStartBoard);
    CPPUNIT_TEST(incrementalEvaluationPos2);
    CPPUNIT_TEST(incrementalEvaluationPos3);
    CPPUNIT_TEST(incrementalEvaluationPos4);
    CPPUNIT_TEST(incrementalEvaluationPos5);
    CPPUNIT_TEST(incrementalEvaluationPos6);

    CPPUNIT_TEST_SUITE_END();

public:
    evaltestclass();
    virtual ~evaltestclass();
    void setUp();
    void tearDown();

private:
    void incrementalEvaluationStartBoard();
    void incrementalEvaluationPos2();
    void incrementalEvaluationPos3();
    void incrementalEvaluationPos4();
    void incrementalEvaluationPos5();
    void incrementalEvaluationPos6();

};

#endif /* EVALTESTCLASS_H */

