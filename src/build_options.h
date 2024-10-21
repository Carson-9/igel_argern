#pragma once

/*

    Auteur : William
    Contenu : Des gardes permettant de facilement passer d'un build DEBUG à un build PRODUCTION (enrichissement futur ?)

    Dépendances : 
        Aucune

*/


//#define DEBUG_BUILD

#ifndef DEBUG_BUILD
    #define PROD_BUILD
#endif