/* See LICENSE file for copyright and license details. */
#include "common.h"


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
	libglitter_free_render_context(this);
	return NULL;
}
