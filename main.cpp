
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



int main2(int argc, char** argv) {
    init();
    play(4);
    return 0;
}

int main() {
    init();
    play( 5 );
}