#include "bmp_lib.h"
#include <stdio.h>
#include <malloc.h>
#include <math.h>

void bmp_init(bmp_file *img, int width, int height, unsigned short bitCount)
{
    img->reserved = 0;
    img->dataOffset = 0;
    img->fileSize = 0;

    bmp_alloc(img);
}
int bmp_read_file(bmp_file *img, char *path)
{
    FILE *f;
    f = fopen(path, "rb");

    if(f == NULL)
    {
        bmp_debug_log("Error opening file, incorrect path... \n");
        return BMP_FILE_ERROR;
    }
    if(bmp_check_header(f) != BMP_SUCCESS)
    {
        bmp_debug_log("Wrong header type");
        return BMP_HEADER_ERROR;
    }
    unsigned char header[14];
    if(fread(header, sizeof(unsigned char), 14, f) != 14)
    {
        bmp_debug_log("Error reading ..\n");
        return BMP_FILE_ERROR;
    }

    img->fileSize = header[2] | header[3] << 8 | header[4] << 16 | header[5] << 24;
    img->dataOffset = header[10] | header[11] << 8 | header[12] << 16 | header[13] << 24;
    img->header_version = (bmp_version)bmp_get_header_size(f);

    if(img->header_version != BMPHEADER_40) //TODO: other headers
    {
        printf("Header size %d \n", img->headerSize);
        bmp_debug_log("Wrong BMP header type...\n");
        return BMP_HEADER_ERROR;
    }

    int width = bmp_get_width(img);
    int height = bmp_get_height(img);
    bmp_compression compression = bmp_get_compression(img);

    if(img->rasterData == NULL) {
        bmp_alloc(img);
    }

    if(img->compression == NO_COMPRESSION && img->bitCount == 8 && img->dataOffset != 54)
    {
        unsigned char colorTable[1024];
        color_value colorArray[256];

        size_t bytesReaded = fread(colorTable, sizeof(unsigned char), 1024, f);
        //printf("color table bytes %lld\n", bytesReaded);

        for(int i = 0; i < 256; i++) {
            colorArray[i] = colorTable[4 * i] | colorTable[4 * i + 1] << 8 | colorTable[4 * i + 2] << 16 | colorTable[4 * i + 3] << 24;
            //printf("%x \n", colorArray[i]);
        }

        unsigned char *row = malloc(sizeof(unsigned char) * img->width);
        for(int i = img->height-1; i >= 0; i--)
        {
            size_t rowBytes = fread(row, sizeof(unsigned char), img->width, f);
            //printf("row bytes %lld\n", rowBytes);
            for(int j = 0; j < img->width; j++)
            {
                if(img->rasterData != NULL)
                    img->rasterData[i][j] = colorArray[(int)row[j]];
            }
        }
        free(row);
    }
    else
    {
        printf("DUPA DUPA DUPA\n");
        for(int i = img->height-1; i >= 0; i--)
        {
            unsigned char *row = malloc(sizeof(unsigned char) * img->width * (img->bitCount/8));
            size_t rowByte = fread(row, sizeof(unsigned char), img->width*(img->bitCount/8), f);
            //printf("row bytes %lld\n", rowByte);
            for(int j = 0; j < img->width; j++)
            {
                if(img->rasterData != NULL && img->rasterData[i] != NULL)
                    img->rasterData[i][j] = row[j*3] | row[j*3+1] << 8 | row[j*3+2] << 16;
                else
                    printf("O kurwa\n");
            }
            fseek(f, img->width % 4, SEEK_CUR); //Row padding
            free(row);
        }
    }
    fclose(f);
    return 0;
}
int bmp_check_header(FILE *file)
{
    unsigned char header[2] = {0};

    if(fread(header, sizeof(unsigned char), 2, file) != 2)
    {
        bmp_debug_log("Error reading ..\n");
        return BMP_FILE_ERROR;
    }
    if((header[0] << 8 | header[1]) != BMP_MAGIC_NUM)
        return BMP_HEADER_ERROR;

    fseek(f, 0, SEEK_SET);

    return BMP_SUCCESS;
}
int bmp_get_header_size(FILE *f)
{
    unsigned char headerSize[4];
    int result = 0;

    if(fread(header, sizeof(unsigned char), 2, file) != 2)
    {
        bmp_debug_log("Error reading ..\n");
        return BMP_FILE_ERROR;
    }

    for(int i = 0; i < 4; i++)
        result |= headerSize[i] << 8 * (3 - i);

    fseek(f, 14, SEEK_SET);
    return result;
}
int bmp_read_header(bmp_file *img, FILE *file)
{
    if(img == NULL)
    {
        bmp_debug_log("Img is null");
        return BMP_FILE_ERROR;
    }

    switch(img->headerSize)
    {
        case OS2_16:
        {
            OS2BMPFILEHEADER header = {0};

            if(fread(&header, sizeof(OS2BMPFILEHEADER), 1, file) != 1)
            {
                bmp_debug_log("Error reading header");
                return BMP_FILE_ERROR;
            }

            img->header_version = OS2_16;
            img->header.os2bmpfileheader = header;
            break;
        }
        case BMPHEADER_40:
        {
            BITMAPINFOHEADER header = {0};

            if(fread(&header, sizeof(BMPHEADER_40), 1, file) != 1)
            {
                bmp_debug_log("Error reading header");
                return BMP_FILE_ERROR;
            }

            img->header_version = BMPHEADER_40;
            img->header.bitmapinfoheader = header;
            break;
        }
        default:
        {
            bmp_debug_log("Unknown header");
            return BMP_HEADER_ERROR;
        }
    }
    return BMP_SUCCESS;
}
int bmp_get_width(bmp_file *img)
{
    switch(img->headerSize) {
        case OS2_16: {
            return img->header.os2bmpfileheader.Width;
        }
        case BMPHEADER_40:
        {
            return img->header.bitmapinfoheader.biWidth;
        }
        default:
            return -1;
    }
}
int bmp_get_height(bmp_file *img)
{
    switch(img->headerSize)
    {
        case OS2_16:
        {
            return img->header.os2bmpfileheader.Height;
        }
        case BMPHEADER_40:
        {
            return img->header.bitmapinfoheader.biHeight;
        }
        default:
            return -1;
    }
}
bmp_compression bmp_get_compression(bmp_file *img)
{
    switch(img->headerSize)
    {
        case OS2_16:
        {
            return img->header.os2bmpfileheader.BitsPerPixel;
        }
        case BMPHEADER_40:
        {
            return img->header.bitmapinfoheader.biHeight;
        }
        default:
            return -1;
    }
}
int bmp_save_file(bmp_file *img, char *path)
{
    FILE *f;
    f = fopen(path, "wb");

    if(f == NULL)
    {
        bmp_debug_log("Error opening file, incorrect path... \n");
        return BMP_FILE_ERROR;
    }

    unsigned short magic_num = BMP_MAGIC_NUM;
    fwrite(&magic_num, sizeof(unsigned short), 1, f);

    if(img->fileSize == 0)
        img->fileSize = calc_file_size(img);

    fwrite(&img->fileSize, sizeof(unsigned int), 1, f);
    fwrite(&img->reserved, sizeof(unsigned int), 1, f);
    fwrite(&img->dataOffset, sizeof(unsigned int), 1, f);
    fwrite(&img->headerSize, sizeof(unsigned int), 1, f);
    fwrite(&img->width, sizeof(int), 1, f);
    fwrite(&img->height, sizeof(int), 1, f);
    fwrite(&img->planes, sizeof(unsigned short), 1, f);
    fwrite(&img->bitCount, sizeof(unsigned short), 1, f);
    fwrite(&img->compression, sizeof(int), 1, f);
    fwrite(&img->imageSize, sizeof(unsigned int), 1, f);
    fwrite(&img->xPixelPerM, sizeof(int), 1, f);
    fwrite(&img->yPixelsPerM, sizeof(int), 1, f);
    fwrite(&img->colorUsed, sizeof(int), 1, f);
    fwrite(&img->colorsImportant, sizeof(int), 1, f);

    if(img->rasterData == NULL)
        return BMP_FILE_ERROR;

    if(img->bitCount <= 8) {
        color_value *colorTable;

        unsigned char **encodedRasterData = malloc(sizeof(unsigned char *) * img->height);
        for (int i = 0; i < img->height; i++)
            encodedRasterData[i] = malloc(sizeof(unsigned char) * img->width);

        colorTable = bmp_create_color_table(img, encodedRasterData);

        fwrite(colorTable, sizeof(color_value), (int)(pow(2, img->bitCount)), f);

        for (int i = img->height - 1; i >= 0; i--) {
            fwrite(encodedRasterData[i], sizeof(unsigned char), img->width, f);
            free(encodedRasterData[i]);
        }
        free(encodedRasterData);
    }
    else {
        int paddingSize = img->width % 4;
        unsigned char padding[paddingSize];
        for (int i = img->height - 1; i >= 0; i--) {
            for (int j = 0; j < img->width; j++) {
                unsigned char colorData[3];
                colorData[0] = (img->rasterData[i][j] >> 0) & 0xFF;
                colorData[1] = (img->rasterData[i][j] >> 8) & 0xFF;
                colorData[2] = (img->rasterData[i][j] >> 16) & 0xFF;
                fwrite(colorData, sizeof(unsigned char), 3, f);
            }
            fwrite(padding, sizeof(unsigned char), paddingSize, f);
        }
    }

    fclose(f);
    return 0;
}
color_value *bmp_create_color_table(bmp_file *img, unsigned char **encodedRasterData)
{
    size_t size = (size_t)pow(2, img->bitCount);
    color_value *colorTable = malloc(sizeof(color_value) * size);

    int colorCount = 0;

    for (int i = img->height-1; i >= 0; i--)
    {
        for(int j = 0; j < img->width; j++)
        {
            int k = 0, found = 0;
            for(k; k < colorCount; k++) {
                if (colorTable[k] == img->rasterData[i][j]) {
                    found = 1;
                    break;
                }
            }
            if(!found)
            {
                colorTable[colorCount] = img->rasterData[i][j];
                colorCount += 1;
            }
            (encodedRasterData[i][j]) = k;
        }
    }

    return colorTable;
}
void bmp_print(bmp_file *img)
{
    if(img == NULL)
        return;
    printf("...............................................\n");
    printf("HEADER:\n");
    printf("File size - %d\n", img->fileSize);
    printf("Data offset - %d\n", img->dataOffset);
    printf("INFO HEADER:\n");
    printf("Info Header size - %d\n", img->headerSize);
    printf("Width - %d\n", img->width);
    printf("Height - %d\n", img->height);
    printf("Planes - %d\n", img->planes);
    printf("BitCount - %d\n", img->bitCount);
    printf("Compression - %d\n", img->compression);
    printf("Image size - %d\n", img->imageSize);
    printf("x rasterData per m - %d\n", img->xPixelPerM);
    printf("y rasterData per m - %d\n", img->yPixelsPerM);
    printf("Colors used - %d\n", img->colorUsed);
    printf("Colors important - %d\n", img->colorsImportant);
    printf("...............................................\n");
}
bmp_error bmp_alloc(bmp_file *image)
{
    if(image == NULL)
    {
        bmp_debug_log("Bmp is null... \n");
        return BMP_ALLOC_ERROR;
    }

    image->rasterData = malloc(sizeof(color_value *) * image->height);

    if(image->rasterData == NULL)
    {
        bmp_debug_log("Error allocating memory... \n");
        return BMP_ALLOC_ERROR;
    }

    for(int i = 0; i < image->height; i++)
    {
        image->rasterData[i] = malloc(sizeof(color_value) * image->width);
    }

    return BMP_SUCCESS;
}
void bmp_free(bmp_file *img)
{
    if(img == NULL)
        return;

    for(int i = 0; i < img->height; i++)
        free(img->rasterData[i]);

    free(img->rasterData);
    free(img);
}
void bmp_debug_log(char *errorMessage)
{
    #ifdef BMP_DEBUG
        printf("%s", errorMessage);
    #endif
}
unsigned int calc_file_size(bmp_file *img)
{
    return 0;
}