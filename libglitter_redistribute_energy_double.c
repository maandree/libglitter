/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


static void
vconvolute(double *restrict raster, size_t rowsize, size_t width, size_t height, size_t kernelsize, const double *kernel)
{
	size_t y, x, i;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++)
			raster[x] *= kernel[0];
		for (i = 1; i < kernelsize; i++)
			for (x = 0; x < width; x++)
				raster[x] = fma(raster[i * rowsize + x], kernel[i], raster[x]);
		raster = &raster[rowsize];
	}
}


static void
hconvolute(double *restrict raster, size_t rowsize, size_t width, size_t height, size_t kernelsize, const double *kernel)
{
	size_t y, x, i;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			raster[x] *= kernel[0];
			for (i = 1; i < kernelsize; i++)
				raster[x] = fma(raster[x + i], kernel[i], raster[x]);
		}
		raster = &raster[rowsize];
	}
}


void
libglitter_redistribute_energy_double(double *restrict raster, size_t rowsize, size_t width, size_t height, /* TODO add man page */
                                      size_t hkernelsize, size_t vkernelsize, const double *hkernel, const double *vkernel)
{
	if (vkernelsize > 1) {
		raster -= (hkernelsize - 1) * rowsize;
		vconvolute(raster, rowsize, width, height, vkernelsize, vkernel);
		height += hkernelsize - 1;
	}
	if (hkernelsize > 1) {
		raster -= hkernelsize - 1;
		hconvolute(raster, rowsize, width, height, hkernelsize, hkernel);
		width += hkernelsize - 1;
	}
}


#else


#define TOLERANCE 0.0001

static int
eq(double a, double b)
{
	double r = a - b;
	return (r < 0 ? -r : r) < TOLERANCE;
}

int
main(void)
{
	return 0; /* TODO add test */
}


#endif
