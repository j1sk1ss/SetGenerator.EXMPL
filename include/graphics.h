#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef struct {
    int x;
    int y;
} point_t;

void fprintfl(FILE* stream, const char* format, ...);
int print_center(const char* text, int y);
char wait_any_key();
char wait_key(char target);

int gotoxy(int x, int y);
int get_console_size(point_t* sizes);
int clear_screen();

#endif