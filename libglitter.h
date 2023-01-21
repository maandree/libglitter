/* See LICENSE file for copyright and license details. */
#ifndef LIBGLITTER_H
#define LIBGLITTER_H

#include <stddef.h>
#include <stdint.h>


/**
 * Create one raster per monitor colour from a raster of
 * subpixels (which may be further divided in the raster)
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
 * @param  ncellweights     For each `i` in [0, `noutputs`), `ncellvalues[i]`
 *                          shall have the value `1. / x`, where `x` is the
 *                          number of times the value `i` occurs in `cellmap`
 * 
 * The initialisation requirement on the buffers in `outputs`
 * does not apply if `noutputs` is 3 and either `widthmul` or
 * `heightmul` is 3 and the other one is 1; additionally, in
 * this case `ncellweights` will not be used as it is preknown
 * that all it's values are 1
 */
void libglitter_compose_double(double **, size_t, const double *restrict,
                               size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                               const uint8_t *restrict, const double *restrict);

/**
 * Create one raster per monitor colour from a raster of
 * subpixels (which may be further divided in the raster)
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
 * @param  ncellweights     For each `i` in [0, `noutputs`), `ncellvalues[i]`
 *                          shall have the value `1. / x`, where `x` is the
 *                          number of times the value `i` occurs in `cellmap`
 * 
 * The initialisation requirement on the buffers in `outputs`
 * does not apply if `noutputs` is 3 and either `widthmul` or
 * `heightmul` is 3 and the other one is 1; additionally, in
 * this case `ncellweights` will not be used as it is preknown
 * that all it's values are 1
 */
void libglitter_compose_float(float **, size_t, const float *restrict,
                              size_t, size_t, size_t, size_t, size_t, size_t, size_t,
                              const uint8_t *restrict, const float *restrict);

/* TODO add integer versions */
/* TODO add colour space conversion and saturation */


#endif
