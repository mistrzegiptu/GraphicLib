#ifndef GRAPHIC_FILTERS
#define GRAPHIC_FILTERS

typedef int color_value;

void negative(color_value **, int height, int width);
void sepia(color_value **raster_data, int height, int width);
void monochromatic(color_value **raster_data, int height, int width);
void contrast_change(color_value **raster_data, int height, int width, float k);
void brightness_change(color_value **raster_data, int height, int width, int delta);
void sobel_operator(color_value **raster_data, int height, int width);
void real_mono(color_value **raster_data, int height, int width);

#endif //GRAPHIC_FILTERS
