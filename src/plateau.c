#include "plateau.h"

b8 point_is_in_board(u8 line, u8 row, u8 line_count, u8 row_count){
    return (line < line_count && row < row_count);
}

u16 point_to_index_conversion(u8 line, u8 row, u8 line_count, u8 row_count){

    // Le modulo permet juste d'éviter les problèmes d'accès mémoire non autorisés, log warning pour les builds debug

    if(!point_is_in_board(line, row, line_count, row_count)) WARN_TERMINAL("point_to_index_conversion -> Les points considérés sont en dehors du tableau!!");
    return (line * row_count + row) % (line_count * row_count) ;
}



board_t* board_alloc(u8 line_count, u8 row_count, u8 player_count, u8 hedgehog_count, u8 hedgehog_winning_count){
   
    board_t* new_board = (board_t*)malloc(sizeof(board_t));
    
    if(!new_board){  // TODO  Créer une macro magique pour observer les bad allocations et les log avec __FILE__ __LINE__ ...
        ERROR_TERMINAL("board_init -> L'allocation du plateau n'a pas réussi!");
        return NULL;
    }

    new_board->row_count = row_count;
    new_board->line_count = line_count;
    new_board->player_count = player_count;
    new_board->hedgehog_count = hedgehog_count;
    new_board->hedgehog_winning_count = hedgehog_winning_count;

    new_board->cells = (cell_t*)malloc(line_count * row_count * sizeof(cell_t));
    new_board->cleared_hedgehog_count = (u8*)malloc(player_count * sizeof(u8));

    if(!new_board->cells || !new_board->cleared_hedgehog_count){  // TODO: Créer une macro magique pour observer les bad allocations et les log avec __FILE__ __LINE__ ...
        ERROR_TERMINAL("board_init -> L'allocation des tableaux interne n'a pas réussi!");
        if(new_board->cells) free(new_board->cells);
        if(new_board->cleared_hedgehog_count) free(new_board->cleared_hedgehog_count);
        free(new_board);
        return NULL;
    }

    for(u16 cur_cell = 0; cur_cell < line_count * row_count; cur_cell++){
            
        new_board->cells[cur_cell].stack = (u8*)malloc(player_count * hedgehog_count * sizeof(u8));
        new_board->cells[cur_cell].s_top = 0;
        new_board->cells[cur_cell].is_trap = false;
        
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
    free(board->cleared_hedgehog_count);
    free(board->cells);
    free(board);
}




void board_setup_default_traps(board_t* board){

    // Mise en place des cases piégées, fixées universellement sur tout tableau

    u16 piege_1_case = point_to_index_conversion(0, 2, board->line_count, board->row_count);
    u16 piege_2_case = point_to_index_conversion(1, 6, board->line_count, board->row_count);
    u16 piege_3_case = point_to_index_conversion(2, 4, board->line_count, board->row_count);
    u16 piege_4_case = point_to_index_conversion(3, 5, board->line_count, board->row_count);
    u16 piege_5_case = point_to_index_conversion(4, 3, board->line_count, board->row_count);
    u16 piege_6_case = point_to_index_conversion(5, 7, board->line_count, board->row_count);

    if(point_is_in_board(0, 2, board->line_count, board->row_count)) board->cells[piege_1_case].is_trap = true;
    if(point_is_in_board(1, 6, board->line_count, board->row_count)) board->cells[piege_2_case].is_trap = true;
    if(point_is_in_board(2, 4, board->line_count, board->row_count)) board->cells[piege_3_case].is_trap = true;
    if(point_is_in_board(3, 5, board->line_count, board->row_count)) board->cells[piege_4_case].is_trap = true;
    if(point_is_in_board(4, 3, board->line_count, board->row_count)) board->cells[piege_5_case].is_trap = true;
    if(point_is_in_board(5, 7, board->line_count, board->row_count)) board->cells[piege_6_case].is_trap = true;

}




void board_push(board_t* b, u8 line, u8 row, u8 hedgehog){
    
    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    c->stack[c->s_top++] = hedgehog;

    //Si un hérisson a bougé dans la dernière colonne, on l'ajoute au compte de ceux qui ont fini
    if(row == b->row_count - 1)board_add_cleared_hedgehog(b, hedgehog - 'A');

}

u8 board_pop(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    if(c->s_top) return c->stack[--c->s_top];      // Conversion booléenne implicite

    else{
        ERROR_TERMINAL("board_pop -> La pile est vide!");
        return 0;
    }

}

i32 board_height(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    return c->s_top;

}

u8 board_top(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    if(c->s_top) return c->stack[c->s_top - 1]; // Conversion booléenne implicite
    
    else{
        ERROR_TERMINAL("board_top -> Le hérisson demandé n'existe pas!");
        return 0;
    }

}

u8 board_peek(board_t* b, u8 line, u8 row, u8 pos){

    cell_t* c = &b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];

    if(pos >= c->s_top){
        ERROR_TERMINAL("board_peek -> Peeked position is outside of the stack!");
        return 0;
    }

    return c->stack[(c->s_top - 1) - pos];

}


b8 board_is_cell_trap(board_t* b, u8 line, u8 row){

    u16 projected_index = point_to_index_conversion(line, row, b->line_count, b->row_count);
    return (b->cells[projected_index].is_trap);    
    
}

u8 board_add_cleared_hedgehog(board_t* b, u16 player){

    // Les joueurs sont indicés à partir de zéro!

    if(player >= b->player_count){
        ERROR_TERMINAL("board_add_cleared_hedgehog -> Le joueur n'est pas connu!");
        return 0;
    }

    return ++(b->cleared_hedgehog_count[player]);
}   



void cell_print(board_t* b, u8 line, u8 row, u8 slice){

    u8 top = 'X';   // Permet de voir des cas non-traités
    i8 uppercase_to_lowercase_offset = 'a' - 'A';

    // OK d'avoir une copie locale, nous ne faisons que lire
    
    cell_t c = b->cells[point_to_index_conversion(line, row, b->line_count, b->row_count)];
    u32 hedgehog_count = board_height(b, line, row);
    if(hedgehog_count) top = board_top(b, line, row);   // Conversion booléenne implicite

    switch(slice){

        case 0: // Haut de la case

            if(c.is_trap) printf(" vvv ");
            else printf(" --- ");
            break;

        case 1: // Première ligne interne

            if(c.is_trap) printf(">");
            else printf("|");

            if(hedgehog_count) printf("%c%c%c", top, top, top);     // Conversion booléenne implicite
            else printf("   ");
            
            if(c.is_trap) printf("<");
            else printf("|");
            
            break;

        case 2: // Deuxième ligne interne

            if(c.is_trap) printf(">");
            else printf("|");

            if(hedgehog_count == 0) printf("   ");

            else if (hedgehog_count == 1) printf("%c%c%c", top, top, top);

            else if (hedgehog_count == 2){

                u8 second_hedgehog = board_peek(b, line, row, 1) + uppercase_to_lowercase_offset;
                printf("%c%c%c", second_hedgehog, second_hedgehog, second_hedgehog);
            }

            else if (hedgehog_count == 3){

                u8 second_hedgehog = board_peek(b, line, row, 1) + uppercase_to_lowercase_offset;
                u8 third_hedgehog = board_peek(b, line, row, 2) + uppercase_to_lowercase_offset;
                printf("%c %c", second_hedgehog, third_hedgehog);
            }

            else{

                u8 second_hedgehog = board_peek(b, line, row, 1) + uppercase_to_lowercase_offset;
                u8 third_hedgehog = board_peek(b, line, row, 2);
                u8 fourth_hedgehog = board_peek(b, line, row, 3) + uppercase_to_lowercase_offset;
                printf("%c%c%c", second_hedgehog, third_hedgehog, fourth_hedgehog);
            }


            if(c.is_trap) printf("<");
            else printf("|");

            break;

        case 3: // Bas de la case

            if(hedgehog_count >= 2 && hedgehog_count <= 9){
                if(c.is_trap) printf(" ^%d^ ", hedgehog_count);
                else printf(" -%d- ", hedgehog_count);
            }

            else if(hedgehog_count >= 10 && hedgehog_count <= 99){
                if(c.is_trap) printf(" %d^ ", hedgehog_count);
                else printf(" %d- ", hedgehog_count);
            }

            else if (hedgehog_count >= 100 && hedgehog_count <= 999){
                if(c.is_trap) printf(" %d ", hedgehog_count);
                else printf(" %d ", hedgehog_count);
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

    for(u8 row = 0; row < b->row_count; row++){
        printf("  ");
        cell_print(b, line, row, 0);
    }

    printf("\nline%c", highlight_character);
    for(u8 row = 0; row < b->row_count; row++){
         printf("  ");
         cell_print(b, line, row, 1);
    }

    if(line >= 9) printf("\n %d %c", line + 1, highlight_character);
    else printf("\n  %d %c", line + 1, highlight_character);
    for(u8 row = 0; row < b->row_count; row++){
         printf("  ");
         cell_print(b, line, row, 2);
    }

    printf("\n    %c", highlight_character);
    for(u8 row = 0; row < b->row_count; row++){
         printf("  ");
         cell_print(b, line, row, 3);
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

    for(u8 line = 0; line < b->line_count; line++) line_print(b, line, line == highlighted_line);
    
    bottom_print(b);
    printf("\n");
}

