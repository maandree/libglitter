/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic ignored "-Wunsuffixed-float-constants"
#endif


void
libglitter_desaturate_double(double **rasters, size_t nrasters, size_t rowsize, size_t cellsize,
                             size_t width, size_t height, double saturation, const double *restrict primary_ys)
{
	size_t y, x, ch, raster_y, raster_i;
	double intensity;

	rowsize *= cellsize;

	if (nrasters == 3) {
		double *raster1 = rasters[0];
		double *raster2 = rasters[1];
		double *raster3 = rasters[2];
		double primary1_y = primary_ys ? primary_ys[0] : (double)0.212673370378408277403536885686;
		double primary2_y = primary_ys ? primary_ys[1] : (double)0.715151730491031756287156895269;
		double primary3_y = primary_ys ? primary_ys[2] : (double)0.072174899130559869164791564344;
		for (y = 0, raster_y = 0; y < height; y++, raster_y += rowsize) {
			for (x = 0, raster_i = raster_y; x < width; x++, raster_i += cellsize) {
				intensity  = raster1[raster_i] * primary1_y;
				intensity += raster2[raster_i] * primary2_y;
				intensity += raster3[raster_i] * primary3_y;
				raster1[raster_i] = fma(raster1[raster_i] - intensity, saturation, intensity);
				raster2[raster_i] = fma(raster2[raster_i] - intensity, saturation, intensity);
				raster3[raster_i] = fma(raster3[raster_i] - intensity, saturation, intensity);
			}
		}

	} else {
		for (y = 0, raster_y = 0; y < height; y++, raster_y += rowsize) {
			for (x = 0, raster_i = raster_y; x < width; x++, raster_i += cellsize) {
				intensity = 0;
				for (ch = 0; ch < nrasters; ch++)
					intensity += rasters[ch][raster_i] * primary_ys[ch];
				for (ch = 0; ch < nrasters; ch++)
					rasters[ch][raster_i] = fma(rasters[ch][raster_i] - intensity, saturation, intensity);
			}
		}
	}
}


#else


int
main(void)
{
	double rasters1[4 * 10 * 3][4], *rasters1list[4], primary_ys[4], saturation;
	double rasters2[4 * 10 * 3][4], *rasters2list[4], saturations[4];
	size_t i, j;

	void *randptr = malloc(1);
	srand((unsigned int)(uintptr_t)randptr);
	free(randptr);

#define RANDOMISE(SAT)\
	do {\
		saturation = (SAT);\
		saturations[0] = saturation;\
		saturations[1] = saturation;\
		saturations[2] = saturation;\
		saturations[3] = saturation;\
		for (i = 0; i < 4; i++) {\
			rasters1list[i] = rasters1[i];\
			rasters2list[i] = rasters2[i];\
			primary_ys[i] = rand() / (double)RAND_MAX;\
			for (j = 0; j < 4 * 10 * 3; j++)\
				rasters1[j][i] = rand() / (double)RAND_MAX;\
		}\
		memcpy(rasters2, rasters1, sizeof(rasters1));\
	} while (0)

	RANDOMISE(rand() / (double)RAND_MAX);
	libglitter_desaturate_double(rasters1list, 4, 10, 3, 7, 4, saturation, primary_ys);
	libglitter_per_channel_desaturate_double(rasters2list, 4, 10, 3, 7, 4, saturations, primary_ys);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(rand() / (double)RAND_MAX);
	libglitter_desaturate_double(rasters1list, 3, 10, 3, 7, 4, saturation, primary_ys);
	libglitter_per_channel_desaturate_double(rasters2list, 3, 10, 3, 7, 4, saturations, primary_ys);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(rand() / (double)RAND_MAX);
	libglitter_desaturate_double(rasters1list, 3, 10, 3, 7, 4, saturation, NULL);
	libglitter_per_channel_desaturate_double(rasters2list, 3, 10, 3, 7, 4, saturations, NULL);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(0);
	libglitter_desaturate_double(rasters1list, 4, 10, 3, 7, 4, saturation, primary_ys);
	libglitter_per_channel_desaturate_double(rasters2list, 4, 10, 3, 7, 4, saturations, primary_ys);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(0);
	libglitter_desaturate_double(rasters1list, 3, 10, 3, 7, 4, saturation, primary_ys);
	libglitter_per_channel_desaturate_double(rasters2list, 3, 10, 3, 7, 4, saturations, primary_ys);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(0);
	libglitter_desaturate_double(rasters1list, 3, 10, 3, 7, 4, saturation, NULL);
	libglitter_per_channel_desaturate_double(rasters2list, 3, 10, 3, 7, 4, saturations, NULL);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(1);
	libglitter_desaturate_double(rasters1list, 4, 10, 3, 7, 4, saturation, primary_ys);
	libglitter_per_channel_desaturate_double(rasters2list, 4, 10, 3, 7, 4, saturations, primary_ys);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(1);
	libglitter_desaturate_double(rasters1list, 3, 10, 3, 7, 4, saturation, primary_ys);
	libglitter_per_channel_desaturate_double(rasters2list, 3, 10, 3, 7, 4, saturations, primary_ys);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	RANDOMISE(1);
	libglitter_desaturate_double(rasters1list, 3, 10, 3, 7, 4, saturation, NULL);
	libglitter_per_channel_desaturate_double(rasters2list, 3, 10, 3, 7, 4, saturations, NULL);
	ASSERT(!memcmp(rasters1, rasters2, sizeof(rasters1)));

	return 0;
}


#endif
