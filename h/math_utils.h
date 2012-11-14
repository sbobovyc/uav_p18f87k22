/**
 * This is a collection of math functions specifically designed to work with IMU data.
 * @file math_utils.h
 *
 * @date Feb 21, 2011
 * @author sbobovyc
 * @details
 *
 */

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

#define VECT_ROWS 5	///< Defines the number of rows in Nx3 matrices used by various math functions

/**
 * This function takes a Nx3 matrix of floats and averages the columns.
 * @param values is a pointer to a Nx3 matrix
 * @param a is the average of the first column
 * @param b is the average of the second column
 * @param c is the average of the third column
 */

void math_vect_avg_float(float (*values)[VECT_ROWS][3], float * a, float * b, float * c);

/**
 * This function takes a Nx3 matrix of integers and averages the columns.
 * @param values is a pointer to a Nx3 matrix
 * @param a is the average of the first column
 * @param b is the average of the second column
 * @param c is the average of the third column
 */
void math_vect_avg_int(int (*values)[VECT_ROWS][3], int * a, int * b, int * c);

/**
 * This function takes an array of values and computes their variance.
 * @param values is pointer to the input array
 * @return variance of all the values in the supplied values
 */
float math_variance_i(int * values);

/**
 * This function takes an array of values and computes their variance.
 * @param values is pointer to the input array
 * @return variance of all the values in the supplied values
 */
float math_variance_f(float * values);
#endif /* MATH_UTILS_H_ */
