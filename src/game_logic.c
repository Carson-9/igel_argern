#include "game_logic.h"


u8 dice_roll(u8 max_value){
    return rand() % (max_value + 1);
}

u8 random_line(board_t* b){
    return rand() % b->line_count;
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
    if(b && player) return; 
}

void play_round_every_player(board_t* b){
    if(b) return;
}



void print_players_rank(board_t* b){
    if(b) return;
}