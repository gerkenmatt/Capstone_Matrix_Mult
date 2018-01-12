/*
 * matrix_mult.h
 *
 *  Created on: Jan 11, 2018
 *      Author: gerke
 */

#ifndef MATRIX_MULT_H_
#define MATRIX_MULT_H_


#define     side                12
#define     robust_printing     1
#define     change_rate         500


void init_matrices();
void matrix_multiply(int f_matrix[][side], int s_matrix[][side], unsigned long r_matrix[][side]);
int checker(unsigned long golden_matrix[][side], unsigned long results_matrix[][side]);
void matrix_multiply_test();


#endif /* MATRIX_MULT_H_ */
