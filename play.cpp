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
    struct move_t history[269];
    struct move_t * p = history;
    int num_moves = 0;
    colour side;

    if (b.gameover()) {
        std::cout << "Game is complete!\n";
        return;
    }

    while (!b.gameover()) {
        b.print_board();
        b.getSide( &side );
        switch ( side ) {
            case white:
                player_move = input_move( b );
                print_move(player_move);
                b = doMove( b, player_move );
                *p++ = player_move;
                break;
            case black:
                std::cout << "Computer thinking...\n";
                comp_move = Negamax::search( b, plies, black );
                b = doMove( b, comp_move );
                *p++ = comp_move;
                break;
            default:
                break;
        }
        num_moves++;
    }

    b.print_board();
    for (int i=0; i<num_moves; i++) {
        print_move(history[i]);
        std::cout << std::endl;
    }

    switch (b.is_checkmate()) {
        case 1:
            std::cout << "Computer wins" << std::endl;
            break;
        case -1:
            std::cout << "Player wins" << std::endl;
            break;
        case 0:
            std::cout << "Draw" << std::endl;
            break;
    }

}


void play_black(int plies) {
    board b;
    play_black(plies, b);
}

void play_black(int plies, board b) {
    init();
    struct move_t comp_move;
    struct move_t player_move;
    struct move_t history[269];
    struct move_t * p = history;
    colour side;

    if (b.gameover()) {
        std::cout << "Game is complete!\n";
        return;
    }

    while (!b.gameover()) {
        b.print_board();
        b.getSide( &side );
        switch ( side ) {
            case black:
                player_move = input_move( b );
                b = doMove( b, player_move );
                *p++ = player_move;
                break;
            case white:
                std::cout << "Computer thinking...\n";
                comp_move = Negamax::search( b, plies, white );
                b = doMove( b, comp_move );
                *p++ = comp_move;
                break;
            default:
                break;
        }
    }

    b.print_all();

    switch (b.is_checkmate()) {
        case -1:
            std::cout << "Computer wins" << std::endl;
            break;
        case 1:
            std::cout << "Player wins" << std::endl;
            break;
        case 0:
            std::cout << "Draw" << std::endl;
            break;
    }

}


void play(int plies, colour side) {
    if (side==white) play_white(plies);
    else             play_black(plies);
}

void play(int plies, board b, colour side) {
    if (side==white) play_white(plies,b);
    else             play_black(plies,b);
}
