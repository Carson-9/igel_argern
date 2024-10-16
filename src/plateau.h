#pragma once

#include "build_options.h"
#include "types.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>

/*

    Auteur : Simon et William
    Contenu : La définition et création du plateau et des cases le composant. Opérations de base sur les stacks et affichage du tableau

    Dépendances : 
        - build_options.h   Permet de déterminer si le build actuel est de debug ou bien de production
        - types.h           Donne des définitions pour les types de base, permettant un contrôle accru
        - logging.h         Fonctions générales de log, pour build debug ou de production
        - stdio.h           Pour interfaçer avec les fonctions de print usuelles        
        - stdlib.h          Pour l'allocation des tableaux et cases considérées
*/



#define DEFAULT_LINE_COUNT 6
#define DEFAULT_ROW_COUNT 9
#define DEFAULT_PLAYER_COUNT 4
#define DEFAULT_HEDGEHOG_COUNT 4


#define PRINT_FORMAT_LINE_INFO_SIZE 6
#define PRINT_FORMAT_LINE_CELL_SIZE 5
#define PRINT_FORMAT_LINE_SPACING 1

typedef struct cell_s{

    // Le stack sera pseudo-statique : L'utilisation d'un malloc est obligatoire, mais nous nous permetterons d'allouer un tableau de taille maximale dès le début

    u8* stack;      // Stocke les "hérissons" comme des caractères MAJUSCULE, toute apparition de minuscule doit résulter d'un offset imputé au caller.
    u32 s_top;      // Le haut de la pile se lit a stack[s_top]
    b8 is_trap;

} cell_t;

typedef struct board_s{

    u8 row_count;
    u8 line_count;
    cell_t* cells;    // La manipulation d'un unique indice s'avère beaucoup plus simple finalement ...
} board_t;


u16 point_to_index_conversion(u8 line, u8 row, u8 line_count, u8 row_count);

board_t* board_alloc(u8 line_count, u8 row_count, u8 player_count, u8 hedgehog_count);
void board_free(board_t* board);

void board_push(board_t* b, u8 line, u8 row, u8 ctn);

u8 board_pop(board_t* b, u8 line, u8 row);

i32 board_height(board_t* b, u8 line, u8 row);

u8 board_top(board_t* b, u8 line, u8 row);

u8 board_peek(board_t* b, u8 line, u8 row, u8 pos); // pos=0 => top

void cell_print(board_t* b, u8 line, u8 row, u8 slice);

void board_print(board_t* b, u8 highlighted_line); // hl_line=-1 => rien


void top_print(board_t* b);                                 // Affiche la bordurde haute
void line_print(board_t* b, u8 line, b8 is_highlighted);    // Controle l'affichage ligne par ligne (dont la bordure)
void bottom_print(board_t* b);                              // Affiche la bordure basse