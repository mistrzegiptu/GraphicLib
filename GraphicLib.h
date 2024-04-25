#ifndef GRAPHIC_LIB
#define GRAPHIC_LIB

#include <malloc.h>
#include <stdio.h>
#include <zlib.h>
//#include <png.h>
//BASIC COLORS
#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLUE 0x0000FF
#define COLOR_GREEN 0x00FF00
#define COLOR_RED 0xFF0000

typedef unsigned char color_channel;
typedef int color_val;

int** gl_innit(int width, int height);
int gl_fill_color(int **display, int width, int height, int color);
int gl_save_ppm(int **display, int width, int height, char *filePath);
int gl_fill_rect(int **display, int width, int height, int x1, int y1, int x2, int y2, color_val color);
int gl_draw_line(int **display, int width, int height, int x1, int y1, int x2, int y2, color_val color);
int gl_draw_triangle(int **display, int width, int height, int x1, int y1, int x2, int y2, int x3, int y3, color_val color);
int gl_flooding(int **display, int width, int height, int y, int x, color_val color);
int gl_save_png(int **display, int width, int height, char *filePath);
int gl_read_bmp(int **display, char *filePath);
int getVal(char c);

#endif
