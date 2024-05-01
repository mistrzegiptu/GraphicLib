#ifndef GRAPHIC_LIB
#define GRAPHIC_LIB

//BASIC COLORS
#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLUE 0x0000FF
#define COLOR_GREEN 0x00FF00
#define COLOR_RED 0xFF0000

typedef unsigned char color_channel;
typedef unsigned int color_val;

color_val** gl_innit(int width, int height);
int gl_fill_color(color_val **display, int width, int height, color_val color);
int gl_save_ppm(color_val **display, int width, int height, char *filePath);
int gl_fill_rect(color_val **display, int width, int height, int x1, int y1, int x2, int y2, color_val color);
int gl_draw_line(color_val **display, int width, int height, int x1, int y1, int x2, int y2, color_val color);
int gl_draw_triangle(color_val **display, int width, int height, int x1, int y1, int x2, int y2, int x3, int y3, color_val color);
int gl_flooding(color_val **display, int width, int height, int y, int x, color_val color);
int gl_save_png(color_val **display, int width, int height, char *filePath);
int gl_read_bmp(color_val **display, char *filePath);
int gl_free_display(color_val **display, int width, int height);

#endif
