
/*
 * File:   searchtestclass.h
 * Author: pi
 *
 * Created on Jun 30, 2020, 10:25:03 PM
 */

#ifndef SEARCHTESTCLASS_H
#define SEARCHTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class searchtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(searchtestclass);
    
    CPPUNIT_TEST(testSearch_tree);

    CPPUNIT_TEST_SUITE_END();

public:
    searchtestclass();
    virtual ~searchtestclass();
    void setUp();
    void tearDown();

private:
    void testSearch_tree();
};

#endif /* SEARCHTESTCLASS_H */

