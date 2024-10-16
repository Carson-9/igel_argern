#include "plateau.h"

u16 point_to_index_conversion(u8 line, u8 row, u8 line_count, u8 row_count){

    // Le modulo permet juste d'éviter les problèmes d'accès mémoire non autorisés, log warning pour les builds debug

    if(line >= line_count || row >= row_count) WARN_TERMINAL("point_to_index_conversion -> Les points considérés sont en dehors du tableau!!");
    return (line * row_count + row) % (line_count * row_count) ;
}


board_t* board_alloc(u8 line_count, u8 row_count, u8 player_count, u8 hedgehog_count){
    board_t* new_board = (board_t*)malloc(sizeof(board_t));
    
    if(!new_board){  // TODO  Créer une macro magique pour observer les bad allocations et les log avec __FILE__ __LINE__ ...
        ERROR_TERMINAL("board_init -> L'allocation du plateau n'a pas réussi!");
        return NULL;
    }

    new_board->row_count = row_count;
    new_board->line_count = line_count;
    new_board->cells = (cell_t*)malloc(line_count * row_count * sizeof(cell_t));

    if(!new_board->cells){  // TODO: Créer une macro magique pour observer les bad allocations et les log avec __FILE__ __LINE__ ...
        ERROR_TERMINAL("board_init -> L'allocation des cases n'a pas réussi!");
        free(new_board);
        return NULL;
    }

    for(u16 cur_cell = 0; cur_cell < line_count * row_count; cur_cell++){
            
        new_board->cells[cur_cell].stack = (u8*)malloc(player_count * hedgehog_count * sizeof(u8));
        
        if(!new_board->cells[cur_cell].stack){  // TODO  Créer une macro magique pour observer les bad allocations et les log avec __FILE__ __LINE__ ...
            ERROR_TERMINAL("board_init -> L'allocation d'un stack n'a pas réussi!");
            
            for(i16 cell_back = cur_cell - 1; cell_back >= 0; cell_back--)
                free(new_board->cells[cell_back].stack);
            
            free(new_board->cells);
            free(new_board);
            return NULL;
        }
    }

    return new_board;
}


void board_free(board_t* board){
    for(u16 cell_index = 0; cell_index < board->line_count * board->row_count; cell_index++) free(board->cells[cell_index].stack);
    free(board->cells);
    free(board);
}


void board_push(board_t* b, u8 line, u8 row, u8 ctn){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];

    c->s_top++;
    c->stack[c->s_top] = ctn;

}

u8 board_pop(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    u8 top = c->stack[c->s_top];

    c->s_top--;

    return top;

}

i32 board_height(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];

    return c->s_top + 1;
}

u8 board_top(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    u8 top = c->stack[c->s_top];

    return top;

}

u8 board_peek(board_t* b, u8 line, u8 row, u8 pos){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    u8 peeked = c->stack[c->s_top - pos];

    return peeked;

}

void cell_print(board_t* b, u8 line, u8 row, u8 slice){

    i8 case_diff = 'A' - 'a';
    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    i32 height = board_height(b, line, row);
    u8 top = board_top(b, line, row);

    if(slice==3 && height > 1){
        if(c->is_trap){
            printf(" ^%d^ ", height);
        }
        else{
            printf(" -%d- ", height);
        }
    }
    else if(slice == 3 || slice == 0){
        if(c->is_trap){
            if(slice == 3){
                printf(" ^^^ ");
            }
            else{
                printf (" VVV ");
            }
        }
        else{
            printf(" --- ");
        }
    }
    else if(slice == 1){
        if(c->is_trap){
            printf(">%c%c%c<", top, top, top);
        }
        else{
            printf("|%c%c%c|", top, top, top);
        }
    }
    else if(slice == 2){
        u8 c1, c2, c3;

        if(height == 1){
            c1 = c2 = c3 = top;
        }
        else if(height == 2){
            c1 = c2 = c3 = board_peek(b, line, row, 1) - case_diff;
        }
        else{
            c1 = board_peek(b, line, row, 1) - case_diff;
            if(height == 3){
                c2 = ' ';
                c3 = board_peek(b, line, row, 2) - case_diff;
            }
            else{
                c2 = board_peek(b, line, row, 2) - case_diff;
                c3 = board_peek(b, line, row, 3) - case_diff;
            }
        }

        if(c->is_trap){
            printf(">%c%c%c<", c1, c2, c3);
        }
        else{
            printf("|%c%c%c|", c1, c2, c3);
        }
    }
}

void top_print(board_t* b){

    #ifdef DEBUG_BUILD
        printf("START (DBG)");
    #else
        printf("      START");
    #endif
    
    for(int i = 0; i < b->row_count-2; i++){
        printf("    ");
    }
    printf("FINISH\n\n");

    printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("  row ");
    }
    printf("\n");

    printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("   %c  ", 'a'+i);
    }
    printf("\n");
}

void line_print(board_t* b, u8 line, b8 is_highlighted){
    char last = ' ';
    if(is_highlighted){
        last = '>';
    }
    
    printf("    %c", last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 0);
    }

    printf("\nline%c", last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 1);
    }

    printf("\n  %d %c", line, last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 2);
    }

    printf("\n    %c", last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 3);
    }

    printf("\n\n");
}

void bottom_print(board_t* b){
     printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("  row ");
    }
    printf("\n");

    printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("   %c  ", 'a'+i);
    }
    printf("\n");

}

void board_print(board_t* b, u8 highlighted_line){
    top_print(b);
    for(u8 i = 0; i < b->row_count; i++){
        line_print(b, i, i==highlighted_line);
    }
    bottom_print(b);
}

