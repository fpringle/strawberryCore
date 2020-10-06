#include "board.h"
#include "move.h"
#include "action.h"
#include "init.h"
#include "hash.h"
#include "eval.h"
#include "twiddle.h"
//#include "structures.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <limits>
#include "ntree.h"

#define BASE 2

//std::map<

/*
void analyse( board b , int depth, std::vector<int32_t> * vec, int base_depth=-1, int parent_index=1 ) {
    if ( depth==0 ) return;
    if (base_depth==-1) {
        base_depth=depth;
        std::cout << parent_index << std::endl;
    }
    move_t movelist[BASE];
    int n_moves;
    int i,j;
    board child;
    int index;

    n_moves = b.gen_legal_moves( movelist );
    n_moves = BASE;

    for ( i=0; i<n_moves; i++ ) {
        index = parent_index*BASE + i;
        child = doMove( b, movelist[i] );
        (*vec)[index] = child.getValue();
        for (j=0; j<=base_depth-depth; j++) std::cout << "-";
        std::cout << " " << index << "  " << (*vec)[index] << " ";
        print_move(movelist[i]);
        std::cout << std::endl;
        analyse( child, depth-1, vec, base_depth, index );
    }
}

void anlayse_bfs( board b, int depth, std::vector<int32_t> * vec ) {
    queue<board> Q;
    Q.enqueue(b);
    board v,child;
    int num_moves,i;
    move_t movelist[256];

    while (!Q.is_empty()) {
        v = Q.dequeue();
        (*vec).push_back(v.getValue());
        num_moves = v.gen_legal_moves(movelist);
        for (i=0; i<num_moves; i++) {
            child = doMove(v,movelist[i]);
            Q.enqueue(child);
        }
    }

}
*/
ntreeNode<int32_t> * analyse_tree( board b, int depth ) {
    ntreeNode<int32_t> * root = new ntreeNode<int32_t>;
    root->data = b.getValue();
    if (depth==0) return root;

    int num_moves,i;
    move_t movelist[256];
    board child;
    num_moves = b.gen_legal_moves(movelist);
    for (i=0;i<num_moves;i++) {
        child = doMove(b,movelist[i]);
        add_child<int32_t>(root,analyse_tree(child,depth-1));
    }
    return root;
}


int main() {
    init();
    board b( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 " );
    b.print_board();
    ntreeNode<int32_t> * valuetree = analyse_tree(b,2);
    print_tree(valuetree);
    return 0;
}
