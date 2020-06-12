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
    move_t moves[256];
    int n_moves = b.gen_moves( moves );
    move_t ret;
    
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

void play(int plies) {
  init();
  board b;
  int count=0;
  struct move_t moves[256];
  struct move_t comp_move;
  struct move_t play_move;
  struct move_t move_list[256];
  int num_moves;
  char from_str[2];
  char to_str[2];
  int from_square,to_square;

  b.print_board(std::cout);

  struct move_t * p = move_list;

  while (true) {
    num_moves = b.gen_moves( moves );

    bool false_move = true;
    while ( false_move ) {
      std::cout << "From square: ";
      std::cin >> from_str;
      std::cout << "To square: ";
      std::cin >> to_str;

      from_square = stoi(from_str);
      to_square = stoi(to_str);

      for (int i=0; i<num_moves; i++) {
        if ( ( moves[i].from_sq() == from_square ) && ( moves[i].to_sq() == to_square ) ) {
          play_move = moves[i];
          false_move = false;
          break;
        }
      }
      if ( false_move ) std::cout << "Invalid move.\n";
    }


    b = doMove( b, play_move );

    *p = play_move;
    p++;
    count++;

    b.print_board(std::cout);
    std::cout << "Computer thinking...\n";

    comp_move = search_negamaxAB( b, plies, black );
    b = doMove( b, comp_move );
    *p = play_move;
    p++;
    count++;

    b.print_board(std::cout);
  }
}
