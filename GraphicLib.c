#include "GraphicLib.h"

color_val** gl_innit(int width, int height)
{
    color_val **display = malloc(sizeof(color_val *) * height);
    for(int i = 0; i < height; i++)
    {
        display[i] = malloc(sizeof(color_val *) * width);
    }
    printf("Innit succes");
    return display;
}
int gl_fill_color(color_val **display, int width, int height, color_val color)
{
    printf("Dupa1");
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            display[i][j] = color;
        }
    }
    return 0;
}
int gl_save_ppm(color_val **display, int width, int height, char *filePath)
{
    FILE *f;
    f = fopen(filePath, "wb");
    printf("DUPA\n");
    if(f==NULL)
    {
        printf("File error\n");
        return -1;
    }
    printf("Writing to file\n");
    fprintf(f, "P3\n%d %d\n255\n", width, height);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            color_val color = display[i][j];
            color_channel R = (color>>8*2)&0xFF;
            color_channel G = (color>>8*1)&0xFF;
            color_channel B = (color>>8*0)&0xFF;
            fprintf(f, "%d %d %d\n", R, G, B);
        }
    }

    fclose(f);
    return 0;
}
int gl_fill_rect(color_val **display, int width, int height, int x1, int y1, int x2, int y2, color_val color)
{
    for(int i = y1; i <= y2 && i < height; i++)
    {
        for(int j = x1; j <= x2 && j < width; j++)
        {
            display[i][j] = color;
        }
    }
    return 0;
}
int gl_draw_line(color_val **display, int width, int height, int x1, int y1, int x2, int y2, color_val color)
{
    int d, dx, dy, ai, bi, xi, yi;
    int x = x1, y = y1;
    if (x1 < x2)
    {
        xi = 1;
        dx = x2 - x1;
    }
    else
    {
        xi = -1;
        dx = x1 - x2;
    }
    if (y1 < y2)
    {
        yi = 1;
        dy = y2 - y1;
    }
    else
    {
        yi = -1;
        dy = y1 - y2;
    }
    if(y >= 0 && y <= height && x >= 0 && x <= width)
        display[y][x] = color;
    if (dx > dy)
    {
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d = bi - dx;
        while (x != x2)
        {
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                x += xi;
            }
            if(y >= 0 && y <= height && x >= 0 && x <= width)
                display[y][x] = color;
        }
    }
    else
    {
        ai = ( dx - dy ) * 2;
        bi = dx * 2;
        d = bi - dy;
        while (y != y2)
        {
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                y += yi;
            }
            if(y >= 0 && y <= height && x >= 0 && x <= width)
                display[y][x] = color;
        }
    }
    return 0;
}
int gl_draw_triangle(color_val **display, int width, int height, int x1, int y1, int x2, int y2, int x3, int y3, color_val color)
{
    gl_draw_line(display, width, height, x1, y1, x2, y2, color);
    gl_draw_line(display, width, height, x1, y1, x3, y3, color);
    gl_draw_line(display, width, height, x2, y2, x3, y3, color);
    int x = (x1 + x2 + x3) / 3;
    int y = (y1 + y2 + y3) / 3;
    display[y][x] = color;
    gl_flooding(display, width, height, y, x, color);
    return 0;
}
int gl_flooding(color_val **display, int width, int height, int y, int x, color_val color)
{
    if(x>=0&&x<width && y>=0&&y<height)
        display[y][x] = color;
    if(display[y+1][x]!=color)
        gl_flooding(display, width, height, y+1, x, color);
    if(display[y-1][x]!=color)
        gl_flooding(display, width, height, y-1, x, color);
    if(display[y][x+1]!=color)
        gl_flooding(display, width, height, y, x+1, color);
    if(display[y][x-1]!=color)
        gl_flooding(display, width, height, y, x-1, color);
    return 0;
}
int gl_read_bmp(color_val **display, char *filePath)
{
    FILE *f;
    f = fopen(filePath, "rb");
    printf("\n");
    if(f==NULL)
    {
        printf("File error");
        return -1;
    }
    unsigned char header[14];
    unsigned char infoHeader[40];

    fread(header, sizeof(header), 1, f);
    fread(infoHeader, sizeof(infoHeader), 1, f);
    for(int i = 0; i < 14; i++)
    {
        printf("%x ", header[i]);
    }
    for(int i = 0; i < 40; i++)
        printf("%x ", infoHeader[i]);

    size_t fileSize = (header[2]) | (header[3] << 8) | (header[4] << 16) | header[5]<<24;
    int width = 0, height = 0, bitCount = 0, colorsUsed = 0;
    for(int i = 0; i < 4; i++)
    {
        width |= infoHeader[4+i] << 8 * i;
        height |= infoHeader[8+i] << 8 * i;
        colorsUsed |= infoHeader[32+i] << 8 * i;
    }
    colorsUsed = 256;
    bitCount = infoHeader[14] | infoHeader[15] << 8;
    //display = gl_innit(width, height);
    color_val _display[32][32] = {0};
    printf("File size = %I64llu \n", fileSize);
    printf("width = %d height = %d\n", width, height);
    printf("Colors used = %d\n", colorsUsed);
    printf("Bit Count = %d\n", bitCount);

    color_channel colorTable[1024] = {0};
    //color_val colorTable[32*32] = {0};
    //fseek(f, 54, SEEK_SET);
    printf("bytes readed: %lld\n", fread(colorTable, sizeof(color_channel), colorsUsed*4, f));

    for(int i = 0; i < 1024; i++)
        printf("%x ", colorTable[i]);
    color_val colorArray[256] = {0};
    for(int i = 0; i < 256; i++)
        colorArray[i] = colorTable[4*i] | colorTable[4*i+1] << 8 | colorTable[4*i+2] << 16 | colorTable[4*i+3] << 24;

    for(int i = 0; i < 256; i++)
        printf("%x \n", colorArray[i]);

    for(int i = 0; i < height; i++)
    {
        color_channel row[32] = {0};
        printf("bytes of row %d readed: %lld\n",i, fread(row, sizeof(row), 1, f));

        for(int j = 0; j < 32; j++)
        {
            printf("%d ", row[j]);
            _display[i][j] = colorArray[(int)row[j]];
        }
        printf("\n");
    }

    FILE *ff;
    ff = fopen("kurwa.ppm", "wb");
    printf("DUPA\n");
    if(ff==NULL)
    {
        printf("File error\n");
        return -1;
    }
    printf("Writing to file\n");
    fprintf(ff, "P3\n%d %d\n255\n", width, height);

    for(int i = height-1; i >= 0; i--)
    {
        for(int j = 0; j < width; j++)
        {
            color_val color = _display[i][j];
            color_channel R = (color>>8*2)&0xFF;
            color_channel G = (color>>8*1)&0xFF;
            color_channel B = (color>>8*0)&0xFF;
            fprintf(ff, "%d %d %d\n", R, G, B);
        }
    }

    fclose(ff);
    //gl_save_ppm(rowPointers, 32, 32, "hggjkh.ppm");
    fclose(f);
    //free(colorTable);
    return 0;
}
int gl_free_display(color_val **display, int width, int height)
{
    for(int i = 0; i < height; i++)
        free(display[i]);
    free(display);
    return 0;
}
/*int gl_save_png(int **display, int height, int width, char *filePath)
{
    FILE *pngFile = fopen(filePath, "wb");
    
    if(!pngFile)  return -1;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png)
    {
        png_destroy_write_struct(&png, NULL);
        fclose(pngFile);
        return -1;
    }

    png_infop info = png_create_info_struct(png);
    if(!info)
    {
        png_destroy_write_struct(&png, NULL);
        fclose(pngFile);
        return -1;
    }

    if(setjmp(png_jmpbuf(png)))
    {
        png_destroy_write_struct(&png, NULL);
        fclose(pngFile);
        return -1;
    }

    png_init_io(png, pngFile);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    png_byte *row = png_malloc(png, sizeof(char) * width * 3);
    png_byte *start = row;
    for(int y = 0; y < height; y++)
    {
        row = start;
        for(int x = 0; x < width; x++)
        {
            int color = display[y][x];
            *row++ = (color & 0x0000FF) >> 0;
            *row++ = (color & 0x00FF00) >> 8;
            *row++ = (color & 0xFF0000) >> 16;
        }
        png_write_row(png, row);
    }
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(pngFile);
}*/