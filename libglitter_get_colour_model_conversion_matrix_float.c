/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


void
libglitter_get_colour_model_conversion_matrix_float(float matrix[3][3], float c1x, float c1y,
                                                    float c2x, float c2y, float c3x, float c3y,
                                                    float white_x, float white_y, float white_Y,
                                                    int xyz, float *c1Yp, float *c2Yp, float *c3Yp)
{
	double double_matrix[3][3], c1Y, c2Y, c3Y;
	libglitter_get_colour_model_conversion_matrix_double(double_matrix,
	                                                     (double)c1x, (double)c1y,
	                                                     (double)c2x, (double)c2y,
	                                                     (double)c3x, (double)c3y,
	                                                     (double)white_x, (double)white_y, (double)white_Y,
	                                                     xyz, &c1Y, &c2Y, &c3Y);
	matrix[0][0] = (float)double_matrix[0][0];
	matrix[0][1] = (float)double_matrix[0][1];
	matrix[0][2] = (float)double_matrix[0][2];
	matrix[1][0] = (float)double_matrix[1][0];
	matrix[1][1] = (float)double_matrix[1][1];
	matrix[1][2] = (float)double_matrix[1][2];
	matrix[2][0] = (float)double_matrix[2][0];
	matrix[2][1] = (float)double_matrix[2][1];
	matrix[2][2] = (float)double_matrix[2][2];
	if (c1Yp)
		*c1Yp = (float)c1Y;
	if (c2Yp)
		*c2Yp = (float)c2Y;
	if (c3Yp)
		*c3Yp = (float)c3Y;
}


#else


int
main(void)
{
	return 0; /* TODO add test */
}


#endif
