#ifndef BMP_LIB
#define BMP_LIB

typedef unsigned int color_value;

typedef struct _bmp_header
{
    unsigned int fileSize = 0;
    unsigned int headerSize = 0;
    unsigned int width = 0;
    unsigned int height = 0;

    unsigned short planes = 1;
    unsigned short bitCount = 0;

    unsigned int compression = 0;
    unsigned int imageSize = 0;
}bmp_header;

typedef struct _bmp_file
{
    bmp_header header = {0};
    color_value **pixels = {0};
}bmp_file;
#endif
