/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


int
libglitter_enable_acceleration(uint64_t features, int async, void (*callback)(int r, int e, void *u), void *userdata, void *unused)
{
	(void) features;
	(void) async;
	(void) unused;

	/* TODO add support for hardware acceleration */

	if (callback)
		callback(0, 0, userdata);

	return 0;
}


#else


static size_t callback_calls = 0;

static void
callback(int ret, int err, void *user)
{
	callback_calls++;
	ASSERT(!ret);
	ASSERT(!err);
	ASSERT(user);
	ASSERT(*(const char *)user == 'u');
}

static void
callback_null(int ret, int err, void *user)
{
	callback_calls++;
	ASSERT(!ret);
	ASSERT(!err);
	ASSERT(!user);
}
int
main(void)
{
	char data = 'u';
	ASSERT(!libglitter_enable_acceleration(0, 0, NULL, NULL, NULL));
	ASSERT(!libglitter_enable_acceleration(0, 1, NULL, NULL, NULL));
	ASSERT(!libglitter_enable_acceleration(~0, 0, NULL, NULL, NULL));
	ASSERT(!libglitter_enable_acceleration(~0, 1, NULL, NULL, NULL));
	ASSERT(!libglitter_enable_acceleration(0, 0, NULL, &data, NULL));
	ASSERT(!libglitter_enable_acceleration(0, 1, NULL, &data, NULL));
	ASSERT(!libglitter_enable_acceleration(~0, 0, NULL, &data, NULL));
	ASSERT(!libglitter_enable_acceleration(~0, 1, NULL, &data, NULL));
	ASSERT(callback_calls == 0);
	ASSERT(!libglitter_enable_acceleration(0, 0, callback, &data, NULL));
	ASSERT(callback_calls == 1);
	ASSERT(!libglitter_enable_acceleration(0, 1, callback, &data, NULL));
	ASSERT(callback_calls == 2);
	ASSERT(!libglitter_enable_acceleration(~0, 0, callback, &data, NULL));
	ASSERT(callback_calls == 3);
	ASSERT(!libglitter_enable_acceleration(~0, 1, callback, &data, NULL));
	ASSERT(callback_calls == 4);
	ASSERT(!libglitter_enable_acceleration(0, 0, callback_null, NULL, NULL));
	ASSERT(callback_calls == 5);
	ASSERT(!libglitter_enable_acceleration(0, 1, callback_null, NULL, NULL));
	ASSERT(callback_calls == 6);
	ASSERT(!libglitter_enable_acceleration(~0, 0, callback_null, NULL, NULL));
	ASSERT(callback_calls == 7);
	ASSERT(!libglitter_enable_acceleration(~0, 1, callback_null, NULL, NULL));
	ASSERT(callback_calls == 8);
	return 0;
}


#endif
