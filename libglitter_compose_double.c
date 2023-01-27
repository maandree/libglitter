/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


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


# ifndef ONLY_INT_COMPATIBLE


static void
generic(double **outputs, size_t noutputs, const double *restrict input,
        size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
        size_t width, size_t height, size_t widthmul, size_t heightmul,
        const uint8_t *restrict cellmap, const double *restrict cellweights)
{
	size_t y, x, iy, ix, i, output_y, output_i, input_blanking;
	uint8_t channel;
	double value;

	output_rowsize *= output_cellsize;
	input_blanking = input_rowsize * heightmul - width * widthmul;

	for (y = 0, output_y = 0; y < height; y++, output_y += output_rowsize) {
		for (x = 0, output_i = output_y; x < width; x++, output_i += output_cellsize) {

			for (i = 0; i < noutputs; i++)
				outputs[i][output_i] = 0;
			for (iy = 0; iy < heightmul; iy++) {
				for (ix = 0; ix < widthmul; ix++) {
					channel = cellmap[iy * widthmul + ix];

					value = input[iy * input_rowsize + ix];
					value *= cellweights[channel];

					outputs[channel][output_i] += value;
				}
			}
			input = &input[widthmul];
		}
		input = &input[input_blanking];
	}
}


void
libglitter_compose_double(double **outputs, const double *restrict input, size_t output_rowsize, size_t output_cellsize,
                          size_t width, size_t height, const LIBGLITTER_RENDER_CONTEXT *render_ctx)
{
	if (render_ctx->render_method == RENDER_METHOD_VSTRIPS) {
		vstrips(outputs, input, output_rowsize, output_cellsize, render_ctx->rowsize, width, height, render_ctx->cellmap);
	} else if (render_ctx->render_method == RENDER_METHOD_HSTRIPS) {
		hstrips(outputs, input, output_rowsize, output_cellsize, render_ctx->rowsize, width, height, render_ctx->cellmap);
	} else {
		generic(outputs, render_ctx->noutputs, input, output_rowsize, output_cellsize, render_ctx->rowsize, width, height,
		        render_ctx->widthmul, render_ctx->heightmul, render_ctx->cellmap, render_ctx->cellweights_double);
	}
}


# endif


#else


int
main(void)
{
	double input[20 * 60];
	double output1[3 * 10 * 4];
	double output2[sizeof(output1) / sizeof(*output1)];
	double output3[sizeof(output1) / sizeof(*output1)];
	double zeroes[sizeof(input) / sizeof(*input)];
	double *outputs[3];
	LIBGLITTER_RENDER_CONTEXT *ctx;
	uint8_t cellmap[12];
	uint8_t ncellvalues[3];
	size_t input_rowsize;
	size_t output_rowsize;
	size_t output_cellsize;
	size_t wmul, hmul;
	size_t i;

#define OPOS(X, Y) ((Y) * output_rowsize * output_cellsize + (X) * output_cellsize)
#define IPOS(X, Y, IX, IY) (((Y) * hmul + (IY)) * input_rowsize + ((X) * wmul + (IX)))
#define V(VAL) ((double)(VAL))
#define VV(VAL) (V(VAL) + (double)1 / 2)

	memset(zeroes, 0, sizeof(zeroes));

	/* vstrips */
	for (i = 0; i < 2; i++) {
		input_rowsize = 5 * 3;
		output_rowsize = 10;
		output_cellsize = 4;
		wmul = 3;
		hmul = 1;
		ncellvalues[0] = 1;
		ncellvalues[1] = 1;
		ncellvalues[2] = 1;
		cellmap[0] = !i ? 0 : 1;
		cellmap[1] = !i ? 1 : 2;
		cellmap[2] = !i ? 2 : 0;
		outputs[0] = !i ? output1 : output3;
		outputs[1] = !i ? output2 : output1;
		outputs[2] = !i ? output3 : output2;
		memset(input, 0, sizeof(input));
		memset(output1, 0, sizeof(output1));
		memset(output2, 0, sizeof(output2));
		memset(output3, 0, sizeof(output3));
		input[IPOS(0, 0, 0, 0)] = V(111);
		input[IPOS(0, 0, 1, 0)] = V(112);
		input[IPOS(0, 0, 2, 0)] = V(113);
		input[IPOS(0, 1, 0, 0)] = V(121);
		input[IPOS(0, 1, 1, 0)] = V(122);
		input[IPOS(0, 1, 2, 0)] = V(123);
		input[IPOS(1, 0, 0, 0)] = V(211);
		input[IPOS(1, 0, 1, 0)] = V(212);
		input[IPOS(1, 0, 2, 0)] = V(213);
		input[IPOS(1, 1, 0, 0)] = V(221);
		input[IPOS(1, 1, 1, 0)] = V(222);
		input[IPOS(1, 1, 2, 0)] = V(223);
		input[IPOS(2, 0, 0, 0)] = V(311);
		input[IPOS(2, 0, 1, 0)] = V(312);
		input[IPOS(2, 0, 2, 0)] = V(313);
		input[IPOS(2, 1, 0, 0)] = V(321);
		input[IPOS(2, 1, 1, 0)] = V(322);
		input[IPOS(2, 1, 2, 0)] = V(323);
		ctx = libglitter_create_render_context(3, input_rowsize, wmul, hmul, cellmap, ncellvalues);
		ASSERT(ctx);
		ASSERT(ctx->render_method == RENDER_METHOD_VSTRIPS);
		libglitter_compose_double(outputs, input, output_rowsize, output_cellsize, 3, 2, ctx);
		ASSERT(output1[OPOS(0, 0)] == V(111));
		ASSERT(output2[OPOS(0, 0)] == V(112));
		ASSERT(output3[OPOS(0, 0)] == V(113));
		ASSERT(output1[OPOS(0, 1)] == V(121));
		ASSERT(output2[OPOS(0, 1)] == V(122));
		ASSERT(output3[OPOS(0, 1)] == V(123));
		ASSERT(output1[OPOS(1, 0)] == V(211));
		ASSERT(output2[OPOS(1, 0)] == V(212));
		ASSERT(output3[OPOS(1, 0)] == V(213));
		ASSERT(output1[OPOS(1, 1)] == V(221));
		ASSERT(output2[OPOS(1, 1)] == V(222));
		ASSERT(output3[OPOS(1, 1)] == V(223));
		ASSERT(output1[OPOS(2, 0)] == V(311));
		ASSERT(output2[OPOS(2, 0)] == V(312));
		ASSERT(output3[OPOS(2, 0)] == V(313));
		ASSERT(output1[OPOS(2, 1)] == V(321));
		ASSERT(output2[OPOS(2, 1)] == V(322));
		ASSERT(output3[OPOS(2, 1)] == V(323));
		output1[OPOS(0, 0)] = 0;
		output2[OPOS(0, 0)] = 0;
		output3[OPOS(0, 0)] = 0;
		output1[OPOS(0, 1)] = 0;
		output2[OPOS(0, 1)] = 0;
		output3[OPOS(0, 1)] = 0;
		output1[OPOS(1, 0)] = 0;
		output2[OPOS(1, 0)] = 0;
		output3[OPOS(1, 0)] = 0;
		output1[OPOS(1, 1)] = 0;
		output2[OPOS(1, 1)] = 0;
		output3[OPOS(1, 1)] = 0;
		output1[OPOS(2, 0)] = 0;
		output2[OPOS(2, 0)] = 0;
		output3[OPOS(2, 0)] = 0;
		output1[OPOS(2, 1)] = 0;
		output2[OPOS(2, 1)] = 0;
		output3[OPOS(2, 1)] = 0;
		ASSERT(!memcmp(output1, zeroes, sizeof(output1)));
		ASSERT(!memcmp(output2, zeroes, sizeof(output2)));
		ASSERT(!memcmp(output3, zeroes, sizeof(output3)));
		libglitter_free_render_context(ctx);
	}

	/* hstrips */
	for (i = 0; i < 2; i++) {
		input_rowsize = 5;
		output_rowsize = 10;
		output_cellsize = 4;
		wmul = 1;
		hmul = 3;
		ncellvalues[0] = 1;
		ncellvalues[1] = 1;
		ncellvalues[2] = 1;
		cellmap[0] = !i ? 0 : 1;
		cellmap[1] = !i ? 1 : 2;
		cellmap[2] = !i ? 2 : 0;
		outputs[0] = !i ? output1 : output3;
		outputs[1] = !i ? output2 : output1;
		outputs[2] = !i ? output3 : output2;
		memset(input, 0, sizeof(input));
		memset(output1, 0, sizeof(output1));
		memset(output2, 0, sizeof(output2));
		memset(output3, 0, sizeof(output3));
		input[IPOS(0, 0, 0, 0)] = V(111);
		input[IPOS(0, 0, 0, 1)] = V(112);
		input[IPOS(0, 0, 0, 2)] = V(113);
		input[IPOS(1, 0, 0, 0)] = V(121);
		input[IPOS(1, 0, 0, 1)] = V(122);
		input[IPOS(1, 0, 0, 2)] = V(123);
		input[IPOS(0, 1, 0, 0)] = V(211);
		input[IPOS(0, 1, 0, 1)] = V(212);
		input[IPOS(0, 1, 0, 2)] = V(213);
		input[IPOS(1, 1, 0, 0)] = V(221);
		input[IPOS(1, 1, 0, 1)] = V(222);
		input[IPOS(1, 1, 0, 2)] = V(223);
		input[IPOS(0, 2, 0, 0)] = V(311);
		input[IPOS(0, 2, 0, 1)] = V(312);
		input[IPOS(0, 2, 0, 2)] = V(313);
		input[IPOS(1, 2, 0, 0)] = V(321);
		input[IPOS(1, 2, 0, 1)] = V(322);
		input[IPOS(1, 2, 0, 2)] = V(323);
		ctx = libglitter_create_render_context(3, input_rowsize, wmul, hmul, cellmap, ncellvalues);
		ASSERT(ctx);
		ASSERT(ctx->render_method == RENDER_METHOD_HSTRIPS);
		libglitter_compose_double(outputs, input, output_rowsize, output_cellsize, 2, 3, ctx);
		ASSERT(output1[OPOS(0, 0)] == V(111));
		ASSERT(output2[OPOS(0, 0)] == V(112));
		ASSERT(output3[OPOS(0, 0)] == V(113));
		ASSERT(output1[OPOS(1, 0)] == V(121));
		ASSERT(output2[OPOS(1, 0)] == V(122));
		ASSERT(output3[OPOS(1, 0)] == V(123));
		ASSERT(output1[OPOS(0, 1)] == V(211));
		ASSERT(output2[OPOS(0, 1)] == V(212));
		ASSERT(output3[OPOS(0, 1)] == V(213));
		ASSERT(output1[OPOS(1, 1)] == V(221));
		ASSERT(output2[OPOS(1, 1)] == V(222));
		ASSERT(output3[OPOS(1, 1)] == V(223));
		ASSERT(output1[OPOS(0, 2)] == V(311));
		ASSERT(output2[OPOS(0, 2)] == V(312));
		ASSERT(output3[OPOS(0, 2)] == V(313));
		ASSERT(output1[OPOS(1, 2)] == V(321));
		ASSERT(output2[OPOS(1, 2)] == V(322));
		ASSERT(output3[OPOS(1, 2)] == V(323));
		output1[OPOS(0, 0)] = 0;
		output2[OPOS(0, 0)] = 0;
		output3[OPOS(0, 0)] = 0;
		output1[OPOS(1, 0)] = 0;
		output2[OPOS(1, 0)] = 0;
		output3[OPOS(1, 0)] = 0;
		output1[OPOS(0, 1)] = 0;
		output2[OPOS(0, 1)] = 0;
		output3[OPOS(0, 1)] = 0;
		output1[OPOS(1, 1)] = 0;
		output2[OPOS(1, 1)] = 0;
		output3[OPOS(1, 1)] = 0;
		output1[OPOS(0, 2)] = 0;
		output2[OPOS(0, 2)] = 0;
		output3[OPOS(0, 2)] = 0;
		output1[OPOS(1, 2)] = 0;
		output2[OPOS(1, 2)] = 0;
		output3[OPOS(1, 2)] = 0;
		ASSERT(!memcmp(output1, zeroes, sizeof(output1)));
		ASSERT(!memcmp(output2, zeroes, sizeof(output2)));
		ASSERT(!memcmp(output3, zeroes, sizeof(output3)));
		libglitter_free_render_context(ctx);
	}

	/* simple */
	for (i = 0; i < 2; i++) {
		input_rowsize = 14;
		output_rowsize = 4;
		output_cellsize = 2;
		wmul = !i ? 4 : 1;
		hmul = !i ? 1 : 4;
		ncellvalues[0] = !i ? 1 : 2;
		ncellvalues[1] = !i ? 1 : 1;
		ncellvalues[2] = !i ? 2 : 1;
		cellmap[0] = !i ? 0 : 1;
		cellmap[1] = !i ? 1 : 2;
		cellmap[2] = !i ? 2 : 0;
		cellmap[3] = !i ? 2 : 0;
		outputs[0] = !i ? output1 : output3;
		outputs[1] = !i ? output2 : output1;
		outputs[2] = !i ? output3 : output2;
		memset(input, 0, sizeof(input));
		memset(output1, 0, sizeof(output1));
		memset(output2, 0, sizeof(output2));
		memset(output3, 0, sizeof(output3));
		input[IPOS(0, 0, !i ? 0 : 0, i ? 0 : 0)] = V(111);
		input[IPOS(0, 0, !i ? 1 : 0, i ? 1 : 0)] = V(112);
		input[IPOS(0, 0, !i ? 2 : 0, i ? 2 : 0)] = V(113);
		input[IPOS(0, 0, !i ? 3 : 0, i ? 3 : 0)] = V(115);
		input[IPOS(1, 0, !i ? 0 : 0, i ? 0 : 0)] = V(121);
		input[IPOS(1, 0, !i ? 1 : 0, i ? 1 : 0)] = V(122);
		input[IPOS(1, 0, !i ? 2 : 0, i ? 2 : 0)] = V(123);
		input[IPOS(1, 0, !i ? 3 : 0, i ? 3 : 0)] = V(124);
		input[IPOS(0, 1, !i ? 0 : 0, i ? 0 : 0)] = V(211);
		input[IPOS(0, 1, !i ? 1 : 0, i ? 1 : 0)] = V(212);
		input[IPOS(0, 1, !i ? 2 : 0, i ? 2 : 0)] = V(213);
		input[IPOS(0, 1, !i ? 3 : 0, i ? 3 : 0)] = V(215);
		input[IPOS(1, 1, !i ? 0 : 0, i ? 0 : 0)] = V(221);
		input[IPOS(1, 1, !i ? 1 : 0, i ? 1 : 0)] = V(222);
		input[IPOS(1, 1, !i ? 2 : 0, i ? 2 : 0)] = V(223);
		input[IPOS(1, 1, !i ? 3 : 0, i ? 3 : 0)] = V(224);
		input[IPOS(0, 2, !i ? 0 : 0, i ? 0 : 0)] = V(311);
		input[IPOS(0, 2, !i ? 1 : 0, i ? 1 : 0)] = V(312);
		input[IPOS(0, 2, !i ? 2 : 0, i ? 2 : 0)] = V(313);
		input[IPOS(0, 2, !i ? 3 : 0, i ? 3 : 0)] = V(315);
		input[IPOS(1, 2, !i ? 0 : 0, i ? 0 : 0)] = V(321);
		input[IPOS(1, 2, !i ? 1 : 0, i ? 1 : 0)] = V(322);
		input[IPOS(1, 2, !i ? 2 : 0, i ? 2 : 0)] = V(323);
		input[IPOS(1, 2, !i ? 3 : 0, i ? 3 : 0)] = V(324);
		ctx = libglitter_create_render_context(3, input_rowsize, wmul, hmul, cellmap, ncellvalues);
		ASSERT(ctx);
		ASSERT(ctx->render_method == RENDER_METHOD_SIMPLE);
		libglitter_compose_double(outputs, input, output_rowsize, output_cellsize, 2, 3, ctx);
		ASSERT(output1[OPOS(0, 0)] == V(111));
		ASSERT(output2[OPOS(0, 0)] == V(112));
		ASSERT(output3[OPOS(0, 0)] == V(114));
		ASSERT(output1[OPOS(1, 0)] == V(121));
		ASSERT(output2[OPOS(1, 0)] == V(122));
		ASSERT(output3[OPOS(1, 0)] == VV(123));
		ASSERT(output1[OPOS(0, 1)] == V(211));
		ASSERT(output2[OPOS(0, 1)] == V(212));
		ASSERT(output3[OPOS(0, 1)] == V(214));
		ASSERT(output1[OPOS(1, 1)] == V(221));
		ASSERT(output2[OPOS(1, 1)] == V(222));
		ASSERT(output3[OPOS(1, 1)] == VV(223));
		ASSERT(output1[OPOS(0, 2)] == V(311));
		ASSERT(output2[OPOS(0, 2)] == V(312));
		ASSERT(output3[OPOS(0, 2)] == V(314));
		ASSERT(output1[OPOS(1, 2)] == V(321));
		ASSERT(output2[OPOS(1, 2)] == V(322));
		ASSERT(output3[OPOS(1, 2)] == VV(323));
		output1[OPOS(0, 0)] = 0;
		output2[OPOS(0, 0)] = 0;
		output3[OPOS(0, 0)] = 0;
		output1[OPOS(1, 0)] = 0;
		output2[OPOS(1, 0)] = 0;
		output3[OPOS(1, 0)] = 0;
		output1[OPOS(0, 1)] = 0;
		output2[OPOS(0, 1)] = 0;
		output3[OPOS(0, 1)] = 0;
		output1[OPOS(1, 1)] = 0;
		output2[OPOS(1, 1)] = 0;
		output3[OPOS(1, 1)] = 0;
		output1[OPOS(0, 2)] = 0;
		output2[OPOS(0, 2)] = 0;
		output3[OPOS(0, 2)] = 0;
		output1[OPOS(1, 2)] = 0;
		output2[OPOS(1, 2)] = 0;
		output3[OPOS(1, 2)] = 0;
		ASSERT(!memcmp(output1, zeroes, sizeof(output1)));
		ASSERT(!memcmp(output2, zeroes, sizeof(output2)));
		ASSERT(!memcmp(output3, zeroes, sizeof(output3)));
		libglitter_free_render_context(ctx);
	}

	/* generic */
	for (i = 0; i < 1; i++) {
		input_rowsize = 14;
		output_rowsize = 4;
		output_cellsize = 2;
		wmul = !i ? 3 : 1;
		hmul = !i ? 1 : 3;
		ncellvalues[0] = !i ? 1 : 2;
		ncellvalues[1] = !i ? 2 : 1;
		cellmap[0] = !i ? 0 : 1;
		cellmap[1] = !i ? 1 : 0;
		cellmap[2] = !i ? 1 : 0;
		outputs[0] = !i ? output1 : output2;
		outputs[1] = !i ? output2 : output1;
		outputs[2] = NULL;
		memset(input, 0, sizeof(input));
		memset(output1, 0, sizeof(output1));
		memset(output2, 0, sizeof(output2));
		input[IPOS(0, 0, !i ? 0 : 0, i ? 0 : 0)] = V(111);
		input[IPOS(0, 0, !i ? 1 : 0, i ? 1 : 0)] = V(112);
		input[IPOS(0, 0, !i ? 2 : 0, i ? 2 : 0)] = V(114);
		input[IPOS(1, 0, !i ? 0 : 0, i ? 0 : 0)] = V(121);
		input[IPOS(1, 0, !i ? 1 : 0, i ? 1 : 0)] = V(122);
		input[IPOS(1, 0, !i ? 2 : 0, i ? 2 : 0)] = V(123);
		input[IPOS(0, 1, !i ? 0 : 0, i ? 0 : 0)] = V(211);
		input[IPOS(0, 1, !i ? 1 : 0, i ? 1 : 0)] = V(212);
		input[IPOS(0, 1, !i ? 2 : 0, i ? 2 : 0)] = V(214);
		input[IPOS(1, 1, !i ? 0 : 0, i ? 0 : 0)] = V(221);
		input[IPOS(1, 1, !i ? 1 : 0, i ? 1 : 0)] = V(222);
		input[IPOS(1, 1, !i ? 2 : 0, i ? 2 : 0)] = V(223);
		input[IPOS(0, 2, !i ? 0 : 0, i ? 0 : 0)] = V(311);
		input[IPOS(0, 2, !i ? 1 : 0, i ? 1 : 0)] = V(312);
		input[IPOS(0, 2, !i ? 2 : 0, i ? 2 : 0)] = V(314);
		input[IPOS(1, 2, !i ? 0 : 0, i ? 0 : 0)] = V(321);
		input[IPOS(1, 2, !i ? 1 : 0, i ? 1 : 0)] = V(322);
		input[IPOS(1, 2, !i ? 2 : 0, i ? 2 : 0)] = V(323);
		ctx = libglitter_create_render_context(2, input_rowsize, wmul, hmul, cellmap, ncellvalues);
		ASSERT(ctx);
		ASSERT(ctx->render_method == RENDER_METHOD_GENERIC);
		libglitter_compose_double(outputs, input, output_rowsize, output_cellsize, 2, 3, ctx);
		ASSERT(output1[OPOS(0, 0)] == V(111));
		ASSERT(output2[OPOS(0, 0)] == V(113));
		ASSERT(output1[OPOS(1, 0)] == V(121));
		ASSERT(output2[OPOS(1, 0)] == VV(122));
		ASSERT(output1[OPOS(0, 1)] == V(211));
		ASSERT(output2[OPOS(0, 1)] == V(213));
		ASSERT(output1[OPOS(1, 1)] == V(221));
		ASSERT(output2[OPOS(1, 1)] == VV(222));
		ASSERT(output1[OPOS(0, 2)] == V(311));
		ASSERT(output2[OPOS(0, 2)] == V(313));
		ASSERT(output1[OPOS(1, 2)] == V(321));
		ASSERT(output2[OPOS(1, 2)] == VV(322));
		output1[OPOS(0, 0)] = 0;
		output2[OPOS(0, 0)] = 0;
		output1[OPOS(1, 0)] = 0;
		output2[OPOS(1, 0)] = 0;
		output1[OPOS(0, 1)] = 0;
		output2[OPOS(0, 1)] = 0;
		output1[OPOS(1, 1)] = 0;
		output2[OPOS(1, 1)] = 0;
		output1[OPOS(0, 2)] = 0;
		output2[OPOS(0, 2)] = 0;
		output1[OPOS(1, 2)] = 0;
		output2[OPOS(1, 2)] = 0;
		ASSERT(!memcmp(output1, zeroes, sizeof(output1)));
		ASSERT(!memcmp(output2, zeroes, sizeof(output2)));
		libglitter_free_render_context(ctx);
	}

	return 0;
}


#endif
