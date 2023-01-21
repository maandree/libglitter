/* See LICENSE file for copyright and license details. */
#ifndef LIBGLITTER_H
#define LIBGLITTER_H

#include <stddef.h>
#include <stdint.h>


/**
 * Rendering context for an input raster's specifications
 * 
 * Render context's are thread-safe, however updating them are not
 */
typedef struct libglitter_render_context LIBGLITTER_RENDER_CONTEXT;


/**
 * Create a render context for an input raster's specifications
 * 
 * Render context's are thread-safe
 * 
 * @param  noutputs     The number of output buffers
 * @param  rowsize      The number of cells per row in the input raster
 * @param  widthmul     The horizontal number of cells per pixel in the input raster
 * @param  heightmul    The vertical number of cells per pixel in the input raster
 * @param  cellmap      Subpixel map of size `heightmul * widthmul` that maps a
 *                      cell for a pixel in the input raster to an output raster
 *                      (the output raster's index); this is row-major map, and
 *                      each valid value must appear at least once
 * @param  ncellvalues  For each `i` in [0, `noutputs`), `ncellvalues[i]`
 *                      shall be the number of times the value `i` occurs
 *                      in `cellmap`
 * @return              A render context that can be deallocate using
 *                      `libglitter_free_render_context` or updated with
 *                      `libglitter_update_render_context`; `NULL` on failure
 *
 * @throws  ENOMEM  Couldn't not allocate enough memory
 * 
 * If `noutputs` is 3 and either `widthmul` or `heightmul` is 3 and
 * the other one is 1, `ncellvalues` will not be used as it is preknown
 * that all its values are 1
 */
LIBGLITTER_RENDER_CONTEXT *libglitter_create_render_context(size_t, size_t, size_t, size_t,
                                                            const uint8_t *, const uint8_t *);

/**
 * Update a render context (created by `libglitter_create_render_context`)
 * for a new input raster buffer
 * 
 * Updating a render context is not thread-safe
 * 
 * @param  this     The render context to refresh
 * @param  rowsize  The number of cells per row in the input raster
 */
void libglitter_update_render_context(LIBGLITTER_RENDER_CONTEXT *, size_t);

/**
 * Deallocates a render context (created by `libglitter_create_render_context`)
 * 
 * @param  this  The render context to deallocate
 */
void libglitter_free_render_context(LIBGLITTER_RENDER_CONTEXT *);


/**
 * Create one raster per monitor colour from a raster of
 * subpixels (which may be further divided in the raster)
 * 
 * Note that this function does not regard the colour
 * model's transfer function: it does not directly give
 * you appropriate sRGB values
 * 
 * @param  outputs          Array of output rasters, on for each subpixel colour.
 *                          The function may change the offset for each raster,
 *                          as such, the given pointer shall not be used anywhere
 *                          else during the execution of the function and the
 *                          inner pointers shall be considered undefined after
 *                          the execution of the function
 * @param  input            Input raster; cells are adjacent
 * @param  output_rowsize   The number of cells in a row in each output raster
 * @param  output_cellsize  The number of values stored in each output raster,
 *                          between each cell, plus 1 (that is, the number of
 *                          values per cell)
 * @param  width            The horizontal number of pixels in the rasters
 * @param  height           The vertical number of pixels in the rasters
 * @param  render_ctx       Rendering context created for the input raster's
 *                          specification
 */
void libglitter_compose_double(double **, const double *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *);

/**
 * This value is identical to `libglitter_compose_double`,
 * apart from it parameter types, see `libglitter_compose_double`
 * for details about this function
 */
void libglitter_compose_float(float **, const float *restrict, size_t, size_t,
                              size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *);

/**
 * Create one raster per monitor colour from a raster of
 * subpixels (which may be further divided in the raster)
 * 
 * Note that this function does not regard the colour
 * model's transfer function: it does not directly give
 * you appropriate sRGB values
 * 
 * @param  outputs          Array of output rasters, on for each subpixel colour.
 *                          The function may change the offset for each raster,
 *                          as such, the given pointer shall not be used anywhere
 *                          else during the execution of the function and the
 *                          inner pointers shall be considered undefined after
 *                          the execution of the function
 * @param  input            Input raster; cells are adjacent
 * @param  output_rowsize   The number of cells in a row in each output raster
 * @param  output_cellsize  The number of values stored in each output raster,
 *                          between each cell, plus 1 (that is, the number of
 *                          values per cell)
 * @param  width            The horizontal number of pixels in the rasters
 * @param  height           The vertical number of pixels in the rasters
 * @param  render_ctx       Rendering context created for the input raster's
 *                          specification
 */
void libglitter_compose_uint64(uint64_t **, const uint64_t *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
void libglitter_compose_uint32(uint32_t **, const uint32_t *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
void libglitter_compose_uint16(uint16_t **, const uint16_t *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
void libglitter_compose_uint8(uint8_t **, const uint8_t *restrict, size_t, size_t,
                              size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *);


#endif
