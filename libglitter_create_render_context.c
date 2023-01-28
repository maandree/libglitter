/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


LIBGLITTER_RENDER_CONTEXT *
libglitter_create_render_context(size_t noutputs, size_t rowsize, size_t widthmul, size_t heightmul,
                                 const uint8_t *cellmap, const uint8_t *ncellvalues)
{
	struct libglitter_render_context *this;
	size_t i;

	this = calloc(1, sizeof(*this));
	if (!this)
		return NULL;

	this->cellmap = cellmap;
	this->ncellvalues = ncellvalues;
	this->noutputs = noutputs;
	this->widthmul = widthmul;
	this->heightmul = heightmul;
	
	this->cellweights_double = calloc(noutputs, sizeof(*this->cellweights_double));
	if (!this->cellweights_double)
		goto fail;
	this->cellweights_float = calloc(noutputs, sizeof(*this->cellweights_float));
	if (!this->cellweights_float)
		goto fail;

	this->render_method =
		(noutputs == 3 && widthmul == 3 && heightmul == 1) ? RENDER_METHOD_VSTRIPS :
		(noutputs == 3 && widthmul == 1 && heightmul == 3) ? RENDER_METHOD_HSTRIPS :
		(noutputs == 3 && ncellvalues[0] <= 2 && ncellvalues[1] <= 2 && ncellvalues[2] <= 2) ? RENDER_METHOD_SIMPLE :
		RENDER_METHOD_GENERIC;

	for (i = 0; i < noutputs; i++) {
		this->cellweights_double[i] = 1 / (double)ncellvalues[i];
		this->cellweights_float[i] = 1 / (float)ncellvalues[i];
	}

	libglitter_update_render_context(this, rowsize);

	return this;

fail:
	free(this->cellweights_double);
	free(this);
	return NULL;
}


#else


int
main(void)
{
#define POS(Y, X) ((Y) * ctx->rowsize + (X))

	LIBGLITTER_RENDER_CONTEXT *ctx;
	uint8_t cellmap[6];
	uint8_t ncellvalues[4];
	uint8_t a, b, c, i;

	ncellvalues[0] = 2;
	ncellvalues[1] = 16;
	ncellvalues[2] = 4;
	ncellvalues[3] = 8;
	cellmap[0] = 0, cellmap[1] = 1, cellmap[2] = 2;
	cellmap[3] = 1, cellmap[4] = 2, cellmap[5] = 0;
	ctx = libglitter_create_render_context(4, 10, 3, 2, cellmap, ncellvalues);
	ASSERT(ctx);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 0);
	ASSERT(ctx->cellmap[1] == 1);
	ASSERT(ctx->cellmap[2] == 2);
	ASSERT(ctx->cellmap[3] == 1);
	ASSERT(ctx->cellmap[4] == 2);
	ASSERT(ctx->cellmap[5] == 0);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == 2);
	ASSERT(ctx->ncellvalues[1] == 16);
	ASSERT(ctx->ncellvalues[2] == 4);
	ASSERT(ctx->ncellvalues[3] == 8);
	ASSERT(ctx->noutputs == 4);
	ASSERT(ctx->widthmul == 3);
	ASSERT(ctx->heightmul == 2);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == (double)1 / 2);
	ASSERT(ctx->cellweights_double[1] == (double)1 / 16);
	ASSERT(ctx->cellweights_double[2] == (double)1 / 4);
	ASSERT(ctx->cellweights_double[3] == (double)1 / 8);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == (float)1 / 2);
	ASSERT(ctx->cellweights_float[1] == (float)1 / 16);
	ASSERT(ctx->cellweights_float[2] == (float)1 / 4);
	ASSERT(ctx->cellweights_float[3] == (float)1 / 8);
	ASSERT(ctx->render_method == RENDER_METHOD_GENERIC);
	ASSERT(ctx->rowsize == 10);
	libglitter_update_render_context(ctx, 20);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 0);
	ASSERT(ctx->cellmap[1] == 1);
	ASSERT(ctx->cellmap[2] == 2);
	ASSERT(ctx->cellmap[3] == 1);
	ASSERT(ctx->cellmap[4] == 2);
	ASSERT(ctx->cellmap[5] == 0);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == 2);
	ASSERT(ctx->ncellvalues[1] == 16);
	ASSERT(ctx->ncellvalues[2] == 4);
	ASSERT(ctx->ncellvalues[3] == 8);
	ASSERT(ctx->noutputs == 4);
	ASSERT(ctx->widthmul == 3);
	ASSERT(ctx->heightmul == 2);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == (double)1 / 2);
	ASSERT(ctx->cellweights_double[1] == (double)1 / 16);
	ASSERT(ctx->cellweights_double[2] == (double)1 / 4);
	ASSERT(ctx->cellweights_double[3] == (double)1 / 8);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == (float)1 / 2);
	ASSERT(ctx->cellweights_float[1] == (float)1 / 16);
	ASSERT(ctx->cellweights_float[2] == (float)1 / 4);
	ASSERT(ctx->cellweights_float[3] == (float)1 / 8);
	ASSERT(ctx->render_method == RENDER_METHOD_GENERIC);
	ASSERT(ctx->rowsize == 20);
	libglitter_free_render_context(ctx);

	ncellvalues[0] = 1;
	ncellvalues[1] = 1;
	ncellvalues[2] = 1;
	cellmap[0] = 1, cellmap[1] = 0, cellmap[2] = 2;
	ctx = libglitter_create_render_context(3, 6, 3, 1, cellmap, ncellvalues);
	ASSERT(ctx);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 1);
	ASSERT(ctx->cellmap[1] == 0);
	ASSERT(ctx->cellmap[2] == 2);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == 1);
	ASSERT(ctx->ncellvalues[1] == 1);
	ASSERT(ctx->ncellvalues[2] == 1);
	ASSERT(ctx->noutputs == 3);
	ASSERT(ctx->widthmul == 3);
	ASSERT(ctx->heightmul == 1);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == 1);
	ASSERT(ctx->cellweights_double[1] == 1);
	ASSERT(ctx->cellweights_double[2] == 1);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == 1);
	ASSERT(ctx->cellweights_float[1] == 1);
	ASSERT(ctx->cellweights_float[2] == 1);
	ASSERT(ctx->render_method == RENDER_METHOD_VSTRIPS);
	ASSERT(ctx->rowsize == 6);
	libglitter_update_render_context(ctx, 15);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 1);
	ASSERT(ctx->cellmap[1] == 0);
	ASSERT(ctx->cellmap[2] == 2);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == 1);
	ASSERT(ctx->ncellvalues[1] == 1);
	ASSERT(ctx->ncellvalues[2] == 1);
	ASSERT(ctx->noutputs == 3);
	ASSERT(ctx->widthmul == 3);
	ASSERT(ctx->heightmul == 1);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == 1);
	ASSERT(ctx->cellweights_double[1] == 1);
	ASSERT(ctx->cellweights_double[2] == 1);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == 1);
	ASSERT(ctx->cellweights_float[1] == 1);
	ASSERT(ctx->cellweights_float[2] == 1);
	ASSERT(ctx->render_method == RENDER_METHOD_VSTRIPS);
	ASSERT(ctx->rowsize == 15);
	libglitter_free_render_context(ctx);

	ncellvalues[0] = 1;
	ncellvalues[1] = 1;
	ncellvalues[2] = 1;
	cellmap[0] = 1, cellmap[1] = 0, cellmap[2] = 2;
	ctx = libglitter_create_render_context(3, 6, 1, 3, cellmap, ncellvalues);
	ASSERT(ctx);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 1);
	ASSERT(ctx->cellmap[1] == 0);
	ASSERT(ctx->cellmap[2] == 2);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == 1);
	ASSERT(ctx->ncellvalues[1] == 1);
	ASSERT(ctx->ncellvalues[2] == 1);
	ASSERT(ctx->noutputs == 3);
	ASSERT(ctx->widthmul == 1);
	ASSERT(ctx->heightmul == 3);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == 1);
	ASSERT(ctx->cellweights_double[1] == 1);
	ASSERT(ctx->cellweights_double[2] == 1);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == 1);
	ASSERT(ctx->cellweights_float[1] == 1);
	ASSERT(ctx->cellweights_float[2] == 1);
	ASSERT(ctx->render_method == RENDER_METHOD_HSTRIPS);
	ASSERT(ctx->rowsize == 6);
	libglitter_update_render_context(ctx, 15);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 1);
	ASSERT(ctx->cellmap[1] == 0);
	ASSERT(ctx->cellmap[2] == 2);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == 1);
	ASSERT(ctx->ncellvalues[1] == 1);
	ASSERT(ctx->ncellvalues[2] == 1);
	ASSERT(ctx->noutputs == 3);
	ASSERT(ctx->widthmul == 1);
	ASSERT(ctx->heightmul == 3);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == 1);
	ASSERT(ctx->cellweights_double[1] == 1);
	ASSERT(ctx->cellweights_double[2] == 1);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == 1);
	ASSERT(ctx->cellweights_float[1] == 1);
	ASSERT(ctx->cellweights_float[2] == 1);
	ASSERT(ctx->render_method == RENDER_METHOD_HSTRIPS);
	ASSERT(ctx->rowsize == 15);
	libglitter_free_render_context(ctx);

	for (i = 1; i < 8; i++) {
		ncellvalues[0] = a = 2 + !!(i & 1);
		ncellvalues[1] = b = 2 + !!(i & 2);
		ncellvalues[2] = c = 2 + !!(i & 4);
		cellmap[0] = 0, cellmap[1] = 1, cellmap[2] = 2;
		cellmap[3] = 1, cellmap[4] = 2, cellmap[5] = 0;
		ctx = libglitter_create_render_context(3, 8, 3, 2, cellmap, ncellvalues);
		ASSERT(ctx);
		ASSERT(ctx->cellmap);
		ASSERT(ctx->cellmap[0] == 0);
		ASSERT(ctx->cellmap[1] == 1);
		ASSERT(ctx->cellmap[2] == 2);
		ASSERT(ctx->cellmap[3] == 1);
		ASSERT(ctx->cellmap[4] == 2);
		ASSERT(ctx->cellmap[5] == 0);
		ASSERT(ctx->ncellvalues);
		ASSERT(ctx->ncellvalues[0] == a);
		ASSERT(ctx->ncellvalues[1] == b);
		ASSERT(ctx->ncellvalues[2] == c);
		ASSERT(ctx->noutputs == 3);
		ASSERT(ctx->widthmul == 3);
		ASSERT(ctx->heightmul == 2);
		ASSERT(ctx->cellweights_double);
		ASSERT(ctx->cellweights_double[0] == (double)1 / a);
		ASSERT(ctx->cellweights_double[1] == (double)1 / b);
		ASSERT(ctx->cellweights_double[2] == (double)1 / c);
		ASSERT(ctx->cellweights_float);
		ASSERT(ctx->cellweights_float[0] == (float)1 / a);
		ASSERT(ctx->cellweights_float[1] == (float)1 / b);
		ASSERT(ctx->cellweights_float[2] == (float)1 / c);
		ASSERT(ctx->render_method == RENDER_METHOD_GENERIC);
		ASSERT(ctx->rowsize == 8);
		libglitter_update_render_context(ctx, 30);
		ASSERT(ctx->cellmap);
		ASSERT(ctx->cellmap[0] == 0);
		ASSERT(ctx->cellmap[1] == 1);
		ASSERT(ctx->cellmap[2] == 2);
		ASSERT(ctx->cellmap[3] == 1);
		ASSERT(ctx->cellmap[4] == 2);
		ASSERT(ctx->cellmap[5] == 0);
		ASSERT(ctx->ncellvalues);
		ASSERT(ctx->ncellvalues[0] == a);
		ASSERT(ctx->ncellvalues[1] == b);
		ASSERT(ctx->ncellvalues[2] == c);
		ASSERT(ctx->noutputs == 3);
		ASSERT(ctx->widthmul == 3);
		ASSERT(ctx->heightmul == 2);
		ASSERT(ctx->cellweights_double);
		ASSERT(ctx->cellweights_double[0] == (double)1 / a);
		ASSERT(ctx->cellweights_double[1] == (double)1 / b);
		ASSERT(ctx->cellweights_double[2] == (double)1 / c);
		ASSERT(ctx->cellweights_float);
		ASSERT(ctx->cellweights_float[0] == (float)1 / a);
		ASSERT(ctx->cellweights_float[1] == (float)1 / b);
		ASSERT(ctx->cellweights_float[2] == (float)1 / c);
		ASSERT(ctx->render_method == RENDER_METHOD_GENERIC);
		ASSERT(ctx->rowsize == 30);
		libglitter_free_render_context(ctx);
	}

	for (i = 0; i < 8; i++) {
		ncellvalues[0] = a = 1 + !!(i & 1);
		ncellvalues[1] = b = 1 + !!(i & 2);
		ncellvalues[2] = c = 1 + !!(i & 4);
		cellmap[0] = 0, cellmap[1] = 1, cellmap[2] = 2;
		cellmap[3] = 1, cellmap[4] = 2, cellmap[5] = 0;
		ctx = libglitter_create_render_context(3, 8, 3, 2, cellmap, ncellvalues);
		ASSERT(ctx);
		ASSERT(ctx->cellmap);
		ASSERT(ctx->cellmap[0] == 0);
		ASSERT(ctx->cellmap[1] == 1);
		ASSERT(ctx->cellmap[2] == 2);
		ASSERT(ctx->cellmap[3] == 1);
		ASSERT(ctx->cellmap[4] == 2);
		ASSERT(ctx->cellmap[5] == 0);
		ASSERT(ctx->ncellvalues);
		ASSERT(ctx->ncellvalues[0] == a);
		ASSERT(ctx->ncellvalues[1] == b);
		ASSERT(ctx->ncellvalues[2] == c);
		ASSERT(ctx->noutputs == 3);
		ASSERT(ctx->widthmul == 3);
		ASSERT(ctx->heightmul == 2);
		ASSERT(ctx->cellweights_double);
		ASSERT(ctx->cellweights_double[0] == (double)1 / a);
		ASSERT(ctx->cellweights_double[1] == (double)1 / b);
		ASSERT(ctx->cellweights_double[2] == (double)1 / c);
		ASSERT(ctx->cellweights_float);
		ASSERT(ctx->cellweights_float[0] == (float)1 / a);
		ASSERT(ctx->cellweights_float[1] == (float)1 / b);
		ASSERT(ctx->cellweights_float[2] == (float)1 / c);
		ASSERT(ctx->render_method == RENDER_METHOD_SIMPLE);
		ASSERT(ctx->rowsize == 8);
		ASSERT(ctx->cells[0][0] == POS(0, 0) || ctx->cells[0][0] == POS(1, 2));
		ASSERT(ctx->cells[0][1] == POS(1, 2) || ctx->cells[0][1] == POS(0, 0));
		ASSERT(ctx->cells[1][0] == POS(0, 1) || ctx->cells[1][0] == POS(1, 0));
		ASSERT(ctx->cells[1][1] == POS(1, 0) || ctx->cells[1][1] == POS(0, 1));
		ASSERT(ctx->cells[2][0] == POS(0, 2) || ctx->cells[2][0] == POS(1, 1));
		ASSERT(ctx->cells[2][1] == POS(1, 1) || ctx->cells[2][1] == POS(0, 2));
		ASSERT(ctx->cells[0][0] != ctx->cells[0][1]);
		ASSERT(ctx->cells[1][0] != ctx->cells[1][1]);
		ASSERT(ctx->cells[2][0] != ctx->cells[2][1]);
		libglitter_update_render_context(ctx, 30);
		ASSERT(ctx->cellmap);
		ASSERT(ctx->cellmap[0] == 0);
		ASSERT(ctx->cellmap[1] == 1);
		ASSERT(ctx->cellmap[2] == 2);
		ASSERT(ctx->cellmap[3] == 1);
		ASSERT(ctx->cellmap[4] == 2);
		ASSERT(ctx->cellmap[5] == 0);
		ASSERT(ctx->ncellvalues);
		ASSERT(ctx->ncellvalues[0] == a);
		ASSERT(ctx->ncellvalues[1] == b);
		ASSERT(ctx->ncellvalues[2] == c);
		ASSERT(ctx->noutputs == 3);
		ASSERT(ctx->widthmul == 3);
		ASSERT(ctx->heightmul == 2);
		ASSERT(ctx->cellweights_double);
		ASSERT(ctx->cellweights_double[0] == (double)1 / a);
		ASSERT(ctx->cellweights_double[1] == (double)1 / b);
		ASSERT(ctx->cellweights_double[2] == (double)1 / c);
		ASSERT(ctx->cellweights_float);
		ASSERT(ctx->cellweights_float[0] == (float)1 / a);
		ASSERT(ctx->cellweights_float[1] == (float)1 / b);
		ASSERT(ctx->cellweights_float[2] == (float)1 / c);
		ASSERT(ctx->render_method == RENDER_METHOD_SIMPLE);
		ASSERT(ctx->rowsize == 30);
		ASSERT(ctx->cells[0][0] == POS(0, 0) || ctx->cells[0][0] == POS(1, 2));
		ASSERT(ctx->cells[0][1] == POS(1, 2) || ctx->cells[0][1] == POS(0, 0));
		ASSERT(ctx->cells[1][0] == POS(0, 1) || ctx->cells[1][0] == POS(1, 0));
		ASSERT(ctx->cells[1][1] == POS(1, 0) || ctx->cells[1][1] == POS(0, 1));
		ASSERT(ctx->cells[2][0] == POS(0, 2) || ctx->cells[2][0] == POS(1, 1));
		ASSERT(ctx->cells[2][1] == POS(1, 1) || ctx->cells[2][1] == POS(0, 2));
		ASSERT(ctx->cells[0][0] != ctx->cells[0][1]);
		ASSERT(ctx->cells[1][0] != ctx->cells[1][1]);
		ASSERT(ctx->cells[2][0] != ctx->cells[2][1]);
		libglitter_free_render_context(ctx);
	}

	ncellvalues[0] = a = 1;
	ncellvalues[1] = b = 2;
	ncellvalues[2] = c = 1;
	cellmap[0] = 1, cellmap[1] = 2;
	cellmap[2] = 0, cellmap[3] = 1;
	ctx = libglitter_create_render_context(3, 8, 2, 2, cellmap, ncellvalues);
	ASSERT(ctx);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 1);
	ASSERT(ctx->cellmap[1] == 2);
	ASSERT(ctx->cellmap[2] == 0);
	ASSERT(ctx->cellmap[3] == 1);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == a);
	ASSERT(ctx->ncellvalues[1] == b);
	ASSERT(ctx->ncellvalues[2] == c);
	ASSERT(ctx->noutputs == 3);
	ASSERT(ctx->widthmul == 2);
	ASSERT(ctx->heightmul == 2);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == (double)1 / a);
	ASSERT(ctx->cellweights_double[1] == (double)1 / b);
	ASSERT(ctx->cellweights_double[2] == (double)1 / c);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == (float)1 / a);
	ASSERT(ctx->cellweights_float[1] == (float)1 / b);
	ASSERT(ctx->cellweights_float[2] == (float)1 / c);
	ASSERT(ctx->render_method == RENDER_METHOD_SIMPLE);
	ASSERT(ctx->rowsize == 8);
	ASSERT(ctx->cells[0][0] == POS(1, 0));
	ASSERT(ctx->cells[1][0] == POS(0, 0) || ctx->cells[1][0] == POS(1, 1));
	ASSERT(ctx->cells[1][1] == POS(1, 1) || ctx->cells[1][1] == POS(0, 0));
	ASSERT(ctx->cells[2][0] == POS(0, 1));
	ASSERT(ctx->cells[0][0] == ctx->cells[0][1]);
	ASSERT(ctx->cells[1][0] != ctx->cells[1][1]);
	ASSERT(ctx->cells[2][0] == ctx->cells[2][1]);
	libglitter_update_render_context(ctx, 30);
	ASSERT(ctx->cellmap);
	ASSERT(ctx->cellmap[0] == 1);
	ASSERT(ctx->cellmap[1] == 2);
	ASSERT(ctx->cellmap[2] == 0);
	ASSERT(ctx->cellmap[3] == 1);
	ASSERT(ctx->ncellvalues);
	ASSERT(ctx->ncellvalues[0] == a);
	ASSERT(ctx->ncellvalues[1] == b);
	ASSERT(ctx->ncellvalues[2] == c);
	ASSERT(ctx->noutputs == 3);
	ASSERT(ctx->widthmul == 2);
	ASSERT(ctx->heightmul == 2);
	ASSERT(ctx->cellweights_double);
	ASSERT(ctx->cellweights_double[0] == (double)1 / a);
	ASSERT(ctx->cellweights_double[1] == (double)1 / b);
	ASSERT(ctx->cellweights_double[2] == (double)1 / c);
	ASSERT(ctx->cellweights_float);
	ASSERT(ctx->cellweights_float[0] == (float)1 / a);
	ASSERT(ctx->cellweights_float[1] == (float)1 / b);
	ASSERT(ctx->cellweights_float[2] == (float)1 / c);
	ASSERT(ctx->render_method == RENDER_METHOD_SIMPLE);
	ASSERT(ctx->rowsize == 30);
	ASSERT(ctx->cells[0][0] == POS(1, 0));
	ASSERT(ctx->cells[1][0] == POS(0, 0) || ctx->cells[1][0] == POS(1, 1));
	ASSERT(ctx->cells[1][1] == POS(1, 1) || ctx->cells[1][1] == POS(0, 0));
	ASSERT(ctx->cells[2][0] == POS(0, 1));
	ASSERT(ctx->cells[0][0] == ctx->cells[0][1]);
	ASSERT(ctx->cells[1][0] != ctx->cells[1][1]);
	ASSERT(ctx->cells[2][0] == ctx->cells[2][1]);
	libglitter_free_render_context(ctx);

#undef POS

	return 0;
}


#endif
