#ifndef __PLAY_H
#define __PLAY_H

#include <string>
#include "board.h"
#include "move.h"


// defined in play.cpp
move_t input_move( board );
void play_white(int);
void play_white(int,board);
void play_black(int,board);


#endif
