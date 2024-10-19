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

void play_round_single_player(board_t* b, u8 player){
    
    u8 dice_val = dice_roll(MAX_DICE_VALUE);
    
    
    printf("* Joueur %c - Le dé donne :\n\n", 'A' + player);
    print_dice(dice_val);

    if(b) return;

}

void play_round_every_player(board_t* b){
    play_round_single_player(b, 0);
     if(b) return;
}



void print_players_rank(board_t* b){
    if(b) return;
}