// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "implot.h"
#include "Graphing.h"

/* function to carry out one movement in the model */
float step(float x_n, float mu)
{
	float x_n1 = mu * x_n * (1 - x_n);
	return(x_n1);
}

void func(float* mus, float* xs, int number) {
	/*
	ImPlot::PlotScatter("Data 1", mus, xs, number);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.05f);
	ImPlot::PopStyleVar();
	*/

	static float xs1[1001], ys1[1001];
	static float xs2[1001], ys2[1001];
	float div = 1001;
	float mu = 3.8;
	for (int i = 0; i < 1001; ++i) {
		xs1[i] = i * 0.001f;
		ys1[i] = mu * xs1[i] * (1 - xs1[i]);
		ys2[i] = (float)i/div;
	}

	ImPlot::SetupAxes("x", "y");
	ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
	ImPlot::PlotLine("g(x)", xs1, ys2, 1001);
		
}

// Simple free list based allocator
int main()
{
	float ummy[1], dumm[1];
	Graph(func, ummy, dumm, 1);




	/* initialising variables */
	/*
	float mu_min = 3.84;
	float mu_max = 3.86;
	int const mu_step = 1000;
	float x_0 = 0.9;
	int const no_of_steps = 1000;
	int const cutoff = 550;
	int const number = (mu_step + 1) * (no_of_steps - cutoff - 1);

	static float mus[number], xs[number];

	int mudummy = 0;
	for (float mu = mu_min; mu <= mu_max; mu += ((mu_max - mu_min) / (float) mu_step)) {
		// simulating the network 
		float blacklist[no_of_steps - cutoff - 1];
		float x[no_of_steps + 1] = {};
		x[0] = x_0;
		for (int t = 0; t < no_of_steps; t++)
		{
			x[t + 1] = step(x[t], mu);

			if (t > cutoff) {
				//std::cout << "x[" << t << "] = " << x[t] << ", mu = " << mu << std::endl;
				mus[mudummy] = mu;
				xs[mudummy] = x[t];
				mudummy++;
			}
		}
	}
	
	Graph(func, mus, xs, number);
	*/

	return(0);
}