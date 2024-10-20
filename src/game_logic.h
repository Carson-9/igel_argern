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
        - stdlib.h          Pour l'allocation des tableaux et cases considérées
*/

#define MAX_DICE_VALUE 6

#define SUCCESS 0
#define ORIGIN_OOB 1
#define CELL_EMPTY 2
#define TARGET_OOB 3
#define WRONG_PLAY 4
#define MOVE_FROM_TRAP 5

u8 dice_roll(u8 max_value);
u8 random_line(board_t* b);
u8 bullet_or_empty_char(b8 cond);
void print_dice(u8 dice_val);

u8 to_upper(u8 c);

b8 is_an_active_trap(board_t* b, u8 line, u8 row);
b8 exists_legal_move(board_t* b, u8 line);

u8 winner(board_t* b);

void init_board_default(board_t* b);

u8 move_vertically(board_t* b, u8 player, u8 line, u8 row, b8 is_going_up);

u8 move_horizontally(board_t* b, u8 line, u8 row);

void play_round_single_player(board_t* b, u8 player);
void play_round_every_player(board_t* b);



void print_players_rank(board_t* b);






