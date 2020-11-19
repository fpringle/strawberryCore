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
#include "search.h"
#include "typedefs.h"



namespace chessCore {

std::ostream& operator<<(std::ostream &out, const record_t &rec) {
    value_t val;

    out << "Best/refutation move:     " << rec.best_ref_move << std::endl
        << "Depth searched:           " << +rec.depth << std::endl
        << "Clock when last searched: " << +rec.age << std::endl
        << "Bound flag:               ";

    switch (rec.IBV_score % 4) {
        case -1:
        case 3:
            out << "Upper" << std::endl;
            val = (rec.IBV_score + 1) / 4;
            break;
        case 1:
        case -3:
            out << "Lower" << std::endl;
            val = (rec.IBV_score - 1) / 4;
            break;
        case 0:
        case -4:
        default:
            out << "Exact" << std::endl;
            val = rec.IBV_score / 4;
            break;
    }

    out << "Value:                    " << val << std::endl;
    return out;
}

Player::Player() : board::board() {
}

Player::Player(bitboard * startPositions, bool * castling, bool ep, int dpp,
        uint8_t clock, uint8_t full_clock, colour side,
        value_t open_val, value_t end_val, uint64_t hash) :
    board::board(startPositions, castling, ep, dpp, clock, full_clock,
        side, open_val, end_val, hash) {
}

Player::Player(Player& p1) : board::board(p1) {
    trans_table = p1.getTable();
    move_history = p1.getHistory();
    move_history_san = p1.getHistorySAN();
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

std::vector<std::string> Player::getHistorySAN() {
    return move_history_san;
}

std::map<uint32_t, record_t> Player::getTable() {
    return trans_table;
}

void Player::print_history(std::ostream& cout) {
    int sz = move_history.size();
    for (int i=0; i<sz; i+=2) {
        cout << int((i / 2) + 1) << ". " << move_history[i];
        if (i + 1 < sz) {
            cout << "    " << move_history[i + 1] << std::endl;
        }
        else {
            cout << std::endl;
        }
    }
}

void Player::print_history_san(std::ostream& cout) {
    int sz = move_history_san.size();
    for (int i=0; i<sz; i+=2) {
        cout << int((i / 2) + 1) << ". " << move_history_san[i];
        if (i + 1 < sz) {
            cout << "    " << move_history_san[i + 1] << std::endl;
        }
        else {
            cout << std::endl;
        }
    }
}

void Player::print_table(std::ostream& cout) {
    std::map<uint32_t, record_t>::iterator it;
    for (it = trans_table.begin(); it != trans_table.end(); it++) {
        cout << it->first << ":" << std::endl
             << it->second << std:: endl;
    }
}

void Player::save_state(std::string filename) {
    std::ofstream fil(filename);
//    fil << FEN() << std::endl;
//    fil << std::endl;
//    for (auto& move : move_history) {
//        fil << move << std::endl;
//    }
//    fil << std::endl;
    record_t rec;
    std::map<uint32_t, record_t>::iterator it;
    for (it = trans_table.begin(); it != trans_table.end(); it++) {
        fil << it->first << ",";
        rec = it->second;
        fil << rec.signature << ","
            << rec.best_ref_move.give() << ","
            << + rec.depth << ","
            << rec.IBV_score << ","
            << + rec.age << std::endl;
    }
    fil.close();
}

void Player::load_state(std::string filename) {
    std::string line;
    std::ifstream fil(filename);
    std::stringstream num;
    int comma_1, comma_2, length;
    record_t rec;
    uint32_t ind;
    uint64_t sig;
    uint16_t movedata;
    move_t move;
    uint8_t depth;
    value_t ibv;
    uint8_t age;
//    fil.seek(0);
    while (fil) {
        fil >> line;

        comma_1 = 0;
        comma_2 = line.find(",",comma_1 + 1);
        length = comma_2 - comma_1;
        num << line.substr(comma_1, length);
        num >> ind;
        num.str("");
        num.clear();

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",",comma_1);
        length = comma_2 - comma_1;
        num << line.substr(comma_1, length);
        num >> sig;
        num.str("");
        num.clear();

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",",comma_1);
        length = comma_2 - comma_1;
        movedata = std::stoi(line.substr(comma_1, length));
        move = move_t(movedata);

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",",comma_1);
        length = comma_2 - comma_1;
        depth = std::stoi(line.substr(comma_1, length));

        comma_1 = comma_2 + 1;
        comma_2 = line.find(",",comma_1);
        length = comma_2 - comma_1;
        ibv = std::stoi(line.substr(comma_1, length));

        comma_1 = comma_2 + 1;
        age = std::stoi(line.substr(comma_1));

        rec = {sig, move, depth, ibv, age};
        trans_table[ind] = rec;
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
        std::cout << "Sorry, that's not a valid move.\n"
                  << "Enter move as FileRankFileRank (e.g. e2e4): ";
        std::cin >> inp;
        ret = stom(moves, n_moves, inp);
    }
    return ret;
}

void Player::play(colour playerSide, int plies) {
//    init();
    move_t comp_move;
    move_t player_move;
    int num_moves = 0;
    colour movingSide;
    int log = 1;
    std::stringstream ss;

    if (gameover()) {
        std::cout << "Game is complete!\n";
        return;
    }

    while (! gameover()) {
        if (playerSide == white) print_board();
        else print_board_flipped();

        getSide(&movingSide);

        if (movingSide == playerSide) {
            player_move = input_move();
            doMoveInPlace(player_move);
        }
        else {
            std::cout << "Computer thinking...    " << std::endl;
            comp_move = search(plies);
            std::cout << "Computer move: " << comp_move << std::endl;
            doMoveInPlace(comp_move);
            ss << "/home/freddy/Documents/cpl/chess_net/log/log"
               << log << ".log";
            save_state(ss.str());
            ss.str("");
            log++;
        }
        num_moves++;
    }

    if (playerSide == white) print_board();
    else print_board_flipped();
    print_history();

    switch (is_checkmate()) {
    case 1:
        if (playerSide == white) {
            std::cout << "Computer wins" << std::endl;
        }
        else {
            std::cout << "Player wins" << std::endl;
        }
        break;
    case -1:
        if (playerSide == white) {
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

} // end of chessCore namespace
