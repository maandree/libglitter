/* See LICENSE file for copyright and license details. */
#include "common.h"


void
libglitter_free_render_context(LIBGLITTER_RENDER_CONTEXT *this)
{
	if (this) {
		free(this->cellweights_double);
		free(this->cellweights_float);
		free(this);
	}
}
