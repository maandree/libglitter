/* See LICENSE file for copyright and license details. */
#include "common.h"

#define X(x, y) ((x) / (y))
#define Z(x, y) ((1 - (x)) / (y) - 1)


#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunsuffixed-float-constants"
#endif


/**
 * CIE XYZ-to-sRGB conversion matrix
 */
static const double srgb[3][3] = {
	{ 3.240446254647737056586720427731, -1.537134761820080353089679192635, -0.498530193022728773666329971093},
	{-0.969266606244679751469561779231,  1.876011959788370209167851498933,  0.041556042214430065351304932619},
	{ 0.055643503564352832235773149705, -0.204026179735960239147729566866,  1.057226567722703292062647051353}};


#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic pop
#endif


static void
invert(double m[3][4])
{
	double t;
	size_t i;

	/* Set m[0][0] = 1 */ for (t = m[0][0], i = 0; i < 4; i++)  m[0][i] /= t;
	/* Set m[1][0] = 0 */ for (t = m[1][0], i = 0; i < 4; i++)  m[1][i] -= t * m[0][i];
	/* Set m[2][0] = 0 */ for (t = m[2][0], i = 0; i < 4; i++)  m[2][i] -= t * m[0][i];

	/* Set m[1][1] = 1 */ for (t = m[1][1], i = 1; i < 4; i++)  m[1][i] /= t;
	/* Set m[2][1] = 0 */ for (t = m[2][1], i = 1; i < 4; i++)  m[2][i] -= t * m[1][i];
	/* Set m[0][1] = 0 */ for (t = m[0][1], i = 1; i < 4; i++)  m[0][i] -= t * m[1][i];

	/* Set m[2][2] = 1 */ for (t = m[2][2], i = 2; i < 4; i++)  m[2][i] /= t;
	/* Set m[1][2] = 0 */ for (t = m[1][2], i = 2; i < 4; i++)  m[1][i] -= t * m[2][i];
	/* Set m[0][2] = 0 */ for (t = m[0][2], i = 2; i < 4; i++)  m[0][i] -= t * m[2][i];
}


void
libglitter_get_colour_space_conversion_matrix_double(double matrix[3][3], double c1x, double c1y,
                                                     double c2x, double c2y, double c3x, double c3y,
                                                     double white_x, double white_y, double white_Y)
{
	double mat[3][4];
	double x1, x2, x3;
	double y1, y2, y3;
	double z1, z2, z3;

	/* Get colour space in CIE XYZ */
	mat[0][0] = x1 = X(c1x, c1y);
	mat[0][1] = x2 = X(c2x, c2y);
	mat[0][2] = x3 = X(c3x, c3y);
	mat[1][3] = X(white_x, white_y) * white_Y;
	mat[1][0] = 1;
	mat[1][1] = 1;
	mat[1][2] = 1;
	mat[1][3] = white_Y;
	mat[2][0] = z1 = Z(c1x, c1y);
	mat[2][1] = z2 = Z(c2x, c2y);
	mat[2][2] = z3 = Z(c3x, c3y);
	mat[2][3] = Z(white_x, white_y) * white_Y;
	invert(mat);
	y1 = mat[0][3];
	y2 = mat[1][3];
	y3 = mat[2][3];

	/* [[x1, x2, x3], [y1, y2, y3], [z1, z2, z3]] is
	 * the output RGB-to-CIE XYZ conversion matrix. It
	 * is multiplied by the CIE XYZ-to-sRGB conversion
	 * matrix to get the output RGB-to-sRGB conversion
	 * matrix. */
	matrix[0][0] = x1 * srgb[0][0] + x2 * srgb[1][0] + x3 * srgb[2][0];
	matrix[0][1] = x1 * srgb[0][1] + x2 * srgb[1][1] + x3 * srgb[2][1];
	matrix[0][2] = x1 * srgb[0][2] + x2 * srgb[1][2] + x3 * srgb[2][2];
	matrix[1][0] = y1 * srgb[0][0] + y2 * srgb[1][0] + y3 * srgb[2][0];
	matrix[1][1] = y1 * srgb[0][1] + y2 * srgb[1][1] + y3 * srgb[2][1];
	matrix[1][2] = y1 * srgb[0][2] + y2 * srgb[1][2] + y3 * srgb[2][2];
	matrix[2][0] = z1 * srgb[0][0] + z2 * srgb[1][0] + z3 * srgb[2][0];
	matrix[2][1] = z1 * srgb[0][1] + z2 * srgb[1][1] + z3 * srgb[2][1];
	matrix[2][2] = z1 * srgb[0][2] + z2 * srgb[1][2] + z3 * srgb[2][2];
}
