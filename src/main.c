/*
 *	Authored 2021, Chatura Samarakoon.
 *
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *	*	Redistributions of source code must retain the above
 *		copyright notice, this list of conditions and the following
 *		disclaimer.
 *	*	Redistributions in binary form must reproduce the above
 *		copyright notice, this list of conditions and the following
 *		disclaimer in the documentation and/or other materials
 *		provided with the distribution.
 *	*	Neither the name of the author nor the names of its
 *		contributors may be used to endorse or promote products
 *		derived from this software without specific prior written
 *		permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uncertain.h>

typedef enum
{
	kBenchmarkModesCustomMonteCarlo = 1,
} BenchmarkModes;

double calcPosterior_bySampling(double prior, double likelihood, const int n_samples);
double calcPosterior(double prior, double likelihood, double marginal_likelihood);
float  calcM(double Phi, float alpha);
float  calcTheta(double Phi);
double bayesianQPEupdate(double Phi, float M, float theta, BenchmarkModes benchmarkMode);

int
main(int argc, char * argv[])
{
	BenchmarkModes mode = 0;
	float          alpha = 0;
	float          mu_0 = M_PI_2;
	float          sigma_0 = 0.5;
	int            QPEsteps = 5;
	double         Phi[QPEsteps + 1];
	float          M[QPEsteps + 1];
	float          theta[QPEsteps + 1];

	/*
	 *	Get mode from arguments.
	 */
	if ((argc == 2) && (strcmp("1", argv[1]) == 0))
	{
		mode = kBenchmarkModesCustomMonteCarlo;
		printf("Custom Monte Carlo mode.\n");
	}
	else
	{
		printf("Uncertain Propagation mode.\n");
	}

	M[0] = 1 / pow(sigma_0, alpha);
	theta[0] = mu_0 - sigma_0;
	/*
	 *	Define Phi Prior
	 */
	Phi[0] = libUncertainDoubleUniformDist(-M_PI, M_PI);

	printf("Phi (iter 0): %lf\n", Phi[0]);
	for (size_t i = 0; i < QPEsteps; i++)
	{
		Phi[i + 1] = bayesianQPEupdate(Phi[i], M[i], theta[i], mode);
		M[i + 1] = calcM(Phi[i + 1], alpha);
		theta[i + 1] = calcTheta(Phi[i + 1]);
		printf("Phi (iter %d): %lf\n", i + 1, Phi[i + 1]);
	}

	return EXIT_SUCCESS;
}

float
calcM(double Phi, float alpha)
{
	return 1 / pow(sqrt(libUncertainDoubleNthMoment(Phi, 2)), alpha);
}

float
calcTheta(double Phi)
{
	return libUncertainDoubleNthMoment(Phi, 1) - sqrt(libUncertainDoubleNthMoment(Phi, 2));
}

double
bayesianQPEupdate(double Phi, float M, float theta, BenchmarkModes benchmarkMode)
{
	double E0_given_phi;
	double E1_given_phi;
	double Phi_given_E0;
	double Phi_given_E1;
	double Phi_given_E01;
	/*
	 *	using Laplace
	 */
	if (benchmarkMode != kBenchmarkModesCustomMonteCarlo)
	{
		E0_given_phi = ((1 + cos(M * (Phi - theta))) / 2);
		E1_given_phi = ((1 - cos(M * (Phi - theta))) / 2);
	}
	/*
	 *	using MC
	 */
	else
	{
		const int n_samples = 3000;
		double    E0_given_phi_samples[n_samples];
		double    E1_given_phi_samples[n_samples];
		for (size_t i = 0; i < n_samples; i++)
		{
			E0_given_phi_samples[i] =
				((1 + cos(M * (libUncertainDoubleSample(Phi) - theta))) / 2);
			E1_given_phi_samples[i] =
				((1 - cos(M * (libUncertainDoubleSample(Phi) - theta))) / 2);
		}
		E0_given_phi = libUncertainDoubleDistFromSamples(E0_given_phi_samples, n_samples);
		E1_given_phi = libUncertainDoubleDistFromSamples(E1_given_phi_samples, n_samples);
	}

	Phi_given_E0 = E0_given_phi * Phi; // proportional to
	Phi_given_E1 = E1_given_phi * Phi; // proportional to

	Phi_given_E01 = libUncertainDoubleMixture(Phi_given_E0, Phi_given_E1, 0.5);
	return Phi_given_E01;
}

	double
calcPosterior(double prior, double likelihood, double marginal_likelihood)
{
	return (prior * likelihood) / marginal_likelihood;
}

double
calcPosterior_bySampling(double prior, double likelihood, const int n_samples)
{
	double sample_array[n_samples];
	int    i;
	for (i = 0; i < n_samples; i++)
	{
		sample_array[i] =
			libUncertainDoubleSample(prior) * libUncertainDoubleSample(likelihood);
	}
	return libUncertainDoubleDistFromSamples(sample_array, n_samples);
}
