/* See LICENSE file for copyright and license details. */
#include "libglitter.h"


static void
vstrips(double **outputs_, const double *restrict input, size_t output_rowsize, size_t output_cellsize,
        size_t input_rowsize, size_t width, size_t height, const uint8_t *restrict cellmap)
{
	double *outputs[3];
	size_t y, x, output_y, output_i, input_blanking;

	outputs[0] = outputs_[cellmap[0]];
	outputs[1] = outputs_[cellmap[1]];
	outputs[2] = outputs_[cellmap[2]];

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize - width * 3;

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			outputs[0][output_i] = input[0];
			outputs[1][output_i] = input[1];
			outputs[2][output_i] = input[2];
			input = &input[3];
		}
		input = &input[input_blanking];
	}
}


static void
hstrips(double **outputs_, const double *restrict input, size_t output_rowsize, size_t output_cellsize,
        size_t input_rowsize, size_t width, size_t height, const uint8_t *restrict cellmap)
{
	double *outputs[3];
	size_t y, x, output_y, output_i, input_blanking;

	outputs[0] = outputs_[cellmap[0]];
	outputs[1] = outputs_[cellmap[1]];
	outputs[2] = outputs_[cellmap[2]];

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize * 3 - width;

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			outputs[0][output_i] = input[0 * input_rowsize];
			outputs[1][output_i] = input[1 * input_rowsize];
			outputs[2][output_i] = input[2 * input_rowsize];
			input = &input[1];
		}
		input = &input[input_blanking];
	}
}


#ifndef ONLY_INT_COMPATIBLE


static void
generic(double **outputs, const double *restrict input,
        size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
        size_t width, size_t height, size_t widthmul, size_t heightmul,
        const uint8_t *restrict cellmap, const double *restrict ncellweights)
{
	size_t y, x, iy, ix, output_y, output_i, input_blanking;
	uint8_t channel;
	double value;

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize * heightmul - width * widthmul;

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			for (iy = 0; iy < heightmul; iy++) {
				for (ix = 0; ix < widthmul; ix++) {
					channel = cellmap[iy * widthmul + ix];

					value = input[iy * input_rowsize + ix];
					value *= ncellweights[channel];

					outputs[channel][output_i] += value;
				}
			}
			input = &input[widthmul];
		}
		input = &input[input_blanking];
	}
}


void
libglitter_compose_double(double **outputs, size_t noutputs, const double *restrict input,
                          size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
                          size_t width, size_t height, size_t widthmul, size_t heightmul,
                          const uint8_t *restrict cellmap, const double *restrict ncellweights)
{
	if (noutputs == 3 && widthmul == 3 && heightmul == 1) {
		vstrips(outputs, input, output_rowsize, output_cellsize, input_rowsize, width, height, cellmap);
	} else if (noutputs == 3 && widthmul == 1 && heightmul == 3) {
		hstrips(outputs, input, output_rowsize, output_cellsize, input_rowsize, width, height, cellmap);
	} else {
		generic(outputs, input, output_rowsize, output_cellsize, input_rowsize,
		        width, height, widthmul, heightmul, cellmap, ncellweights);
	}
}


#endif
