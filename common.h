/* See LICENSE file for copyright and license details. */
#ifndef LIBGLITTER_COMMON_H
#define LIBGLITTER_COMMON_H

#include "libglitter.h"
#include <alloca.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic ignored "-Wunsuffixed-float-constants"
# pragma GCC diagnostic ignored "-Wunused-macros"
# pragma GCC diagnostic ignored "-Wpadded"
#elif defined(__clang__)
# pragma clang diagnostic ignored "-Wunused-macros"
# pragma clang diagnostic ignored "-Wkeyword-macro"
# pragma clang diagnostic ignored "-Walloca"
# pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
# pragma clang diagnostic ignored "-Wcomma"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wvla"
#endif


#define RENDER_METHOD_VSTRIPS 0
#define RENDER_METHOD_HSTRIPS 1
#define RENDER_METHOD_SIMPLE 2
#define RENDER_METHOD_GENERIC 3


struct libglitter_render_context {
	const uint8_t *restrict cellmap;
	const uint8_t *restrict ncellvalues;
	size_t noutputs;
	size_t rowsize;
	size_t widthmul;
	size_t heightmul;
	uint8_t render_method;
	size_t cells[3][2];
	double *restrict cellweights_double;
	float *restrict cellweights_float;
};


#ifdef TEST

#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic ignored "-Wfloat-equal"
# pragma GCC diagnostic ignored "-Wfloat-conversion"
# pragma GCC diagnostic ignored "-Wdouble-promotion"
#elif defined(__clang__)
# pragma clang diagnostic ignored "-Wfloat-equal"
# pragma clang diagnostic ignored "-Wdouble-promotion"
# pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#endif

# define ASSERT(ASSERTION)\
	do {\
		if (!(ASSERTION)) {\
			fprintf(stderr, "Failed assertion at line %i: %s\n", __LINE__, #ASSERTION);\
			exit(1);\
		}\
	} while (0)

#endif

#endif
