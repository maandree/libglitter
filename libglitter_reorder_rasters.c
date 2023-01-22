/* See LICENSE file for copyright and license details. */
#include "common.h"


void
libglitter_reorder_rasters(void **rasters, enum libglitter_colour colour1,
                           enum libglitter_colour colour2, enum libglitter_colour colour3)
{
	void *channel1 = rasters[colour1];
	void *channel2 = rasters[colour2];
	void *channel3 = rasters[colour3];
	rasters[0] = channel1;
	rasters[1] = channel2;
	rasters[2] = channel3;
}
