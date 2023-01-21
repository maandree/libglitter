/* See LICENSE file for copyright and license details. */
#include "libglitter.h"
typedef uint_fast64_t greater_t;
#define greater_t greater_t
#define libglitter_compose_uint64 libglitter_compose_uint32
#define uint64_t uint32_t
#include "libglitter_compose_uint64.c"
