#include <stdio.h>
#include <stdlib.h>

static void
output_partial_tile(unsigned char *buf, size_t size)
{
	unsigned char a = size>0 ? buf[0] : 0;
	unsigned char b = size>8 ? buf[8] : 0;
	unsigned int i;
	for (i = 0x80U; i; i >>= 1)
	{
		fputc((!!(b&i))<<1 | !!(a&i), stdout);
	}
}

static void
output_pixel_row(unsigned char *buf, size_t size)
{
	size_t i;
	for (i = 0; i < 16; ++i)
	{
		output_partial_tile(buf, size);
		buf += 16;
		size = size<16 ? 0 : size-16;
	}
}

static void
output_tile_row(unsigned char *buf, size_t size)
{
	size_t i;
	for (i = 0; i < 8; ++i)
	{
		output_pixel_row(buf, size);
		++buf;
		size = size<1 ? 0 : size-1;
	}
}

int
main(int argc, char **argv)
{
	size_t capacity = 1024;
	unsigned char *buf = malloc(capacity);
	unsigned char *tmp = NULL;
	size_t size = 0;
	size_t i = 0;
	size_t ntiles = 0;
	if (!buf) { exit(EXIT_FAILURE); }
	while (!feof(stdin)) {
		if (size == capacity) {
			capacity *= 2;
			tmp = realloc(buf, capacity);
			if (!tmp) { free(buf); exit(EXIT_FAILURE); }
			buf = tmp;
			tmp = NULL;
		}
		i = fread(buf+size, 1, capacity - size, stdin);
		size += i;
	}
	ntiles = (size+15)/16;
	printf("P5\n%u %lu\n3\n",128U,((ntiles+15)/16)*8);
	for (i = 0; i < (ntiles+15)/16; ++i)
	{
		output_tile_row(buf+256*i, size < 256*i ? 0 : size-256*i);
	}
	free(buf);
	return EXIT_SUCCESS;
}
