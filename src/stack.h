#pragma once

/*
    STACK.H

    Définition d'un type stack (template) et de sa structure sous-jacente de liste simplement chaînée.
    Toutes nos définitions reposent sur des instructions préprocesseur #define, ce qui n'est qu'une manière d'éviter le copié-collé intensif

    Aucune dépendance

*/

// 

#define MAKE_LINKED_LIST(type) struct linked_list_s{    \
    type val;                                           \
    struct linked_list_s* next;                         \
}                                                       \
                                                        \
typedef struct linked_list_s type_linked_list_t;        \

/*                                                        

type_linked_list* type_list_create(void){                  
    
}

*/


