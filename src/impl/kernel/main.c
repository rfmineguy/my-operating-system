#include "../../intf/tty.h"
#include "../../intf/input.h"
#include "../../intf/math.h"
//specified 'kernel_main' as entry point in main64.asm

char* WELCOME_MESSAGE[8] = {
" *******   ********         *******    ********",
"/**////** /**/////         **/////**  **////// ",
"/**   /** /**             **     //**/** ",      
"/*******  /*******       /**      /**/*********",
"/**///**  /**////        /**      /**////////**",
"/**  //** /**            //**     **        /**",
"/**   //**/**             //*******   ********  ",
"//     // //               ///////   ////////  "
};

//clear before calling this function
void print_welcome() {
    for (size_t i = 0; i < 8; i++) {
        tty_print_str(WELCOME_MESSAGE[i]);
        tty_print_new_line();
    }
    tty_print_new_line();

    tty_print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    tty_print_str_endl("Welcome to RF OS. A kernel being developed by Riley Fischer");
    tty_print_str_endl("It currently does absolutely nothing.");
}

void kernel_main() {
    tty_initialize();
    tty_print_clear();
    tty_print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_welcome();

    tty_print_new_line();
    tty_print_new_line();

    tty_print_int_dec(312973131);
    tty_print_new_line();
    tty_print_int_hex(9742123);
    tty_print_new_line();
    tty_print_int_hex(7542);
    tty_print_new_line();
    tty_print_int_hex(9832);
    tty_print_new_line();
    tty_print_int_hex(983298);
    tty_print_new_line();
    tty_print_int_hex(983212);
    tty_print_new_line();
    tty_print_str("Hello World");

    prompt_ch();
}
