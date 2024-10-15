#pragma once


typedef enum error_level_e{
    log_level,
    info_level,
    warning_level,
    error_level
} error_level_t;


void message_terminal();
void message_error;
void write_file();