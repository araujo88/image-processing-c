#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned char *buffer;
	size_t size;

	fp = fopen(argv[1], "rb");

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	printf("Size: %.2f kB\n", size / 1024.);

	buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);

	fread(buffer, size, 1, fp);

	for (unsigned int i = 0; i < size; i++)
	{
		printf("%04x ", buffer[i]);
		if (i % 9 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");

	free(buffer);
	fclose(fp);

	return 0;
}
