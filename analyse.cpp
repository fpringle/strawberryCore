#include "board.h"
#include "move.h"
#include "action.h"
#include "init.h"
#include "hash.h"
#include "eval.h"
#include "twiddle.h"
#include "search.h"
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

struct tree_record {
    int32_t val;
    std::string lastmove;
    friend std::ostream& operator<<(std::ostream &out, const tree_record &tr);
};

std::ostream& operator<<(std::ostream &out, const tree_record &tr) {
    out << tr.val << ", " << tr.lastmove;
    return out;
}

ntreeNode<tree_record> * analyse_tree(board b, int depth,
                                      std::string lastmv = "") {
    ntreeNode<tree_record> * root = new ntreeNode<tree_record>;
    root->data = {b.getValue(), lastmv};
    if (depth == 0) return root;

    int num_moves, i;
    move_t movelist[256];
    board child;
    num_moves = b.gen_legal_moves(movelist);
    move_t move;
    std::stringstream new_move;
    ntreeNode<tree_record> * child_tree;

    for (i = 0; i < num_moves; i++) {
        move = movelist[i];
        child = doMove(b, move);
        print_move(move, new_move);
        child_tree = analyse_tree(child, depth - 1, new_move.str());
        new_move.str("");
        add_child(root, child_tree);
    }
    return root;
}

int main(int argc, char ** argv) {
    init();

    int depth = std::stoi(argv[2]);
    std::string board_string = argv[1];

    board b;

    if (board_string != "default") b = board(board_string);

    b.print_board();

    ntreeNode<tree_record> * valuetree = analyse_tree(b, depth);
    // print_tree(valuetree);

    move_t best_move = NegamaxAB::search(b, depth, white);
    std::cout << "Best move: ";
    print_move(best_move);
    std::cout << std::endl;

    return 0;
}
