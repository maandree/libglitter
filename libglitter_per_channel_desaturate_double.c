/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


void
libglitter_per_channel_desaturate_double(double *restrict *rasters, size_t nrasters, size_t rowsize, size_t cellsize,
                                         size_t width, size_t height, const double *restrict saturations,
                                         const double *restrict primary_ys)
{
	size_t y, x, ch, raster_y, raster_i;
	double intensity;

	rowsize *= cellsize;

	if (nrasters == 3) {
		double *raster1 = rasters[0], saturation1 = saturations[0];
		double *raster2 = rasters[1], saturation2 = saturations[1];
		double *raster3 = rasters[2], saturation3 = saturations[2];
		double primary1_y = primary_ys ? primary_ys[0] : (double)0.212673370378408277403536885686;
		double primary2_y = primary_ys ? primary_ys[1] : (double)0.715151730491031756287156895269;
		double primary3_y = primary_ys ? primary_ys[2] : (double)0.072174899130559869164791564344;
		for (y = 0, raster_y = 0; y < height; y++, raster_y += rowsize) {
			for (x = 0, raster_i = raster_y; x < width; x++, raster_i += cellsize) {
				intensity  = raster1[raster_i] * primary1_y;
				intensity += raster2[raster_i] * primary2_y;
				intensity += raster3[raster_i] * primary3_y;
				raster1[raster_i] = fma(raster1[raster_i] - intensity, saturation1, intensity);
				raster2[raster_i] = fma(raster2[raster_i] - intensity, saturation2, intensity);
				raster3[raster_i] = fma(raster3[raster_i] - intensity, saturation3, intensity);
			}
		}

	} else {
		for (y = 0, raster_y = 0; y < height; y++, raster_y += rowsize) {
			for (x = 0, raster_i = raster_y; x < width; x++, raster_i += cellsize) {
				intensity = 0;
				for (ch = 0; ch < nrasters; ch++)
					intensity += rasters[ch][raster_i] * primary_ys[ch];
				for (ch = 0; ch < nrasters; ch++)
					rasters[ch][raster_i] = fma(rasters[ch][raster_i] - intensity, saturations[ch], intensity);
			}
		}
	}
}


#else


#define TOLERANCE 0.0001

#define RY 0.212673370378408277403536885686
#define GY 0.715151730491031756287156895269
#define BY 0.072174899130559869164791564344


static int
eq(double a, double b)
{
	double r = a - b;
	return (r < 0 ? -r : r) < TOLERANCE;
}

int
main(void)
{
	double zeroes[2 * 6 * 3];
	double raster1[2 * 6 * 3];
	double raster2[2 * 6 * 3];
	double raster3[2 * 6 * 3];
	double *rasters[3], saturations[3], primary_ys[3];
	size_t i;

#define POS(Y, X) ((Y) * 6 * 3 + (X) * 3)
#define ASSERT2(A, B) do { ASSERT(A); ASSERT(B); } while (0)
#define ASSERT3(A, B, C) do { ASSERT(A); ASSERT(B); ASSERT(C); } while (0)

	memset(zeroes, 0, sizeof(zeroes));

	for (i = 0; i <= 2; i++) {
		double v = (double)i / 2;

		memset(raster1, 0, sizeof(raster1));
		memset(raster2, 0, sizeof(raster2));
		memset(raster3, 0, sizeof(raster3));
		raster1[POS(0, 0)] = v, raster2[POS(0, 0)] = v;
		raster1[POS(0, 1)] = v, raster2[POS(0, 1)] = v;
		raster1[POS(0, 2)] = v, raster2[POS(0, 2)] = v;
		raster1[POS(0, 3)] = v, raster2[POS(0, 3)] = v;
		raster1[POS(1, 0)] = v, raster2[POS(1, 0)] = v;
		raster1[POS(1, 1)] = v, raster2[POS(1, 1)] = v;
		raster1[POS(1, 2)] = v, raster2[POS(1, 2)] = v;
		raster1[POS(1, 3)] = v, raster2[POS(1, 3)] = v;
		rasters[0] = raster1;
		rasters[1] = raster2;
		rasters[2] = raster3;
		saturations[0] = 0.5;
		saturations[1] = 0.25;
		primary_ys[0] = 0.75;
		primary_ys[1] = 0.25;
		libglitter_per_channel_desaturate_double(rasters, 2, 6, 3, 4, 2, saturations, primary_ys);
		ASSERT2(raster1[POS(0, 0)] == v, raster2[POS(0, 0)] == v);
		ASSERT2(raster1[POS(0, 1)] == v, raster2[POS(0, 1)] == v);
		ASSERT2(raster1[POS(0, 2)] == v, raster2[POS(0, 2)] == v);
		ASSERT2(raster1[POS(0, 3)] == v, raster2[POS(0, 3)] == v);
		ASSERT2(raster1[POS(1, 0)] == v, raster2[POS(1, 0)] == v);
		ASSERT2(raster1[POS(1, 1)] == v, raster2[POS(1, 1)] == v);
		ASSERT2(raster1[POS(1, 2)] == v, raster2[POS(1, 2)] == v);
		ASSERT2(raster1[POS(1, 3)] == v, raster2[POS(1, 3)] == v);
		raster1[POS(0, 0)] = 0, raster2[POS(0, 0)] = 0;
		raster1[POS(0, 1)] = 0, raster2[POS(0, 1)] = 0;
		raster1[POS(0, 2)] = 0, raster2[POS(0, 2)] = 0;
		raster1[POS(0, 3)] = 0, raster2[POS(0, 3)] = 0;
		raster1[POS(1, 0)] = 0, raster2[POS(1, 0)] = 0;
		raster1[POS(1, 1)] = 0, raster2[POS(1, 1)] = 0;
		raster1[POS(1, 2)] = 0, raster2[POS(1, 2)] = 0;
		raster1[POS(1, 3)] = 0, raster2[POS(1, 3)] = 0;
		ASSERT(!memcmp(raster1, zeroes, sizeof(raster1)));
		ASSERT(!memcmp(raster2, zeroes, sizeof(raster2)));
		ASSERT(!memcmp(raster3, zeroes, sizeof(raster3)));

		memset(raster1, 0, sizeof(raster1));
		memset(raster2, 0, sizeof(raster2));
		memset(raster3, 0, sizeof(raster3));
		raster1[POS(0, 0)] = v, raster2[POS(0, 0)] = v, raster3[POS(0, 0)] = v;
		raster1[POS(0, 1)] = v, raster2[POS(0, 1)] = v, raster3[POS(0, 1)] = v;
		raster1[POS(0, 2)] = v, raster2[POS(0, 2)] = v, raster3[POS(0, 2)] = v;
		raster1[POS(0, 3)] = v, raster2[POS(0, 3)] = v, raster3[POS(0, 3)] = v;
		raster1[POS(1, 0)] = v, raster2[POS(1, 0)] = v, raster3[POS(1, 0)] = v;
		raster1[POS(1, 1)] = v, raster2[POS(1, 1)] = v, raster3[POS(1, 1)] = v;
		raster1[POS(1, 2)] = v, raster2[POS(1, 2)] = v, raster3[POS(1, 2)] = v;
		raster1[POS(1, 3)] = v, raster2[POS(1, 3)] = v, raster3[POS(1, 3)] = v;
		rasters[0] = raster1;
		rasters[1] = raster2;
		rasters[2] = raster3;
		saturations[0] = 0.5;
		saturations[1] = 0.25;
		saturations[2] = 0.125;
		primary_ys[0] = 0.375;
		primary_ys[1] = 0.50;
		primary_ys[2] = 0.125;
		libglitter_per_channel_desaturate_double(rasters, 3, 6, 3, 4, 2, saturations, primary_ys);
		ASSERT3(raster1[POS(0, 0)] == v, raster2[POS(0, 0)] == v, raster3[POS(0, 0)] == v);
		ASSERT3(raster1[POS(0, 1)] == v, raster2[POS(0, 1)] == v, raster3[POS(0, 1)] == v);
		ASSERT3(raster1[POS(0, 2)] == v, raster2[POS(0, 2)] == v, raster3[POS(0, 2)] == v);
		ASSERT3(raster1[POS(0, 3)] == v, raster2[POS(0, 3)] == v, raster3[POS(0, 3)] == v);
		ASSERT3(raster1[POS(1, 0)] == v, raster2[POS(1, 0)] == v, raster3[POS(1, 0)] == v);
		ASSERT3(raster1[POS(1, 1)] == v, raster2[POS(1, 1)] == v, raster3[POS(1, 1)] == v);
		ASSERT3(raster1[POS(1, 2)] == v, raster2[POS(1, 2)] == v, raster3[POS(1, 2)] == v);
		ASSERT3(raster1[POS(1, 3)] == v, raster2[POS(1, 3)] == v, raster3[POS(1, 3)] == v);
		raster1[POS(0, 0)] = 0, raster2[POS(0, 0)] = 0, raster3[POS(0, 0)] = 0;
		raster1[POS(0, 1)] = 0, raster2[POS(0, 1)] = 0, raster3[POS(0, 1)] = 0;
		raster1[POS(0, 2)] = 0, raster2[POS(0, 2)] = 0, raster3[POS(0, 2)] = 0;
		raster1[POS(0, 3)] = 0, raster2[POS(0, 3)] = 0, raster3[POS(0, 3)] = 0;
		raster1[POS(1, 0)] = 0, raster2[POS(1, 0)] = 0, raster3[POS(1, 0)] = 0;
		raster1[POS(1, 1)] = 0, raster2[POS(1, 1)] = 0, raster3[POS(1, 1)] = 0;
		raster1[POS(1, 2)] = 0, raster2[POS(1, 2)] = 0, raster3[POS(1, 2)] = 0;
		raster1[POS(1, 3)] = 0, raster2[POS(1, 3)] = 0, raster3[POS(1, 3)] = 0;
		ASSERT(!memcmp(raster1, zeroes, sizeof(raster1)));
		ASSERT(!memcmp(raster2, zeroes, sizeof(raster2)));
		ASSERT(!memcmp(raster3, zeroes, sizeof(raster3)));

		memset(raster1, 0, sizeof(raster1));
		memset(raster2, 0, sizeof(raster2));
		memset(raster3, 0, sizeof(raster3));
		raster1[POS(0, 0)] = v, raster2[POS(0, 0)] = v, raster3[POS(0, 0)] = v;
		raster1[POS(0, 1)] = v, raster2[POS(0, 1)] = v, raster3[POS(0, 1)] = v;
		raster1[POS(0, 2)] = v, raster2[POS(0, 2)] = v, raster3[POS(0, 2)] = v;
		raster1[POS(0, 3)] = v, raster2[POS(0, 3)] = v, raster3[POS(0, 3)] = v;
		raster1[POS(1, 0)] = v, raster2[POS(1, 0)] = v, raster3[POS(1, 0)] = v;
		raster1[POS(1, 1)] = v, raster2[POS(1, 1)] = v, raster3[POS(1, 1)] = v;
		raster1[POS(1, 2)] = v, raster2[POS(1, 2)] = v, raster3[POS(1, 2)] = v;
		raster1[POS(1, 3)] = v, raster2[POS(1, 3)] = v, raster3[POS(1, 3)] = v;
		rasters[0] = raster1;
		rasters[1] = raster2;
		rasters[2] = raster3;
		saturations[0] = 0.5;
		saturations[1] = 0.25;
		saturations[2] = 0.125;
		primary_ys[0] = 0.375;
		primary_ys[1] = 0.5;
		primary_ys[2] = 0.125;
		libglitter_per_channel_desaturate_double(rasters, 3, 6, 3, 4, 2, saturations, NULL);
		ASSERT3(eq(raster1[POS(0, 0)], v), eq(raster2[POS(0, 0)], v), eq(raster3[POS(0, 0)], v));
		ASSERT3(eq(raster1[POS(0, 1)], v), eq(raster2[POS(0, 1)], v), eq(raster3[POS(0, 1)], v));
		ASSERT3(eq(raster1[POS(0, 2)], v), eq(raster2[POS(0, 2)], v), eq(raster3[POS(0, 2)], v));
		ASSERT3(eq(raster1[POS(0, 3)], v), eq(raster2[POS(0, 3)], v), eq(raster3[POS(0, 3)], v));
		ASSERT3(eq(raster1[POS(1, 0)], v), eq(raster2[POS(1, 0)], v), eq(raster3[POS(1, 0)], v));
		ASSERT3(eq(raster1[POS(1, 1)], v), eq(raster2[POS(1, 1)], v), eq(raster3[POS(1, 1)], v));
		ASSERT3(eq(raster1[POS(1, 2)], v), eq(raster2[POS(1, 2)], v), eq(raster3[POS(1, 2)], v));
		ASSERT3(eq(raster1[POS(1, 3)], v), eq(raster2[POS(1, 3)], v), eq(raster3[POS(1, 3)], v));
		raster1[POS(0, 0)] = 0, raster2[POS(0, 0)] = 0, raster3[POS(0, 0)] = 0;
		raster1[POS(0, 1)] = 0, raster2[POS(0, 1)] = 0, raster3[POS(0, 1)] = 0;
		raster1[POS(0, 2)] = 0, raster2[POS(0, 2)] = 0, raster3[POS(0, 2)] = 0;
		raster1[POS(0, 3)] = 0, raster2[POS(0, 3)] = 0, raster3[POS(0, 3)] = 0;
		raster1[POS(1, 0)] = 0, raster2[POS(1, 0)] = 0, raster3[POS(1, 0)] = 0;
		raster1[POS(1, 1)] = 0, raster2[POS(1, 1)] = 0, raster3[POS(1, 1)] = 0;
		raster1[POS(1, 2)] = 0, raster2[POS(1, 2)] = 0, raster3[POS(1, 2)] = 0;
		raster1[POS(1, 3)] = 0, raster2[POS(1, 3)] = 0, raster3[POS(1, 3)] = 0;
		ASSERT(!memcmp(raster1, zeroes, sizeof(raster1)));
		ASSERT(!memcmp(raster2, zeroes, sizeof(raster2)));
		ASSERT(!memcmp(raster3, zeroes, sizeof(raster3)));
	}

	memset(raster1, 0, sizeof(raster1));
	memset(raster2, 0, sizeof(raster2));
	memset(raster3, 0, sizeof(raster3));
	raster1[POS(0, 0)] =  4, raster2[POS(0, 0)] =   8;
	raster1[POS(0, 1)] = 16, raster2[POS(0, 1)] =   2;
	raster1[POS(0, 2)] =  1, raster2[POS(0, 2)] =   8;
	raster1[POS(0, 3)] = 32, raster2[POS(0, 3)] =   4;
	raster1[POS(1, 0)] =  8, raster2[POS(1, 0)] =   8;
	raster1[POS(1, 1)] = -4, raster2[POS(1, 1)] =   2;
	raster1[POS(1, 2)] = 16, raster2[POS(1, 2)] = -16;
	raster1[POS(1, 3)] =  8, raster2[POS(1, 3)] =   2;
	rasters[0] = raster1;
	rasters[1] = raster2;
	rasters[2] = raster3;
	saturations[0] = 0.5;
	saturations[1] = 0.25;
	primary_ys[0] = 0.75;
	primary_ys[1] = 0.25;
	libglitter_per_channel_desaturate_double(rasters, 2, 6, 3, 4, 2, saturations, primary_ys);
#define G(x, y) ((x) * 0.75 + (y) * 0.25)
#define X(x, y) (((x) - G(x, y)) * 0.5  + G(x, y))
#define Y(x, y) (((y) - G(x, y)) * 0.25 + G(x, y))
	ASSERT2(raster1[POS(0, 0)] == X(4, 8),    raster2[POS(0, 0)] == Y(4, 8));
	ASSERT2(raster1[POS(0, 1)] == X(16, 2),   raster2[POS(0, 1)] == Y(16, 2));
	ASSERT2(raster1[POS(0, 2)] == X(1, 8),    raster2[POS(0, 2)] == Y(1, 8));
	ASSERT2(raster1[POS(0, 3)] == X(32, 4),   raster2[POS(0, 3)] == Y(32, 4));
	ASSERT2(raster1[POS(1, 0)] == X(8, 8),    raster2[POS(1, 0)] == Y(8, 8));
	ASSERT2(raster1[POS(1, 1)] == X(-4, 2),   raster2[POS(1, 1)] == Y(-4, 2));
	ASSERT2(raster1[POS(1, 2)] == X(16, -16), raster2[POS(1, 2)] == Y(16, -16));
	ASSERT2(raster1[POS(1, 3)] == X(8, 2),    raster2[POS(1, 3)] == Y(8, 2));
#undef G
#undef X
#undef Y
	raster1[POS(0, 0)] = 0, raster2[POS(0, 0)] = 0;
	raster1[POS(0, 1)] = 0, raster2[POS(0, 1)] = 0;
	raster1[POS(0, 2)] = 0, raster2[POS(0, 2)] = 0;
	raster1[POS(0, 3)] = 0, raster2[POS(0, 3)] = 0;
	raster1[POS(1, 0)] = 0, raster2[POS(1, 0)] = 0;
	raster1[POS(1, 1)] = 0, raster2[POS(1, 1)] = 0;
	raster1[POS(1, 2)] = 0, raster2[POS(1, 2)] = 0;
	raster1[POS(1, 3)] = 0, raster2[POS(1, 3)] = 0;
	ASSERT(!memcmp(raster1, zeroes, sizeof(raster1)));
	ASSERT(!memcmp(raster2, zeroes, sizeof(raster2)));
	ASSERT(!memcmp(raster3, zeroes, sizeof(raster3)));

	memset(raster1, 0, sizeof(raster1));
	memset(raster2, 0, sizeof(raster2));
	memset(raster3, 0, sizeof(raster3));
	raster1[POS(0, 0)] =  4, raster2[POS(0, 0)] =   8, raster3[POS(0, 0)] =   4;
	raster1[POS(0, 1)] = 16, raster2[POS(0, 1)] =   2, raster3[POS(0, 1)] =  16;
	raster1[POS(0, 2)] =  1, raster2[POS(0, 2)] =   8, raster3[POS(0, 2)] =   2;
	raster1[POS(0, 3)] = 32, raster2[POS(0, 3)] =   4, raster3[POS(0, 3)] =   8;
	raster1[POS(1, 0)] =  8, raster2[POS(1, 0)] =   8, raster3[POS(1, 0)] =   8;
	raster1[POS(1, 1)] = -4, raster2[POS(1, 1)] =   2, raster3[POS(1, 1)] =  16;
	raster1[POS(1, 2)] = 16, raster2[POS(1, 2)] = -16, raster3[POS(1, 2)] = -16;
	raster1[POS(1, 3)] =  8, raster2[POS(1, 3)] =   2, raster3[POS(1, 3)] =   4;
	rasters[0] = raster1;
	rasters[1] = raster2;
	rasters[2] = raster3;
	saturations[0] = 0.5;
	saturations[1] = 0.25;
	saturations[2] = 0.125;
	primary_ys[0] = 0.375;
	primary_ys[1] = 0.5;
	primary_ys[2] = 0.125;
	libglitter_per_channel_desaturate_double(rasters, 3, 6, 3, 4, 2, saturations, primary_ys);
#define G(x, y, z) ((x) * 0.375 + (y) * 0.5 + (z) * 0.125)
#define X(x, y, z) (((x) - G(x, y, z)) * 0.5   + G(x, y, z))
#define Y(x, y, z) (((y) - G(x, y, z)) * 0.25  + G(x, y, z))
#define Z(x, y, z) (((z) - G(x, y, z)) * 0.125 + G(x, y, z))
	ASSERT3(raster1[POS(0, 0)] == X(4, 8, 4),      raster2[POS(0, 0)] == Y(4, 8, 4),      raster3[POS(0, 0)] == Z(4, 8, 4));
	ASSERT3(raster1[POS(0, 1)] == X(16, 2, 16),    raster2[POS(0, 1)] == Y(16, 2, 16),    raster3[POS(0, 1)] == Z(16, 2, 16));
	ASSERT3(raster1[POS(0, 2)] == X(1, 8, 2),      raster2[POS(0, 2)] == Y(1, 8, 2),      raster3[POS(0, 2)] == Z(1, 8, 2));
	ASSERT3(raster1[POS(0, 3)] == X(32, 4, 8),     raster2[POS(0, 3)] == Y(32, 4, 8),     raster3[POS(0, 3)] == Z(32, 4, 8));
	ASSERT3(raster1[POS(1, 0)] == X(8, 8, 8),      raster2[POS(1, 0)] == Y(8, 8, 8),      raster3[POS(1, 0)] == Z(8, 8, 8));
	ASSERT3(raster1[POS(1, 1)] == X(-4, 2, 16),    raster2[POS(1, 1)] == Y(-4, 2, 16),    raster3[POS(1, 1)] == Z(-4, 2, 16));
	ASSERT3(raster1[POS(1, 2)] == X(16, -16, -16), raster2[POS(1, 2)] == Y(16, -16, -16), raster3[POS(1, 2)] == Z(16, -16, -16));
	ASSERT3(raster1[POS(1, 3)] == X(8, 2, 4),      raster2[POS(1, 3)] == Y(8, 2, 4),      raster3[POS(1, 3)] == Z(8, 2, 4));
#undef G
#undef X
#undef Y
#undef Z
	raster1[POS(0, 0)] = 0, raster2[POS(0, 0)] = 0, raster3[POS(0, 0)] = 0;
	raster1[POS(0, 1)] = 0, raster2[POS(0, 1)] = 0, raster3[POS(0, 1)] = 0;
	raster1[POS(0, 2)] = 0, raster2[POS(0, 2)] = 0, raster3[POS(0, 2)] = 0;
	raster1[POS(0, 3)] = 0, raster2[POS(0, 3)] = 0, raster3[POS(0, 3)] = 0;
	raster1[POS(1, 0)] = 0, raster2[POS(1, 0)] = 0, raster3[POS(1, 0)] = 0;
	raster1[POS(1, 1)] = 0, raster2[POS(1, 1)] = 0, raster3[POS(1, 1)] = 0;
	raster1[POS(1, 2)] = 0, raster2[POS(1, 2)] = 0, raster3[POS(1, 2)] = 0;
	raster1[POS(1, 3)] = 0, raster2[POS(1, 3)] = 0, raster3[POS(1, 3)] = 0;
	ASSERT(!memcmp(raster1, zeroes, sizeof(raster1)));
	ASSERT(!memcmp(raster2, zeroes, sizeof(raster2)));
	ASSERT(!memcmp(raster3, zeroes, sizeof(raster3)));

	memset(raster1, 0, sizeof(raster1));
	memset(raster2, 0, sizeof(raster2));
	memset(raster3, 0, sizeof(raster3));
	raster1[POS(0, 0)] =  4, raster2[POS(0, 0)] =   8, raster3[POS(0, 0)] =   4;
	raster1[POS(0, 1)] = 16, raster2[POS(0, 1)] =   2, raster3[POS(0, 1)] =  16;
	raster1[POS(0, 2)] =  1, raster2[POS(0, 2)] =   8, raster3[POS(0, 2)] =   2;
	raster1[POS(0, 3)] = 32, raster2[POS(0, 3)] =   4, raster3[POS(0, 3)] =   8;
	raster1[POS(1, 0)] =  8, raster2[POS(1, 0)] =   8, raster3[POS(1, 0)] =   8;
	raster1[POS(1, 1)] = -4, raster2[POS(1, 1)] =   2, raster3[POS(1, 1)] =  16;
	raster1[POS(1, 2)] = 16, raster2[POS(1, 2)] = -16, raster3[POS(1, 2)] = -16;
	raster1[POS(1, 3)] =  8, raster2[POS(1, 3)] =   2, raster3[POS(1, 3)] =   4;
	rasters[0] = raster1;
	rasters[1] = raster2;
	rasters[2] = raster3;
	saturations[0] = 0.5;
	saturations[1] = 0.25;
	saturations[2] = 0.125;
	libglitter_per_channel_desaturate_double(rasters, 3, 6, 3, 4, 2, saturations, NULL);
#define G(x, y, z) ((x) * RY + (y) * GY + (z) * BY)
#define X(x, y, z) (((x) - G(x, y, z)) * 0.5   + G(x, y, z))
#define Y(x, y, z) (((y) - G(x, y, z)) * 0.25  + G(x, y, z))
#define Z(x, y, z) (((z) - G(x, y, z)) * 0.125 + G(x, y, z))
	ASSERT(eq(raster1[POS(0, 0)], X(4, 8, 4)));
	ASSERT(eq(raster1[POS(0, 1)], X(16, 2, 16)));
	ASSERT(eq(raster1[POS(0, 2)], X(1, 8, 2)));
	ASSERT(eq(raster1[POS(0, 3)], X(32, 4, 8)));
	ASSERT(eq(raster1[POS(1, 0)], X(8, 8, 8)));
	ASSERT(eq(raster1[POS(1, 1)], X(-4, 2, 16)));
	ASSERT(eq(raster1[POS(1, 2)], X(16, -16, -16)));
	ASSERT(eq(raster1[POS(1, 3)], X(8, 2, 4)));
	ASSERT(eq(raster2[POS(0, 0)], Y(4, 8, 4)));
	ASSERT(eq(raster2[POS(0, 1)], Y(16, 2, 16)));
	ASSERT(eq(raster2[POS(0, 2)], Y(1, 8, 2)));
	ASSERT(eq(raster2[POS(0, 3)], Y(32, 4, 8)));
	ASSERT(eq(raster2[POS(1, 0)], Y(8, 8, 8)));
	ASSERT(eq(raster2[POS(1, 1)], Y(-4, 2, 16)));
	ASSERT(eq(raster2[POS(1, 2)], Y(16, -16, -16)));
	ASSERT(eq(raster2[POS(1, 3)], Y(8, 2, 4)));
	ASSERT(eq(raster3[POS(0, 0)], Z(4, 8, 4)));
	ASSERT(eq(raster3[POS(0, 1)], Z(16, 2, 16)));
	ASSERT(eq(raster3[POS(0, 2)], Z(1, 8, 2)));
	ASSERT(eq(raster3[POS(0, 3)], Z(32, 4, 8)));
	ASSERT(eq(raster3[POS(1, 0)], Z(8, 8, 8)));
	ASSERT(eq(raster3[POS(1, 1)], Z(-4, 2, 16)));
	ASSERT(eq(raster3[POS(1, 2)], Z(16, -16, -16)));
	ASSERT(eq(raster3[POS(1, 3)], Z(8, 2, 4)));
#undef G
#undef X
#undef Y
#undef Z
	raster1[POS(0, 0)] = 0, raster2[POS(0, 0)] = 0, raster3[POS(0, 0)] = 0;
	raster1[POS(0, 1)] = 0, raster2[POS(0, 1)] = 0, raster3[POS(0, 1)] = 0;
	raster1[POS(0, 2)] = 0, raster2[POS(0, 2)] = 0, raster3[POS(0, 2)] = 0;
	raster1[POS(0, 3)] = 0, raster2[POS(0, 3)] = 0, raster3[POS(0, 3)] = 0;
	raster1[POS(1, 0)] = 0, raster2[POS(1, 0)] = 0, raster3[POS(1, 0)] = 0;
	raster1[POS(1, 1)] = 0, raster2[POS(1, 1)] = 0, raster3[POS(1, 1)] = 0;
	raster1[POS(1, 2)] = 0, raster2[POS(1, 2)] = 0, raster3[POS(1, 2)] = 0;
	raster1[POS(1, 3)] = 0, raster2[POS(1, 3)] = 0, raster3[POS(1, 3)] = 0;
	ASSERT(!memcmp(raster1, zeroes, sizeof(raster1)));
	ASSERT(!memcmp(raster2, zeroes, sizeof(raster2)));
	ASSERT(!memcmp(raster3, zeroes, sizeof(raster3)));

	return 0;
}


#endif
