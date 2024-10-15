#pragma once

#include "build_options.h"
#include <stdio.h>

/*

    Auteur : William
    Contenu : Des fonctions générales de log, permettant un formatage constant et éliminant des logs informatifs
    dans un build de production

    Dépendances : 
        - build_options.h
        - stdio.h
*/


typedef enum log_level_e{
    trace_level,
    info_level,
    warning_level,
    error_level
} log_level_t;


void message_terminal(log_level_t type, char* message);
void message_error_channel(log_level_t type, char* message);
void write_file(log_level_t, char* file, char* message);

void trace_terminal(char* message);
void info_terminal(char* message);
void warn_terminal(char* message);
void error_terminal(char* message);


#define INFO_TERMINAL(msg) info_terminal(msg)
#define ERROR_TERMINAL(msg) error_terminal(msg)

#ifdef DEBUG_BUILD
    #define TRACE_TERMINAL(msg) trace_terminal(msg)
    #define WARN_TERMINAL(msg) warn_terminal(msg)
#endif
#ifdef PROD_BUILD
    #define TRACE_TERMINAL(msg)
    #define WARN_TERMINAL(msg)
#endif
