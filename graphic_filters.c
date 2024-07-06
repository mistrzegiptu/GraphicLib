#include "graphic_filters.h"

void negative(color_value **raster_data, int height, int width)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            raster_data[i][j] = 0xFFFFFF - raster_data[i][j];
        }
    }
}
void sepia(color_value **raster_data, int height, int width)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
             float R = (float)((raster_data[i][j] >> 0) & 0xFF);
             float G = (float)((raster_data[i][j] >> 8) & 0xFF);
             float B = (float)((raster_data[i][j] >> 16) & 0xFF);
             int Rs = (int)(0.393*R+0.769*G+0.189*B);
             int Gs = (int)(0.349*R+0.686*G+0.168*B);
             int Bs = (int)(0.272*R+0.534*G+0.131*B);
             Rs = Rs > 255 ? 255 : Rs;
             Gs = Gs > 255 ? 255 : Gs;
             Bs = Bs > 255 ? 255 : Bs;
             raster_data[i][j] = (Rs << 16) | (Gs << 8) | (Bs << 0);
        }
    }
}
void monochromatic(color_value **raster_data, int height, int width)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float R = (float)((raster_data[i][j] >> 0) & 0xFF) * 0.299;
            float G = (float)((raster_data[i][j] >> 8) & 0xFF) * 0.587;
            float B = (float)((raster_data[i][j] >> 16) & 0xFF) * 0.114;
            float gray = R * 0.299 + G * 0.587 + B * 0.114;
            int grayInt = (int)gray;
            raster_data[i][j] = (grayInt << 0) | (grayInt << 8) | (grayInt << 16);
        }
    }
}
void contrast_change(color_value **raster_data, int height, int width, float k)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int R = (raster_data[i][j] >> 16) & 0xFF;
            int G = (raster_data[i][j] >> 8) & 0xFF;
            int B = (raster_data[i][j] >> 0) & 0xFF;
            R = (int)((R - 128) * k + 128);
            G = (int)((G - 128) * k + 128);
            B = (int)((B - 128) * k + 128);
            R = R > 255 ? 255 : (R < 0 ? 0 : R);
            G = G > 255 ? 255 : (G < 0 ? 0 : G);
            B = B > 255 ? 255 : (B < 0 ? 0 : B);
            raster_data[i][j] = (R << 16) | (G << 8) | (B << 0);
        }
    }
}
void brightness_change(color_value **raster_data, int height, int width, int delta)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int R = ((raster_data[i][j] >> 16) & 0xFF) + delta;
            int G = ((raster_data[i][j] >> 8) & 0xFF) + delta;
            int B = ((raster_data[i][j] >> 0) & 0xFF) + delta;
            R = R > 255 ? 255 : (R < 0 ? 0 : R);
            G = G > 255 ? 255 : (G < 0 ? 0 : G);
            B = B > 255 ? 255 : (B < 0 ? 0 : B);
            raster_data[i][j] = (R << 16) | (G << 8) | (B << 0);
        }
    }
}