/* See LICENSE file for copyright and license details. */
#ifndef LIBGLITTER_H
#define LIBGLITTER_H

#include <stddef.h>
#include <stdint.h>


/**
 * The application will use `double`-typed rasters
 */
#define LIBGLITTER_FEATURE_DOUBLE_TYPE  UINT64_C(0x0000000000000001)

/**
 * The application will use `float`-typed rasters
 */
#define LIBGLITTER_FEATURE_FLOAT_TYPE   UINT64_C(0x0000000000000002)

/**
 * The application will use `uint64_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT64_TYPE  UINT64_C(0x0000000000000004)

/**
 * The application will use `uint32_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT32_TYPE  UINT64_C(0x0000000000000008)

/**
 * The application will use `uint16_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT16_TYPE  UINT64_C(0x0000000000000010)

/**
 * The application will use `uint8_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT8_TYPE   UINT64_C(0x0000000000000020)

/**
 * The allocation will use at least one of the functions
 * `libglitter_compose_double`,
 * `libglitter_compose_float`,
 * `libglitter_compose_uint64`,
 * `libglitter_compose_uint32`,
 * `libglitter_compose_uint16`, and
 * `libglitter_compose_uint8`
 */
#define LIBGLITTER_FEATURE_COMPOSE      UINT64_C(0x0000000000000040)

/**
 * The allocation will use at least one of the functions
 * `libglitter_desaturate_double`, and
 * `libglitter_desaturate_float`
 */
#define LIBGLITTER_FEATURE_DESATURATION UINT64_C(0x0000000000000080)

/**
 * The allocation will use at least one of the functions
 * `libglitter_colour_space_convert_rasters_double`, and
 * `libglitter_colour_space_convert_rasters_float`
 */
#define LIBGLITTER_FEATURE_COLOUR_SPACE UINT64_C(0x0000000000000100)


/**
 * The CIE xyY values of the D65 illuminant
 */
#define LIBGLITTER_ILLUMINANT_D65\
	0.312726871026564878786047074755,\
	0.329023206641284038376227272238, 1.0\


/**
 * Rendering context for an input raster's specifications
 * 
 * Render context's are thread-safe, however updating them are not
 */
typedef struct libglitter_render_context LIBGLITTER_RENDER_CONTEXT;


/**
 * Output primary colour
 */
enum libglitter_colour {
	/**
	 * Red primary colour
	 */
	LIBGLITTER_CHANNEL_RED = 0,

	/**
	 * Green primary colour
	 */
	LIBGLITTER_CHANNEL_GREEN = 1,

	/**
	 * Blue primary colour
	 */
	LIBGLITTER_CHANNEL_BLUE = 2
};


/**
 * Attempt to enable hardware acceleration
 * 
 * Currently this function doesn't do anything but call the
 * callback function and return 0; it is provided for application
 * to use know and get hardware acceleration when it is implemented
 * 
 * @param   features  Set of features to enable hardware acceleration
 *                    for, unrecognised bits are silently ignored.
 *                    Recognised bit values are:
 *                    - LIBGLITTER_FEATURE_DOUBLE_TYPE,
 *                    - LIBGLITTER_FEATURE_FLOAT_TYPE,
 *                    - LIBGLITTER_FEATURE_UINT64_TYPE,
 *                    - LIBGLITTER_FEATURE_UINT32_TYPE,
 *                    - LIBGLITTER_FEATURE_UINT16_TYPE,
 *                    - LIBGLITTER_FEATURE_UINT8_TYPE,
 *                    - LIBGLITTER_FEATURE_COMPOSE,
 *                    - LIBGLITTER_FEATURE_DESATURATION, and
 *                    - LIBGLITTER_FEATURE_COLOUR_SPACE.
 * @param   async     Whether the function shall return immediately
 *                    rather than when complete
 * @param   callback  Callback function that is called at function
 *                    completion, the first argument will be either
 *                    1 on success with hardware acceleration enabled,
 *                    0 on success without hardware acceleration enabled,
 *                    or -1 on failure; the second argument will be 0
 *                    on success or an error code on failure; the third
 *                    argument will be `userdata`. This argument may
 *                    also be `NULL` if no callback function shall be
 *                    called
 * @param   userdata  User-specific data to pass into `callback`, may be `NULL`
 * @return            1 on success with hardware acceleration enabled,
 *                    0 on success without hardware acceleration enabled,
 *                    or -1 on failure
 */
int libglitter_enable_acceleration(uint64_t, int, void (*)(int, int, void *), void *);


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
 * @param  outputs          Array of output rasters, one for each subpixel colour.
 *                          The function may change the offset for each raster,
 *                          as such, the given pointer shall not be used anywhere
 *                          else during the execution of the function and the
 *                          inner pointers shall be considered undefined after
 *                          the execution of the function.
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
 * @param  outputs          Array of output rasters, one for each subpixel colour.
 *                          The function may change the offset for each raster,
 *                          as such, the given pointer shall not be used anywhere
 *                          else during the execution of the function and the
 *                          inner pointers shall be considered undefined after
 *                          the execution of the function.
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


/**
 * Reorders a set of three rasters
 * 
 * When the function `libglitter_create_render_context` is called,
 * the parameter named `cellmap` contains the values 0, 1, and 2,
 * representing different channels: indices in the raster array.
 * The values are not necessarily 0 for red, 1 for green, and 2 for blue,
 * but this function lets the user reorder the rasters so that before
 * calling this function the rasters may be in this order, but when
 * call a `libglitter_compose_*` function after calling this function,
 * that function will write to the correct rasters
 * 
 * @param  rasters  The array of the three rasters, on input the rasters
 *                  shall be in the order (0) red, (1) green, (2) blue;
 *                  the function will reorder them
 * @param  colour1  The colour of the channel value 0 represents in `cellmap`
 * @param  colour2  The colour of the channel value 1 represents in `cellmap`
 * @param  colour3  The colour of the channel value 2 represents in `cellmap`
 *
 * The values `colour1`, `colour2`, `colour3` must be valid but distinct
 */
void libglitter_reorder_rasters(void **, enum libglitter_colour, enum libglitter_colour, enum libglitter_colour);


/**
 * Splits a `uint64_t` raster into one `uint16_t` raster per channel
 * 
 * @param  rasters   Output array for the rasters, they will be in the
 *                   order (0) red, (1) green, (2) blue
 * @param  alphap    Output parameter for the alpha mask raster, or `NULL`
 * @param  raster    The raster that is being split
 * @param  red       The value `0xFFFF` shifted such that value
 *                   expresses pure red (closest primary colour)
 * @param  green     The value `0xFFFF` shifted such that value
 *                   expresses pure green (closest primary colour)
 * @param  blue      The value `0xFFFF` shifted such that value
 *                   expresses pure blue (closest primary colour)
 */
void libglitter_split_uint64_raster(uint16_t *[3], uint16_t **, uint64_t *, uint64_t, uint64_t, uint64_t);

/**
 * Splits a `uint32_t` raster into one `uint8_t` raster per channel
 * 
 * @param  rasters   Output array for the rasters, they will be in the
 *                   order (0) red, (1) green, (2) blue
 * @param  alphap    Output parameter for the alpha mask raster, or `NULL`
 * @param  raster    The raster that is being split
 * @param  red       The value `0xFF` shifted such that value
 *                   expresses pure red (closest primary colour)
 * @param  green     The value `0xFF` shifted such that value
 *                   expresses pure green (closest primary colour)
 * @param  blue      The value `0xFF` shifted such that value
 *                   expresses pure blue (closest primary colour)
 */
void libglitter_split_uint32_raster(uint8_t *[3], uint8_t **, uint32_t *, uint32_t, uint32_t, uint32_t);


/**
 * Transform rasters from fully using subpixel rendering to
 * balance between subpixel rendering and greyscale antialiasing
 * 
 * @param  rasters     Array of rasters, one for each subpixel colour.
 *                     The function may change the offset for each raster,
 *                     as such, the given pointer shall not be used anywhere
 *                     else during the execution of the function and the
 *                     inner pointers shall be considered undefined after
 *                     the execution of the function
 * @param  nrasters    The number of rasters
 * @param  rowsize     The number of cells in a row in each raster
 * @param  cellsize    The number of values stored in each raster,
 *                     between each cell, plus 1 (that is, the number of
 *                     values per cell)
 * @param  width       The horizontal number of pixels in the rasters
 * @param  height      The vertical number of pixels in the rasters
 * @param  saturation  The subpixel rendering saturation, 1 for regular
 *                     subpixel rendering, 0 for greyscale, values
 *                     in between for a compromise
 * @param  primary_ys  The CIE Y value (in e.g. CIE xyY or CIE XYZ) for
 *                     each subpixel colour; or `NULL` for the sRGB values
 *                     (this is only allowed if there are exactly 3 rasters)
 */
void libglitter_desaturate_double(double **, size_t, size_t, size_t, size_t, size_t, double, const double *restrict);

/**
 * This value is identical to `libglitter_desaturate_double`,
 * apart from it parameter types, see `libglitter_desaturate_double`
 * for details about this function
 */
void libglitter_desaturate_float(float **, size_t, size_t, size_t, size_t, size_t, float, const float *restrict);

/**
 * Transform rasters from fully using subpixel rendering to
 * balance between subpixel rendering and greyscale antialiasing
 * 
 * @param  rasters      Array of rasters, one for each subpixel colour.
 *                      The function may change the offset for each raster,
 *                      as such, the given pointer shall not be used anywhere
 *                      else during the execution of the function and the
 *                      inner pointers shall be considered undefined after
 *                      the execution of the function.
 * @param  nrasters     The number of rasters
 * @param  rowsize      The number of cells in a row in each raster
 * @param  cellsize     The number of values stored in each raster,
 *                      between each cell, plus 1 (that is, the number of
 *                      values per cell)
 * @param  width        The horizontal number of pixels in the rasters
 * @param  height       The vertical number of pixels in the rasters
 * @param  saturations  The subpixel rendering saturation for each raster,
 *                      1 for regular subpixel rendering, 0 for greyscale,
 *                      values in between for a compromise
 * @param  primary_ys   The CIE Y value (in e.g. CIE xyY or CIE XYZ) for
 *                      each subpixel colour; or `NULL` for the sRGB values
 *                      (this is only allowed if there are exactly 3 rasters)
 */
void libglitter_per_channel_desaturate_double(double **, size_t, size_t, size_t, size_t, size_t,
                                              const double *restrict, const double *restrict);

/**
 * This value is identical to `libglitter_per_channel_desaturate_double`,
 * apart from it parameter types, see `libglitter_per_channel_desaturate_double`
 * for details about this function
 */
void libglitter_per_channel_desaturate_float(float **, size_t, size_t, size_t, size_t, size_t,
                                             const float *restrict, const float *restrict);


/**
 * Get the matrix each pixel shall be multiplied with
 * to convert it from the output's colour space to sRGB
 * or CIE XYZ
 * 
 * This is useful when the output does not use sRGB, or
 * CIE XYZ, but the application does. If the application
 * uses some other colour space, this function can output
 * the conversion matrix for the CIE XYZ colour space,
 * which can that be right-hand multiplied to get the
 * conversion matrix for some other colour; but be aware
 * that the output matrix is in column-major order, not
 * row-major order
 * 
 * @param  matrix   Output buffer for the conversion matrix, in column-major order
 * @param  c1x      The CIE x value (as in CIE xyY) of the output's first primary colour
 * @param  c1y      The CIE y value (as in CIE xyY) of the output's first primary colour
 * @param  c2x      The CIE x value (as in CIE xyY) of the output's second primary colour
 * @param  c2y      The CIE y value (as in CIE xyY) of the output's second primary colour
 * @param  c3x      The CIE x value (as in CIE xyY) of the output's third primary colour
 * @param  c3y      The CIE y value (as in CIE xyY) of the output's third primary colour
 * @param  white_x  The CIE x value (as in CIE xyY) of the output's white point
 * @param  white_y  The CIE y value (as in CIE xyY) of the output's white point
 * @param  white_Y  The CIE Y value (as in CIE xyY) of the output's white point, normally 1
 * @param  xyz      Whether the output conversion matrix should be to CIE XYZ rather the sRGB
 * 
 * `LIBGLITTER_ILLUMINANT_D65` can be input in place of
 * `white_x, white_y, white_Y` (it expands to three arguments)
 * if the output's whitepoint is the D65 illuminant
 */
void libglitter_get_colour_space_conversion_matrix_double(double[3][3], double, double, double, double,
                                                          double, double, double, double, double, int);

/**
 * This value is identical to `libglitter_get_colour_space_conversion_matrix_double`,
 * apart from it parameter types, see `libglitter_get_colour_space_conversion_matrix_double`
 * for details about this function
 */
void libglitter_get_colour_space_conversion_matrix_float(float[3][3], float, float, float, float,
                                                         float, float, float, float, float, int);


/**
 * Convert set of rasters from one colour space to another
 * 
 * @param  n                The number of input rasters
 * @param  m                The number of output rasters
 * @param  outputs          Array of output rasters. The function may change
 *                          the offset for each raster, as such, the given
 *                          pointer shall not be used anywhere else during
 *                          the execution of the function and the inner
 *                          pointers shall be considered undefined after the
 *                          execution of the function.
 * @param  inputs           Array of input rasters. The function may change
 *                          the offset for each raster, as such, the given
 *                          pointer shall not be used anywhere else during
 *                          the execution of the function and the inner
 *                          pointers shall be considered undefined after the
 *                          execution of the function.
 * @param  output_rowsize   The number of cells in a row in each output raster
 * @param  output_cellsize  The number of values stored in each output raster,
 *                          between each cell, plus 1 (that is, the number of
 *                          values per cell)
 * @param  input_rowsize    The number of cells in a row in each input raster
 * @param  input_cellsize   The number of values stored in each input raster,
 *                          between each cell, plus 1 (that is, the number of
 *                          values per cell)
 * @param  width            The horizontal number of pixels in the rasters
 * @param  height           The vertical number of pixels in the rasters
 * @param  matrix           Colour space conversion matrix, in column-major order
 */
void libglitter_colour_space_convert_rasters_double(size_t n, size_t m, double **, const double **,
                                                    size_t, size_t, size_t, size_t, size_t, size_t, const double[n][m]);

/**
 * This value is identical to `libglitter_colour_space_convert_rasters_double`,
 * apart from it parameter types, see `libglitter_colour_space_convert_rasters_double`
 * for details about this function
 */
void libglitter_colour_space_convert_rasters_float(size_t n, size_t m, float **, const float **,
                                                   size_t, size_t, size_t, size_t, size_t, size_t, const float[n][m]);


#endif
