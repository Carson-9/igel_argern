#include "plateau.h"


void board_push(board_t* b, u8 line, u8 row, u8 ctn){

    cell_t* c = &b->cells[line][row];

    c->s_top++;
    c->stack[c->s_top] = ctn;

}

u8 board_pop(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[line][row];
    u8 top = c->stack[c->s_top];

    c->s_top--;

    return top;

}

i32 board_height(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[line][row];

    return c->s_top + 1;
}

u8 board_top(board_t* b, u8 line, u8 row){

    cell_t* c = &b->cells[line][row];
    u8 top = c->stack[c->s_top];

    return top;

}

u8 board_peek(board_t* b, u8 line, u8 row, u8 pos){

    cell_t* c = &b->cells[line][row];
    u8 peeked = c->stack[c->s_top - pos];

    return peeked;

}

void cell_print(board_t* b, u8 line, u8 row, u8 slice){

    i8 case_diff = 'A' - 'a';
    cell_t* c = &b->cells[line][row];
    i32 height = board_height(b, line, row);
    u8 top = board_top(b, line, row);

    if(slice==3 && height > 1){
        if(c->is_trap){
            printf(" ^%d^ ");
        }
        else{
            printf(" -%d- ", height);
        }
    }
    else if(slice == 3 || slice == 0){
        if(c->is_trap){
            if(slice == 3){
                printf(" ^^^ ");
            }
            else{
                printf (" VVV ");
            }
        }
        else{
            printf(" --- ");
        }
    }
    else if(slice == 1){
        if(c->is_trap){
            printf(">%c%c%c<", top, top, top);
        }
        else{
            printf("|%c%c%c|", top, top, top);
        }
    }
    else if(slice == 2){
        u8 c1, c2, c3;

        if(height == 1){
            c1 = c2 = c3 = top;
        }
        else if(height == 2){
            c1 = c2 = c3 = board_peek(b, line, row, 1) - case_diff;
        }
        else{
            c1 = board_peek(b, line, row, 1) - case_diff;
            if(height == 3){
                c2 = ' ';
                c3 = board_peek(b, line, row, 2) - case_diff;
            }
            else{
                c2 = board_peek(b, line, row, 2) - case_diff;
                c3 = board_peek(b, line, row, 3) - case_diff;
            }
        }

        if(c->is_trap){
            printf(">%c%c%c<", c1, c2, c3);
        }
        else{
            printf("|%c%c%c|", c1, c2, c3);
        }
    }
}

void top_print(board_t* b){
    printf("      START");
    for(int i = 0; i < b->row_count-2; i++){
        printf("    ");
    }
    printf("FINISH\n\n");

    printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("  row ");
    }
    printf("\n");

    printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("   %c  ", 'a'+i);
    }
    printf("\n");
}

void line_print(board_t* b, u8 line, b8 is_highlighted){
    char last = ' ';
    if(is_highlighted){
        last = '>';
    }
    
    printf("    %c", last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 0);
    }

    printf("\nline%c", last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 1);
    }

    printf("\n  %d %c", line, last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 2);
    }

    printf("\n    %c", last);
    for(int i = 0; i < b->row_count; i++){
         cell_print(b, line, i, 3);
    }

    printf("\n\n");
}

void bottom_print(board_t* b){
     printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("  row ");
    }
    printf("\n");

    printf("     ");
    for(int i = 0; i < b->row_count; i++){
        printf("   %c  ", 'a'+i);
    }
    printf("\n");

}

void board_print(board_t* b, u8 highlighted_line){
    top_print(b);
    for(u8 i = 0; i < b->row_count; i++){
        line_print(b, i, i==highlighted_line);
    }
    bottom_print(b);
}

