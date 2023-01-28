/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


void
libglitter_split_uint64_raster(uint16_t *rasters[3], uint16_t **alphap, uint64_t *raster,
                               uint64_t red, uint64_t green, uint64_t blue)
{
	uint64_t alpha, map;
	size_t position;
	unsigned char channel;

	alpha = ~(red | green | blue);
	red   /= 0xFF;
	green /= 0xFF;
	blue  /= 0xFF;
	alpha /= 0xFF;
	red   *= 0;
	green *= 1;
	blue  *= 2;
	alpha *= 3;
	map = red | green | blue | alpha;

	for (position = 0; position < 4; position++) {
		channel = ((const unsigned char *)&map)[position * sizeof(uint16_t)];
		if (channel < 3)
			rasters[channel] = &((uint16_t *)raster)[position];
		else if (alphap)
			*alphap = &((uint16_t *)raster)[position];
	}
}


#else


static void
check(int with_alpha, int r, int g, int b, int a)
{
	int red_shift    = r * (int)sizeof(uint16_t) * 8;
	int green_shift  = g * (int)sizeof(uint16_t) * 8;
	int blue_shift   = b * (int)sizeof(uint16_t) * 8;
	int alpha_shift  = a * (int)sizeof(uint16_t) * 8;
	uint16_t full  = (uint16_t)~0U;
	uint16_t red   = (uint16_t)~0U / (uint16_t)0xF * (uint16_t)0x5;
	uint16_t green = (uint16_t)~0U / (uint16_t)0xF * (uint16_t)0x8;
	uint16_t blue  = (uint16_t)~0U / (uint16_t)0xF * (uint16_t)0xA;
	uint16_t alpha = (uint16_t)~0U / (uint16_t)0xF * (uint16_t)0x9;
	uint64_t red_mask   = (uint64_t)red   << red_shift;
	uint64_t green_mask = (uint64_t)green << green_shift;
	uint64_t blue_mask  = (uint64_t)blue  << blue_shift;
	uint64_t alpha_mask = (uint64_t)alpha << alpha_shift;
	uint64_t raster = red_mask | green_mask | blue_mask | alpha_mask;
	uint16_t *rasters[4] = {NULL, NULL, NULL, NULL};

	ASSERT(r != g && r != b && r != a);
	ASSERT(g != b && g != a);
	ASSERT(b != a);

	libglitter_split_uint64_raster(rasters,
	                               with_alpha ? &rasters[3] : NULL,
	                               &raster,
	                               (uint64_t)full << red_shift,
	                               (uint64_t)full << green_shift,
	                               (uint64_t)full << blue_shift);
	ASSERT(rasters[0]);
	ASSERT(rasters[1]);
	ASSERT(rasters[2]);
	ASSERT(*rasters[0] == red);
	ASSERT(*rasters[1] == green);
	ASSERT(*rasters[2] == blue);
	if (with_alpha) {
		ASSERT(rasters[3]);
		ASSERT(*rasters[3] == alpha);
	} else {
		ASSERT(!rasters[3]);
	}
}

int
main(void)
{
	check(0, 0, 1, 2, 3);
	check(0, 0, 1, 3, 2);
	check(0, 0, 2, 1, 3);
	check(0, 0, 2, 3, 1);
	check(0, 0, 3, 1, 2);
	check(0, 0, 3, 2, 1);
	check(0, 1, 0, 2, 3);
	check(0, 1, 0, 3, 2);
	check(0, 1, 2, 0, 3);
	check(0, 1, 2, 3, 0);
	check(0, 1, 3, 0, 2);
	check(0, 1, 3, 2, 0);
	check(0, 2, 0, 1, 3);
	check(0, 2, 0, 3, 1);
	check(0, 2, 1, 0, 3);
	check(0, 2, 1, 3, 0);
	check(0, 2, 3, 0, 1);
	check(0, 2, 3, 1, 0);
	check(0, 3, 0, 1, 2);
	check(0, 3, 0, 2, 1);
	check(0, 3, 1, 0, 2);
	check(0, 3, 1, 2, 0);
	check(0, 3, 2, 0, 1);
	check(0, 3, 2, 1, 0);
	check(1, 0, 1, 2, 3);
	check(1, 0, 1, 3, 2);
	check(1, 0, 2, 1, 3);
	check(1, 0, 2, 3, 1);
	check(1, 0, 3, 1, 2);
	check(1, 0, 3, 2, 1);
	check(1, 1, 0, 2, 3);
	check(1, 1, 0, 3, 2);
	check(1, 1, 2, 0, 3);
	check(1, 1, 2, 3, 0);
	check(1, 1, 3, 0, 2);
	check(1, 1, 3, 2, 0);
	check(1, 2, 0, 1, 3);
	check(1, 2, 0, 3, 1);
	check(1, 2, 1, 0, 3);
	check(1, 2, 1, 3, 0);
	check(1, 2, 3, 0, 1);
	check(1, 2, 3, 1, 0);
	check(1, 3, 0, 1, 2);
	check(1, 3, 0, 2, 1);
	check(1, 3, 1, 0, 2);
	check(1, 3, 1, 2, 0);
	check(1, 3, 2, 0, 1);
	check(1, 3, 2, 1, 0);
	return 0;
}


#endif
