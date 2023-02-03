#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef unsigned char bytes;

void write_bytes(bytes *dest, size_t *pos, size_t padding, size_t num_args, ...)
{
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++)
    {
        dest[*pos + i] = (bytes)va_arg(args, int);
    }
    va_end(args);
    *pos = *pos + num_args + padding;
}

void set_BMP_header(bytes *buffer, size_t *pos)
{
    write_bytes(buffer, pos, 0, 2, 0x42, 0x4D); // ID field (BM)
    write_bytes(buffer, pos, 3, 1, 0x46);       // total header size (54 bytes)
    write_bytes(buffer, pos, 1, 1, 0x00);       // application specific
    write_bytes(buffer, pos, 1, 1, 0x00);       // application specific
    write_bytes(buffer, pos, 3, 1, 0x36);       // offset where the pixel array (bitmap data) can be found
}

void set_DIB_header(bytes *buffer, size_t *pos)
{
    write_bytes(buffer, pos, 3, 1, 0x28);       // remaining header size (40 bytes)
    write_bytes(buffer, pos, 3, 1, 0x03);       // width of the bitmap in pixels
    write_bytes(buffer, pos, 3, 1, 0x03);       // height of the bitmap in pixels
    write_bytes(buffer, pos, 1, 1, 0x01);       // number of color planes being used
    write_bytes(buffer, pos, 1, 1, 0x20);       // number of bits per pixel (32 bits)
    write_bytes(buffer, pos, 3, 1, 0x00);       // BI_RGB, no pixel array compression used
    write_bytes(buffer, pos, 3, 1, 0x10);       // size of the raw bitmap data (including padding) - 36 bytes
    write_bytes(buffer, pos, 2, 2, 0x13, 0x0B); // 2835 pixels/metre horizontal
    write_bytes(buffer, pos, 2, 2, 0x13, 0x0B); // 2835 pixels/metre vertical
    write_bytes(buffer, pos, 3, 1, 0x00);       // number of colors in the palette
    write_bytes(buffer, pos, 3, 1, 0x00);       // 0 means all colors are important
}

int main(void)
{
    FILE *fp;
    bytes *buffer;
    size_t pos = 0;
    size_t size = 90;

    fp = fopen("bitmap_example.bmp", "wb");

    buffer = (bytes *)malloc(sizeof(bytes) * size);

    memset(buffer, 0, size);

    set_BMP_header(buffer, &pos);

    set_DIB_header(buffer, &pos);

    // Start of pixel array (bitmap data) // LITTLE ENDIAN

    write_bytes(buffer, &pos, 0, 4, 0xFF, 0x00, 0x00, 0xFF); // Blue, Pixel (x=0, y=2) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0x00, 0xFF, 0x00, 0xFF); // Green, Pixel (x=1, y=2) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0x00, 0x00, 0xFF, 0xFF); // Red, Pixel (x=2, y=2) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0xFF, 0xFF, 0xFF, 0xFF); // White, Pixel (x=0, y=1) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0xFF, 0x00, 0x00, 0xFF); // Blue, Pixel (x=1, y=1) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0x00, 0xFF, 0x00, 0xFF); // Green, Pixel (x=2, y=1) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0x00, 0x00, 0xFF, 0xFF); // Red, Pixel (x=0, y=0) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0xFF, 0xFF, 0xFF, 0xFF); // White, Pixel (x=1, y=0) alpha 255
    write_bytes(buffer, &pos, 0, 4, 0x00, 0x00, 0x00, 0xFF); // Black, Pixel (x=2, y=0) alpha 255

    fwrite(buffer, size, 1, fp);

    free(buffer);
    fclose(fp);

    return 0;
}
