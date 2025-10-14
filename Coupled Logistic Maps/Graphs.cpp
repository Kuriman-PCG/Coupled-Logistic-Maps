// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "implot.h"
#include "Graphing.h"
#include <vector>

using namespace std;

/* function to carry out one movement in the model */
float step(float x_n, float mu)
{
	float x_n1 = mu * x_n * (1 - x_n);
	return(x_n1);
}

void cobfunc(float* mus, float* xs, float mu) {

	static float xs1[1001], ys1[1001];
	static float xs2[1001], ys2[1001];
	float div = 1001;
	for (int i = 0; i < 1001; ++i) {
		xs1[i] = i * 0.001f;
		ys1[i] = step(xs1[i], (float)mu);
		std::cout << "x[" << i << "] = " << xs1[i] << ", mu = " << (float)mu << std::endl;
		ys2[i] = (float)i / div;
	}

	ImPlot::SetupAxes("x", "y");
	ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
	ImPlot::PlotLine("g(x)", xs1, ys2, 1001);
	ImPlot::PlotLine("cobweb", mus, xs, 41);
}

void cobweb(float mu)
{
	float static x0 = 0.9;
	float static xs[41], ys[41];
	xs[0] = x0;
	ys[0] = x0;

	for (int i = 1; i < 21; i++) {
		xs[2 * i - 1] = xs[2 * i - 2];
		ys[2 * i - 1] = step(xs[2 * i - 2], mu);
		xs[2 * i] = ys[2 * i - 1];
		ys[2 * i] = ys[2 * i - 1];
	}

	Graph(cobfunc, xs, ys, mu);

}


float truncate(float x) {
	return (trunc(x * 100000) / 100000);
}

bool search(vector<float> a, float b) {
	for (int i = 0; i < a.size(); i++)
		if (truncate(a[i]) == truncate(b))
			return true;
	return false;
}

void logfunc(float* mus, float* xs, float number) {
	ImPlot::PlotScatter("Data 1", mus, xs, (int)number);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.05f);
	ImPlot::PopStyleVar();
}

void logistic() {

	/* initialising variables */

	// mu
	float mu_min = 0;
	float mu_max = 4;
	int const mu_step = 2000;

	//starting value of x
	float x_0 = 0.9;

	//resolution
	int const no_of_steps = 500;
	int const cutoff = 200;

	//number of total points
	int const number = (mu_step + 1) * (no_of_steps - cutoff - 1);
	static float mus[number], xs[number];

	int mudummy = 0;
	for (float mu = mu_min; mu <= mu_max; mu += ((mu_max - mu_min) / (float) mu_step)) {
		// simulating the network
		vector<float> blacklist = { 0 };
		float x[no_of_steps + 1] = {};
		x[0] = x_0;
		for (int t = 0; t < no_of_steps; t++)
		{
			//std::cout << "x[" << t << "] = " << x[t] << ", mu = " << mu << std::endl;
			x[t + 1] = step(x[t], mu);

			if (t > cutoff && !search(blacklist, x[t])) {
				//std::cout << "Unique found" << std::endl;
				mus[mudummy] = mu;
				xs[mudummy] = x[t];
				mudummy++;
				blacklist.push_back(truncate(x[t]));
			}
		}
	}

	Graph(logfunc, mus, xs, number);
}