#include "plateau.h"
#include "logging.h"
#include <stdlib.h>


int main(int argc, char** argv){

    board_t* new_board = board_alloc(DEFAULT_LINE_COUNT, DEFAULT_ROW_COUNT, DEFAULT_PLAYER_COUNT, DEFAULT_HEDGEHOG_COUNT);
    board_print(new_board, -1);

    return EXIT_SUCCESS;
}