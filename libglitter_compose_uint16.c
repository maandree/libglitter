/* See LICENSE file for copyright and license details. */
#include "libglitter.h"
typedef uint_fast32_t greater_t;
#define greater_t greater_t
#define libglitter_compose_uint64 libglitter_compose_uint16
#define uint64_t uint16_t
#include "libglitter_compose_uint64.c"
