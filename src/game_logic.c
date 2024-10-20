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


u8 to_upper(u8 character){
    if(character <= 'Z') return character;
    else return character - ('a' - 'A');
}


b8 is_an_active_trap(board_t* b, u8 line, u8 row){

    if(!board_is_cell_trap(b, line, row)) return false;

    for(int curr_row = 0; curr_row < row; curr_row++){
        if(board_height(b, line, curr_row) > 0) return true;
    }

    return false;
}

b8 exists_legal_move(board_t* b, u8 line){

    for(int curr_row = 0; curr_row < b->row_count-1; curr_row++){
        if(board_height(b, line, curr_row) > 0) return true;
    }

    return false;
}

b8 is_player_winning(board_t* b, u8 player){
    return (b->cleared_hedgehog_count[player] >= b->hedgehog_winning_count);
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

move_value_t move_vertically(board_t* b, u8 player, u8 line, u8 row, b8 is_going_up){

    i16 offset = 1;
    if(is_going_up) offset = -offset;
    i16 new_line = line + offset;

    //Si le coup demandé par les arguments n'est pas possible, il est ignoré et, dans un build de debug, on log un warning
    //Ensuite, on renvoie un code d'erreur approprié
    if(!point_is_in_board(line, row, b->line_count, b->row_count)){
        
        WARN_TERMINAL("move_vertically -> La case de départ est hors du plateau !");
        return origin_oob;
    }

    if(board_height(b, line, row) == 0){

        WARN_TERMINAL("move_vertically -> La case d'origine est vide !");
        return cell_empty;
    }

    if(!point_is_in_board(new_line, row, b->line_count, b->row_count)){
        
        WARN_TERMINAL("move_vertically -> La case d'arrivée est hors du plateau !");
        return target_oob;
    }

    if(is_an_active_trap(b, line, row)){

        WARN_TERMINAL("move_vertically -> La case de départ est un piège actif !");
        return move_from_trap;
    }

    u8 moving_hedgehog = board_pop(b, line, row);

    if(moving_hedgehog != 'A' + player){

        board_push(b, line, row, moving_hedgehog);
        WARN_TERMINAL("move_vertically -> Le hérisson n'est pas dans l'équipe du joueur !");
        return wrong_play;
    }
        
    else board_push(b, new_line, row, moving_hedgehog);
    return success;

}

move_value_t move_horizontally(board_t* b, u8 line, u8 row){

    u8 new_row = row + 1;

    if(!point_is_in_board(line, row, b->line_count, b->row_count)){
        
        WARN_TERMINAL("move_horizontally -> La case de départ est hors du plateau !");
        return origin_oob;
    }

    if(board_height(b, line, row) == 0){

        WARN_TERMINAL("move_horizontally -> La case d'origine est vide !");
        return cell_empty;
    }

    if(!point_is_in_board(line, new_row, b->line_count, b->row_count)){
        
        WARN_TERMINAL("move_horizontally -> La case d'arrivée est hors du plateau !");
        return target_oob;
    }

    if(is_an_active_trap(b, line, row)){

        WARN_TERMINAL("move_horizontally -> La case de départ est un piège actif !");
        return move_from_trap;
    }

    u8 moving_hedgehog = board_pop(b, line, row);
    
    //L'éventuel enregistrement d'un hérisson parmi ceux qui ont fini se fait dans board_push
    board_push(b, line, new_row, moving_hedgehog);

    return success;

}


void print_players_rank(board_t* b){

    // Réalisation d'une liste des rangs grâce à quicksort

    // -Wpedantic râle sur le fait que les fonctions imbriquées soient interdites selon ISO
    // Ici, besoin du board local dans l'appel de fonction, solution la plus propre à mon avis
    // TODO Trouver mieux ?
    #pragma GCC diagnostic ignored "-Wpedantic"

    int compare_players_fun(const void* first_player, const void* second_player){
        // retourne un nombre négatif si first_player a réussi à plaçer plus de hérissons
        // Nous trions par nombre décroissant de cleared_hedgehog
        return -(b->cleared_hedgehog_count[*(const int*)first_player] - b->cleared_hedgehog_count[*(const int*)second_player]);
    }

    u8 player_rank_table[b->player_count];
    for(u8 player = 0; player < b->player_count; player++) player_rank_table[player] = player;

    qsort(player_rank_table, b->player_count, sizeof(u8), compare_players_fun);

    // player_rank_table contient les indices de joueurs par nombre de cleared_hedgehog décroissant

    u8 virtual_rank = 0;
    u8 actual_rank = 0;
    u8 current_cleared_hedgehog_count = b->cleared_hedgehog_count[player_rank_table[0]];

    printf(" --- La partie est terminée !! ---\n\n");

    while(virtual_rank < b->player_count){
        
        printf(" - Place #%d : ", actual_rank + 1);
        
        while((virtual_rank < b->player_count) && (current_cleared_hedgehog_count == b->cleared_hedgehog_count[virtual_rank])){
            printf("Équipe %c, ", 'A' + player_rank_table[virtual_rank]);
            virtual_rank++;
        }

        if(current_cleared_hedgehog_count > 1) printf("(Avec %d hérissons)\n", current_cleared_hedgehog_count);
        else printf("(Avec %d hérisson)\n", current_cleared_hedgehog_count);

        if(virtual_rank < b->player_count){
            current_cleared_hedgehog_count = b->cleared_hedgehog_count[virtual_rank];
            actual_rank = virtual_rank;
        }

    }

}



void play_round_single_player(board_t* b, u8 player){
    
    u8 dice_val = dice_roll(b->line_count);

    u8 does_vertical_movement;
    u8 vertical_movement_line;
    u8 vertical_movement_row;
    b8 vertical_direction;
    b8 vertical_going_up;
    move_value_t vertical_move_status = cell_empty;

    u8 horizontal_movement_row;
    move_value_t horizontal_move_status = cell_empty;
    
    board_print(b, dice_val - 1);       // La ligne sur laquelle nous pouvons avancer est mise en valeur

    printf("* Joueur %c - Le dé donne :\n\n", 'A' + player);
    if(b->line_count <= MAX_DICE_VALUE) print_dice(dice_val);
    else printf("  --- %d --- \n\n", dice_val);

    printf("Voulez-vous bouger un hérisson verticalement (N/o) ?\n");
    scanf(" %c", &does_vertical_movement);

    while(to_upper(does_vertical_movement) == 'O' && vertical_move_status != success){

        printf("Depuis quelle ligne voulez-vous bouger ?\n");
        scanf("%hhu", &vertical_movement_line);
        vertical_movement_line = vertical_movement_line-1;

        printf("Depuis quelle colonne voulez-vous bouger ?\n");
        scanf(" %c", &vertical_movement_row);
        vertical_movement_row = to_upper(vertical_movement_row) - 'A';

        printf("Voulez-vous bouger vers le haut ou le bas (H/b) ?\n");
        scanf(" %c", &vertical_direction);

        vertical_going_up = (to_upper(vertical_direction) != 'B');

        vertical_move_status = move_vertically(b, player, vertical_movement_line, vertical_movement_row, vertical_going_up);

        if(vertical_move_status == success){

            board_print(b, dice_val-1);
            printf("Vous avez bougé un hérisson verticalement.\n");
            printf("Joueur %c - Le dé donne :\n\n", 'A' + player);
            if(b->line_count <= MAX_DICE_VALUE) print_dice(dice_val);
        }

        else{

            switch(vertical_move_status){

                case origin_oob:
                
                    printf("La case donnée est hors du plateau.\n\n");

                    break;

                case cell_empty:
                
                    printf("La case donnée est vide.\n\n");

                    break;
                
                case target_oob:
                
                    printf("La case d'arrivée est hors du plateau.\n\n");

                    break;
                
                case wrong_play:
                
                    printf("Le hérisson du dessus de cette case n'est pas dans votre équipe.\n\n");

                    break;

                case move_from_trap:

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

    printf("\n-----Déplacement horizontal-----\n\n");

    if(!exists_legal_move(b, dice_val-1)){

        printf("Aucun coup légal dans cette position.\n");
        printf("Le tour du joueur %c est passé.\n\n", 'A'+player);
        return;
    }

    while(!horizontal_move_status == success){

        printf("Depuis quelle colonne voulez-vous bouger ?\n");
        scanf(" %c", &horizontal_movement_row);
        horizontal_movement_row = to_upper(horizontal_movement_row) - 'A';

        horizontal_move_status = move_horizontally(b, dice_val - 1, horizontal_movement_row);

        switch(horizontal_move_status){

            case success:
            
                printf("Vous aves bougé un hérisson horizontalement.\n\n");

                break;

            case origin_oob:
                
                printf("La case donnée est hors du plateau.\n\n");

                break;

            case cell_empty:
                
                printf("La case donnée est vide.\n\n");

                break;
                
            case target_oob:
                
                printf("La case d'arrivée est hors du plateau.\n\n");

                break;

            case move_from_trap:

                printf("Le hérisson est sur une case piégée, et il y a encore au moins un hérisson derrière lui.\n\n");

                break;
                
            default:
                
                printf("Une erreur inconnue est survenue.\n\n");

                break;
        }
    }
}

void default_game_loop(board_t* b){

    b8 has_a_player_won = false;

    while(!has_a_player_won){
        for(u8 current_player = 0; current_player < b->player_count; current_player++){
            play_round_single_player(b, current_player);
            has_a_player_won |= is_player_winning(b, current_player);
        }
    }

    // Au moins un joueur à gagné, affichage du classement
    print_players_rank(b);

}

