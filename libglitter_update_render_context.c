/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


void
libglitter_update_render_context(LIBGLITTER_RENDER_CONTEXT *this, size_t rowsize)
{
	size_t y, x;
	uint8_t channel;
	size_t cellsi[3] = {0, 0, 0};

	if (this && this->render_method == RENDER_METHOD_SIMPLE && this->rowsize != rowsize) {
		this->rowsize = rowsize;
		for (y = 0; y < this->heightmul; y++) {
			for (x = 0; x < this->widthmul; x++) {
				channel = this->cellmap[y * this->widthmul + x];
				this->cells[channel][cellsi[channel]] = y * rowsize + x;
				cellsi[channel] += 1;
			}
		}
		if (cellsi[0] == 1)
			this->cells[0][1] = this->cells[0][0];
		if (cellsi[1] == 1)
			this->cells[1][1] = this->cells[1][0];
		if (cellsi[2] == 1)
			this->cells[2][1] = this->cells[2][0];
	}
}


#else


int
main(void)
{
	return 0; /* TODO add test */
}


#endif
