#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

typedef unsigned char bytes;

void print_bytes(bytes *buffer, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%04x ", buffer[i]);
        if (i % 9 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

u_int8_t urandint8(u_int8_t min, u_int8_t max)
{
    if (min > max)
    {
        exit(EXIT_FAILURE);
    }
    return rand() % (max - min) + min + 1;
}

void write_bytes(bytes *dest, size_t *pos, size_t size, size_t num_args, ...)
{
    size_t i;
    va_list args;
    va_start(args, num_args);
    for (i = 0; i < num_args; i++)
    {
        dest[*pos + i] = (bytes)va_arg(args, int);
    }
    va_end(args);
    *pos = *pos + size;
}

void set_BMP_header(bytes *buffer, size_t *pos)
{
    write_bytes(buffer, pos, 2, 2, 0x42, 0x4D); // ID field (BM)
    write_bytes(buffer, pos, 4, 1, 0x46);       // total header size (54 bytes)
    write_bytes(buffer, pos, 2, 1, 0x00);       // application specific
    write_bytes(buffer, pos, 2, 1, 0x00);       // application specific
    write_bytes(buffer, pos, 4, 1, 0x36);       // offset where the pixel array (bitmap data) can be found
}

void set_DIB_header(bytes *buffer, size_t *pos, size_t image_size, size_t width, size_t height)
{
    bytes w1, w2;
    bytes h1, h2;

    if (width > 0xFF)
    {
        w2 = (width & 0xff00) >> 8;
        w1 = width & 0x00ff;
    }
    else
    {
        w1 = width;
        w2 = 0;
    }

    if (height > 0xFF)
    {
        h2 = (height & 0xff00) >> 8;
        h1 = height & 0x00ff;
    }
    else
    {
        h1 = height;
        h2 = 0;
    }

    write_bytes(buffer, pos, 4, 1, 0x28);       // remaining header size (40 bytes)
    write_bytes(buffer, pos, 4, 2, w1, w2);     // width of the bitmap in pixels
    write_bytes(buffer, pos, 4, 2, h1, h2);     // height of the bitmap in pixels
    write_bytes(buffer, pos, 2, 1, 0x01);       // number of color planes being used
    write_bytes(buffer, pos, 2, 1, 0x20);       // number of bits per pixel (32 bits)
    write_bytes(buffer, pos, 4, 1, 0x00);       // BI_RGB, no pixel array compression used
    write_bytes(buffer, pos, 4, 1, image_size); // size of the raw bitmap data (including padding)
    write_bytes(buffer, pos, 4, 2, 0x13, 0x0B); // 2835 pixels/metre horizontal
    write_bytes(buffer, pos, 4, 2, 0x13, 0x0B); // 2835 pixels/metre vertical
    write_bytes(buffer, pos, 4, 1, 0x00);       // number of colors in the palette
    write_bytes(buffer, pos, 4, 1, 0x00);       // 0 means all colors are important
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n./generate_noise <width> <height>\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp;
    bytes *buffer;
    size_t pos = 0;
    size_t width = atoi(argv[1]);
    size_t height = atoi(argv[2]);
    size_t image_size = 4 * width * height;
    size_t size = 54 + image_size;
    size_t num_pixels = width * height;

    srand(time(NULL));

    fp = fopen("noise.bmp", "wb");

    buffer = (bytes *)malloc(sizeof(bytes) * size);

    memset(buffer, 0, size);

    set_BMP_header(buffer, &pos);

    set_DIB_header(buffer, &pos, image_size, width, height);

    // Start of pixel array (bitmap data) // LITTLE ENDIAN

    for (unsigned int i = 0; i < num_pixels; i++)
        write_bytes(buffer, &pos, 4, 4, urandint8(0x00, 0xFF), urandint8(0x00, 0xFF), urandint8(0x00, 0xFF), 0xFF);

    fwrite(buffer, size, 1, fp);

    free(buffer);
    fclose(fp);

    return 0;
}
