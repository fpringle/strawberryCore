#include "board.h"
#include "move.h"
#include "play.h"
#include "action.h"
#include "search.h"
#include "eval.h"
#include "init.h"

#include <string>
#include <iostream>


move_t input_move( board b ) {
    move_t ret;
    move_t moves[256];
    int n_moves = b.gen_legal_moves( moves );
    
    std::string inp;
    std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
    std::cin >> inp;
    ret = stom( moves, n_moves, inp );
    
    while ( ret.give() == 0 ) {
        std::cout << "Sorry, that's not a valid move.\n";
        std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
        std::cin >> inp;
        ret = stom( moves, n_moves, inp );
    }
    return ret;
}

void play_white(int plies) {
    board b;
    play_white(plies, b);
}

void play_white(int plies, board b) {
  init();
  struct move_t comp_move;
  struct move_t player_move;
  struct move_t move_list[256];

  b.print_board();

  struct move_t * p = move_list;

  while (true) {
    
    player_move = input_move( b );

    b = doMove( b, player_move );

    *p = player_move;
    p++;

    b.print_board();
    
    std::cout << "Computer thinking...\n";

//    comp_move = search_negamaxAB( b, plies, black );
    comp_move = search_negamax( b, plies, black );
    b = doMove( b, comp_move );
    *p = comp_move;
    p++;

    b.print_board();
  }
}

void play_black(int plies) {
  init();
  board b;
  struct move_t comp_move;
  struct move_t player_move;
  struct move_t move_list[256];

  b.print_board();

  struct move_t * p = move_list;

  while (true) {
    std::cout << "Computer thinking...\n";

    comp_move = search_negamaxAB( b, plies, white );
    b = doMove( b, comp_move );
    *p = comp_move;
    p++;

    b.print_board();
    
    player_move = input_move( b );

    b = doMove( b, player_move );

    *p = player_move;
    p++;

    b.print_board();
  }
}
