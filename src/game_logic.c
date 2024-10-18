#include "game_logic.h"


u8 dice_roll(u8 max_value){
    return rand() % (max_value + 1);
}