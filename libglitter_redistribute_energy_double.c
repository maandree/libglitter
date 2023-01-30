/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


static void
hselfconvolute(double *restrict raster, size_t rowsize, size_t width, size_t height, size_t kernelsize, const double *hkernel)
{
	/* TODO */
}


static void
hconvolute(double *restrict output, const double *restrict input, size_t output_rowsize, size_t input_rowsize,
           size_t width, size_t height, size_t kernelsize, const double *kernel)
{
	/* TODO */
}


static void
vconvolute(double *restrict output, const double *restrict input, size_t output_rowsize, size_t input_rowsize,
           size_t width, size_t height, size_t kernelsize, const double *kernel)
{
	/* TODO */
}


static void
copyraster(double *restrict output, const double *restrict input, size_t output_rowsize,
           size_t input_rowsize, size_t width, size_t height)
{
	size_t y;
	for (y = 0; y < height; y++) {
		memcpy(output, input, width * sizeof(double));
		output = &output[output_rowsize];
		input = &input[input_rowsize];
	}
}


void
libglitter_redistribute_energy_double(double *restrict output, const double *restrict input, /* TODO add man page */
                                      size_t output_rowsize, size_t input_rowsize, size_t width,
                                      size_t height, size_t hkernelsize, size_t vkernelsize,
                                      const double *hkernel, const double *vkernel)
{
	/* TODO Can we allow output==input ? */
	if (vkernelsize > 1) {
		vconvolute(output, input, output_rowsize, input_rowsize, width, height, vkernelsize, vkernel);
		if (hkernelsize > 1)
			hselfconvolute(output, output_rowsize, width, height, hkernelsize, hkernel);
	} else if (hkernelsize > 1) {
		vconvolute(output, input, output_rowsize, input_rowsize, width, height, hkernelsize, hkernel);
	} else {
		copyraster(output, input, output_rowsize, input_rowsize, width, height);
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
