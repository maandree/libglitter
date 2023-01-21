/* See LICENSE file for copyright and license details. */
#ifndef LIBGLITTER_H
#define LIBGLITTER_H

#include <stddef.h>
#include <stdint.h>


/**
 * Create one raster per monitor colour from a raster of
 * subpixels (which may be further divided in the raster)
 * 
 * Note that this function does not regard the colour
 * model's transfer function: it does not directly give
 * you appropriate sRGB values
 * 
 * @param  outputs          Array of output rasters, on for each subpixel colour;
 *                          each raster must be initialised to have the value 0
 *                          in each cell. The function may change the offset for
 *                          each raster, as such, the given pointer shall not be
 *                          used anywhere else during the execution of the function
 *                          and the inner pointers shall be considered undefined
 *                          after the execution of the function
 * @param  noutputs         The number of output buffers
 * @param  input            Input raster; cells are adjacent
 * @param  output_rowsize   The number of cells in a row in each output raster
 * @param  output_cellsize  The number of values stored in each output raster,
 *                          between each cell, plus 1 (that is, the number of
 *                          values per cell)
 * @param  input_rowsize    The number of cells in a row in `input`
 * @param  width            The horizontal number of pixels in the rasters
 * @param  height           The vertical number of pixels in the rasters
 * @param  widthmul         The horizontal number of cells per pixel in `input`
 * @param  heightmul        The vertical number of cells per pixel in `input`
 * @param  cellmap          Subpixel map of size `heightmul * widthmul` that
 *                          maps a cell for a pixel in `input` to an output
 *                          raster (the value of the cell is the output's index
 *                          in `outputs`); this is row-major map, and each valid
 *                          value must appear at least once
 * @param  ncellweights     For each `i` in [0, `noutputs`), `ncellweights[i]`
 *                          shall have the value `1. / x`, where `x` is the
 *                          number of times the value `i` occurs in `cellmap`
 * 
 * The initialisation requirement on the buffers in `outputs`
 * does not apply if `noutputs` is 3 and either `widthmul` or
 * `heightmul` is 3 and the other one is 1; additionally, in
 * this case `ncellweights` will not be used as it is preknown
 * that all its values are 1
 */
void libglitter_compose_double(double **, size_t, const double *restrict,
                               size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                               const uint8_t *restrict, const double *restrict);

/**
 * This value is identical to `libglitter_compose_double`,
 * apart from it parameter types, see `libglitter_compose_double`
 * for details about this function
 */
void libglitter_compose_float(float **, size_t, const float *restrict,
                              size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                              const uint8_t *restrict, const float *restrict);

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
 * @param  noutputs         The number of output buffers
 * @param  input            Input raster; cells are adjacent
 * @param  output_rowsize   The number of cells in a row in each output raster
 * @param  output_cellsize  The number of values stored in each output raster,
 *                          between each cell, plus 1 (that is, the number of
 *                          values per cell)
 * @param  input_rowsize    The number of cells in a row in `input`
 * @param  width            The horizontal number of pixels in the rasters
 * @param  height           The vertical number of pixels in the rasters
 * @param  widthmul         The horizontal number of cells per pixel in `input`
 * @param  heightmul        The vertical number of cells per pixel in `input`
 * @param  cellmap          Subpixel map of size `heightmul * widthmul` that
 *                          maps a cell for a pixel in `input` to an output
 *                          raster (the value of the cell is the output's index
 *                          in `outputs`); this is row-major map, and each valid
 *                          value must appear at least once
 * @param  ncellvalues      For each `i` in [0, `noutputs`), `ncellvalues[i]`
 *                          shall be the number of times the value `i` occurs
 *                          in `cellmap`
 * 
 * If `noutputs` is 3 and either `widthmul` or `heightmul` is 3 and
 * the other one is 1, `ncellvalues` will not be used as it is preknown
 * that all its values are 1
 */
void libglitter_compose_uint64(uint64_t **, size_t, const uint64_t *restrict,
                               size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                               const uint8_t *restrict, const uint8_t *restrict);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
void libglitter_compose_uint32(uint32_t **, size_t, const uint32_t *restrict,
                               size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                               const uint8_t *restrict, const uint8_t *restrict);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
void libglitter_compose_uint16(uint16_t **, size_t, const uint16_t *restrict,
                               size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                               const uint8_t *restrict, const uint8_t *restrict);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
void libglitter_compose_uint8(uint8_t **, size_t, const uint8_t *restrict,
                              size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                              const uint8_t *restrict, const uint8_t *restrict);

/* TODO add colour space conversion and saturation */


#endif
