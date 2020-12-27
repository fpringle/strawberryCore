/* Copyright 2020 Freddy Pringle */
#include "play.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "action.h"
#include "board.h"
#include "init.h"
#include "move.h"
#include "parse.h"
#include "search.h"
#include "typedefs.h"



namespace chessCore {

Player::Player() : board::board() {
    user_colour = white;
    iterative_deepening_timeout = 60;
    searcher = new Searcher(&trans_table);
}

Player::Player(colour userColour) : board::board() {
    user_colour = userColour;
    iterative_deepening_timeout = 60;
    searcher = new Searcher(&trans_table);
}

Player::Player(bitboard * startPositions, bool * castling, bool ep, int dpp,
        uint8_t clock, uint8_t full_clock, colour side,
        value_t open_val, value_t end_val, uint64_t hash) :
    board::board(startPositions, castling, ep, dpp, clock, full_clock,
        side, open_val, end_val, hash) {
    user_colour = white;
    iterative_deepening_timeout = 60;
    searcher = new Searcher(&trans_table);
}

Player::Player(const Player& p1) : board::board(p1) {
    trans_table = p1.getTable();
    move_history = p1.getHistory();
    move_history_san = p1.getHistorySAN();
    user_colour = white;
    iterative_deepening_timeout = 60;
    searcher = new Searcher(&trans_table);
}

Player::Player(std::string fen) : board::board(fen) {
    user_colour = white;
    iterative_deepening_timeout = 60;
    searcher = new Searcher(&trans_table);
}

void Player::setTimeout(int timeout) {
    if (timeout > 0) iterative_deepening_timeout = timeout;
}

int Player::getTimeout() {
    return iterative_deepening_timeout;
}

namespace {
    std::string upper_string(std::string s) {
        std::stringstream ss;
        for (char c : s) {
            ss << char(toupper(c));
        }
        return ss.str();
    }
}

void Player::read_config(std::string filename) {
    std::cout << "Reading config file\n";
    Config cfg;

    try {
        parse_file(filename, &cfg);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return;
    }

    Config::iterator it;

    // iterative deepening timeout
    it = cfg.find("MAX_SEARCH_TIME");
    if (it != cfg.end()) {
        try {
            int max_search_time = std::stoi(it->second);
            iterative_deepening_timeout = max_search_time;
        }
        catch(...) {
            std::cerr << "Unable to parse config file for MAX_SEARCH_TIME."
                      << std::endl << "Using default value of "
                      << iterative_deepening_timeout << "." << std::endl;
        }
    } else {
        std::cerr << "Unable to parse config file for MAX_SEARCH_TIME."
                  << std::endl << "Using default value of "
                  << iterative_deepening_timeout << "." << std::endl;
    }

    // user colour
    it = cfg.find("USER_COLOUR");
    if (it != cfg.end()) {
        try {
            std::string parsed_user_colour = upper_string(it->second);
            if (parsed_user_colour == "WHITE") {
                user_colour = white;
            } else if (parsed_user_colour == "BLACK") {
                user_colour = black;
            } else {
                throw;
            }
        }
        catch(...) {
            std::cerr << "Unable to parse config file for USER_COLOUR."
                      << std::endl << "Using default value of " << user_colour
                      << "." << std::endl;
        }
    } else {
        std::cerr << "Unable to parse config file for USER_COLOUR." << std::endl
                  << "Using default value of " << user_colour
                  << "." << std::endl;
    }
}

std::vector<move_t> Player::getHistory() const {
    return move_history;
}

std::vector<std::string> Player::getHistorySAN() const {
    return move_history_san;
}

TransTable Player::getTable() const {
    return trans_table;
}

void Player::print_history(std::ostream& cout) const {
    int sz = move_history.size();
    for (int i=0; i < sz; i+=2) {
        cout << int((i / 2) + 1) << ". " << mtos(move_history[i]);
        if (i + 1 < sz) {
            cout << "    " << mtos(move_history[i + 1]) << std::endl;
        } else {
            cout << std::endl;
        }
    }
}

void Player::print_history_san(std::ostream& cout) const {
    int sz = move_history_san.size();
    for (int i=0; i < sz; i+=2) {
        cout << int((i / 2) + 1) << ". " << move_history_san[i];
        if (i + 1 < sz) {
            cout << "    " << move_history_san[i + 1] << std::endl;
        } else {
            cout << std::endl;
        }
    }
}

void Player::print_table(std::ostream& cout) {
    TransTable::iterator it;
    for (it = trans_table.begin(); it != trans_table.end(); it++) {
        cout << it->first << ":" << std::endl
             << it->second << std:: endl;
    }
}

void Player::print_board(std::ostream& cout) const {
    if (user_colour == white) {
        board::print_board(cout);
    } else {
        board::print_board_flipped(cout);
    }
}

void Player::save_table(std::string filename) {
    std::ofstream fil(filename);
    record_t rec;
    TransTable::iterator it;
    for (it = trans_table.begin(); it != trans_table.end(); it++) {
        fil << it->first << ",";
        rec = it->second;
        fil << rec.signature << ","
            << + rec.best_move << ","
            << + rec.depth << ","
            << rec.score << ","
            << + rec.age << ","
            << (rec.flag == EXACT ? "EXACT" :
                rec.flag == LOWER ? "LOWER" :
                "UPPER") << std::endl;
    }
    fil.close();
}

void Player::load_table(std::string filename) {
    std::string line;
    std::ifstream fil(filename);
    std::stringstream num;
    int comma_1, comma_2, length;
    record_t rec;
    uint32_t ind;
    uint64_t sig;
    move_t move;
    uint8_t depth;
    value_t ibv;
    uint8_t age;
    std::string flag_str;
    valueType flag;

    while (fil) {
        fil >> line;

        comma_1 = 0;
        comma_2 = line.find(",", comma_1 + 1);
        length = comma_2 - comma_1;
        num << line.substr(comma_1, length);
        num >> ind;
        num.str("");
        num.clear();

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",", comma_1);
        length = comma_2 - comma_1;
        num << line.substr(comma_1, length);
        num >> sig;
        num.str("");
        num.clear();

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",", comma_1);
        length = comma_2 - comma_1;
        move = std::stoi(line.substr(comma_1, length));

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",", comma_1);
        length = comma_2 - comma_1;
        depth = std::stoi(line.substr(comma_1, length));

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",", comma_1);
        length = comma_2 - comma_1;
        ibv = std::stoi(line.substr(comma_1, length));

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",", comma_1);
        length = comma_2 - comma_1;
        age = std::stoi(line.substr(comma_1, length));

        comma_1 = comma_2 + 1;
        flag_str = line.substr(comma_1);
        flag = flag_str == "EXACT" ? EXACT :
               flag_str == "LOWER" ? LOWER : UPPER;

        rec = {sig, move, depth, ibv, age, flag};
        trans_table[ind] = rec;
    }
}


move_t Player::input_move() const {
    move_t ret;
    MoveList moves = gen_legal_moves();

    std::string inp;
    std::cout << "Enter move as FileRankFileRank (e.g. e2e4): ";
    std::cin >> inp;
    ret = stom(moves, inp);

    while (ret == 0) {
        std::cout << "Sorry, that's not a valid move.\n"
                  << "Enter move as FileRankFileRank (e.g. e2e4): ";
        std::cin >> inp;
        ret = stom(moves, inp);
    }
    return ret;
}

move_t Player::search() {
    return searcher->search(reinterpret_cast<board*>(this),
                            iterative_deepening_timeout, true);
}

move_t Player::search(int timeout) {
    return searcher->search(reinterpret_cast<board*>(this), timeout, true);
}

void Player::play(colour playerSide, int timeout) {
//    init();
    move_t comp_move;
    move_t player_move;
    int num_moves = 0;
    colour movingSide;

    if (gameover()) {
        std::cout << "Game is complete!\n";
        return;
    }

    while (!gameover()) {
        if (playerSide == white) {
            print_board();
        } else {
            print_board_flipped();
        }

        getSide(&movingSide);

        if (movingSide == playerSide) {
            player_move = input_move();
            doMoveInPlace(player_move);
        } else {
            std::cout << "Computer thinking...    " << std::endl;
            std::cout << "Timeout: " << iterative_deepening_timeout
                      << std::endl;
            comp_move = search(timeout);
            std::cout << "Computer move: " << mtos(comp_move)
                      << std::endl;
            doMoveInPlace(comp_move);
        }
        num_moves++;
    }

    if (playerSide == white) {
        print_board();
    } else {
        print_board_flipped();
    }
    print_history();

    if (is_checkmate()) {
        getSide(&movingSide);
        if (movingSide == playerSide) {
            std::cout << "Computer wins" << std::endl;
        } else {
            std::cout << "Player wins" << std::endl;
        }
    } else if (is_stalemate()) {
        std::cout << "Draw" << std::endl;
    }
}

void Player::play() {
    move_t comp_move;
    move_t player_move;
    int num_moves = 0;
    colour movingSide;

    if (gameover()) {
        std::cout << "Game is complete!\n";
        return;
    }

    while (!gameover()) {
        print_board();
        getSide(&movingSide);
        if (movingSide == user_colour) {
            player_move = input_move();
            doMoveInPlace(player_move);
        } else {
            std::cout << "Computer thinking...    " << std::endl;
            std::cout << "Timeout: " << iterative_deepening_timeout
                      << std::endl;
            comp_move = search();
            std::cout << "Computer move: " << SAN_pre_move(comp_move)
                      << std::endl;
            doMoveInPlace(comp_move);
        }
        num_moves++;
    }

    print_board();
    print_history();

    if (is_checkmate()) {
        getSide(&movingSide);
        if (movingSide == user_colour) {
            std::cout << "Computer wins" << std::endl;
        } else {
            std::cout << "Player wins" << std::endl;
        }
    } else if (is_stalemate()) {
        std::cout << "Draw" << std::endl;
    }
}


void two_players() {
    Player* gamestate = new Player();
    colour movingSide;
    move_t player_move;

    while (!gamestate->gameover()) {
        gamestate->print_board();

        gamestate->getSide(&movingSide);

        std::cout << (movingSide == white ? "White" : "Black") << " to move.\n";

        player_move = gamestate->input_move();
        gamestate->doMoveInPlace(player_move);
    }

    gamestate->print_board();
    gamestate->print_history();

    if (gamestate->is_checkmate()) {
        gamestate->getSide(&movingSide);
        std::cout << movingSide << " wins!" << std::endl;
    } else if (gamestate->is_stalemate()) {
        std::cout << "Draw" << std::endl;
    }
}

void two_computers() {
    board* gamestate = new board;
    Player* whiteComp = new Player();
    Player* blackComp = new Player();
    colour movingSide;
    std::string side;
    move_t comp_move;

    while (!gamestate->gameover()) {
        gamestate->print_board();
        gamestate->getSide(&movingSide);
        side = (movingSide == white ? "White" : "Black");
        std::cout << side << " thinking...    " << std::endl;
        if (movingSide == white) {
            comp_move = whiteComp->search();
        } else {
            comp_move = blackComp->search();
        }
        std::cout << side << " move: " << mtos(comp_move) << std::endl;
        whiteComp->doMoveInPlace(comp_move);
        blackComp->doMoveInPlace(comp_move);
        gamestate->doMoveInPlace(comp_move);
    }

    gamestate->print_board();
    whiteComp->print_history();

    if (gamestate->is_checkmate()) {
        gamestate->getSide(&movingSide);
        std::cout << movingSide << " wins!" << std::endl;
    } else if (gamestate->is_stalemate()) {
        std::cout << "Draw" << std::endl;
    }
}


}   // namespace chessCore
