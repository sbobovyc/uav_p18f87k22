/**
 *
 * @file math_utils.c
 *
 * @date Feb 21, 2011
 * @author sbobovyc
 * @details
 *
 */
#include <math.h>
#include "math_utils.h"


void math_vect_avg_float(float (*values)[VECT_ROWS][3], float * a, float * b, float * c)
{
	int i;
	float tmp_a = 0;
	float tmp_b = 0;
	float tmp_c = 0;
	float * ptr = (float *)values;

	// Tricky pointer arithmetic is used to loop over a two dimensional array.
	for(i=0; i<VECT_ROWS; i++)
	{
		tmp_a += *(ptr+i*3);
		tmp_b += *(ptr+i*3+1);
		tmp_c += *(ptr+i*3+2);
	}
	//TODO To prevent rounding errors, such as 3.9 = 3, a rounding function should be used.
	*a = ((float)tmp_a / VECT_ROWS);
	*b = ((float)tmp_b / VECT_ROWS);
	*c = ((float)tmp_c / VECT_ROWS);
}

void math_vect_avg_int(int (*values)[VECT_ROWS][3], int * a, int * b, int * c)
{
	int i;
	long int tmp_a = 0;
	long int tmp_b = 0;
	long int tmp_c = 0;
	int * ptr = (int *)values;

	// Tricky pointer arithmetic is used to loop over a two dimensional array.
	for(i=0; i<VECT_ROWS; i++)
	{
		tmp_a += *(ptr+i*3);
		tmp_b += *(ptr+i*3+1);
		tmp_c += *(ptr+i*3+2);
	}
	//TODO To prevent rounding errors, such as 3.9 = 3, a rounding function should be used.
	*a = (int)((float)tmp_a / VECT_ROWS);
	*b = (int)((float)tmp_b / VECT_ROWS);
	*c = (int)((float)tmp_c / VECT_ROWS);
}

/**
	The formula for calculating the variance of an entire statistical population of size ''n'' is: \n
	\f$ \sigma^2 = \displaystyle\frac {\sum_{i=1}^n x_i^2 - (\sum_{i=1}^n x_i)^2/n}{n}. \f$ \n
	The formula for calculating an unbiased estimate of the population variance from a finite statistical sample of ''n'' observations is: \n
	\f$ s^2 = \displaystyle\frac {\sum_{i=1}^n x_i^2 - (\sum_{i=1}^n x_i)^2/n}{n-1}. \f$ \n
	Due to roundoff errors, a compensated summation algorithm is used.
*/
float math_variance_i(int * values)
{
    int i = 0;
    int sum1 = 0;
    float sum2 = 0;
    float sum3 = 0;
    float mean = 0;

    for(i=0; i<VECT_ROWS; i++)
    {
    	sum1 += values[i];
    }

    mean = (float)sum1/VECT_ROWS;

    for(i=0; i<VECT_ROWS; i++)
    {
        sum2 = sum2 + (values[i] - mean)*(values[i] - mean);
        sum3 = sum3 + (values[i] - mean);
    }

    return (sum2 - (sum3*sum3)/VECT_ROWS)/(VECT_ROWS - 1);
}

float math_variance_f(float * values)
{
    int i = 0;
    float sum1 = 0;
    float sum2 = 0;
    float sum3 = 0;
    float mean = 0;

    for(i=0; i<VECT_ROWS; i++)
    {
    	sum1 += values[i];
    }

    mean = (float)sum1/VECT_ROWS;

    for(i=0; i<VECT_ROWS; i++)
    {
        sum2 = sum2 + (values[i] - mean)*(values[i] - mean);
        sum3 = sum3 + (values[i] - mean);
    }

    return (sum2 - (sum3*sum3)/VECT_ROWS)/(VECT_ROWS - 1);
}