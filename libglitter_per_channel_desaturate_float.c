/* See LICENSE file for copyright and license details. */
#include "common.h"
#define libglitter_per_channel_desaturate_double libglitter_per_channel_desaturate_float
#define double float
#define fma fmaf
#include "libglitter_per_channel_desaturate_double.c"
