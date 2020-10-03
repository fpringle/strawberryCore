
/*
* File:   main.cpp
* Author: freddy
*
* Created on 08 June 2020, 13:36
*/

#include <cstdlib>
#include "action.h"
#include "board.h"
#include "eval.h"
//#include "exceptions.h"
#include "hash.h"
#include "init.h"
#include "move.h"
#include "play.h"
#include "search.h"
//#include "structures.h"
#include "twiddle.h"



int main() {
    init();
    //    board b ( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" );
    board b ( "5k2/1P6/8/6B1/1R6/Q7/8/4K2R w K -" );

    play_white( 3, b );
    /*    b.print_all();
    move_t h1h8(7,63,0,0,0,0);
    b = doMove(b,h1h8);
    b.print_all();
    */
    return 0;
}
