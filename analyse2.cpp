#include "board.h"
#include "move.h"
#include "action.h"
#include "init.h"
#include "hash.h"
#include "eval.h"
#include "twiddle.h"
#include "search.h"
//#include "structures.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <math.h>
#include <limits>
#include "ntree.h"


int main() {
    init();
//    board b( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 " );
    board b;
    b.print_board();
    move_t best_move = search_minimax(b,5,white);
    print_move(best_move);
    std::cout << std::endl;
    return 0;
}
