
/*
 * File:   structurestestclass.h
 * Author: pi
 *
 * Created on Jun 13, 2020, 11:03:13 PM
 */

#ifndef STRUCTURESTESTCLASS_H
#define STRUCTURESTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class structurestestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(structurestestclass);
    
    //CPPUNIT_TEST(testGen_tree);

    CPPUNIT_TEST_SUITE_END();

public:
    structurestestclass();
    virtual ~structurestestclass();
    void setUp();
    void tearDown();

private:
    void testGen_tree();
};

#endif /* STRUCTURESTESTCLASS_H */

