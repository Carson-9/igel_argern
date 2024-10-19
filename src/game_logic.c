#include "game_logic.h"


u8 dice_roll(u8 max_value){
    return (rand() % max_value) + 1;
}


u8 random_line(board_t* b){
    return rand() % b->line_count;
}


u8 bullet_or_empty_char(b8 cond){
    if(cond) return '@';
    else return ' ';
}

void print_dice(u8 dice_val){

    u8 top_left_dot = bullet_or_empty_char(dice_val >= 2);
    u8 mid_left_dot = bullet_or_empty_char(dice_val == 6);
    u8 bot_left_dot = bullet_or_empty_char(dice_val >= 4);

    u8 mid_dot = bullet_or_empty_char(((dice_val % 2) == 1));

    u8 top_right_dot = bullet_or_empty_char(dice_val >= 4);
    u8 mid_right_dot = bullet_or_empty_char(dice_val == 6);
    u8 bot_right_dot = bullet_or_empty_char(dice_val >= 2);

    printf("      ----- \n     | %c %c |\n     | %c%c%c |\n     | %c %c |\n      ----- \n\n", 
        top_left_dot, top_right_dot, mid_left_dot, mid_dot, mid_right_dot, bot_left_dot, bot_right_dot);
}


u8 to_upper(u8 c){
    if(c < 96) return(c);
    else return c - 32;
}


b8 is_an_active_trap(board_t* b, u8 line, u8 row){

    if(!b->cells[!point_to_index_conversion(line, row, b->line_count, b->row_count)].is_trap)return false;

    for(int curr_row = 0; curr_row < row; curr_row++){
        if(board_height(b, line, curr_row) > 0)return true;
    }

    return false;
}


void init_board_default(board_t* b){

    board_setup_default_traps(b);

    for(u8 hedgehog = 0; hedgehog < b->hedgehog_count; hedgehog++){
        for(u8 player = 0; player < b->player_count; player++){

            // Pour une meilleure répartition initiale, chaque nouvel hérisson introduit un offset
            // servant à décaler la première équipe à tirer un hérisson,
            // Ceci n'est pas parfait mais semble plus équilibré qu'un ajout dans le même ordre
            
            board_push(b, random_line(b), 0, 'A' + ((player + hedgehog) % b->player_count));
        }
    }
}

u8 move_vertically(board_t* b, u8 player, u8 line, u8 row, b8 is_going_up){

    u8 offset = 1;
    if(is_going_up)offset = -offset;
    u8 new_line = line + offset;

    //Si le coup demandé par les arguments n'est pas possible, il est ignoré et, dans un build de debug, on log un warning
    //Ensuite, on renvoie un code d'erreur approprié
    if(!point_is_in_board(line, row, b->line_count, b->row_count)){
        
        WARN_TERMINAL("move_vertically -> La case de départ est hors du plateau !");
        return ORIGIN_OOB;
    }

    if(board_height(b, line, row) == 0){

        WARN_TERMINAL("move_vertically -> La case d'origine est vide !");
        return CELL_EMPTY;
    }

    if(!point_is_in_board(line, new_line, b->line_count, b->row_count)){
        
        WARN_TERMINAL("move_vertically -> La case d'arrivée est hors du plateau !");
        return TARGET_OOB;
    }

    if(is_an_active_trap(b, line, row)){

        WARN_TERMINAL("move-vertically -> La case de départ est piégée");
        return MOVE_FROM_TRAP;
    }

    u8 moving_hedgehog = board_pop(b, line, row);

    if(moving_hedgehog != 'A' + player){

        WARN_TERMINAL("move_vertically -> Le hérisson n'est pas dans l'équipe du joueur");
        return WRONG_PLAY;
    }
        
    board_push(b, new_line, row, moving_hedgehog);
    return SUCCESS;

}

void play_round_single_player(board_t* b, u8 player){
    
    u8 dice_val = dice_roll(MAX_DICE_VALUE);

    u8 does_vertical_movement;
    u8 vertical_movement_line;
    u8 vertical_movement_row;
    b8 vertical_direction;
    b8 vertical_going_up;
    u8 vertical_move_status = SUCCESS + 1;
    
    
    printf("* Joueur %c - Le dé donne :\n\n", 'A' + player);
    print_dice(dice_val);

    printf("Voulez-vous bouger un hérisson verticalement (N/o) ?\n");
    scanf(" %c", &does_vertical_movement);

    while(to_upper(does_vertical_movement) == 'O' && vertical_move_status != SUCCESS){

        printf("Depuis quelle ligne voulez-vous bouger ?\n");
        scanf("%hhu", &vertical_movement_line);
        vertical_movement_line = vertical_movement_line-1;

        printf("Depuis quelle colonne voulez-vous bouger ?\n");
        scanf(" %c", &vertical_movement_row);
        vertical_movement_row = to_upper(vertical_movement_row) - 'A';

        printf("Voulez-vous bouger vers le haut ou le bas (H/b) ?\n");
        scanf(" %c", &vertical_direction);

        vertical_going_up = (to_upper(vertical_direction) != 'B');

        printf("%d, %d\n", (int)vertical_movement_line, (int)vertical_movement_row);

        vertical_move_status = move_vertically(b, player, vertical_movement_line, vertical_movement_row, vertical_going_up);

        if(vertical_move_status == SUCCESS){

            board_print(b, dice_val-1);
            printf("Vous avez bougé verticalement.\n");
            printf("Joueur %c - Le dé donne :\n\n", 'A' + player);
            print_dice(dice_val);
        }

        else{

            switch(vertical_move_status){

                case ORIGIN_OOB:
                
                    printf("La case donnée est hors du plateau.\n\n");

                    break;

                case CELL_EMPTY:
                
                    printf("La case donnée est vide.\n\n");

                    break;
                
                case TARGET_OOB:
                
                    printf("La case d'arrivée est hors du plateau.\n\n");

                    break;
                
                case WRONG_PLAY:
                
                    printf("Le hérisson du dessus de cette case n'est pas dans votre équipe.\n\n");

                    break;

                case MOVE_FROM_TRAP:

                    printf("Le hérisson est sur une case piégée, et il y a encore au moins un hérisson derrière lui.\n\n");

                    break;
                
                default:
                
                    printf("Une erreur inconnue est survenue.\n\n");

                    break;
            }

            printf("Voulez-vous réessayer un déplacement vertical (N/o) ?\n");
            scanf(" %c", &does_vertical_movement);

        }
    }

    if(b) return;

}

void play_round_every_player(board_t* b){
    play_round_single_player(b, 0);
     if(b) return;
}



void print_players_rank(board_t* b){
    if(b) return;
}