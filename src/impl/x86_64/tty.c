#include "../../intf/tty.h"
#include "../../intf/math.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
struct Char empty;

size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;     //defined in print.h

void tty_initialize() {
    empty = (struct Char) { ' ', color };
}

void tty_move_row_col(size_t _row, size_t _col) {
    row = _row % NUM_ROWS;
    col = _col % NUM_COLS;
}

void tty_clear_row(size_t row) {
    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

//Resets the column to 0 and avances the row
void tty_print_new_line() {
    col = 0;
    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char ch = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row-1)] = ch;
        }
    }

    tty_clear_row(row);
}

//Clears all the rows of the buffer (sets to empty chars)
void tty_print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        tty_clear_row(i);
    }
}

//Prints a char to the buffer screen buffer
//  advances the column when col
void tty_print_char(char c) {
    if (c == '\n') {
        tty_print_new_line();
        return;
    }
    if (col > NUM_COLS) {
        tty_print_new_line();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        (uint8_t) c,
        color
    };
    col++;
}

//break down the integer into single characters
//  and print those individually
//  no end line
void tty_print_int_dec(int dec) {
    int numDigits = count_digits(dec) - 1;
    for (int i = numDigits; i >=0; i--) {
        int digit = get_digit(dec, i);
        tty_print_char((char)(digit+48));
    }
}

void tty_print_int_hex(int i) {
    int counter = 0;
    static char hex[100] = {' '};       //'global buffer' for displaying the string
    while (1) {
        int remainder = i % 16;
        i /= 16;
        char hexChar;
        if (remainder < 10) {
            hex[counter] = (char)(remainder + 48);
        }
        else {
            //a-f (10-15)
            hex[counter] = (char)(remainder + 87);
        }
        counter++;
        if (i == 0) {
            break;
        }
    }
    for (size_t i = counter - 1; i > 0; i--) {
        tty_print_char(hex[i]);
    }
    tty_print_char(hex[0]);
}

//Prints a null terminated char*
void tty_print_str(char* str) {
    //keep printing the characters until you reach the null terminator
    for (size_t i = 0; 1; i++) {
        char ch = (uint8_t) str[i];
        if (ch == '\0') {
            return;
        }
        tty_print_char(ch);
    }
}

//Prints a null terminated char* and then 'prints a new line' 
void tty_print_str_endl(char* str) {
    tty_print_str(str);
    tty_print_new_line();
}

//Changes the color of the forground and background in the tty
void tty_print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}
