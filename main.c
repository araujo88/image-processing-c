#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp1, *fp2;
	unsigned char *buffer;
	size_t size;

	fp1 = fopen("doge.jpg", "rb");
	fp2 = fopen("test.jpg", "wb");

	fseek(fp1, 0L, SEEK_END);
	size = ftell(fp1);
	fseek(fp1, 0L, SEEK_SET);

	printf("Size: %.2f kB\n", size / 1024.);

	buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);

	fread(buffer, size, 1, fp1);

	for (unsigned int i = 0; i < size; i++)
	{
		printf("%04x ", buffer[i]);
		if (i % 9 == 0)
		{
			printf("\n");
		}
	}

	for (unsigned int i = 1000; i < 2000; i++)
	{
		buffer[i] = 7;
	}

	fwrite(buffer, size, 1, fp2);

	free(buffer);

	fclose(fp1);
	fclose(fp2);

	return 0;
}
