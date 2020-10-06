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
#include <fstream>
#include <string>
#include <sstream>
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

struct tree_record {
    int32_t val;
    std::string lastmove;
    friend std::ostream& operator<<(std::ostream &out, const tree_record &tr);
};

std::ostream& operator<<(std::ostream &out, const tree_record &tr) {
    out << tr.val << ", " << tr.lastmove;
    return out;
}

ntreeNode<tree_record> * analyse_tree( board b, int depth, std::string lastmv = "" ) {
    ntreeNode<tree_record> * root = new ntreeNode<tree_record>;
    root->data = {b.getValue(), lastmv};
    if (depth==0) return root;

    int num_moves,i;
    move_t movelist[256];
    board child;
    num_moves = b.gen_legal_moves(movelist);
    move_t move;
    std::stringstream new_move;
    ntreeNode<tree_record> * child_tree;

    for (i=0;i<num_moves;i++) {
        move = movelist[i];
        child = doMove(b,move);
        print_move(move, new_move);
        child_tree = analyse_tree(child,depth-1,new_move.str());
        new_move.str("");
        add_child(root,child_tree);
    }
    return root;
}



int main() {
    init();
//    board b( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 " );
    board b;
//    b.print_board();
    ntreeNode<tree_record> * valuetree = analyse_tree(b,5);

    print_tree(valuetree);
//    num_nodes_per_level(valuetree);

    return 0;
}
