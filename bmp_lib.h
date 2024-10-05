#ifndef BMP_LIB
#define BMP_LIB

#define BMP_DEBUG 0

#define BMP_MAGIC_NUM 0x424D

#define READ_INT(src, i) (src[i] | src[i+1] << 8 | src[i+2] << 16 | src[i+3] << 24)
#define READ_SHORT(src, i) (src[i] | src[i+1] << 8)

typedef int color_value;

typedef enum
{
    BMP_SUCCESS = 0,
    BMP_FILE_ERROR = 1,
    BMP_HEADER_ERROR = 2,
    BMP_ALLOC_ERROR = 3
}bmp_error;

typedef enum
{
    NO_COMPRESSION = 0,
    RLE_8_BITS = 1,
    RLE_4_BITS = 2
}bmp_compression;

typedef struct //40 bytes header
{
    unsigned int    biSize;
    long            biWidth;
    long            biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    long            biXPelsPerMeter;
    long            biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
} BITMAPINFOHEADER;

typedef struct //16 bytes header
{
    unsigned int    Size;
    unsigned int    Width;
    unsigned int    Height;
    unsigned short  NumPlanes;
    unsigned short  BitsPerPixel;
} OS2BMPFILEHEADER;

typedef union
{
    BITMAPINFOHEADER bitmapinfoheader;
    OS2BMPFILEHEADER os2bmpfileheader;
} bmp_header;

typedef enum
{
    OS2_16 = 16,
    BMPHEADER_40 = 40
} bmp_version;

typedef struct
{
    unsigned int    fileSize;
    unsigned int    reserved;
    unsigned int    dataOffset;

    bmp_header      header;
    bmp_version     header_version;

    color_value     **rasterData;
} bmp_file;

void bmp_init(bmp_file *img, int width, int height, unsigned short bitCount);
int bmp_read_file(bmp_file *img,  char *path);
int bmp_save_file(bmp_file *img, char *path);
color_value *bmp_create_color_table(bmp_file *img, unsigned char **encodedRasterData);
int bmp_read_header(bmp_file *img, FILE *file);
void bmp_print(bmp_file *img);
void bmp_alloc(bmp_file *img);
void bmp_free(bmp_file *img);
void bmp_debug_log(char *errorMessage);
unsigned int calc_file_size(bmp_file *img);
//size_t bmp_get_header_size(FILE *f);

#endif