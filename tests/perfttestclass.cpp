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
#include <iostream>
#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION(perfttestclass);

perfttestclass::perfttestclass() {
}

perfttestclass::~perfttestclass() {
}

void perfttestclass::setUp() {
}

void perfttestclass::tearDown() {
}

unsigned long long int PERFT( board b, int depth, board **nodes ) {
    move_t movelist[256];
    int n_moves, i;
    unsigned long long int ret = 0;
    board child;
    colour side;
    b.getSide( &side );
    
    if ( depth==0 ) {
//        b.print_board();
        **nodes = b;
        (*nodes)++;
        return 1;
    }
    
    n_moves = b.gen_moves( movelist );
    //std::cout << "number of moves: " << n_moves;
    for ( i=0; i<n_moves; i++ ) {
        child = doMove( b, movelist[i] );
        if ( child.is_check(side) ) continue;
        //print_move( movelist[i] );
        //std::cout << std::endl;
        ret += PERFT( child, depth-1, nodes );
        //std::cout << "ret = " << ret << std::endl;
    }
    return ret;
}


void perfttestclass::testStartBoard() {
    init_rays();
    bool to_file = true;
    board _board;
    board * nodes = new board[5000000];
    unsigned long long int n_nodes;
    n_nodes = PERFT( _board, 4, &nodes );
    std::cout << n_nodes << " nodes\n";
    int checks=0;
    if ( to_file ) {
        ofstream myfile_all;
        ofstream myfile_checks;
        myfile_all.open("tests/perft_results_all.txt");
        myfile_checks.open("tests/perft_results_checks.txt");
        for ( int i=-n_nodes; i<0; i++ ) {
            nodes[i].print_board( myfile_all );
            if ( nodes[i].is_check( black ) || nodes[i].is_check( white ) ) {
                nodes[i].print_board( myfile_checks );
                checks++;
            }
        }
        std::cout << checks << " checks\n";
        myfile_all.close();
        myfile_checks.close();
    }
}

//void perfttestclass::testStartBoard() {
//    init_rays();
//    board b;
//    move_t m1 = { 12, 28, 0, 0, 0, 1 };
//    move_t m2 = { 53, 45, 0, 0, 0, 0 };
//    
//    b = doMove( b, m1 );
//    b = doMove( b, m2 );
//    
//    move_t moveset[256];
//    
//    int n_moves = b.gen_moves( moveset );
//    board child;
//    
//    for ( int i=0; i<n_moves; i++ ) {
//        child = doMove( b, moveset[i] );
//        print_move( moveset[i] );
//        std::cout << ":\n";
//        child.print_board();
//        std::cout << "\n\n";
//    }
//}