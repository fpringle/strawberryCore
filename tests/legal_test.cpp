#include "board.h"
#include "move.h"
#include "init.h"
#include "twiddle.h"
#include <iostream>
#include <chrono>


#define DEBUG 0
#define TIMER 1
bool verify_legalgen(board);

int main() {
    init();
    board b("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
    if (verify_legalgen(b)) std::cout << "\nTest passed\n";

    return 0;
}

bool verify_legalgen(board b) {
#if DEBUG
    b.print_board();
#endif
    int i;
    move_t moves[256];
    move_t legal[256];
    move_t legal_gen[256];
#if TIMER
    auto _p2 = std::chrono::high_resolution_clock::now();
#endif
    int n_moves = b.gen_moves(moves);
#if TIMER
    auto p2 = std::chrono::high_resolution_clock::now();
#endif
    int n_legal = 0;

    std::cout << n_moves << " pseudo-legal moves";
#if DEBUG
    std::cout << ":";
#endif
    std::cout << std::endl;

    for (i = 0; i < n_moves; i++) {
#if DEBUG
        std::cout << "  ";
        std::cout << moves[i];
#endif
        if (b.is_legal(moves[i])) {
            legal[n_legal] = moves[i];
            n_legal++;
#if DEBUG
            std::cout << std::endl;
#endif
        }
#if DEBUG
        else {
            std::cout << " (illegal)\n";
        }
#endif
    }
#if TIMER
    auto p3 = std::chrono::high_resolution_clock::now();
#endif

    std::cout << n_legal << " legal moves (checked)";
#if DEBUG
    std::cout << ":";
#endif
    std::cout << std::endl;

#if DEBUG
    for (i = 0; i < n_legal; i++) {
        std::cout << "  ";
        std::cout << legal[i];
        std::cout << std::endl;
    }
#endif

#if TIMER
    auto p4 = std::chrono::high_resolution_clock::now();
#endif
    int n_gen_legal = b.gen_legal_moves(legal_gen);
#if TIMER
    auto p5 = std::chrono::high_resolution_clock::now();
#endif

    std::cout << n_gen_legal << " generated legal moves";
#if DEBUG
    std::cout << ":";
#endif
    std::cout << std::endl;

#if DEBUG
    for (i = 0; i < n_gen_legal; i++) {
        std::cout << "  ";
        std::cout << legal_gen[i];
        std::cout << std::endl;
    }
#endif

#if TIMER
    auto gen_dur = std::chrono::duration_cast<std::chrono::microseconds>(p2 - _p2);
    auto legalcheck_dur = std::chrono::duration_cast<std::chrono::microseconds>(p3 - p2);
    auto legalgen_dur = std::chrono::duration_cast<std::chrono::microseconds>(p5 - p4);

    std::cout << std::endl;
    std::cout << "Generate pseudo-legal moves (using board::gen_moves):           " << gen_dur.count() << " microseconds\n";
    std::cout << "Check pseudo-legal moves for legality (using board::is_legal):  " << legalcheck_dur.count() << " microseconds\n";
    std::cout << "Generate legal moves (using board::gen_legal_moves):            " << legalgen_dur.count() << " microseconds\n";
#endif

    return n_legal == n_gen_legal;
}

