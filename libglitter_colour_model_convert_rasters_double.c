/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


static void
multiply_33(double **outputs, size_t opos, const double **inputs, size_t ipos, const double matrix[3][3])
{
	double i0 = inputs[0][ipos], i1 = inputs[1][ipos], i2 = inputs[2][ipos];
	outputs[0][opos] = i0 * matrix[0][0] + i1 * matrix[0][1] + i2 * matrix[0][2];
	outputs[1][opos] = i0 * matrix[1][0] + i1 * matrix[1][1] + i2 * matrix[1][2];
	outputs[2][opos] = i0 * matrix[2][0] + i1 * matrix[2][1] + i2 * matrix[2][2];
}


static void
multiply_nn(size_t n, double **outputs, size_t opos, const double **inputs, size_t ipos,
            const double matrix[n][n], double buffer[n])
{
	size_t i, j;
	for (j = 0; j < n; j++)
		buffer[j] = inputs[j][ipos];
	for (i = 0; i < n; i++) {
		outputs[i][opos] = buffer[0] * matrix[i][0];
		for (j = 1; j < n; j++)
			outputs[i][opos] += buffer[j] * matrix[i][j];
	}
}


static void
multiply_nm(size_t n, size_t m, double **outputs /* m */, size_t opos, const double **inputs /* n */,
            size_t ipos, const double matrix[m][n], double buffer[n])
{
	size_t i, j;
	for (j = 0; j < n; j++)
		buffer[j] = inputs[j][ipos];
	for (i = 0; i < m; i++) {
		outputs[i][opos] = buffer[0] * matrix[i][0];
		for (j = 1; j < n; j++)
			outputs[i][opos] += buffer[j] * matrix[i][j];
	}
}


void
libglitter_colour_model_convert_rasters_double(size_t n, size_t m, double **outputs /* m */, const double **inputs /* n */,
                                               size_t output_rowsize, size_t output_cellsize, size_t input_rowsize,
                                               size_t input_cellsize, size_t width, size_t height, const double matrix[m][n])
{
	size_t y, x, output_i, input_i, output_blanking, input_blanking;
	double *buffer;

	output_blanking = (output_rowsize - width) * output_cellsize;
	input_blanking = (input_rowsize - width) * input_cellsize;
	output_i = 0;
	input_i = 0;

	if (n == 3 && m == 3) {
		for (y = 0; y < height; y++, output_i += output_blanking, input_i += input_blanking)
			for (x = 0; x < width; x++, output_i += output_cellsize, input_i += input_cellsize)
				multiply_33(outputs, output_i, inputs, input_i, matrix);

	} else if (n == m) {
		buffer = alloca(n * sizeof(*buffer));
		for (y = 0; y < height; y++, output_i += output_blanking, input_i += input_blanking)
			for (x = 0; x < width; x++, output_i += output_cellsize, input_i += input_cellsize)
				multiply_nn(n, outputs, output_i, inputs, input_i, matrix, buffer);

	} else {
		buffer = alloca(n * sizeof(*buffer));
		for (y = 0; y < height; y++, output_i += output_blanking, input_i += input_blanking)
			for (x = 0; x < width; x++, output_i += output_cellsize, input_i += input_cellsize)
				multiply_nm(n, m, outputs, output_i, inputs, input_i, matrix, buffer);
	}
}


#else


int
main(void)
{
	return 0; /* TODO add test */
}


#endif
