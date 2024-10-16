#pragma once

#include "types.h"
#include <stdio.h>

#define DEFAULT_ROW_COUNT 9
#define DEFAULT_LINE_COUNT 6


typedef struct cell_s{

    u8* stack;
    i32 s_top; //Le haut de la pile se lit a stack[s_top]
    b8 is_trap;

} cell_t;

typedef struct board_s{

    u8 row_count;
    u8 line_count;
    cell_t** cells; 

} board_t;


void board_push(board_t* b, u8 line, u8 row, u8 ctn);

u8 board_pop(board_t* b, u8 line, u8 row);

i32 board_height(board_t* b, u8 line, u8 row);

u8 board_top(board_t* b, u8 line, u8 row);

u8 board_peek(board_t* b, u8 line, u8 row, u8 pos); // pos=0 => top

void cell_print(board_t* b, u8 line, u8 row, u8 slice);

void board_print(board_t* b, u8 highlighted_line); // hl_line=-1 => rien
