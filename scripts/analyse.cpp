#include "board.h"
#include "move.h"
#include "action.h"
#include "init.h"
#include "hash.h"
#include "eval.h"
#include "twiddle.h"
#include "search.h"
#include "tree.h"

#include <iostream>
#include <string>
#include <sstream>

struct tree_record {
    int32_t val;
    std::string lastmove;
    friend std::ostream& operator<<(std::ostream &out, const tree_record &tr);
};

std::ostream& operator<<(std::ostream &out, const tree_record &tr) {
    out << tr.val << ", " << tr.lastmove;
    return out;
}

treeNode<tree_record> * analyse_tree(board b, int depth,
                                      std::string lastmv = "") {
    treeNode<tree_record> * root = new treeNode<tree_record>;
    root->data = {b.getValue(), lastmv};
    if (depth == 0) return root;

    int num_moves, i;
    move_t movelist[256];
    board child;
    num_moves = b.gen_legal_moves(movelist);
    move_t move;
    std::stringstream new_move;
    treeNode<tree_record> * child_tree;

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

void usage() {
	std::cout << "USAGE: analyse.exe BOARD_STRING DEPTH\n"
			  << "       if BOARD_STRING = 'default', use starting position\n"
			  << "       if DEPTH = -1, use DEPTH = 4\n";
}

int main(int argc, char ** argv) {

	if (argc < 3) {
		usage();
		return 1;
	}

    init();

    int depth = std::stoi(argv[2]);
    std::string board_string = argv[1];
    bool quiet = false;

    if (argc > 3) {
        quiet = true;
    }

    board b;

    if (board_string != "default") b = board(board_string);
    if (depth == -1) depth = 4;

//    b.print_board();

    colour side;
    b.getSide(&side);

    if (! quiet) {
        std::cout << (side == white ? "white" : "black") << std::endl;
        treeNode<tree_record> * valuetree = analyse_tree(b, depth);
        print_tree(valuetree);
    }

    move_t best_move = PVS::search(b, depth);
    std::cout << "Best move: ";
    print_move(best_move);
    std::cout << std::endl;

    return 0;
}
