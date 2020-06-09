
/*
 * File:   hashtestclass.cpp
 * Author: freddy
 *
 * Created on 09-Jun-2020, 07:52:00
 */

#include "hashtestclass.h"
#include "board.h"
#include "hash.h"


CPPUNIT_TEST_SUITE_REGISTRATION(hashtestclass);

hashtestclass::hashtestclass() {
}

hashtestclass::~hashtestclass() {
}

void hashtestclass::setUp() {
}

void hashtestclass::tearDown() {
}

void hashtestclass::testInit_gen() {
    init_gen(0);
    
    if ( MT[0] != 0 ) CPPUNIT_ASSERT(false);
    if ( MT[100] != 14636482285769839324 ) CPPUNIT_ASSERT(false);
    if ( MT[200] != 13637633891753794584 ) CPPUNIT_ASSERT(false);
    if ( MT[300] != 7039234145238618548 ) CPPUNIT_ASSERT(false);
}

void hashtestclass::testExtract_number() {
    init_gen(0);
    
    if ( extract_number() != 9327507593236513508 ) CPPUNIT_ASSERT(false);
    if ( extract_number() != 214307827937933918 ) CPPUNIT_ASSERT(false);
    if ( extract_number() != 1467640691614915310 ) CPPUNIT_ASSERT(false);
}

void hashtestclass::testInit_keys() {
    init_keys(0);
    
    if ( zobristKeys[0]   != 9327507593236513508 )  CPPUNIT_ASSERT(false);
    if ( zobristKeys[100] != 15025598482655818632 ) CPPUNIT_ASSERT(false);
    if ( zobristKeys[200] != 2366861304458481628 )  CPPUNIT_ASSERT(false);
    if ( zobristKeys[300] != 14928638904038037312 ) CPPUNIT_ASSERT(false);
    if ( zobristKeys[400] != 6484946264735705824 )  CPPUNIT_ASSERT(false);
}

void hashtestclass::testZobrist_hash() {
    board _board;
    
    if ( _board.zobrist_hash() != 12678160942179701780 ) CPPUNIT_ASSERT(false);
}