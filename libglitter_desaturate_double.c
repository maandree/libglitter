/* See LICENSE file for copyright and license details. */
#include "common.h"


#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic ignored "-Wunsuffixed-float-constants"
#endif


void
libglitter_desaturate_double(double **rasters, size_t nrasters, size_t rowsize, size_t cellsize,
                             size_t width, size_t height, double saturation, const double *primary_ys)
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
