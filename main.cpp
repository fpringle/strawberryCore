
/* 
 * File:   main.cpp
 * Author: freddy
 *
 * Created on 08 June 2020, 13:36
 */

#include <cstdlib>
#include <iostream>
#include "board.h"

using namespace std;

void testBoard() {
    board _board;
    bitboard whitePawnStart   = 0x000000000000ff00;
    bitboard whiteRookStart   = 0x0000000000000081;
    bitboard whiteKnightStart = 0x0000000000000042;
    bitboard whiteBishopStart = 0x0000000000000024;
    bitboard whiteQueenStart  = 0x0000000000000008;
    bitboard whiteKingStart   = 0x0000000000000010;
    
    bitboard blackPawnStart   = 0x00ff000000000000;
    bitboard blackRookStart   = 0x8100000000000000;
    bitboard blackKnightStart = 0x4200000000000000;
    bitboard blackBishopStart = 0x2400000000000000;
    bitboard blackQueenStart  = 0x0800000000000000;
    bitboard blackKingStart   = 0x1000000000000000;
    
    bitboard pb[12];
    
    pb[0]  = whitePawnStart;
    pb[1]  = whiteRookStart;
    pb[2]  = whiteKnightStart;
    pb[3]  = whiteBishopStart;
    pb[4]  = whiteQueenStart;
    pb[5]  = whiteKingStart;
    pb[6]  = blackPawnStart;
    pb[7]  = blackRookStart;
    pb[8]  = blackKnightStart;
    pb[9]  = blackBishopStart;
    pb[10] = blackQueenStart;
    pb[11] = blackKingStart;
    
    bitboard real[12];
    _board.getBitboards( real );
    
    for ( int i=0;i<12;i++) {
        if ( pb[i] != real[i] ) {
            std::cout << "FAIL" << std::endl;
            return;
        }
    }
    
    std::cout << "SUCCESS" << std::endl;
}

/*
 * 
 */
int main(int argc, char** argv) {
    testBoard();
    return 0;
}

