
/*
 * File:   movetestclass.h
 * Author: freddy
 *
 * Created on 09-Jun-2020, 09:10:00
 */

#ifndef MOVETESTCLASS_H
#define MOVETESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class movetestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(movetestclass);
    
    
    CPPUNIT_TEST(testItos);
    CPPUNIT_TEST(testStoi);
    
    CPPUNIT_TEST(testMove_t);

    CPPUNIT_TEST(testOperatorPrint);
    
    CPPUNIT_TEST(testInit_rays);
    
    CPPUNIT_TEST(testPawnPushNaive);
    CPPUNIT_TEST(testPawnAttackNaive);
    CPPUNIT_TEST(testPawnAttacks);
    CPPUNIT_TEST(testPawnTargets);
    
    CPPUNIT_TEST(testKnightPushNaive);
    CPPUNIT_TEST(testKnightTargets);
    
    CPPUNIT_TEST(testKingPushNaive);
    CPPUNIT_TEST(testKingTargets);
    
    
    CPPUNIT_TEST_SUITE_END();

public:
    movetestclass();
    virtual ~movetestclass();
    void setUp();
    void tearDown();

private:
    void testItos();
    void testStoi();
    
    void testMove_t();

    void testOperatorPrint();
    
    void testInit_rays();
    
    void testPawnPushNaive();
    void testPawnAttackNaive();
    void testPawnAttacks();
    void testPawnTargets();
    
    void testKnightPushNaive();
    void testKnightTargets();
    
    void testKingPushNaive();
    void testKingTargets();
    
    void testBishopPushNaive();
    void testBishopTargets();
    
    void testRookPushNaive();
    void testRookTargets();
    
    void testQueenPushNaive();
    void testQueenTargets();

};

#endif /* MOVETESTCLASS_H */

