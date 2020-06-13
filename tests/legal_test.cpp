#include "board.h"
#include "move.h"
#include "twiddle.h"
#include <iostream>

int main() {
    board b( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 " );
    int i;
    move_t moves[256];
    move_t legal[256];
    int n_moves = b.gen_moves( moves );
    int n_legal = 0;
    
    std::cout << n_moves << " pseudo-legal moves:\n";
    
    for ( i=0; i<n_moves; i++ ) {
        std::cout << "  ";
        print_move( moves[i] );
        std::cout << std::endl;
        if ( b.is_legal( moves[i] ) ) {
            legal[n_legal] = moves[i];
            n_legal++;
        }
    }
    
    std::cout << std::endl << n_moves << " legal moves:\n";
    
    for ( i=0; i<n_legal; i++ ) {
        std::cout << "  ";
        print_move( moves[i] );
        std::cout << std::endl;
    }
    
    return 0;
}