#include "build_options.h"
#include "plateau.h"
#include "game_logic.h"
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
        - game_logic.h      Permet d'appeler la boucle principale du jeu
        - logging.h         Fonctions générales de log, pour build debug ou de production
        - stdio.h           Pour interfaçer avec les fonctions de print et de lecture usuelles        
        - stdlib.h          Pour l'allocation des tableaux et cases considérées
        - string.h          Permet le parsing des arguments via strcmp
        - time.h            Permet l'initialisation du générateur pseudo-aléatoire
*/



int main(int argc, char** argv){

    // Définit les constantes de la partie aux valeurs de base, potentiellement réécrites par des paramètres par la suite

    u8 line_count = DEFAULT_LINE_COUNT;
    u8 row_count = DEFAULT_ROW_COUNT;
    u8 player_count = DEFAULT_PLAYER_COUNT;
    u8 hedgehog_count = DEFAULT_HEDGEHOG_COUNT;

    // Initialise l'aléa

    srand(time(NULL));

    // Parsing des paramètres d'entrée 

    for(int cur_argument = 1; cur_argument < argc; cur_argument++){

        // Des librairies C existent pour facilement parser les arguments du terminal, nous nous contenterons ici de cette magnifique tour de if-else

        if(strcmp(argv[cur_argument], "-h") == 0 || strcmp(argv[cur_argument], "-help") == 0){
            
            // TODO : Compléter le menu d'aide

            printf(" Igel Ärgern - William et Simon - Aide :\n\
                \n\
    * PARAMÈTRES :                                                      \n\
        -h, -help       : Affiche le menu d'aide ci-présent             \n\
        -lignes y       : Permet d'imposer un tableau à y lignes (y <= %d)       \n\
        -colonnes y     : Permet d'imposer un tableau à x colonnes (x <= %d)     \n\
        -joueurs z      : Impose un nombre z de joueurs (z <= %d)                \n\
        -herissons h    : Chaque joueur commencera avec h hérissons (h <= %d)     \n\
                \n\
    * COMMENT JOUER :                                                   \n\
                                                                                    \n\
            ", MAX_LINE_COUNT, MAX_ROW_COUNT, MAX_PLAYER_COUNT, MAX_HEDGEHOG_COUNT);

            return EXIT_SUCCESS;
        }

        // Les paramètres suivants permettent de préciser des constantes numériques, si un string invalide est passé, la fonction atoi() donne un U.B


        else if(strcmp(argv[cur_argument], "-lignes") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -lignes prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            // Avant qu'une guerre civile n'explose à cause de l'utilisation de ++cur_argument,
            // Cet usage permet de passer un tour de boucle et d'éviter des cas désagréables lorsqu'un argument n'est pas connu.
            // Ceci aura pour effet de ne pas considérer une valeur passée à un paramètre (quitte à ce que ce dernier soit un string
            // et que la fonction atoi renvoie n'importe quoi)
    
            else{

                line_count = atoi(argv[++cur_argument]);
                if(line_count >= MAX_LINE_COUNT) WARN_TERMINAL("NOMBRE DE LIGNES TROP ÉLEVÉ!");
                line_count = line_count % MAX_LINE_COUNT;
            }
        }


        else if(strcmp(argv[cur_argument], "-colonnes") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -colonnes prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else{
                row_count = atoi(argv[++cur_argument]);
                if(row_count >= MAX_ROW_COUNT) WARN_TERMINAL("NOMBRE DE COLONNES TROP ÉLEVÉ!");
                row_count = row_count % MAX_ROW_COUNT;
            }
        }

        else if(strcmp(argv[cur_argument], "-joueurs") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -joueurs prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else{
                player_count = atoi(argv[++cur_argument]);
                if(player_count >= MAX_PLAYER_COUNT) WARN_TERMINAL("NOMBRE DE JOUEURS TROP ÉLEVÉ");
                player_count = player_count % MAX_PLAYER_COUNT;
            } 
        }

        else if(strcmp(argv[cur_argument], "-herissons") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -herissons prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else{
                hedgehog_count = atoi(argv[++cur_argument]);
                if(hedgehog_count >= MAX_HEDGEHOG_COUNT) WARN_TERMINAL("NOMBRE DE HÉRISSONS TROP ÉLEVÉ");
                hedgehog_count = hedgehog_count % MAX_HEDGEHOG_COUNT;
            }
        }

        else{       // Le paramètre est inconnu
            WARN_TERMINAL("Un paramètre est inconnu et de ce fait ignoré!");
        }

    }

    board_t* new_board = board_alloc(line_count, row_count, player_count, hedgehog_count);
    init_board_default(new_board);

    board_print(new_board, 3); //TEST, doit passer la main à game_logic.c

    play_round_every_player(new_board);

    board_free(new_board);
    return EXIT_SUCCESS;
}