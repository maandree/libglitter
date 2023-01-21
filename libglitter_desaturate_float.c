/* See LICENSE file for copyright and license details. */
#include "common.h"
#define libglitter_desaturate_double libglitter_desaturate_float
#define double float
#define fma fmaf
#include "libglitter_desaturate_double.c"
