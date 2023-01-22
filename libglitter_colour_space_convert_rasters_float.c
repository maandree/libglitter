/* See LICENSE file for copyright and license details. */
#include "common.h"
#define libglitter_colour_space_convert_rasters_double libglitter_colour_space_convert_rasters_float
#define double float
#define fma fmaf
#include "libglitter_colour_space_convert_rasters_double.c"
