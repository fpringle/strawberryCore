/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   perfttestclass.cpp
 * Author: pi
 *
 * Created on Jun 11, 2020, 9:26:46 AM
 */

#include "perfttestclass.h"
#include "board.h"
#include "move.h"
#include "action.h"
#include "structures.h"
#include <iostream>
#include <fstream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(perfttestclass);

perfttestclass::perfttestclass() {
}

perfttestclass::~perfttestclass() {
}

void perfttestclass::setUp() {
}

void perfttestclass::tearDown() {
}

tree<std::string> pop_perft_tree( board ) {
    std::string s = "";
    tree<std::string> root( "" );
    return root;
}

unsigned int PERFT_simple( board b , int depth ) {
    move_t movelist[256];
    int n_moves, i;
    unsigned int ret = 0;
    board child;
    colour side;
    b.getSide( &side );
    
    if ( depth==0 ) {
        return 1;
    }
    
    n_moves = b.gen_moves( movelist );
    for ( i=0; i<n_moves; i++ ) {
        child = doMove( b, movelist[i] );
        if ( child.is_check(side) ) continue;
        ret += PERFT_simple( child, depth-1 );
    }
    return ret;
}

unsigned int PERFT_legalcheck( board b , int depth ) {
    move_t movelist[256];
    int n_moves, i;
    unsigned int ret = 0;
    board child;
    colour side;
    b.getSide( &side );
    
    if ( depth==0 ) {
        return 1;
    }
    
    n_moves = b.gen_legal_moves( movelist );
    for ( i=0; i<n_moves; i++ ) {
        child = doMove( b, movelist[i] );
//        if ( child.is_check(side) ) continue;
        ret += PERFT_legalcheck( child, depth-1 );
    }
    return ret;
}


void perfttestclass::testStartBoard() {
    init_rays();
    board _board;
    unsigned int n_nodes;
    std::stringstream ss;
    
    unsigned int real[7] = { 1, 20, 400, 8902, 197281,
                             4865609, 119060324 };
    
    for ( int i=0; i< 5; i++ ) {
        n_nodes = PERFT_simple( _board, i );
        ss << "Error checking PERFT result for depth " << i << ". ";
        ss << "Real answer: " << real[i] << std::endl;
        ss << "Our answer:  " << n_nodes << std::endl;
        ss << "(Did you remember to init_rays()?" << std::endl;
//        if ( n_nodes == real[i] ) {
//            std::cout << "Depth " << i << ": " << n_nodes << std::endl;
//        }
        CPPUNIT_ASSERT_MESSAGE( ss.str(), n_nodes == real[i] );
        ss.str("");
    }
}

void perfttestclass::testPosition2() {
    init_rays();
    
    board _board( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 0" );
    
    unsigned int real[6] = { 1, 48, 2039, 97862,
                             4085603, 193690690 };
    
    unsigned int n_nodes;
    std::stringstream ss;
    
    for ( int i=0; i<4; i++ ) {
        n_nodes = PERFT_simple( _board, i );
        ss << "Error checking PERFT result for depth " << i << ". ";
        ss << "Real answer: " << real[i] << std::endl;
        ss << "Our answer:  " << n_nodes << std::endl;
        ss << "(Did you remember to init_rays()?" << std::endl;
//        if ( n_nodes == real[i] ) {
//            std::cout << "Depth " << i << ": " << n_nodes << std::endl;
//        }
        CPPUNIT_ASSERT_MESSAGE( ss.str(), n_nodes == real[i] );
        ss.str("");
    }
}

void perfttestclass::testPosition3() {
    init_rays();
        
    board _board( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1" );
    
    unsigned int real[6] = { 1, 14, 191, 2812,
                             43238, 674624};
    
    unsigned int n_nodes;
    std::stringstream ss;
    
//    _board.print_board();
    
    for ( int i=0; i<6; i++ ) {
        n_nodes = PERFT_simple( _board, i );
        ss << "Error checking PERFT result for depth " << i << ". ";
        ss << "Real answer: " << real[i] << std::endl;
        ss << "Our answer:  " << n_nodes << std::endl;
        ss << "(Did you remember to init_rays()?" << std::endl;
        if ( n_nodes != real[i] ) {
            move_t moves_pseudo[256];
            move_t moves_legal[256];
            int n_pseudo = _board.gen_moves( moves_pseudo );
            int _n_pseudo = n_pseudo;
            int n_legal  = _board.gen_legal_moves(  moves_legal );
            std::cout << "n_pseudo: " << n_pseudo << std::endl;
            std::cout << "n_legal:  " << n_legal << std::endl;
            board child;
            int j;
            std::cout << "Actual moves:\n";
            for ( j=0; j<_n_pseudo; j++ ) {
                child = doMove( _board, moves_pseudo[j] );
                if ( ! child.is_check(white) ) {
                    print_move( moves_pseudo[j] );
                    std::cout << std::endl;
                }
                else n_pseudo--;
            }
            std::cout << "  Total: " << n_pseudo << std::endl;
            std::cout << "Our moves:\n";
            for ( j=0; j<n_legal; j++ ) {
                print_move( moves_legal[j] );
                    std::cout << std::endl;
            }
            std::cout << "  Total: " << n_legal << std::endl;
            std::cout << "Our moves:\n";
        }
        CPPUNIT_ASSERT_MESSAGE( ss.str(), n_nodes == real[i] );
        ss.str("");
    }
}

void perfttestclass::testPosition4() {
    init_rays();
        
    board _board( "r3k2r/Pppp1ppp/1b3nbN/1P6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" );
    
    unsigned int real[6] = { 1, 6, 264, 9467,
                             422333, 15833292 };
    
    unsigned int n_nodes;
    std::stringstream ss;
    
    for ( int i=0; i<5; i++ ) {
        n_nodes = PERFT_simple( _board, i );
        ss << "Error checking PERFT result for depth " << i << ". ";
        ss << "Real answer: " << real[i] << std::endl;
        ss << "Our answer:  " << n_nodes << std::endl;
        ss << "(Did you remember to init_rays()?" << std::endl;
        if ( n_nodes != real[i] ) {
            _board.print_all();
            std::cout << std::endl;
            int j;
            board child;
            move_t moves_pseudo[256];
            move_t moves_legal[256];
            int n_pseudo = _board.gen_moves( moves_pseudo );
            int n_legal  = _board.gen_legal_moves(  moves_legal );
            std::cout << "Real:\n";
            for ( j=0; j<n_pseudo; j++ ) {
                child = doMove( _board, moves_pseudo[j] );
                if ( child.is_check( white ) ) continue;
                std::cout << "   "; 
                print_move( moves_pseudo[j] );
                std::cout << std::endl;
            }
            std::cout << "Ours:\n";
            for ( j=0; j<n_legal; j++ ) {
                std::cout << "   ";
                print_move( moves_legal[j] );
                std::cout << std::endl;
            }
        }
        CPPUNIT_ASSERT_MESSAGE( ss.str(), n_nodes == real[i] );
        ss.str("");
    }
}