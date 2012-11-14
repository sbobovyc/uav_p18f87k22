/**
 *
 * @file Kalman.h
 * One dimensional Kalman filter implementation that uses a gyroscope as an input.
 *
 * @date Feb 5, 2011
 * @author sbobovyc
 * @details

	This is the Kalman state equation:
 	\f$ x_{k+1} = A x_k + B u_k + w_k \f$ \n
	where \f$ x \f$ is the state, \f$ A \f$ and \f$ B \f$ are matrices, \f$ u \f$ is a known input to the system, \f$ w_k \sim N(0,Q_k) \f$ is process noise vector.

	This is the Kalman output equation:
	\f$ y_{k} = C x_k + z_k \f$ \n
	where \f$ y \f$ is the measured output, \f$ C \f$ is a matrix, \f$ z_k \sim N(0,R_k) \f$ is measurement noise vector.

	In this application of the Kalman filter: \n
	\f$ x_k =
	\left[
	\begin{array}{ c }
		angle_k \\
		bias_k
	\end{array} \right]
	\f$

	\f$ A =
	\left[
	\begin{array}{ c c }
		1 &	-dt \\
		0 &	1
	\end{array} \right]
	\f$

	\f$ B =
	\left[
	\begin{array}{ c }
		dt \\
		0
	\end{array} \right]
	\f$

	\f$ C =
	\left[
	\begin{array}{ c }
		1 \\
		0
	\end{array} \right]
	\f$

	The noise covariance matrices \f$ S_w \f$ and \f$ S_z \f$ are defined as such: \n
	Process noise: \n
	\f$ S_w = E(w_k w_{k}^T) \f$ \n
	Observation noise: \n
	\f$ S_z = E(z_k z_{k}^T) \f$ \n

	In this application, assume gyroscope angle and gyroscope bias noise is independent and does not vary with time (linear with dt): \n
	\f$ S_w = Q =
	\left[
	\begin{array}{ c c }
		var(angle) & 0 \\
		0 & var(bias)
	\end{array} \right]
	\f$

	In this application, assume accelerometer angle noise is independent and does not vary with time (linear with dt):
	\f$ S_z = R =
	\left[
	\begin{array}{ c c }
		cov(angle, angle) & 0 \\
		0 & cov(bias, bias)
	\end{array} \right]
	\f$
	\n
	\f$ cov(X,X) = var(X) = E[ (X - E[X])^2] \f$

	These are the Kalman filter equations. \n
	The Kalman gain: \n
	\f$ K_k = A P_k C^T (C P_k C^T + S_z)^{-1} \f$ \n
	The state estimate:\n
	\f$ \hat{x}_{k+1} = (A \hat{x}_{k} + B u_k) + K_k(y_{k+1} - C \hat{x}_{k}) \f$ \n
	The error estimate: \n
	\f$ P_{k+1} = A P_k A^T + S_w - A P_k C^T S_{z}^{-1} C P_k A^T \f$
 */

#ifndef KALMAN_H_
#define KALMAN_H_


struct Gyro1DKalman
{
	// These variables represent our state matrix x
	float x_angle,
	      x_bias;

	// Estimation error covariance matrix
	float P_00,
	      P_01,
	      P_10,
	      P_11;

	/*
	 * Q is a 2x2 covariance matrix. Because we
	 * assume the gyro and accelero noise to be independent
	 * of each other, the covariances on the / diagonal are 0.
	 *
	 * Covariance Q, the process noise, from the assumption
	 *    x = F x + B u + w
	 * with w having a normal distribution with covariance Q.
	 * (covariance = E[ (X - E[X])*(X - E[X])' ]
	 * We assume is linear with dt
	 */
	float Q_angle, Q_gyro;
	/*
	 * Covariance R, our observation noise (from the accelerometer)
	 * Also assumed to be linear with dt
	 */
	float R_angle;
};

/**
 * Initializing the struct.
 * @param *filterdata is a pointer to the struct that contains the state of the filter
 * @param Q_angle is the variance of the angle measurements
 * @param Q_gyro is the variance of the gyroscope
 * @param R_angle is the variance of the accelerometer
 */
void Kalman_init(struct Gyro1DKalman *filterdata, const float Q_angle, const float Q_gyro, const float R_angle);


/**
 * This is the Kalman predict function. It updates 2 variables, model-state x and the 2x2 matrix P.
 * @param *filterdata is a pointer to the struct that contains the state of the filter
 * @param gyro is the current angle (in radians/second) measured by the gyroscope
 * @param dt is the time difference since last measurement
 *
 */
void Kalman_predict(struct Gyro1DKalman *filterdata, const float gyro, const float dt);


/**
 *  This is the Kalman update function. The update function updates our model using
 *  the information from a 2nd measurement.
 *  @param *filterdata is a pointer to the struct that contains the state of the filter
 *  @param angle_m is the angle (in radians) measured by the accelerometer.
 */
float Kalman_update(struct Gyro1DKalman *filterdata, const float angle_m);


#endif /* KALMAN_H_ */
