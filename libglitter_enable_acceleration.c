/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libglitter_enable_acceleration(uint64_t features, int async, void (*callback)(int r, int e, void *u), void *userdata)
{
	(void) features;
	(void) async;

	/* TODO add support for hardware acceleration */

	if (callback)
		callback(0, 0, userdata);

	return 0;
}
