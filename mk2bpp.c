#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
	TILE_SIZE = 8,
	TILE_DATA_SIZE = 2 * TILE_SIZE * TILE_SIZE / CHAR_BIT,
};

struct tile {
	struct tile * next;
	unsigned char data[TILE_DATA_SIZE];
};

static int
is_present(struct tile const * restrict tileset,
           struct tile const * const restrict tile)
{
	if (!tile) { return 0; }
	for (; tileset; tileset = tileset->next)
	{
		if (!memcmp(tileset->data, tile->data, TILE_DATA_SIZE))
		{
			return 1;
		}
	}
	return 0;
}

static struct tile *
mktile(unsigned char * data, struct tile * tileset,
       int w, int h, int x, int y, int cull)
{
	struct tile tile;
	struct tile * out = NULL;
	int dark;
	int light;
	int i;
	int j;
	int mi = h - y < TILE_SIZE ? h - y : TILE_SIZE;
	int mj = w - x < TILE_SIZE ? w - x : TILE_SIZE;
	int red;
	int green;
	int blue;
	int v;
	tile.next = NULL;
	for (i = 0; i < TILE_DATA_SIZE; ++i)
	{
		tile.data[i] = 0;
	}
	for (i = 0; i < mi; ++i)
	{
		for (j = 0; j < mj; ++j)
		{
			v = 0;
			red = data[4 * (i * w + j)];
			green = data[4 * (i * w + j) + 1];
			blue = data[4 * (i * w + j) + 2];
			v = (212*red + 701*green + 87*blue) / 64000;
			if (data[4 * (i * w + j) + 3] < 128) { v = 0; }
			dark  = (dark <<1) | !!(v & 1);
			light = (light<<1) | !!(v & 2);
		}
		for (; j < TILE_SIZE; ++j)
		{
			dark <<= 1;
			light <<= 1;
		}
		for (j = 0; j < (TILE_SIZE + CHAR_BIT - 1)/CHAR_BIT; ++j)
		{
			tile.data[i + j] = dark;
			tile.data[i + TILE_DATA_SIZE/2 + j] = light;
			dark >>= CHAR_BIT;
			light >>= CHAR_BIT;
		}
	}
	if (!cull || !is_present(tileset, &tile))
	{
		out = malloc(sizeof(*out));
		if (!out) { return NULL; }
		out->next = NULL;
		memcpy(out->data, tile.data, TILE_DATA_SIZE);
	}
	return out;
}

int
main(int argc, char ** argv)
{
	struct tile * tileset = NULL;
	struct tile * p;
	struct tile ** tp = &tileset;
	unsigned char * data;
	int a;
	int b;
	int c;
	int i;
	int j;
	int n;
	int x;
	int y;
	for (i = 1; i < argc; ++i)
	{
		if (!strcmp("-C",argv[i])) { c = 0; continue; }
		if (!strcmp("-c",argv[i])) { c = 1; continue; }
		data = stbi_load(argv[i], &x, &y, &n, 4);
		if (!data) { continue; }
		for (a = 0; a < (y+TILE_SIZE-1)/TILE_SIZE; ++a)
		{
			for (b = 0; b < (x+TILE_SIZE-1)/TILE_SIZE; ++b)
			{
				j = 4 * (a*x*TILE_SIZE + b*TILE_SIZE);
				*tp = mktile(data + j,tileset,x,y,b,a,c);
				if (*tp) { tp = &((*tp)->next); }
			}
		}
		stbi_image_free(data);
	}
	while (tileset)
	{
		fwrite(tileset->data, 1, TILE_DATA_SIZE, stdout);
		p = tileset->next;
		free(tileset);
		tileset = p;
	}
	return 0;
}
