/* See LICENSE file for copyright and license details. */
#include "common.h"
#define ONLY_INT_COMPATIBLE
#define double uint64_t
#define libglitter_compose_double libglitter_compose_uint64
#include "libglitter_compose_double.c"
#ifdef greater_t
# define MIX(A, B) (((greater_t)(A) + (greater_t)(B)) >> 1)
#else
# define greater_t double
# define MIX(A, B) (((A) >> 1) + ((B) >> 1) + ((A) & (B) & 1))
#endif
#ifndef TEST


static void
simple(uint64_t **outputs, const uint64_t *restrict input, size_t output_rowsize, size_t output_cellsize,
       size_t input_rowsize, size_t width, size_t height, const LIBGLITTER_RENDER_CONTEXT *render_ctx)
{
	size_t y, x, output_y, output_i, input_blanking;
	size_t cell00 = render_ctx->cells[0][0], cell01 = render_ctx->cells[0][1];
	size_t cell10 = render_ctx->cells[1][0], cell11 = render_ctx->cells[1][1];
	size_t cell20 = render_ctx->cells[2][0], cell21 = render_ctx->cells[2][1];

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize * render_ctx->heightmul - width * render_ctx->widthmul;

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			outputs[0][output_i] = (uint64_t)MIX(input[cell00], input[cell01]);
			outputs[1][output_i] = (uint64_t)MIX(input[cell10], input[cell11]);
			outputs[2][output_i] = (uint64_t)MIX(input[cell20], input[cell21]);
			input = &input[render_ctx->widthmul];
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
				outputs[i][output_i] = (uint64_t)(pixel[i] / (greater_t)ncellvalues[i]);

			input = &input[widthmul];
		}
		input = &input[input_blanking];
	}
}


void
libglitter_compose_uint64(uint64_t **outputs, const uint64_t *restrict input, size_t output_rowsize, size_t output_cellsize,
                          size_t width, size_t height, const LIBGLITTER_RENDER_CONTEXT *render_ctx)
{
	if (render_ctx->render_method == RENDER_METHOD_VSTRIPS) {
		vstrips(outputs, input, output_rowsize, output_cellsize, render_ctx->rowsize, width, height, render_ctx->cellmap);
	} else if (render_ctx->render_method == RENDER_METHOD_HSTRIPS) {
		hstrips(outputs, input, output_rowsize, output_cellsize, render_ctx->rowsize, width, height, render_ctx->cellmap);
	} else if (render_ctx->render_method == RENDER_METHOD_SIMPLE) {
		simple(outputs, input, output_rowsize, output_cellsize, render_ctx->rowsize, width, height, render_ctx);
	} else {
		generic(outputs, render_ctx->noutputs, input, output_rowsize, output_cellsize,
		        render_ctx->rowsize, width, height, render_ctx->widthmul, render_ctx->heightmul,
		        render_ctx->cellmap, render_ctx->ncellvalues);
	}
}


#endif
