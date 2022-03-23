#include "../../intf/input.h"
#include "../../intf/tty.h"

char getch() {
    return ' ';
}

char* input_prompt(char* prompt) {
    tty_print_str(prompt);

    return "";
}
