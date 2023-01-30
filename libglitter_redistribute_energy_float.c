/* See LICENSE file for copyright and license details. */
#include "common.h"
#define libglitter_redistribute_energy_double libglitter_redistribute_energy_float
#define double float
#define fma fmaf
#include "libglitter_redistribute_energy_double.c"
