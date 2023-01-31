/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


static void
vconvolute(double *restrict raster, size_t rowsize, size_t width, size_t height, size_t kernelsize, const double *kernel)
{
	size_t y, x, i;

	if (kernelsize == 3 && kernel[0] == kernel[1] && kernel[1] == kernel[2]) {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++)
				raster[x] += raster[x + 1 * rowsize];
			for (x = 0; x < width; x++) {
				raster[x] += raster[x + 2 * rowsize];
				raster[x] *= kernel[0];
			}
			raster = &raster[rowsize];
		}
		for (x = 0; x < width; x++) {
			raster[x] += raster[x + rowsize];
			raster[x] *= kernel[0];
			raster[x + rowsize] *= kernel[0];
		}

	} else {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++)
				raster[x] *= kernel[0];
			for (i = 1; i < kernelsize; i++)
				for (x = 0; x < width; x++)
					raster[x] = fma(raster[x + i * rowsize], kernel[i], raster[x]);
			raster = &raster[rowsize];
		}
		for (y = 0; y < kernelsize - 1; y++) {
			for (x = 0; x < width; x++)
				raster[x] *= kernel[0];
			for (i = 1; i < kernelsize - 1 - y; i++)
				for (x = 0; x < width; x++)
					raster[x] = fma(raster[x + i * rowsize], kernel[i], raster[x]);
			raster = &raster[rowsize];
		}
	}
}


static void
hconvolute(double *restrict raster, size_t rowsize, size_t width, size_t height, size_t kernelsize, const double *kernel)
{
	size_t y, x, i;

	if (kernelsize == 3 && kernel[0] == kernel[1] && kernel[1] == kernel[2]) {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				raster[x + 1] += raster[x + 2];
				raster[x] += raster[x + 2];
				raster[x] *= kernel[0];
			}
			raster[width] *= kernel[0];
			raster[width + 1] *= kernel[0];
			raster = &raster[rowsize];
		}

	} else {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				raster[x] *= kernel[0];
				for (i = 1; i < kernelsize; i++)
					raster[x] = fma(raster[x + i], kernel[i], raster[x]);
			}
			raster = &raster[width];
			for (x = 0; x < kernelsize - 1; x++) {
				raster[x] *= kernel[0];
				for (i = 1; i < kernelsize - 1 - x; i++)
					raster[x] = fma(raster[x + i], kernel[i], raster[x]);
			}
			raster = &raster[rowsize - width];
		}
	}
}


void
libglitter_redistribute_energy_double(double *restrict raster, size_t rowsize, size_t width, size_t height,
                                      size_t hkernelsize, size_t vkernelsize, const double *hkernel, const double *vkernel)
{
	if (vkernelsize > 1) {
		raster -= (vkernelsize - 1) * rowsize;
		vconvolute(raster, rowsize, width, height, vkernelsize, vkernel);
		height += vkernelsize - 1;
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
	double raster[100], hkern[5], vkern[5];
	size_t i, j;

	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		raster[i] = (double)i;
	hkern[0] = vkern[0] = 0;
	libglitter_redistribute_energy_double(&raster[11], 10, 7, 8, 1, 1, NULL, NULL);
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		ASSERT(raster[i] == (double)i);
	libglitter_redistribute_energy_double(&raster[11], 10, 7, 8, 1, 1, hkern, NULL);
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		ASSERT(raster[i] == (double)i);
	libglitter_redistribute_energy_double(&raster[11], 10, 7, 8, 1, 1, NULL, vkern);
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		ASSERT(raster[i] == (double)i);
	libglitter_redistribute_energy_double(&raster[11], 10, 7, 8, 1, 1, hkern, vkern);
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		ASSERT(raster[i] == (double)i);

	memset(raster, 0, sizeof(raster));
	raster[53] = 1;
	raster[57] = 2;
	hkern[0] = 1 / 3.;
	hkern[1] = 1 / 3.;
	hkern[2] = 1 / 3.;
	libglitter_redistribute_energy_double(&raster[12], 10, 7, 8, 3, 1, hkern, NULL);
	ASSERT(eq(raster[52 - 1], 1 / 3.));
	ASSERT(eq(raster[53 - 1], 1 / 3.));
	ASSERT(eq(raster[54 - 1], 1 / 3.));
	ASSERT(eq(raster[56 - 1], 2 / 3.));
	ASSERT(eq(raster[57 - 1], 2 / 3.));
	ASSERT(eq(raster[58 - 1], 2 / 3.));
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		if (!(51 <= i && i <= 53) && !(55 <= i && i <= 57))
			ASSERT(!raster[i]);

	memset(raster, 0, sizeof(raster));
	raster[53] = 1;
	raster[57] = 2;
	hkern[0] = 1 / 4.;
	hkern[1] = 1 / 2.;
	hkern[2] = 1 / 4.;
	libglitter_redistribute_energy_double(&raster[12], 10, 7, 8, 3, 1, hkern, NULL);
	ASSERT(eq(raster[52 - 1], 1 / 4.));
	ASSERT(eq(raster[53 - 1], 1 / 2.));
	ASSERT(eq(raster[54 - 1], 1 / 4.));
	ASSERT(eq(raster[56 - 1], 2 / 4.));
	ASSERT(eq(raster[57 - 1], 2 / 2.));
	ASSERT(eq(raster[58 - 1], 2 / 4.));
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		if (!(51 <= i && i <= 53) && !(55 <= i && i <= 57))
			ASSERT(!raster[i]);

	memset(raster, 0, sizeof(raster));
	raster[35] = 1;
	raster[75] = 2;
	vkern[0] = 1 / 3.;
	vkern[1] = 1 / 3.;
	vkern[2] = 1 / 3.;
	libglitter_redistribute_energy_double(&raster[21], 10, 8, 7, 1, 3, NULL, vkern);
	ASSERT(eq(raster[25 - 10], 1 / 3.));
	ASSERT(eq(raster[35 - 10], 1 / 3.));
	ASSERT(eq(raster[45 - 10], 1 / 3.));
	ASSERT(eq(raster[65 - 10], 2 / 3.));
	ASSERT(eq(raster[75 - 10], 2 / 3.));
	ASSERT(eq(raster[85 - 10], 2 / 3.));
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		if (i != 15 && i != 25 && i != 35 && i != 55 && i != 65 && i != 75)
			ASSERT(!raster[i]);

	memset(raster, 0, sizeof(raster));
	raster[35] = 1;
	raster[75] = 2;
	vkern[0] = 1 / 4.;
	vkern[1] = 1 / 2.;
	vkern[2] = 1 / 4.;
	libglitter_redistribute_energy_double(&raster[21], 10, 8, 7, 1, 3, NULL, vkern);
	ASSERT(eq(raster[25 - 10], 1 / 4.));
	ASSERT(eq(raster[35 - 10], 1 / 2.));
	ASSERT(eq(raster[45 - 10], 1 / 4.));
	ASSERT(eq(raster[65 - 10], 2 / 4.));
	ASSERT(eq(raster[75 - 10], 2 / 2.));
	ASSERT(eq(raster[85 - 10], 2 / 4.));
	for (i = 0; i < sizeof(raster) / sizeof(*raster); i++)
		if (i != 15 && i != 25 && i != 35 && i != 55 && i != 65 && i != 75)
			ASSERT(!raster[i]);

	memset(raster, 0, sizeof(raster));
	for (j = 20; j < 100; j += 10)
		for (i = 2; i < 10; i += 1)
			raster[j + i] = 1;
	vkern[0] = 1 / 3.;
	vkern[1] = 1 / 3.;
	vkern[2] = 1 / 3.;
	hkern[0] = 1 / 3.;
	hkern[1] = 1 / 3.;
	hkern[2] = 1 / 3.;
	libglitter_redistribute_energy_double(&raster[22], 10, 8, 8, 3, 3, hkern, vkern);
	for (j = 0; j < 100; j += 10)
		for (i = 0; i < 10; i += 1)
			ASSERT(eq(raster[j + i],
			          ((i == 0 || i == 9) ? 1 / 3. :
			           (i == 1 || i == 8) ? 2 / 3. : 1.) *
			          ((j ==  0 || j == 90) ? 1 / 3. :
			           (j == 10 || j == 80) ? 2 / 3. : 1.)));

	memset(raster, 0, sizeof(raster));
	for (j = 20; j < 100; j += 10)
		for (i = 2; i < 10; i += 1)
			raster[j + i] = 1;
	vkern[0] = 1 / 3.;
	vkern[1] = 1 / 3.;
	vkern[2] = 1 / 3.;
	hkern[0] = 2 / 9.;
	hkern[1] = 5 / 9.;
	hkern[2] = 2 / 9.;
	libglitter_redistribute_energy_double(&raster[22], 10, 8, 8, 3, 3, hkern, vkern);
	for (j = 0; j < 100; j += 10)
		for (i = 0; i < 10; i += 1)
			ASSERT(eq(raster[j + i],
			          ((i == 0 || i == 9) ? 2 / 9. :
			           (i == 1 || i == 8) ? 7 / 9. : 1.) *
			          ((j ==  0 || j == 90) ? 1 / 3. :
			           (j == 10 || j == 80) ? 2 / 3. : 1.)));

	memset(raster, 0, sizeof(raster));
	for (j = 20; j < 100; j += 10)
		for (i = 2; i < 10; i += 1)
			raster[j + i] = 1;
	vkern[0] = 2 / 9.;
	vkern[1] = 5 / 9.;
	vkern[2] = 2 / 9.;
	hkern[0] = 1 / 3.;
	hkern[1] = 1 / 3.;
	hkern[2] = 1 / 3.;
	libglitter_redistribute_energy_double(&raster[22], 10, 8, 8, 3, 3, hkern, vkern);
	for (j = 0; j < 100; j += 10)
		for (i = 0; i < 10; i += 1)
			ASSERT(eq(raster[j + i],
			          ((i == 0 || i == 9) ? 1 / 3. :
			           (i == 1 || i == 8) ? 2 / 3. : 1.) *
			          ((j ==  0 || j == 90) ? 2 / 9. :
			           (j == 10 || j == 80) ? 7 / 9. : 1.)));

	memset(raster, 0, sizeof(raster));
	for (j = 20; j < 100; j += 10)
		for (i = 2; i < 10; i += 1)
			raster[j + i] = 1;
	vkern[0] = 2 / 9.;
	vkern[1] = 5 / 9.;
	vkern[2] = 2 / 9.;
	hkern[0] = 2 / 9.;
	hkern[1] = 5 / 9.;
	hkern[2] = 2 / 9.;
	libglitter_redistribute_energy_double(&raster[22], 10, 8, 8, 3, 3, hkern, vkern);
	for (j = 0; j < 100; j += 10)
		for (i = 0; i < 10; i += 1)
			ASSERT(eq(raster[j + i],
			          ((i == 0 || i == 9) ? 2 / 9. :
			           (i == 1 || i == 8) ? 7 / 9. : 1.) *
			          ((j ==  0 || j == 90) ? 2 / 9. :
			           (j == 10 || j == 80) ? 7 / 9. : 1.)));

	memset(raster, 0, sizeof(raster));
	for (j = 40; j < 100; j += 10)
		for (i = 4; i < 10; i += 1)
			raster[j + i] = 1;
	vkern[0] = 1 / 5.;
	vkern[1] = 1 / 5.;
	vkern[2] = 1 / 5.;
	vkern[3] = 1 / 5.;
	vkern[4] = 1 / 5.;
	hkern[0] = 1 / 9.;
	hkern[1] = 2 / 9.;
	hkern[2] = 3 / 9.;
	hkern[3] = 2 / 9.;
	hkern[4] = 1 / 9.;
	libglitter_redistribute_energy_double(&raster[44], 10, 6, 6, 5, 5, hkern, vkern);
	for (j = 0; j < 100; j += 10)
		for (i = 0; i < 10; i += 1)
			ASSERT(eq(raster[j + i],
			          ((i == 0 || i == 9) ? 1 / 9. :
			           (i == 1 || i == 8) ? 3 / 9. :
			           (i == 2 || i == 7) ? 6 / 9. :
			           (i == 3 || i == 6) ? 8 / 9. : 1.) *
			          ((j ==  0 || j == 90) ? 1 / 5. :
			           (j == 10 || j == 80) ? 2 / 5. :
			           (j == 20 || j == 70) ? 3 / 5. :
			           (j == 30 || j == 60) ? 4 / 5. : 1.)));

	return 0;
}


#endif
