
/*
 * File:   boardtestclass.h
 * Author: freddy
 *
 * Created on 08-Jun-2020, 16:34:45
 */

#ifndef BOARDTESTCLASS_H
#define BOARDTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class boardtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(boardtestclass);

    CPPUNIT_TEST(testBoard);
    CPPUNIT_TEST(testBoard2);
    CPPUNIT_TEST(testBoard3);
    
    CPPUNIT_TEST(testPrint_bb);
    CPPUNIT_TEST(testPrint_board);
    CPPUNIT_TEST(testPrint_all);
    CPPUNIT_TEST(testFEN);
    
    CPPUNIT_TEST(testSet_piece);
    CPPUNIT_TEST(testSet_side);
    
    CPPUNIT_TEST(testWhiteSquares);
    CPPUNIT_TEST(testBlackSquares);
    CPPUNIT_TEST(testTakenSquares);
    CPPUNIT_TEST(testEmptySquares);
    
    CPPUNIT_TEST_SUITE_END();

public:
    boardtestclass();
    virtual ~boardtestclass();
    void setUp();
    void tearDown();

private:
    void testBoard();
    void testBoard2();
    void testBoard3();
    
    void testPrint_bb();
    void testPrint_board();
    void testPrint_all();
    void testFEN();
    
    void testSet_piece();
    void testSet_side();
    
    void testWhiteSquares();
    void testBlackSquares();
    void testTakenSquares();
    void testEmptySquares();

};

#endif /* BOARDTESTCLASS_H */

