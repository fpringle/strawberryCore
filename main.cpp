
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
    board b ( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" );
    play_white( 3, b );
}