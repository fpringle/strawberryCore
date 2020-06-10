
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
    std::stringstream ss;
    
    uint64_t real_ans[4] = { 0, 7679169796915961517ULL,
                                9928827280107095932ULL,
                                1835933433882300461ULL };
    
    for ( int i=0; i<4; i+=1) {
        ss << "MT wrong at index " << i << "\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << MT[i*100];
        CPPUNIT_ASSERT_MESSAGE( ss.str(), MT[i*100] == real_ans[i] );
        ss.str("");
    }
}

void hashtestclass::testExtract_number() {
    init_gen(0);
    std::stringstream ss;
    uint64_t ex;
    
    uint64_t real_ans[4] = { 10470742564043646991ULL,
                             18332391566201596577ULL,
                             12416377439523956594ULL,
                             10573902219460348369ULL  };
    
    for ( int i=0; i<4; i+=1) {
        ex = extract_number();
        ss << "extract_number #" << i << " wrong\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << ex;
        //std::cout << ss.str() << std::endl;
        CPPUNIT_ASSERT_MESSAGE( ss.str(), ex == real_ans[i] );
        ss.str("");
    }
}

void hashtestclass::testInit_keys() {
    init_keys(0);
    std::stringstream ss;
    
    uint64_t real_ans[5] = { 10470742564043646991ULL,
                              3678968663291648592ULL,
                              7913751394762574702ULL,
                              3587419481263851835ULL,
                                 55218894715056074ULL };
    
    for ( int i=0; i<5; i++ ) {
        ss << "keys wrong at index " << i*100 << "\n  real answer:  " << real_ans[i];
        ss << "\n  our answer:   " << zobristKeys[i*100];        
        CPPUNIT_ASSERT_MESSAGE( ss.str(), zobristKeys[i*100] == real_ans[i] );
        ss.str("");
    }
    
}

void hashtestclass::testZobrist_hash() {
    board _board;
    
    CPPUNIT_ASSERT_MESSAGE ( "zobrist hash for starting board is wrong", _board.zobrist_hash() == 1758780544675427086ULL );
}