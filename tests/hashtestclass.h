
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

    CPPUNIT_TEST(testInit_gen);
    CPPUNIT_TEST(testExtract_number);
    CPPUNIT_TEST(testInit_keys);
    CPPUNIT_TEST(testZobrist_hash);

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
    void testZobrist_hash();

};

#endif /* HASHTESTCLASS_H */

