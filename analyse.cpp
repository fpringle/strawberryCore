#include "board.h"
#include "move.h"
#include "action.h"
#include "init.h"
#include "hash.h"
#include "eval.h"
#include "twiddle.h"
#include "structures.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>

#define BASE 256

//std::map<


void analyse( board b , int depth, std::vector<int32_t> * vec, int base_depth=-1, int parent_index=1 ) {
    if ( depth==0 ) return;
    if (base_depth==-1) {
        base_depth=depth;
//        std::cout << parent_index << std::endl;
    }
    move_t movelist[BASE];
    int n_moves;
    int i,j;
    board child;
    int index;

    n_moves = b.gen_legal_moves( movelist );
//    n_moves = BASE;

    for ( i=0; i<n_moves; i++ ) {
        index = parent_index*BASE + i;
        child = doMove( b, movelist[i] );
        (*vec)[index] = child.getValue();
        for (j=0; j<=base_depth-depth; j++) std::cout << "-";
        std::cout << " " << index << "  " << (*vec)[index];
        print_move(movelist[i]);
        std::cout << std::endl;
        analyse( child, depth-1, vec, base_depth, index );
    }
}


int main() {
    init();
//    board b;
    board b( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 " );
    b.print_board();
    int depth = 1;
    std::vector<int32_t> vec(pow(BASE,depth+1),0);
    analyse(b,depth,&vec);
//    for (int i=0; i<vec.size(); i++) {
    for (int i=256; i<270; i++) {
        std::cout << "vec[" << i << "]: " << vec[i] << std::endl;
    }
    return 0;
}
