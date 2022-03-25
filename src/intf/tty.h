#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>

enum {
    PRINT_COLOR_BLACK = 0,
	PRINT_COLOR_BLUE = 1,
	PRINT_COLOR_GREEN = 2,
	PRINT_COLOR_CYAN = 3,
	PRINT_COLOR_RED = 4,
	PRINT_COLOR_MAGENTA = 5,
	PRINT_COLOR_BROWN = 6,
	PRINT_COLOR_LIGHT_GRAY = 7,
	PRINT_COLOR_DARK_GRAY = 8,
	PRINT_COLOR_LIGHT_BLUE = 9,
	PRINT_COLOR_LIGHT_GREEN = 10,
	PRINT_COLOR_LIGHT_CYAN = 11,
	PRINT_COLOR_LIGHT_RED = 12,
	PRINT_COLOR_PINK = 13,
	PRINT_COLOR_YELLOW = 14,
	PRINT_COLOR_WHITE = 15,
};

extern size_t col;
extern size_t row;

//first 4 bits are forground, second 4 bits are background
extern uint8_t color;

void tty_initialize();

void tty_move_row_col(size_t row, size_t col);

void tty_print_clear();
void tty_clear_row(size_t row);
void tty_print_char(char c);
void tty_print_int_bin(int i);
void tty_print_int_dec(int i);
void tty_print_int_hex(int i);
void tty_print_str(char* str);
void tty_print_str_endl(char* str);
void tty_print_set_color(uint8_t foreground, uint8_t background);
void tty_print_new_line();

#endif
