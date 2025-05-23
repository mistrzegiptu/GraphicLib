
#include <stdio.h>
#include <stdlib.h>
#include "bmp_lib.h"
#include "graphic_filters.h"
#include "GraphicLib.h"
#include <time.h>

int main(void)
{
    /*color_value **display = {0};
    int width = 1920, height = 1080;
    display = gl_innit(width, height);
    printf("Dupa");
    gl_fill_color(display, width, height, COLOR_BLACK);
    gl_fill_rect(display, width, height, 2, 2, 6, 6, 0xF0F0F0);
    gl_fill_rect(display, width, height, 2, 5, 11, 12, 0xABCDEF);
    gl_draw_line(display, width, height, 10, 4, 50, 170, 0xBFBFBF);
    gl_draw_line(display, width, height, 170, 50, 10, 170, 0x74234F);
    gl_draw_triangle(display, width, height, 10, 10, 150, 30, 40, 180, COLOR_GREEN);
    gl_fill_rect(display, width, height, 2, 5, 100, 100, 0x000000);
    gl_draw_triangle(display, height, width, 20, 20, 100, 200, 400, 300, 0xFFFFFF);


    const int MAXP = 30;
    int x,y;
    int c,r,g,b;
    for(int i = 0; i < MAXP; i++)
    {
      x = ((width - 1) * i) / (MAXP - 1);
      y = ((height - 1) * i) / (MAXP - 1);
      r = (127 * i) / (MAXP - 1);
      b = (255 * i) / (MAXP - 1);
      g = 128 - r;
      c = (r << 16) | (g << 8) | b;
      gl_draw_line(display, height, width, x, 0, 0, height-y-1, c);
      gl_draw_line(display, height, width, width-x-1, height-1, width-1, y, c^0xFFFFFF);

    }

    gl_save_ppm(display, width, height, "output.ppm");*/
    //gl_read_bmp(display, "openttd.bmp");

    //printf("\n%d %d", width, height);
    //gl_free_display(display, width, height);
    clock_t sw;
    bmp_file image = {0};
    //bmp_init(&image, width, height, 24);
    //image.rasterData = display;
    bmp_read_file(&image, "testBmp/marbles.bmp");
    sw = clock();
    gl_print_ascii(image.rasterData, image.width, image.height);
    sw = clock()-sw;
    printf("%f\n", (((double)sw)/CLOCKS_PER_SEC));
    //sepia(image.rasterData, image.height, image.width);
    //contrast_change(image.rasterData, image.height, image.width, 2.5);
    //sobel_operator(image.rasterData, image.height, image.width);
    //real_mono(image.rasterData, image.height, image.width);
    //brightness_change(image.rasterData, image.height, image.width, 60);
    bmp_save_file(&image, "res.bmp");
    bmp_free(&image);
    return 0;
}