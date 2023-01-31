/* See LICENSE file for copyright and license details. */
#ifndef LIBGLITTER_H
#define LIBGLITTER_H

#include <stddef.h>
#include <stdint.h>


#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wpadded"
#elif defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wvla"
#endif



#if defined(__GNUC__) && !defined(__clang__)
# define LIBGLITTER_GCC_ATTRS__(...) __attribute__((__VA_ARGS__))
#else
# define LIBGLITTER_GCC_ATTRS__(...)
#endif


/**
 * The application will use `double`-typed rasters
 */
#define LIBGLITTER_FEATURE_DOUBLE_TYPE     UINT64_C(0x0000000000000001)

/**
 * The application will use `float`-typed rasters
 */
#define LIBGLITTER_FEATURE_FLOAT_TYPE      UINT64_C(0x0000000000000002)

/**
 * The application will use `uint64_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT64_TYPE     UINT64_C(0x0000000000000004)

/**
 * The application will use `uint32_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT32_TYPE     UINT64_C(0x0000000000000008)

/**
 * The application will use `uint16_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT16_TYPE     UINT64_C(0x0000000000000010)

/**
 * The application will use `uint8_t`-typed rasters
 */
#define LIBGLITTER_FEATURE_UINT8_TYPE      UINT64_C(0x0000000000000020)

/**
 * The allocation will use at least one of the functions
 * `libglitter_compose_double`,
 * `libglitter_compose_float`,
 * `libglitter_compose_uint64`,
 * `libglitter_compose_uint32`,
 * `libglitter_compose_uint16`, and
 * `libglitter_compose_uint8`
 */
#define LIBGLITTER_FEATURE_COMPOSE         UINT64_C(0x0000000000000040)

/**
 * The allocation will use at least one of the functions
 * `libglitter_desaturate_double`, and
 * `libglitter_desaturate_float`
 */
#define LIBGLITTER_FEATURE_CU_DESATURATION UINT64_C(0x0000000000000080) /* CU = channel-uniform */

/**
 * The allocation will use at least one of the functions
 * `libglitter_per_channel_desaturate_double`, and
 * `libglitter_per_channel_desaturate_float`
 */
#define LIBGLITTER_FEATURE_PC_DESATURATION UINT64_C(0x0000000000000100) /* PC = per channel */

/**
 * The allocation will use at least one of the functions
 * `libglitter_colour_model_convert_rasters_double`, and
 * `libglitter_colour_model_convert_rasters_float`
 */
#define LIBGLITTER_FEATURE_COLOUR_MODEL    UINT64_C(0x0000000000000200)

/**
 * The allocation will use at least one of the functions
 * `libglitter_redistribute_energy_double`, and
 * `libglitter_redistribute_energy_float`
 */
#define LIBGLITTER_FEATURE_REDISTRIBUTE    UINT64_C(0x0000000000000400)


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
 '                    - LIBGLITTER_FEATURE_REDISTRIBUTE,
 *                    - LIBGLITTER_FEATURE_COMPOSE,
 *                    - LIBGLITTER_FEATURE_CU_DESATURATION,
 *                    - LIBGLITTER_FEATURE_PC_DESATURATION, and
 *                    - LIBGLITTER_FEATURE_COLOUR_MODEL.
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
 * @param   unused    Reserved for future use, shall be `NULL`
 * @return            1 on success with hardware acceleration enabled,
 *                    0 on success without hardware acceleration enabled,
 *                    or -1 on failure
 */
int libglitter_enable_acceleration(uint64_t, int, void (*)(int, int, void *), void *, void *);


/**
 * Deallocates a render context (created by `libglitter_create_render_context`)
 * 
 * @param  this  The render context to deallocate
 */
void libglitter_free_render_context(LIBGLITTER_RENDER_CONTEXT *);

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
 * that all its values are 1; in the future this could also happen for
 * other situations where `noutputs == widthmul * heightmul`
 * 
 * The returned object will contain references to `cellmap` and `ncellvalues`
 */
LIBGLITTER_GCC_ATTRS__(nonnull, warn_unused_result, malloc, malloc(libglitter_free_render_context, 1))
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
 * Apply a horizontal, 1-dimensional convolution kernel
 * and a vertical, 1-dimensional convolution kernel
 * (both are optional) to kernel to blur the text in
 * order to reduce colour fringing
 * 
 * The raster shall be row-major ordered an shall have
 * horizontally adjacent cells adjacent in memory: the
 * is no space between the cells' memory areas if
 * there is no horizontal space between them in the
 * raster
 * 
 * This function will extend the image in raster by
 * `(hkernelsize - 1) / 2` cells to the left and to
 * the right, and `(vkernelsize - 1) / 2` cells to
 * the up as well as down. The caller is responsible
 * for furthering extending the image by `(widthmul`
 * - (hkernelsize - 1) / 2 % widthmul) % widthmul`
 * cells both to on the left and on the right, and
 * by `(heightmul` - (vkernelsize - 1) / 2 % heightmul)
 * % heightmul` cells both up and down (where
 * `widthmul` and `heightmul` are arguments to the
 * `libglitter_create_render_context` function); so that
 * the raster can be input to `libglitter_compose_double`.
 * 
 * @param  raster          The subpixel raster. The must be padded with
 *                         zero-initialised cells on the left side and
 *                         on the top. This padding must `(hkernelsize - 1)`
 *                         cells wide the left side and `(vkernelsize - 1)`
 *                         cells tall on the top. The new image will be
 *                         written to this raster must shiften upwards
 *                         `(vkernelsize - 1) / 2` cells and to the left
 *                         `(hkernelsize - 1) / 2` cells. The image will
 *                         also be extended by `(hkernelsize - 1) / 2`
 *                         cells on both the left side and the right size
 *                         and by `(vkernelsize - 1) / 2` cells both above
 *                         and below. The input pointer shall point to
 *                         the image location in the pointer, that is
 *                         the first cell after the padding, but once
 *                         the function returns the pointer to first
 *                         cell is in the padding shall be used as the
 *                         first cell in the new image; however the image
 *                         will be offset by `(vkernelsize - 1) / 2` cells
 *                         vertically and `(hkernelsize - 1) / 2` cells
 *                         horizontally.
 * @param  rowsize         The number of cells a pointer to cell in
 *                         `raster` must be offset with to get to the
 *                         cell on the next row but in the same column
 * @param  width           The width of the input raster (excluding
 *                         padding), in cells. The function extends the
 *                         image by `(hkernelsize - 1) / 2` cells
 *                         vertically.
 * @param  height          The height of the input raster (excluding
 *                         padding), in cells. The function extends the
 *                         image by `(hkernelsize - 1) / 2` cells
 *                         horizontally.
 * @param  hkernelsize     The size (number of elements) of `hkernel`;
 *                         must be odd; if 1, `hkernel` is not applied
 * @param  vkernelsize     The size (number of elements) of `vkernel`;
 *                         must be odd; if 1, `vkernel` is not applied
 * @param  hkernel         The horizontal convolution kernel; the sum
 *                         of its elements shall be 1. It can be `NULL`
 *                         if `hkernelsize` is 1, as it will not be
 *                         applied
 * @param  vkernel         The vertical convolution kernel; the sum
 *                         of its elements shall be 1. It can be `NULL`
 *                         if `vkernelsize` is 1, as it will not be
 *                         applied
 * 
 * If the input value of the `widthmul` parameter to the
 * `libglitter_create_render_context` function is `1`, it
 * the suggested `hkernel` is `{1.}`, otherwise the suggested
 * `hkernel` is `{1/3., 1/3., 1/3.}`. Likewise, if the
 * input value of the `heightmul` parameter to the
 * `libglitter_create_render_context` function is `1`, it
 * the suggested `vkernel` is `{1.}`, otherwise the suggested
 * `vkernel` is `{1/3., 1/3., 1/3.}`. Of course user
 * experimentation is required to find the best kernel.
 */
LIBGLITTER_GCC_ATTRS__(nonnull(1))
void libglitter_redistribute_energy_double(double *restrict raster, size_t rowsize, size_t width,
                                           size_t height, size_t hkernelsize, size_t vkernelsize,
                                           const double *hkernel, const double *vkernel);

/**
 * This value is identical to `libglitter_redistribute_energy_double`,
 * apart from it parameter types, see `libglitter_redistribute_energy_double`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull(1))
void libglitter_redistribute_energy_float(float *restrict raster, size_t rowsize, size_t width,
                                          size_t height, size_t hkernelsize, size_t vkernelsize,
                                          const float *hkernel, const float *vkernel);


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
LIBGLITTER_GCC_ATTRS__(nonnull)
void libglitter_compose_double(double *restrict*, const double *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *restrict);

/**
 * This value is identical to `libglitter_compose_double`,
 * apart from it parameter types, see `libglitter_compose_double`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull)
void libglitter_compose_float(float *restrict*, const float *restrict, size_t, size_t,
                              size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *restrict);

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
LIBGLITTER_GCC_ATTRS__(nonnull)
void libglitter_compose_uint64(uint64_t *restrict *, const uint64_t *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *restrict);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull)
void libglitter_compose_uint32(uint32_t *restrict *, const uint32_t *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *restrict);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull)
void libglitter_compose_uint16(uint16_t *restrict *, const uint16_t *restrict, size_t, size_t,
                               size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *restrict);

/**
 * This value is identical to `libglitter_compose_uint64`,
 * apart from it parameter types, see `libglitter_compose_uint64`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull)
void libglitter_compose_uint8(uint8_t *restrict *, const uint8_t *restrict, size_t, size_t,
                              size_t, size_t, const LIBGLITTER_RENDER_CONTEXT *restrict);


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
LIBGLITTER_GCC_ATTRS__(nonnull)
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
LIBGLITTER_GCC_ATTRS__(nonnull(1, 3))
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
LIBGLITTER_GCC_ATTRS__(nonnull(1, 3))
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
LIBGLITTER_GCC_ATTRS__(nonnull(1))
void libglitter_desaturate_double(double *restrict *, size_t, size_t, size_t, size_t, size_t,
                                  double, const double *restrict);

/**
 * This value is identical to `libglitter_desaturate_double`,
 * apart from it parameter types, see `libglitter_desaturate_double`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull(1))
void libglitter_desaturate_float(float *restrict *, size_t, size_t, size_t, size_t, size_t,
                                 float, const float *restrict);

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
LIBGLITTER_GCC_ATTRS__(nonnull(1, 7))
void libglitter_per_channel_desaturate_double(double *restrict *, size_t, size_t, size_t, size_t, size_t,
                                              const double *restrict, const double *restrict);

/**
 * This value is identical to `libglitter_per_channel_desaturate_double`,
 * apart from it parameter types, see `libglitter_per_channel_desaturate_double`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull(1, 7))
void libglitter_per_channel_desaturate_float(float *restrict *, size_t, size_t, size_t, size_t, size_t,
                                             const float *restrict, const float *restrict);


/**
 * Get the matrix each pixel shall be multiplied with
 * to convert it from the output's colour model to sRGB
 * or CIE XYZ
 * 
 * This is useful when the output does not use sRGB, or
 * CIE XYZ, but the application does. If the application
 * uses some other colour model, this function can output
 * the conversion matrix for the CIE XYZ colour model,
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
 * @param  c1Yp     Output parameter for the first primary colour CIE Y value, or `NULL`
 * @param  c2Yp     Output parameter for the second primary colour CIE Y value, or `NULL`
 * @param  c3Yp     Output parameter for the third primary colour CIE Y value, or `NULL`
 * 
 * `LIBGLITTER_ILLUMINANT_D65` can be input in place of
 * `white_x, white_y, white_Y` (it expands to three arguments)
 * if the output's whitepoint is the D65 illuminant
 */
LIBGLITTER_GCC_ATTRS__(nonnull(1))
void libglitter_get_colour_model_conversion_matrix_double(double[3][3], double, double, double, double,
                                                          double, double, double, double, double, int,
                                                          double *restrict, double *restrict, double *restrict);

/**
 * This value is identical to `libglitter_get_colour_model_conversion_matrix_double`,
 * apart from it parameter types, see `libglitter_get_colour_model_conversion_matrix_double`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull(1))
void libglitter_get_colour_model_conversion_matrix_float(float[3][3], float, float, float, float,
                                                         float, float, float, float, float, int,
                                                         float *restrict, float *restrict, float *restrict);


/**
 * Convert set of rasters from one colour model to another
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
 * @param  matrix           Colour model conversion matrix, in column-major order
 */
LIBGLITTER_GCC_ATTRS__(nonnull(3, 4, 11))
void libglitter_colour_model_convert_rasters_double(size_t, size_t m, double *restrict *restrict,
                                                    const double *restrict *restrict, size_t, size_t,
                                                    size_t, size_t, size_t, size_t, const double (*)[m]);

/**
 * This value is identical to `libglitter_colour_model_convert_rasters_double`,
 * apart from it parameter types, see `libglitter_colour_model_convert_rasters_double`
 * for details about this function
 */
LIBGLITTER_GCC_ATTRS__(nonnull(3, 4, 11))
void libglitter_colour_model_convert_rasters_float(size_t, size_t m, float *restrict *restrict,
                                                   const float *restrict *restrict, size_t, size_t,
                                                   size_t, size_t, size_t, size_t, const float (*)[m]);



#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic pop
#elif defined(__clang__)
# pragma clang diagnostic pop
#endif

#undef LIBGLITTER_GCC_ATTRS__
#endif
