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
#include <limits>
#include <algorithm>

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
//        if ( depth==2 ) {
//            print_move( movelist[i] );
//            std::cout << std::endl << std::endl;
//        }
        ret += PERFT_legalcheck( child, depth-1 );
//        if ( depth==1 ) {
//            print_move( movelist[i] );
//            std::cout << std::endl;
//        }
//        if (depth==2) break;
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
        n_nodes = PERFT_legalcheck( _board, i );
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
        n_nodes = PERFT_legalcheck( _board, i );
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
        n_nodes = PERFT_legalcheck( _board, i );
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
//            std::cout << "n_pseudo: " << n_pseudo << std::endl;
//            std::cout << "n_legal:  " << n_legal << std::endl;
            board child;
            int j;
//            std::cout << "Actual moves:\n";
            for ( j=0; j<_n_pseudo; j++ ) {
                child = doMove( _board, moves_pseudo[j] );
                if ( ! child.is_check(white) ) {
//                    print_move( moves_pseudo[j] );
//                    std::cout << std::endl;
                }
                else n_pseudo--;
            }
//            std::cout << "  Total: " << n_pseudo << std::endl;
//            std::cout << "Our moves:\n";
//            for ( j=0; j<n_legal; j++ ) {
//                print_move( moves_legal[j] );
//                std::cout << std::endl;
//            }
//            std::cout << "  Total: " << n_legal << std::endl;
//            std::cout << "Our moves:\n";
        }
        CPPUNIT_ASSERT_MESSAGE( ss.str(), n_nodes == real[i] );
        ss.str("");
    }
}

void perfttestclass::testPosition4() {
    init_rays();
        
    board _board( "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" );
    
    unsigned int real[6] = { 1, 6, 264, 9467,
                             422333, 15833292 };
    
    unsigned int n_nodes;
    std::stringstream ss;
    
    for ( int i=0; i<5; i++ ) {
        n_nodes = PERFT_legalcheck( _board, i );
        ss << "Error checking PERFT result for depth " << i << ". ";
        ss << "Real answer: " << real[i] << std::endl;
        ss << "Our answer:  " << n_nodes << std::endl;
        ss << "(Did you remember to init_rays()?" << std::endl;
        if ( n_nodes != real[i] ) {
//            std::cout << std::endl;
            int j;
            board child;
            move_t moves_pseudo[256];
            move_t moves_legal[256];
            int n_pseudo = _board.gen_moves( moves_pseudo );
            int n_legal  = _board.gen_legal_moves(  moves_legal );
//            std::cout << "Real:\n";
            for ( j=0; j<n_pseudo; j++ ) {
                child = doMove( _board, moves_pseudo[j] );
                if ( child.is_check( white ) ) continue;
//                std::cout << "   "; 
//                print_move( moves_pseudo[j] );
//                std::cout << std::endl;
            }
//            std::cout << "Ours:\n";
//            for ( j=0; j<n_legal; j++ ) {
//                std::cout << "   ";
//                print_move( moves_legal[j] );
//                std::cout << std::endl;
//            }
        }
        CPPUNIT_ASSERT_MESSAGE( ss.str(), n_nodes == real[i] );
        ss.str("");
    }
}

void divide_pseud( board _board, int depth, unsigned int * cache ) {
    bool _print = false;
    move_t moves[256];
    int num_moves = _board.gen_moves( moves );
    int num_children;
    board _child;
    int total=0;
    colour side;
    _board.getSide(&side);
    int num_perft;
    
    for ( int i=0; i<num_moves; i++ ) {
        _child = doMove( _board, moves[i] );
        if ( _child.is_check( side ) ) continue;
        num_perft = PERFT_simple( _child, depth-1 );
        if ( _print ) {
            print_move( moves[i] );
            std::cout << ": " << num_perft << std::endl;
        }
        cache[ moves[i].from_sq()*64 + moves[i].to_sq() ] = num_perft;
        total += num_perft;
    }
    
    if ( _print) std::cout << "  Total: " << total << std::endl << std::endl;
}

bool divide_legal( board _board, int depth, unsigned int * cache ) {
    bool _print = false;
    move_t moves[256];
    int num_legal = _board.gen_legal_moves( moves );
//    int num_moves = _board.gen_moves( moves );
//    if ( num_moves != num_legal ) return false;
//    else {
//        std::cout << num_legal << std::endl;
//        std::cout << num_moves << std::endl;
//    }
    int num_children;
    board _child;
    int total=0;
    colour side;
    _board.getSide(&side);  
    int num_perft;
    
    for ( int i=0; i<num_legal; i++ ) {
        _child = doMove( _board, moves[i] );
        if ( _child.is_check( side ) ) continue;
        num_perft = PERFT_legalcheck( _child, depth-1 );
        if ( _print ) {
            print_move( moves[i] );
            std::cout << ": " << num_legal << std::endl;
        }
        cache[ moves[i].from_sq()*64 + moves[i].to_sq() ] = num_perft;
        total += num_perft;
    }
    
    if ( _print) std::cout << "  Total: " << total << std::endl << std::endl;
    return true;
}

void perfttestclass::testGen_moves() {
    board _board( "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1" );
//    _board.print_board();
    move_t moves[256];
    int n_moves = _board.gen_legal_moves( moves );
//    for ( int i=0; i<n_moves; i++ ) {
//        print_move( moves[i] );
//        std::cout << std::endl;
//    }
}

void perfttestclass::divideStartboard() {
    board b;
    
    move_t c2c3 ( 10, 18, 0, 0, 0, 0 );
    move_t d7d5 ( 51, 35, 0, 0, 0, 1 );
    move_t d1a4 (  3, 24, 0, 0, 0, 0 );
//    b = doMove( b, c2c3 );
//    b = doMove( b, d7d5 );
//    b = doMove( b, d1a4 );
    
    b.print_board();
    
    int i;
    unsigned int _max = std::numeric_limits<unsigned int>::max();
    unsigned int pseud_cache[4096];
    unsigned int legal_cache[4096];
    int from_sq;
    int to_sq;
    
    for ( i=0; i<4096; i++ ) {
        pseud_cache[i] = _max;
        legal_cache[i] = _max;
    }
    
         divide_pseud( b, 4, pseud_cache );
    if ( divide_legal( b, 4, legal_cache ) ) {

        std::cout << "move   pseud   legal\n";

        for ( i=0; i<4096; i++ ) {
            if ( legal_cache[i] != _max || pseud_cache[i] != _max ) {
                from_sq = i / 64;
                to_sq   = i % 64;
                itos( from_sq, std::cout );
                std::cout << " ";
                itos(   to_sq, std::cout );
                std::cout << ":  ";
                std::cout << pseud_cache[i] << "   ";
                std::cout << legal_cache[i] << "    ";
                if ( pseud_cache[i] != legal_cache[i] ) {
                    std::cout << "ERROR";
                }
                std::cout << std::endl;
            }
        }
    }
    
    else {
        move_t pseud[256];
        move_t legal[256];
        int n_pseud = b.gen_moves( pseud );
        int n_legal = b.gen_legal_moves( legal );
        std::cout << "real num: " << n_pseud << ", our num: " << n_legal << std::endl;
        int i;
        
        for ( i=0; i<std::max(n_pseud, n_legal); i++ ) {
            print_move( pseud[i] );
            std::cout << "   ";
            print_move( legal[i] );
            std::cout << "\n";
        }
    }
}

void perfttestclass::dividePos2() {
    board b ( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 0" );
    
    move_t e1c1 (  4,  2, 0, 0, 1, 1 );
    move_t g7h6 ( 54, 47, 0, 0, 0, 0 );
    b = doMove( b, e1c1 );
    b = doMove( b, g7h6 );
    
    b.print_board();
    
    int i;
    unsigned int _max = std::numeric_limits<unsigned int>::max();
    unsigned int pseud_cache[4096];
    unsigned int legal_cache[4096];
    int from_sq;
    int to_sq;
    
    for ( i=0; i<4096; i++ ) {
        pseud_cache[i] = _max;
        legal_cache[i] = _max;
    }
    
         divide_pseud( b, 1, pseud_cache );
    if ( divide_legal( b, 1, legal_cache ) ) {

        std::cout << "move   pseud   legal\n";

        for ( i=0; i<4096; i++ ) {
            if ( legal_cache[i] != _max || pseud_cache[i] != _max ) {
                from_sq = i / 64;
                to_sq   = i % 64;
                itos( from_sq, std::cout );
                std::cout << " ";
                itos(   to_sq, std::cout );
                std::cout << ":  ";
                std::cout << pseud_cache[i] << "   ";
                std::cout << legal_cache[i] << "    ";
                if ( pseud_cache[i] != legal_cache[i] ) {
                    std::cout << "ERROR";
                }
                std::cout << std::endl;
            }
        }
    }
    
    else {
        move_t pseud[256];
        move_t legal[256];
        int n_pseud = b.gen_moves( pseud );
        int n_legal = b.gen_legal_moves( legal );
        std::cout << "real num: " << n_pseud << ", our num: " << n_legal << std::endl;
        int i;
        
        for ( i=0; i<std::max(n_pseud, n_legal); i++ ) {
            print_move( pseud[i] );
            std::cout << "   ";
            print_move( legal[i] );
            std::cout << "\n";
        }
    }
}
