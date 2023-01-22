/* See LICENSE file for copyright and license details. */
#include "common.h"


void
libglitter_split_uint64_raster(uint16_t *rasters[3], uint16_t **alphap, uint64_t *raster,
                               uint64_t red, uint64_t green, uint64_t blue)
{
	uint64_t alpha, map;
	size_t position;
	size_t channel;

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
		channel = ((const unsigned char *)&map)[position * 2];
		if (channel < 3)
			rasters[channel] = &((uint16_t *)raster)[position];
		else if (alphap)
			*alphap = &((uint16_t *)raster)[position];
	}
}
