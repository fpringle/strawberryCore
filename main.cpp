#include <cstdlib>
#include <limits>

#include "action.h"
#include "board.h"
#include "eval.h"
#include "hash.h"
#include "init.h"
#include "move.h"
#include "play.h"
#include "search.h"
#include "twiddle.h"


using namespace chessCore;


int main() {
    init();
    Player* player = new Player(white);
    player->play();
    return 0;
}
