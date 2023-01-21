/* See LICENSE file for copyright and license details. */
#include "libglitter.h"
#define ONLY_INT_COMPATIBLE
#define double uint64_t
#include "libglitter_compose_double.c"
#include <alloca.h>
#ifdef greater_t
# define MIX(A, B) (((greater_t)(A) + (greater_t)(B)) >> 1)
#else
# define greater_t double
# define MIX(A, B) (((A) >> 1) + ((B) >> 1) + ((A) & (B) & 1))
#endif


static void
simple(uint64_t **outputs, const uint64_t *restrict input,
       size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
       size_t width, size_t height, size_t widthmul, size_t heightmul,
       const uint8_t *restrict cellmap)
{
	size_t y, x, output_y, output_i, input_blanking;
	uint8_t channel;
	size_t cells[3][2];
	size_t cellsi[3] = {0, 0, 0};

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize * heightmul - width * widthmul;

	for (y = 0; y < heightmul; y++) {
		for (x = 0; x < widthmul; x++) {
			channel = cellmap[y * widthmul + x];
			cells[channel][cellsi[channel]] = y * input_rowsize + x;
			cellsi[channel] += 1;
		}
	}
	if (cellsi[0] == 1)
		cells[0][1] = cells[0][0];
	if (cellsi[1] == 1)
		cells[1][1] = cells[1][0];
	if (cellsi[2] == 1)
		cells[2][1] = cells[2][0];

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			outputs[0][output_i] = (uint64_t)MIX(input[cells[0][0]], input[cells[0][1]]);
			outputs[1][output_i] = (uint64_t)MIX(input[cells[0][0]], input[cells[0][1]]);
			outputs[2][output_i] = (uint64_t)MIX(input[cells[0][0]], input[cells[0][1]]);
			input = &input[widthmul];
		}
		input = &input[input_blanking];
	}
}


static void
generic(uint64_t **outputs, size_t noutputs, const uint64_t *restrict input,
        size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
        size_t width, size_t height, size_t widthmul, size_t heightmul,
        const uint8_t *restrict cellmap, const uint8_t *restrict ncellvalues)
{
	size_t y, x, iy, ix, i, output_y, output_i, input_blanking;
	uint8_t channel;
	greater_t *pixel;

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize * heightmul - width * widthmul;

	pixel = alloca(noutputs * sizeof(greater_t));

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			for (i = 0; i < noutputs; i++)
				pixel[i] = 0;

			for (iy = 0; iy < heightmul; iy++) {
				for (ix = 0; ix < widthmul; ix++) {
					channel = cellmap[iy * widthmul + ix];
					pixel[channel] += (greater_t)input[iy * input_rowsize + ix];
				}
			}

			for (i = 0; i < noutputs; i++)
				outputs[i][output_i] = (uint64_t)(pixel[channel] / (greater_t)ncellvalues[i]);

			input = &input[widthmul];
		}
		input = &input[input_blanking];
	}
}


void
libglitter_compose_uint64(uint64_t **outputs, size_t noutputs, const uint64_t *restrict input,
                          size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
                          size_t width, size_t height, size_t widthmul, size_t heightmul,
                          const uint8_t *restrict cellmap, const uint8_t *restrict ncellvalues)
{
	if (noutputs == 3 && widthmul == 3 && heightmul == 1) {
		vstrips(outputs, input, output_rowsize, output_cellsize, input_rowsize, width, height, cellmap);
	} else if (noutputs == 3 && widthmul == 1 && heightmul == 3) {
		hstrips(outputs, input, output_rowsize, output_cellsize, input_rowsize, width, height, cellmap);
	} else if (noutputs == 3 && ncellvalues[0] <= 2 && ncellvalues[1] <= 2 && ncellvalues[2] <= 2) {
		simple(outputs, input, output_rowsize, output_cellsize, input_rowsize,
		       width, height, widthmul, heightmul, cellmap);
	} else {
		generic(outputs, noutputs, input, output_rowsize, output_cellsize,
		        input_rowsize, width, height, widthmul, heightmul, cellmap, ncellvalues);
	}
}
