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
            
            for(i32 cell_back = cur_cell - 1; cell_back >= 0; cell_back--)
                free(new_board->cells[cell_back].stack);
            
            free(new_board->cells);
            free(new_board);
            return NULL;
        }
    }

    return new_board;
}


void board_free(board_t* board){
    for(u16 cell_index = 0; cell_index < board->line_count * board->row_count; cell_index++) 
        free(board->cells[cell_index].stack);
    free(board->cells);
    free(board);
}


void board_push(board_t* b, u8 line, u8 row, u8 hedgehog){

    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];

    c.s_top++;
    c.stack[c.s_top] = hedgehog;

}

u8 board_pop(board_t* b, u8 line, u8 row){

    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    u8 top = c.stack[c.s_top];

    c.s_top--;

    return top;

}

i32 board_height(board_t* b, u8 line, u8 row){

    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    return c.s_top;
}

u8 board_top(board_t* b, u8 line, u8 row){

    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    return c.stack[c.s_top];

}

u8 board_peek(board_t* b, u8 line, u8 row, u8 pos){

    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];

    if(pos > c.s_top){
        ERROR_TERMINAL("board_peek -> Peeked position is outside of the stack!");
        return 0;
    }

    return c.stack[c.s_top - pos];

}

void cell_print(board_t* b, u8 line, u8 row, u8 slice){

    i8 uppercase_to_lowercase_offset = 'a' - 'A';

    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    u32 hedgehog_count = board_height(b, line, row);
    u8 top = board_top(b, line, row);

    //char line_drawing_buffer[PRINT_FORMAT_LINE_INFO_SIZE + b->row_count * (PRINT_FORMAT_LINE_CELL_SIZE + PRINT_FORMAT_LINE_SPACING)];


    switch(slice){

        case 0: // Haut de la case

            if(c.is_trap) printf(" VVV ");
            else printf(" --- ");
            break;

        case 1: // Première ligne interne

            if(hedgehog_count >= 1) printf("|%c%c%c|", top, top, top);
            else printf("|   |");
            break;

        case 2: // Deuxième ligne interne

            if(hedgehog_count == 0) printf("|   |");

            else if (hedgehog_count == 1){
                u8 second_hedgehog = board_peek(b, line, row, 1) + uppercase_to_lowercase_offset;
                printf("|%c%c%c|", second_hedgehog, second_hedgehog, second_hedgehog);
            }

            else{
                u8 second_hedgehog = board_peek(b, line, row, 1) + uppercase_to_lowercase_offset;
                u8 third_hedgehog = board_peek(b, line, row, 2) + uppercase_to_lowercase_offset;
                printf("|%c %c|", second_hedgehog, third_hedgehog);
            }
            break;

        case 3: // Troisième ligne interne

            if(hedgehog_count >= 2){
                if(c.is_trap) printf(" ^%d^ ", hedgehog_count);
                else printf(" -%d- ", hedgehog_count);
            }

            else{
                if(c.is_trap) printf(" ^^^ ");
                else printf(" --- ");
            }

            break;

        default:    // La slice passée en paramètre est invalide
            ERROR_TERMINAL("cell_print -> Invalid slice!");
            break;
    }
}

void top_print(board_t* b){

    // Affichage permettant d'indiquer rapidement si le build actuel est debug ou prood

    #ifdef DEBUG_BUILD
        printf("START (DBG)  ");
    #else
        printf("      START  ");
    #endif

    // L'affichage de start consomme la première colonne, nous passons donc (nb_colonnes - 2) colonnes afin de pouvoir écrire FINISH sur la dernière
    
    for(u16 i = 0; i < b->row_count - 2; i++){
        printf("       ");
    }
    printf("FINISH\n\n");

    printf("      ");
    for(int i = 0; i < b->row_count; i++){
        printf("  row  ");
    }
    printf("\n");

    printf("      ");
    for(int i = 0; i < b->row_count; i++){
        printf("   %c   ", 'a'+i);
    }
    printf("\n");
}

void line_print(board_t* b, u8 line, b8 is_highlighted){
    
    char highlight_character = ' ';
    
    if(is_highlighted) highlight_character = '>';
    
    printf("    %c", highlight_character);


    for(int i = 0; i < b->row_count; i++){
        printf("  ");
        cell_print(b, line, i, 0);
    }

    printf("\nline%c", highlight_character);
    for(int i = 0; i < b->row_count; i++){
         printf("  ");
         cell_print(b, line, i, 1);
    }

    printf("\n  %d %c", line, highlight_character);
    for(int i = 0; i < b->row_count; i++){
         printf("  ");
         cell_print(b, line, i, 2);
    }

    printf("\n    %c", highlight_character);
    for(int i = 0; i < b->row_count; i++){
         printf("  ");
         cell_print(b, line, i, 3);
    }

    printf("\n\n");
}

void bottom_print(board_t* b){
     printf("      ");
    for(int i = 0; i < b->row_count; i++){
        printf("  row  ");
    }
    printf("\n");

    printf("      ");
    for(int i = 0; i < b->row_count; i++){
        printf("   %c   ", 'a'+i);
    }
    printf("\n");

}

void board_print(board_t* b, u8 highlighted_line){

    printf("\n");
    top_print(b);

    for(u8 i = 0; i < b->line_count; i++){
        line_print(b, i, i == highlighted_line);
    }
    bottom_print(b);
    printf("\n");
}

