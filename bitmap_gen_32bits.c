#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void write_bytes(unsigned char *dest, size_t *pos, size_t padding, size_t num_args, ...)
{
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++)
    {
        dest[*pos + i] = (unsigned char)va_arg(args, int);
    }
    va_end(args);
    *pos = *pos + num_args + padding;
}

void set_BMP_header(unsigned char *buffer, size_t *i)
{
    write_bytes(buffer, i, 0, 2, 0x42, 0x4D); // ID field (BM)
    write_bytes(buffer, i, 3, 1, 0x46);       // header size (54 bytes)
    write_bytes(buffer, i, 1, 1, 0x00);       // application specific
    write_bytes(buffer, i, 1, 1, 0x00);       // application specific
    write_bytes(buffer, i, 3, 1, 0x36);       // offset where the pixel array (bitmap data) can be found
}

void set_DIB_header(unsigned char *buffer, size_t *i)
{
    write_bytes(buffer, i, 3, 1, 0x28);       // header size (40 bytes)
    write_bytes(buffer, i, 3, 1, 0x02);       // width of the bitmap in pixels
    write_bytes(buffer, i, 3, 1, 0x02);       // height of the bitmap in pixels
    write_bytes(buffer, i, 1, 1, 0x01);       // number of color planes being used
    write_bytes(buffer, i, 1, 1, 0x20);       // number of bits per pixel (32 bits)
    write_bytes(buffer, i, 3, 1, 0x00);       // BI_RGB, no pixel array compression used
    write_bytes(buffer, i, 3, 1, 0x10);       // size of the raw bitmap data (including padding) - 16 bytes
    write_bytes(buffer, i, 2, 2, 0x13, 0x0B); // 2835 pixels/metre horizontal
    write_bytes(buffer, i, 2, 2, 0x13, 0x0B); // 2835 pixels/metre vertical
    write_bytes(buffer, i, 3, 1, 0x00);       // number of colors in the palette
    write_bytes(buffer, i, 3, 1, 0x00);       // 0 means all colors are important
}

int main(void)
{
    FILE *fp;
    unsigned char *buffer;
    size_t i = 0;
    size_t size = 70;

    fp = fopen("bitmap_example.bmp", "wb");

    buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);

    memset(buffer, 0, size);

    set_BMP_header(buffer, &i);

    set_DIB_header(buffer, &i);

    // Start of pixel array (bitmap data)

    write_bytes(buffer, &i, 0, 4, 0x0, 0x0, 0xFF, 0xFF);   // Red, Pixel (x=0, y=1) alpha 255
    write_bytes(buffer, &i, 0, 4, 0xFF, 0xFF, 0xFF, 0xFF); // White, Pixel (x=1, y=1) alpha 255
    write_bytes(buffer, &i, 0, 4, 0xFF, 0x0, 0x0, 0xFF);   // Blue, Pixel (x=0, y=0) alpha 255
    write_bytes(buffer, &i, 0, 4, 0x0, 0xFF, 0x0, 0xFF);   // Green, Pixel (x=1, y=0) alpha 255

    fwrite(buffer, size, 1, fp);

    free(buffer);
    fclose(fp);

    return 0;
}
