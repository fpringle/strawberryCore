#include "board.h"
#include "move.h"
#include "play.h"
#include "action.h"
#include "search.h"
#include "init.h"

#include <string>
#include <iostream>

#define SEARCHER PVS


move_t input_move(board b) {
    move_t ret;
    move_t moves[256];
    int n_moves = b.gen_legal_moves(moves);

    std::string inp;
    std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
    std::cin >> inp;
    ret = stom(moves, n_moves, inp);

    while (ret.give() == 0) {
        std::cout << "Sorry, that's not a valid move.\n";
        std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
        std::cin >> inp;
        ret = stom(moves, n_moves, inp);
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
        b.getSide(&side);
        switch (side) {
        case white:
            player_move = input_move(b);
            print_move(player_move);
            b = doMove(b, player_move);
            *p++ = player_move;
            break;
        case black:
            std::cout << "Computer thinking...\n";
            comp_move = SEARCHER::search(b, plies);
            b = doMove(b, comp_move);
            *p++ = comp_move;
            break;
        default:
            break;
        }
        num_moves++;
    }

    b.print_board();
    for (int i = 0; i < num_moves; i++) {
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
        b.getSide(&side);
        switch (side) {
        case black:
            player_move = input_move(b);
            b = doMove(b, player_move);
            *p++ = player_move;
            break;
        case white:
            std::cout << "Computer thinking...\n";
            comp_move = SEARCHER::search(b, plies);
            b = doMove(b, comp_move);
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
    if (side == white) play_white(plies);
    else play_black(plies);
}

void play(int plies, board b, colour side) {
    if (side == white) play_white(plies, b);
    else play_black(plies, b);
}

//////////////////////////////////////////////////////

Player::Player() : board::board() {
}

Player::Player(bitboard * startPositions, bool * castling, bool ep, int dpp,
        uint8_t clock, uint8_t full_clock, colour side, int32_t val, uint64_t hash) :
    board::board(startPositions, castling, ep, dpp, clock, full_clock, side, val, hash) {
}

Player::Player(Player& p1) : board::board(p1) {
    trans_table = p1.getTable();
    move_history = p1.getHistory();
}

Player::Player(std::string fen) : board::board(fen) {
}

bool Player::lookup(uint64_t pos_hash, record_t * dest) {
    uint32_t index = (uint32_t) pos_hash;
    std::map<uint32_t, record_t>::iterator it = trans_table.find(index);
    if (it == trans_table.end() || it->second.signature != pos_hash) {
        return false;
    }
    else {
        *dest = it->second;
        return true;
    }
}

std::vector<move_t> Player::getHistory() {
    return move_history;
}

std::map<uint32_t, record_t> Player::getTable() {
    return trans_table;
}

void Player::print_history() {
    for (auto& move : move_history) {
        std::cout << move << std::endl;
    }
}



move_t Player::input_move() {
    move_t ret;
    move_t moves[256];
    int n_moves = gen_legal_moves(moves);

    std::string inp;
    std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
    std::cin >> inp;
    ret = stom(moves, n_moves, inp);

    while (ret.give() == 0) {
        std::cout << "Sorry, that's not a valid move.\n";
        std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
        std::cin >> inp;
        ret = stom(moves, n_moves, inp);
    }
    return ret;
}

void Player::play(colour side, int plies) {
//    init();
    move_t comp_move;
    move_t player_move;
    int num_moves = 0;
    colour movingSide;

    if (gameover()) {
        std::cout << "Game is complete!\n";
        return;
    }

    while (! gameover()) {
        if (side == black) print_board();
        else print_board_flipped();
        getSide(&movingSide);
        if (movingSide != side) {
            player_move = input_move();
            doMoveInPlace(player_move);
        }
        else {
            std::cout << "Computer thinking...    " << std::endl;
            comp_move = search(plies);
            std::cout << "Computer move: " << comp_move << std::endl;
            doMoveInPlace(comp_move);
        }
        num_moves++;
    }

    if (side == black) print_board();
    else print_board_flipped();
    print_history();

    switch (is_checkmate()) {
    case 1:
        if (side == black) {
            std::cout << "Computer wins" << std::endl;
        }
        else {
            std::cout << "Player wins" << std::endl;
        }
        break;
    case -1:
        if (side == black) {
            std::cout << "Player wins" << std::endl;
        }
        else {
            std::cout << "Computer wins" << std::endl;
        }
        break;
    case 0:
        std::cout << "Draw" << std::endl;
        break;
    }
}

