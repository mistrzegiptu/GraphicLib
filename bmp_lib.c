#include "bmp_lib.h"
#include <stdio.h>
#include <malloc.h>
#include <math.h>

void bmp_init(bmp_file *img, int width, int height, unsigned short bitCount)
{
    img->reserved = 0;
    img->dataOffset = 0;
    img->headerSize = 40;
    img->width = width;
    img->height = height;
    img->planes = 1;
    img->bitCount = bitCount;
    img->compression = NO_COMPRESSION;
    img->imageSize = 0;
    img->colorsImportant = 0;

    bmp_alloc(img);
}
int bmp_read_file(bmp_file *img, char *path)
{
    FILE *f;
    f = fopen(path, "rb");

    if(f == NULL)
    {
        printf("Error opening file, incorrect path... \n");
        return -1;
    }
    unsigned char header[18]; //18 bytes since I'm reading InfoHeader size at once
    if(fread(header, sizeof(unsigned char), 18, f) != 18)
    {
        printf("Error reading ..\n");
        return -1;
    }
    if(header[0] != 0x42 || header[1] != 0x4d)
    {
        printf("Wrong file type...\n");
        return -1;
    }

    img->fileSize = header[2] | header[3] << 8 | header[4] << 16 | header[5] << 24;
    img->dataOffset = header[10] | header[11] << 8 | header[12] << 16 | header[13] << 24;
    img->headerSize = header[14] | header[15] << 8 | header[16] << 16 | header[17] << 24;

    if(img->headerSize != 40) //Currently reading only 40bytes InfoHeader
    {
        printf("Header size %d \n", img->headerSize);
        printf("Wrong BMP header type...\n");
        return -1;
    }

    unsigned char infoHeader[36];
    if(fread(infoHeader, sizeof(unsigned char), 36, f) != 36)
    {
        printf("Error reading InfoHeader...\n");
        return -1;
    }

    for(int i = 0; i < 4; i++)
    {
        img->width |= infoHeader[i] << 8*i;
        img->height |= infoHeader[4+i] << 8*i;
        img->compression |= infoHeader[12+i] << 8*i;
        img->imageSize |= infoHeader[16+i] << 8*i;
        img->xPixelPerM |= infoHeader[20+i] << 8*i;
        img->yPixelsPerM |= infoHeader[24+i] << 8*i;
        img->colorUsed |= infoHeader[28+i] << 8*i;
        img->colorsImportant |= infoHeader[32+i] << 8*i;
    }
    img->planes = infoHeader[8] | infoHeader[9] << 8;
    img->bitCount = infoHeader[10] | infoHeader[11] << 8;
    bmp_print(img);
    if(img->rasterData == NULL) {
        printf("allocating for reading\n");
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
    /*FILE *ff;
    ff = fopen("outputBmp.ppm", "wb");
    printf("DUPA\n");
    if(ff==NULL)
    {
        printf("File error\n");
        return -1;
    }
    printf("Writing to file\n");
    fprintf(ff, "P3\n%d %d\n255\n", img->width, img->height);

    for(int i = 0; i < img->height; i++)
    {
        for(int j = 0; j < img->width; j++)
        {
            if(img->rasterData == NULL)
                return -1;
            color_value color = img->rasterData[i][j];
            unsigned char R = (color>>8*2)&0xFF;
            unsigned char G = (color>>8*1)&0xFF;
            unsigned char B = (color>>8*0)&0xFF;
            fprintf(ff, "%d %d %d\n", R, G, B);
        }
    }

    fclose(ff);*/
    fclose(f);
    return 0;
}
int bmp_save_file(bmp_file *img, char *path)
{
    FILE *f;
    f = fopen(path, "wb");

    if(f == NULL)
    {
        printf("Error opening file, incorrect path... \n");
        return -1;
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
        return -1;

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
void bmp_alloc(bmp_file *image)
{
    image->rasterData = malloc(sizeof(color_value *) * image->height);

    for(int i = 0; i < image->height; i++)
    {
        image->rasterData[i] = malloc(sizeof(color_value) * image->width);
    }
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
unsigned int calc_file_size(bmp_file *img)
{
    return 0;
}