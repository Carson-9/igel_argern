/*  
  _____           _    _   _                            
 |_   _|         | |  (_)_(_)                           
   | |  __ _  ___| |    / \   _ __ __ _  ___ _ __ _ __  
   | | / _` |/ _ \ |   / _ \ | '__/ _` |/ _ \ '__| '_ \ 
  _| || (_| |  __/ |  / ___ \| | | (_| |  __/ |  | | | |
 |_____\__, |\___|_| /_/   \_\_|  \__, |\___|_|  |_| |_|
        __/ |                      __/ |                
       |___/                      |___/                 

    (Généré sur https://patorjk.com/software/taag/)

*/




#include "build_options.h"
#include "plateau.h"
#include "game_logic.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*

    Auteur :  William et Simon
    Contenu : Le point d'entrée (fonction main) du programme, Parsing des arguments et initialisation mémoire du plateau

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


u64 match_extension_name(char* extension_name){
    if(strcmp(extension_name, "tube") == 0) return 1;           // 0b1;
    if(strcmp(extension_name, "anarchie") == 0) return 2;       // 0b10;
    printf("L'extension : \"%s\" est inconnue!\n", extension_name);
    return 0;       // 0b0;
}


int main(int argc, char** argv){

    // Définit les constantes de la partie aux valeurs de base, potentiellement réécrites par des paramètres par la suite

    u8 line_count = DEFAULT_LINE_COUNT;
    u8 row_count = DEFAULT_ROW_COUNT;
    u8 player_count = DEFAULT_PLAYER_COUNT;
    u8 hedgehog_count = DEFAULT_HEDGEHOG_COUNT;
    u8 clear_for_win = DEFAULT_HEDGEHOG_WINNING_COUNT;
    u64 extensions_flag = 0;            // 0b0;

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
        -lignes y       : Permet d'imposer un tableau à y lignes (y <= %d) (par défaut : %d)                \n\
        -colonnes x     : Permet d'imposer un tableau à x colonnes (x <= %d) (par défaut : %d)              \n\
        -joueurs z      : Impose un nombre z de joueurs (z <= %d) (par défaut : %d)                         \n\
        -herissons h    : Chaque joueur commencera avec h hérissons (h <= %d) (par défaut : %d)             \n\
        -objectif o     : Un joueur doit amener v hérissons à la fin pour gagner (o <= h) (par défaut : %d) \n\
        -variante v     : Permet d'ajouter une variante au jeu (v = tube | anarchie | )                     \n\
                        (A noter que les extensions sont additives!)                                        \n\
                \n\
        Si l'un des paramètres est plus grand que le maximum (ou entrée invalide), il est ramené modulo le maximum.\n\
                \n\
    * COMMENT JOUER :                                                                       \n\
        Lors du tour d'un joueur, ce dernier lancera (automatiquement) un dé                \n\
        désignant la ligne où il est possible d'avancer un hérisson. Avant cela,            \n\
        un joueur peut choisir de déplacer latéralement d'une case l'un de ses hérissons    \n\
        afin de compliquer la tâche d'autres joueurs, ou bien de faciliter son déplacement. \n\
                                                                        \n", 
            MAX_LINE_COUNT, DEFAULT_LINE_COUNT, MAX_ROW_COUNT, DEFAULT_ROW_COUNT, MAX_PLAYER_COUNT,
             DEFAULT_PLAYER_COUNT, MAX_HEDGEHOG_COUNT, DEFAULT_HEDGEHOG_COUNT, DEFAULT_HEDGEHOG_WINNING_COUNT);

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

        else if(strcmp(argv[cur_argument], "-objectif") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -objectif prend un paramètre numérique!\n");
                return EXIT_FAILURE;
            }

            else{
                clear_for_win = atoi(argv[++cur_argument]);
            }
        }

        else if(strcmp(argv[cur_argument], "-variante") == 0){
            if(cur_argument == argc - 1){
                printf("NOMBRE DE PARAMÈTRES INSUFFISANT : l'option -variante prend un paramètre string!\n");
                return EXIT_FAILURE;
            }

            // Le cas d'une extension inconnue est implicite!
            else extensions_flag |= match_extension_name(argv[++cur_argument]);
            
        }

        else{       // Le paramètre est inconnu
            WARN_TERMINAL("Un paramètre est inconnu et de ce fait ignoré!");
        }

    }


    if(clear_for_win > hedgehog_count){
        WARN_TERMINAL("OBJECTIF TROP ÉLEVÉ");
        clear_for_win = clear_for_win % hedgehog_count;
    }

    board_t* new_board = board_alloc(line_count, row_count, player_count, hedgehog_count, clear_for_win, extensions_flag);
    init_board_default(new_board);

    default_game_loop(new_board);

    board_free(new_board);
    return EXIT_SUCCESS;
}