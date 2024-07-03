#ifndef BMP_LIB
#define BMP_LIB

#define BMP_MAGIC_NUM 19778

typedef int color_value;

typedef enum _bmp_compression
{
    NO_COMPRESSION = 0,
    RLE_8_BITS = 1,
    RLE_4_BITS = 2
}bmp_compression;

typedef struct _bmp_file
{
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int dataOffset;
    unsigned int headerSize;

    int width;
    int height;

    unsigned short planes;
    unsigned short bitCount;

    bmp_compression compression;

    unsigned int imageSize;

    int xPixelPerM;
    int yPixelsPerM;
    int colorUsed;
    int colorsImportant;

    color_value **rasterData;
}bmp_file;

void bmp_init(bmp_file *img, int width, int height);
int bmp_read_file(bmp_file *img,  char *path);
int bmp_save_file(bmp_file *img, char *path);
void bmp_print(bmp_file *img);
void bmp_alloc(bmp_file *img);
void bmp_free(bmp_file *img);
unsigned int calc_file_size(bmp_file *img);

#endif