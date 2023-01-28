/* See LICENSE file for copyright and license details. */
#include "common.h"
#ifndef TEST


static void
multiply_33(double *restrict *restrict outputs, size_t opos,
            const double *restrict *restrict inputs, size_t ipos, const double matrix[3][3])
{
	double i0 = inputs[0][ipos], i1 = inputs[1][ipos], i2 = inputs[2][ipos];
	outputs[0][opos] = i0 * matrix[0][0] + i1 * matrix[0][1] + i2 * matrix[0][2];
	outputs[1][opos] = i0 * matrix[1][0] + i1 * matrix[1][1] + i2 * matrix[1][2];
	outputs[2][opos] = i0 * matrix[2][0] + i1 * matrix[2][1] + i2 * matrix[2][2];
}


static void
multiply_nn(size_t n, double *restrict *restrict outputs, size_t opos,
            const double *restrict *restrict inputs, size_t ipos,
            const double matrix[n][n], double buffer[restrict n])
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
multiply_nm(size_t n, size_t m, double *restrict *restrict outputs /* m */, size_t opos,
            const double *restrict *restrict inputs /* n */, size_t ipos,
            const double matrix[m][n], double buffer[restrict n])
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
libglitter_colour_model_convert_rasters_double(size_t n, size_t m, double *restrict *restrict outputs /* m */,
                                               const double *restrict *restrict inputs /* n */, size_t output_rowsize,
                                               size_t output_cellsize, size_t input_rowsize, size_t input_cellsize,
                                               size_t width, size_t height, const double (*matrix)[m] /* n */)
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


static void
check(int same_rasters)
{
	double input1[4 * 15 * 3], input2[4 * 15 * 3], input3[4 * 15 * 3];
	double output1[4 * 14 * 4], output2[4 * 14 * 4], output3[4 * 14 * 4];
	double *inputs[3], *outputs[3], matrix[3][3], zeroes[4 * 15 * 4];

	memset(zeroes, 0, sizeof(zeroes));

#define RESET\
	do {\
		inputs[0] = input1;\
		inputs[1] = input2;\
		inputs[2] = input3;\
		outputs[0] = same_rasters ? input1 : output1;\
		outputs[1] = same_rasters ? input2 : output2;\
		outputs[2] = same_rasters ? input3 : output3;\
		memset(input1, 0, sizeof(input1));\
		memset(input2, 0, sizeof(input2));\
		memset(input3, 0, sizeof(input3));\
		memset(output1, 0, sizeof(output1));\
		memset(output2, 0, sizeof(output2));\
		memset(output3, 0, sizeof(output3));\
	} while (0)

#define IPOS(Y, X) ((Y) * 15 * 3 + (X) * 3)
#define OPOS(Y, X) (same_rasters ? IPOS(Y, X) : (Y) * 14 * 4 + (X) * 4)

#define SET_(BUF, POS, R1C1, R1C2, R1C3, R1C4, R1C5, R2C1, R2C2, R2C3, R2C4, R2C5, R3C1, R3C2, R3C3, R3C4, R3C5)\
	do {\
		(BUF)[POS(0, 0)] = R1C1;\
		(BUF)[POS(0, 1)] = R1C2;\
		(BUF)[POS(0, 2)] = R1C3;\
		(BUF)[POS(0, 3)] = R1C4;\
		(BUF)[POS(0, 4)] = R1C5;\
		(BUF)[POS(1, 0)] = R2C1;\
		(BUF)[POS(1, 1)] = R2C2;\
		(BUF)[POS(1, 2)] = R2C3;\
		(BUF)[POS(1, 3)] = R2C4;\
		(BUF)[POS(1, 4)] = R2C5;\
		(BUF)[POS(2, 0)] = R3C1;\
		(BUF)[POS(2, 1)] = R3C2;\
		(BUF)[POS(2, 2)] = R3C3;\
		(BUF)[POS(2, 3)] = R3C4;\
		(BUF)[POS(2, 4)] = R3C5;\
	} while (0)

#define SET(IBUF, R1C1, R1C2, R1C3, R1C4, R1C5, R2C1, R2C2, R2C3, R2C4, R2C5, R3C1, R3C2, R3C3, R3C4, R3C5)\
	SET_(IBUF, IPOS, R1C1, R1C2, R1C3, R1C4, R1C5, R2C1, R2C2, R2C3, R2C4, R2C5, R3C1, R3C2, R3C3, R3C4, R3C5)\

#define CHECK(OBUF, R1C1, R1C2, R1C3, R1C4, R1C5, R2C1, R2C2, R2C3, R2C4, R2C5, R3C1, R3C2, R3C3, R3C4, R3C5)\
	do {\
		ASSERT((OBUF)[OPOS(0, 0)] == R1C1);\
		ASSERT((OBUF)[OPOS(0, 1)] == R1C2);\
		ASSERT((OBUF)[OPOS(0, 2)] == R1C3);\
		ASSERT((OBUF)[OPOS(0, 3)] == R1C4);\
		ASSERT((OBUF)[OPOS(0, 4)] == R1C5);\
		ASSERT((OBUF)[OPOS(1, 0)] == R2C1);\
		ASSERT((OBUF)[OPOS(1, 1)] == R2C2);\
		ASSERT((OBUF)[OPOS(1, 2)] == R2C3);\
		ASSERT((OBUF)[OPOS(1, 3)] == R2C4);\
		ASSERT((OBUF)[OPOS(1, 4)] == R2C5);\
		ASSERT((OBUF)[OPOS(2, 0)] == R3C1);\
		ASSERT((OBUF)[OPOS(2, 1)] == R3C2);\
		ASSERT((OBUF)[OPOS(2, 2)] == R3C3);\
		ASSERT((OBUF)[OPOS(2, 3)] == R3C4);\
		ASSERT((OBUF)[OPOS(2, 4)] == R3C5);\
		SET_(OBUF, OPOS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);\
		ASSERT(!memcmp((OBUF), zeroes, same_rasters ? sizeof(*input1) : sizeof(*output1)));\
	} while (0)

#define RUN(NINPUTS, NOUTPUTS)\
	do {\
		double mat[NOUTPUTS][NINPUTS];\
		size_t i, j;\
		for (i = 0; i < (NOUTPUTS); i++)\
			for (j = 0; j < (NINPUTS); j++)\
				mat[i][j] = matrix[i][j];\
		libglitter_colour_model_convert_rasters_double(NINPUTS, NOUTPUTS, outputs, (void *)inputs,\
		                                               same_rasters ? 15 : 14, same_rasters ? 3 : 4,\
		                                               15, 3, 5, 3, (void *)mat);\
		inputs[0] = input1;\
		inputs[1] = input2;\
		inputs[2] = input3;\
		outputs[0] = same_rasters ? input1 : output1;\
		outputs[1] = same_rasters ? input2 : output2;\
		outputs[2] = same_rasters ? input3 : output3;\
	} while (0)


	RESET;
	SET(inputs[0],  1, 5, 2, 6, 3,  9, 6, 4, 3, 1,  8, 7, 4, 9, 5);
	matrix[0][0] = 3;
	RUN(1, 1);
	CHECK(outputs[0],  1*3, 5*3, 2*3, 6*3, 3*3,  9*3, 6*3, 4*3, 3*3, 1*3,  8*3, 7*3, 4*3, 9*3, 5*3);


	RESET;
	SET(inputs[0],  1, 2, 3, 4, 5,  2, 3, 4, 5, 6,  3, 4, 5, 6, 7);
	SET(inputs[1],  4, 5, 6, 7, 8,  5, 6, 7, 8, 9,  6, 7, 8, 9, 1);
	SET(inputs[2],  7, 8, 9, 1, 2,  8, 9, 1, 2, 3,  9, 1, 2, 3, 4);
	matrix[0][0] = 1, matrix[1][0] = 3;
	matrix[0][1] = 4, matrix[1][1] = 2;
	matrix[0][2] = 2, matrix[1][2] = 6;
	RUN(3, 2);
#define X(A, B, C) ((A) * 1 + (B) * 4 + (C) * 2)
	CHECK(outputs[0], X(1,4,7), X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2),
	                  X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3),
	                  X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3), X(7,1,4));
#undef X
#define X(A, B, C) ((A) * 3 + (B) * 2 + (C) * 6)
	CHECK(outputs[1], X(1,4,7), X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2),
	                  X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3),
	                  X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3), X(7,1,4));
#undef X


	RESET;
	SET(inputs[0],  1, 2, 3, 4, 5,  2, 3, 4, 5, 6,  3, 4, 5, 6, 7);
	SET(inputs[1],  4, 5, 6, 7, 8,  5, 6, 7, 8, 9,  6, 7, 8, 9, 1);
	matrix[0][0] = 7, matrix[1][0] = 4;
	matrix[0][1] = 3, matrix[1][1] = 9;
	RUN(2, 2);
#define X(A, B) ((A) * 7 + (B) * 3)
	CHECK(outputs[0], X(1,4), X(2,5), X(3,6), X(4,7), X(5,8),
	                  X(2,5), X(3,6), X(4,7), X(5,8), X(6,9),
	                  X(3,6), X(4,7), X(5,8), X(6,9), X(7,1));
#undef X
#define X(A, B) ((A) * 4 + (B) * 9)
	CHECK(outputs[1], X(1,4), X(2,5), X(3,6), X(4,7), X(5,8),
	                  X(2,5), X(3,6), X(4,7), X(5,8), X(6,9),
	                  X(3,6), X(4,7), X(5,8), X(6,9), X(7,1));
#undef X


	RESET;
	SET(inputs[0],  1, 2, 3, 4, 5,  2, 3, 4, 5, 6,  3, 4, 5, 6, 7);
	SET(inputs[1],  4, 5, 6, 7, 8,  5, 6, 7, 8, 9,  6, 7, 8, 9, 1);
	SET(inputs[2],  7, 8, 9, 1, 2,  8, 9, 1, 2, 3,  9, 1, 2, 3, 4);
	matrix[0][0] = 7, matrix[1][0] = 1, matrix[2][0] = 4;
	matrix[0][1] = 2, matrix[1][1] = 8, matrix[2][1] = 5;
	matrix[0][2] = 6, matrix[1][2] = 7, matrix[2][2] = 6;
	RUN(3, 3);
#define X(A, B, C) ((A) * 7 + (B) * 2 + (C) * 6)
	CHECK(outputs[0], X(1,4,7), X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2),
	                  X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3),
	                  X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3), X(7,1,4));
#undef X
#define X(A, B, C) ((A) * 1 + (B) * 8 + (C) * 7)
	CHECK(outputs[1], X(1,4,7), X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2),
	                  X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3),
	                  X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3), X(7,1,4));
#undef X
#define X(A, B, C) ((A) * 4 + (B) * 5 + (C) * 6)
	CHECK(outputs[2], X(1,4,7), X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2),
	                  X(2,5,8), X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3),
	                  X(3,6,9), X(4,7,1), X(5,8,2), X(6,9,3), X(7,1,4));
#undef X
}

int
main(void)
{
	check(0);
	check(1);
}

#endif
