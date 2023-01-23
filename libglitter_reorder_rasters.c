/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


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


#else


int
main(void)
{
	const char r = 'R', g = 'G', b = 'B';
	const void *rasters[3];

#define T(CH1, CH2, CH3)\
	do {\
		rasters[0] = &r;\
		rasters[1] = &g;\
		rasters[2] = &b;\
		libglitter_reorder_rasters((void *)rasters,\
		                           LIBGLITTER_CHANNEL_##CH1,\
		                           LIBGLITTER_CHANNEL_##CH2,\
		                           LIBGLITTER_CHANNEL_##CH3);\
		ASSERT(*(const char *)rasters[0] == *#CH1);\
		ASSERT(*(const char *)rasters[1] == *#CH2);\
		ASSERT(*(const char *)rasters[2] == *#CH3);\
	} while (0)

	T(RED, GREEN, BLUE);
	T(RED, BLUE, GREEN);
	T(GREEN, RED, BLUE);
	T(GREEN, BLUE, RED);
	T(BLUE, RED, GREEN);
	T(BLUE, GREEN, RED);

	return 0;
}


#endif
