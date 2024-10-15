#include "logging.h"

void message_terminal(log_level_t log_type, char* message){

    switch(log_type){
        case trace_level:
            printf("\033[1;37m[TRACE] ");
            break;

        case info_level:
            printf("[INFO] ");
            break;

        case warning_level:
            printf("\033[1;93m[WARN] ");
            break;

        case error_level:
            printf("\033[1;91m[ERROR] ");
            break;

        default:
            // Comment est-ce arrivé ?
            fprintf(stderr, "message_terminal -> Le type de message rencontré est inconnu!\n");
    }

    printf("%s\033[0m\n", message);
}


void message_error_channel(log_level_t log_type, char* message){

    switch(log_type){
        case trace_level:
            fprintf(stderr, "\033[1;37m[TRACE] %s", message);
            break;

        case info_level:
            fprintf(stderr, "[INFO] %s", message);
            break;

        case warning_level:
            fprintf(stderr, "\033[1;93m[WARN] %s", message);
            break;

        case error_level:
            fprintf(stderr, "\033[1;91m[ERROR] %s", message);
            break;

        default:
            // Comment est-ce arrivé ?
            fprintf(stderr, "message_error_channel -> Le type de message rencontré est inconnu!\n");
    }
}

void write_file(log_level_t log_type, char* file, char* message){

    FILE* file_handle = fopen(file, "ab+");
    
    if(!file_handle){ // Fichier non trouvé : on informe le channel error
        message_error_channel(error_level, "write_file -> Le fichier donné n'a pas pu être trouvé\n"); // TODO : revoir une version safe avec nom de fichier grâce à un sprintf like
        return;
    }

    switch(log_type){
        case trace_level:
            fprintf(file_handle, "[TRACE] %s", message);
            break;

        case info_level:
            fprintf(file_handle, "[INFO] %s", message);
            break;

        case warning_level:
            fprintf(file_handle, "[WARN] %s", message);
            break;

        case error_level:
            fprintf(file_handle, "[ERROR] %s", message);
            break;

        default:
            // Comment est-ce arrivé ?
            fprintf(stderr, "write_file -> Le type de message rencontré est inconnu!\n");
    }

}

// Un raccourci pour les macro à enlever lors d'un build propre
void trace_terminal(char* message){
    message_terminal(trace_level, message);
}

void info_terminal(char* message){
    message_terminal(info_level, message);
}

void warn_terminal(char* message){
    message_terminal(warning_level, message);
}

void error_terminal(char* message){
    message_terminal(error_level, message);
}