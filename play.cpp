#include "board.h"
#include "move.h"
#include "play.h"
#include "action.h"
#include "search.h"
#include "eval.h"
#include "init.h"

#include <string>
#include <iostream>
using namespace std;


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
      cout << "From square: ";
      cin >> from_str;
      cout << "To square: ";
      cin >> to_str;

      from_square = stoi(from_str);
      to_square = stoi(to_str);

      for (int i=0; i<num_moves; i++) {
        if ( ( moves[i].from_sq() == from_square ) && ( moves[i].to_sq() == to_square ) ) {
          play_move = moves[i];
          false_move = false;
          break;
        }
      }
      if ( false_move ) cout << "Invalid move.\n";
    }


    b = doMove( b, play_move );

    *p = play_move;
    p++;
    count++;

    b.print_board(std::cout);
    cout << "Computer thinking...\n";

    comp_move = search_negamaxAB( b, plies, black );
    b = doMove( b, comp_move );
    *p = play_move;
    p++;
    count++;

    b.print_board(std::cout);
  }
}
