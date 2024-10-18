#pragma once

#include "build_options.h"
#include "types.h"
#include "logging.h"
#include "plateau.h"
#include <stdio.h>
#include <stdlib.h>


/*

    Auteur : William
    Contenu : Les fonctions définissant le comportement de la partie

    Dépendances : 
        - build_options.h   Permet de déterminer si le build actuel est de debug ou bien de production
        - types.h           Donne des définitions pour les types de base, permettant un contrôle accru
        - logging.h         Fonctions générales de log, pour build debug ou de production
        - plateau.h         Permet de manipuler le plateau via les fonctions et méthodes fournies (AUCUNE MANIPULATION DIRECTE N'EST AUTORISÉE)
        - stdio.h           Pour interfaçer avec les fonctions de print usuelles        
        - stdlib.h          Pour l'allocation des tableaux et cases considérées
*/



u8 dice_roll(u8 max_value);

void init_board_default(board_t* b);

void play_round_single_player(board_t* b, u8 player);
void play_round_every_player(board_t* b);



void print_players_rank(board_t* b);






