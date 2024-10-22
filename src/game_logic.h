#pragma once

#include "build_options.h"
#include "types.h"
#include "logging.h"
#include "plateau.h"
#include <stdio.h>
#include <stdlib.h>


/*

    Auteur : William et Simon
    Contenu : Les fonctions définissant le comportement de la partie

    Dépendances : 
        - build_options.h   Permet de déterminer si le build actuel est de debug ou bien de production
        - types.h           Donne des définitions pour les types de base, permettant un contrôle accru
        - logging.h         Fonctions générales de log, pour build debug ou de production
        - plateau.h         Permet de manipuler le plateau via les fonctions et méthodes fournies (AUCUNE MANIPULATION DIRECTE N'EST AUTORISÉE)
        - stdio.h           Pour interfaçer avec les fonctions de print usuelles        
        - stdlib.h          Utilisation de qsort
*/

#define MAX_DICE_VALUE 6

typedef enum{
    success,
    origin_oob,
    cell_empty,
    target_oob,
    wrong_play,
    move_from_trap
} move_value_t;


u8 dice_roll(u8 max_value);
u8 random_line(board_t* b);
u8 bullet_or_empty_char(b8 cond);
void print_dice(u8 dice_val);

u8 to_upper(u8 character);

b8 is_an_active_trap(board_t* b, u8 line, u8 row);
b8 exists_legal_move(board_t* b, u8 line);

b8 is_player_winning(board_t* b, u8 player);

void init_board_default(board_t* b);

move_value_t move_vertically(board_t* b, u8 player, u8 line, u8 row, b8 is_going_up);
move_value_t move_horizontally(board_t* b, u8 line, u8 row);

void print_vertical_error(move_value_t move_type);
void print_horizontal_move_status(move_value_t move_type);

void print_players_rank(board_t* b);

void play_round_single_player(board_t* b, u8 player);
void default_game_loop(board_t* b);







