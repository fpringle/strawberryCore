
/*
 * File:   searchtestclass.cpp
 * Author: pi
 *
 * Created on Jun 30, 2020, 10:25:04 PM
 */

#include "searchtestclass.h"
#include "board.h"
#include "move.h"
#include "action.h"
#include "structures.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


CPPUNIT_TEST_SUITE_REGISTRATION(searchtestclass);

searchtestclass::searchtestclass() {
}

searchtestclass::~searchtestclass() {
}

void searchtestclass::setUp() {
}

void searchtestclass::tearDown() {
}


int map_move( move_t move ) {
    uint16_t from_square = move.from_sq();
    uint16_t   to_square =   move.to_sq();
    if ( ! move.is_promotion() ) {
        return from_square*64 + to_square;
    }
    else {
        int _ind = 4096;
        if ( to_square / 8 == 0 ) {
            _ind += to_square * 4;
        }
        else {
            _ind += 32 + ( to_square % 8 ) * 4;
        }
        
        switch ( move.which_promotion() ) {
            case queen:
                _ind += 0;
                break;
            case rook:
                _ind += 1;
                break;
            case bishop:
                _ind += 2;
                break;
            case knight:
                _ind += 3;
                break;
        }
        
        return _ind;
    }
}

std::string unmap_move( int num ) {
    std::stringstream ss;
    int a = num / 64;
    int b = num % 64;
    ss << itos(a) << itos(b);
    return ss.str();
}

std::string unmap_moveset( uint64_t num ) {
    std::stringstream ss;
    int x = num;
    int y;
    while ( x>0 ) {
        ss << unmap_move( x % 4190 ) << " ";
        x /= 4190;
    }
    
    return ss.str();
}

void search_tree( board b, uint64_t ** dest, int num, int depth, int basedepth=-1, uint64_t moves_so_far=0 ) {
    // size of dest == num
    int _basedepth;
    if ( basedepth == -1 )   _basedepth = depth;
    else                     _basedepth = basedepth;
    
    
    if ( depth==0 ) {
        **dest = moves_so_far;
        (*dest)++;
    }
    
    else {
        moves_so_far *= 4160;
        move_t moves[256];
        board child;
        int n_moves = b.gen_legal_moves( moves );
        uint64_t new_moveset;

        for ( int i=0; i<n_moves; i++ ) {
            child = doMove( b, moves[i] );
            new_moveset = moves_so_far + map_move( moves[i] );
            search_tree( child, dest, num, depth-1, basedepth, new_moveset );
        }
    }
}

void searchtestclass::testSearch_tree() {
    std::ofstream fil ("perft3.txt");
    int depth = 3;
    int num   = 8902;
    
    board b;
    uint64_t * dest = new uint64_t[num];
    search_tree( b, &dest, num, depth );
    dest -= num;
    for ( int i=0; i<num; i++ ) {
//        std::cout << dest[i] << "  " << unmap_moveset( dest[i] ) << std::endl;
        fil       << dest[i] << ": " << unmap_moveset( dest[i] ) << std::endl;
    }
    fil.close();
}