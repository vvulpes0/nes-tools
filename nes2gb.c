#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	size_t capacity = 1024;
	unsigned char *buf = malloc(capacity);
	unsigned char *tmp = NULL;
	size_t size = 0;
	size_t i = 0;
	size_t j = 0;
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
	tmp = buf;
	for (i = 0; i < ntiles; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			printf("%c%c",~tmp[j],~tmp[8+j]);
		}
		tmp += 16;
	}
	free(buf);
	return 0;
}
