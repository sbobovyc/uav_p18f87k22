/**
 *
 * @file Kalman.c
 *
 * @date Feb 5, 2011
 * @author sbobovyc
 * @details
 *
 */
#include "Kalman.h"

void Kalman_init(struct Gyro1DKalman *filterdata, const float Q_angle, const float Q_gyro, const float R_angle)
{
		filterdata->Q_angle = Q_angle;
		filterdata->Q_gyro  = Q_gyro;
		filterdata->R_angle = R_angle;
		filterdata->P_00 = 1;
		filterdata->P_01 = 0;
		filterdata->P_10 = 0;
		filterdata->P_11 = 1;
}

/**
 * @details
 * 1. Predicted (a priori) state estimate \n
\f{eqnarray*}{
	\hat{x}_{k+1} &=&
	\left[
	\begin{array}{ c }
		angle \\
		bias
	\end{array} \right]_{k+1} \\
	 &=& A \hat{x}_k + B u_k \\
	 &=&
	\left[
	\begin{array}{ c c }
		1 & -dt \\
		0 & 1
	\end{array} \right] *
	\left[
	\begin{array}{ c }
		angle \\
		bias
	\end{array} \right]_k +
	\left[
	\begin{array}{ c }
		dt \\
		0
	\end{array} \right]_k * u_k \\
	&=&
	\left[
	\begin{array}{ c }
		angle_k - bias_k * dt \\
		bias_k
	\end{array} \right] +
	\left[
	\begin{array}{ c }
		u_k * dt \\
		0
	\end{array} \right] \\
	&=&
	\left[
	\begin{array}{ c }
		angle_k + dt ( u_k - bias_k ) \\
		bias_k
	\end{array} \right]

\f}

* 2. Predicted (a priori) estimate covariance \n
\f{eqnarray*}{
	P_{k+1} &=& A P_k A^T + Q \\
	&=&
	\left[
	\begin{array}{ c c }
		1 & -dt \\
		0 & 1
	\end{array} \right] *
	\left[
	\begin{array}{ c c }
		P_{00} & P_{01} \\
		P_{10} & P_{11}
	\end{array} \right] *
	\left[
	\begin{array}{ c c }
		1 & 0 \\
		-dt & 1
	\end{array} \right] +
	\left[
	\begin{array}{ c c }
		cov(angle, angle) & 0 \\
		0 & cov(bias, bias)
	\end{array} \right]

\f}

The matrices can be multiplied by hand and yield these equations: \n
\f{eqnarray*}{

	P(0,0) &=& P(0,0) - dt * ( P(1,0) + P(0,1) ) + dt^2 * P(1,1) + Q(0,0) \\
	P(0,1) &=& P(0,1) - dt * P(1,1) + Q(0,1) \\
	P(1,0) &=& P(1,0) - dt * P(1,1) + Q(1,0) \\
	P(1,1) &=& P(1,1) + Q(1,1)
\f}

 */
void Kalman_predict(struct Gyro1DKalman *filterdata, const float dotAngle, const float dt)
{
	filterdata->x_angle += dt * (dotAngle - filterdata->x_bias);

	//filterdata->P_00 +=  - dt * (filterdata->P_10 + filterdata->P_01) + filterdata->Q_angle * dt; //this does not match up with the derived equation above
	filterdata->P_00 +=  - dt * (filterdata->P_10 + filterdata->P_01) + dt * dt * filterdata->P_11 + filterdata->Q_angle;
	filterdata->P_01 +=  - dt * filterdata->P_11;
	filterdata->P_10 +=  - dt * filterdata->P_11;
	filterdata->P_11 +=  + filterdata->Q_gyro * dt;
}

/**
	1. Innovation or measurement residual \n
	\f{eqnarray*}{
		\hat{y}_k = z_k - C * \hat{x}_k
	\f}

	2. Innovation (or residual) covariance \n
	\f{eqnarray*}{
	S_k &=& C_k P_k C_{k}^T + R_k \\
		&=&
		\left[
		\begin{array}{ c c }
		1 & 0 \\
		\end{array}\right] * P_k *
		\left[
		\begin{array}{ c }
		1 \\
		0
		\end{array} \right] + R_k \\
    	&=& P(0,0) + R_k
    \f}

	3.Optimal Kalman gain
	\f{eqnarray*}{
	K_k &=& P_k * C_{k}^T * S_{k}^{-1} \\
		&=&
		\left[
		\begin{array}{ c }
		P(0,0) \\
		P(1,0)
		\end{array} \right] / S_k \\
	\f}

	4. Updated (a posteriori) state estimate
 	\f{eqnarray*}{
 		\hat{x} = \hat{x} + K \tilde{y}
 	\f}

	5. Updated (a posteriori) estimate covariance
	\f{eqnarray*}{
		P &=& (I - K_k C_k) P \\
		  &=&
		  \left[
		\left[
		\begin{array}{ c c }
		1 & 0 \\
		0 & 1
		\end{array} \right] -
		\left[
		\begin{array}{ c }
		K_0 \\
		K_1
		\end{array} \right] \right] * P \\
		&=&
		\left[
		\begin{array}{ c c }
		P(0,0)-P(0,0)*K(0) & P(0,1)-P(0,1)*K(0) \\
		P(1,0)-P(0,0)*K(1) & P(1,1)-P(0,1)*K(1)
		\end{array} \right]
	\f}

*/
float Kalman_update(struct Gyro1DKalman *filterdata, const float angle_m)
{
	const float y = angle_m - filterdata->x_angle;

	const float S = filterdata->P_00 + filterdata->R_angle;
	const float K_0 = filterdata->P_00 / S;
	const float K_1 = filterdata->P_10 / S;

	filterdata->x_angle +=  K_0 * y;
	filterdata->x_bias  +=  K_1 * y;

	filterdata->P_00 -= K_0 * filterdata->P_00;
	filterdata->P_01 -= K_0 * filterdata->P_01;
	filterdata->P_10 -= K_1 * filterdata->P_00;
	filterdata->P_11 -= K_1 * filterdata->P_01;

	return filterdata->x_angle;
}
