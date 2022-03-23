#include "../../intf/tty.h"
#include "../../intf/input.h"

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

char* test = " *******   ********         *******    ********";

//clear before calling this function
void print_welcome() {
    for (size_t i = 0; i < 8; i++) {
        tty_print_str(WELCOME_MESSAGE[i]);
        tty_print_new_line();
    }
    tty_print_new_line();
    tty_print_str_endl("Welcome to RF OS. A kernel being developed by Riley Fischer");
}

void kernel_main() {
    tty_initialize();
    tty_print_clear();
    tty_print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_welcome();
}
