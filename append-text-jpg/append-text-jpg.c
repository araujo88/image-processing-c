#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp1, *fp2;
    unsigned char *buffer;
    size_t size;

    if (argc < 3)
    {
        printf("Usage:\n./append-text-jpg <filename> <text>\n");
        exit(EXIT_FAILURE);
    }

    fp1 = fopen(argv[1], "rb");
    fp2 = fopen("output.jpg", "wb");

    fseek(fp1, 0L, SEEK_END);
    size = ftell(fp1);
    fseek(fp1, 0L, SEEK_SET);
    buffer = (unsigned char *)malloc(sizeof(unsigned char) * (size + strlen(argv[2]) + 2));

    fread(buffer, size, 1, fp1);

    for (unsigned int i = size; i < size + strlen(argv[2]); i++)
    {
        buffer[i] = argv[2][i - size];
    }

    buffer[size - 2] = 0xFF;
    buffer[size - 1] = 0xFE;

    buffer[size + strlen(argv[2])] = 0xFF;
    buffer[size + strlen(argv[2]) + 1] = 0xD9;

    fwrite(buffer, size + strlen(argv[2]) + 2, 1, fp2);

    free(buffer);
    fclose(fp1);
    fclose(fp2);

    return 0;
}
