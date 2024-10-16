#include "build_options.h"
#include "plateau.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*

    Auteur :  William
    Contenu : Le point d'entrée (fonction main) du programme, Parsing des arguments et allocation mémoire du plateau

    Dépendances : 
        - build_options.h   Permet de déterminer si le build actuel est de debug ou bien de production
        - plateau.h         Afin d'initialiser le plateau de jeu (ce fichier en est le responsable)
        - logging.h         Fonctions générales de log, pour build debug ou de production
        - stdio.h           Pour interfaçer avec les fonctions de print et de lecture usuelles        
        - stdlib.h          Pour l'allocation des tableaux et cases considérées
        - string.h          Permet le parsing des arguments via strcmp
        - time.h            Permet l'initialisation du générateur pseudo-aléatoire
*/



int main(int argc, char** argv){

    u8 line_count = DEFAULT_LINE_COUNT;
    u8 row_count = DEFAULT_ROW_COUNT;
    u8 player_count = DEFAULT_PLAYER_COUNT;
    u8 hedgehog_count = DEFAULT_HEDGEHOG_COUNT;


    // Parsing des paramètres d'entrée 

    for(int cur_argument = 1; cur_argument < argc; cur_argument++){

        // Des librairies C existent pour facilement parser les arguments du terminal, nous nous contenterons ici de cette magnifique tour de if-else

        if(strcmp(argv[cur_argument], "-h") == 0 || strcmp(argv[cur_argument], "-help") == 0){
            
            // TODO : Compléter le menu d'aide

            printf(" Igel Ärgern - William et Simon - Aide :\n\
                \n\
    * PARAMÈTRES :                                                      \n\
        -h, -help       : Affiche le menu d'aide ci-présent             \n\
        -lignes y       : Permet d'imposer un tableau à y lignes        \n\
        -colonnes y     : Permet d'imposer un tableau à x colonnes      \n\
        -joueurs z      : Impose un nombre z de joueurs                 \n\
        -herissons h    : Chaque joueur commencera avec h hérissons     \n\
                \n\
    * COMMENT JOUER :                                                   \n\
                                                                                    \n\
            ");

            return EXIT_SUCCESS;
        }

        // Les paramètres suivants permettent de préciser des constantes numériques, si un string invalide est passé, la fonction atoi() donne un U.B


        else if(strcmp(argv[cur_argument], "-lignes") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -lignes prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            // Avant qu'une guerre civile n'explose à cause de l'utilisation de ++cur_argument,
            // Cet usage permet de passer un tour de boucle et d'éviter des cas désagréables lorsqu'un argument n'est pas connu
    
            else line_count = atoi(argv[++cur_argument]);  
        }


        else if(strcmp(argv[cur_argument], "-colonnes") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -colonnes prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else row_count = atoi(argv[++cur_argument]);
        }

        else if(strcmp(argv[cur_argument], "-joueurs") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -joueurs prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else player_count = atoi(argv[++cur_argument]);
            
        }

        else if(strcmp(argv[cur_argument], "-herissons") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -herissons prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else hedgehog_count = atoi(argv[++cur_argument]);
        }

        else{       // Le paramètre est inconnu
            WARN_TERMINAL("Un paramètre est inconnu et de ce fait ignoré!");
        }

    }

    board_t* new_board = board_alloc(line_count, row_count, player_count, hedgehog_count);
    board_print(new_board, -1); //TEST, doit passer la main à game_logic.c


    board_free(new_board);
    return EXIT_SUCCESS;
}