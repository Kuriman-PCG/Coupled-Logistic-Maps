#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include "implot.h"
#include "Graphing.h"

void algfunc(float* xs, float* ts, float number) {
	ImPlot::PlotScatter("Data 1", ts, xs, (int)number);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.005f);
	ImPlot::PopStyleVar();
}

void algorithm() {

	std::random_device rd;
	std::mt19937 gen(rd());

	const int no_of_steps = 10000;
	float X[no_of_steps + 1], t[no_of_steps + 1];
	X[0] = 0;
	t[0] = 0;

	float k = 2;
	float gamma = 0.1f;

	for (int i = 0; i < no_of_steps; i++) {
		float current_X = X[i];
		float rates[2] = { k, gamma * current_X };
		float rate_sum = rates[0] + rates[1];

		std::exponential_distribution<float> d(rate_sum);
		float tau = d(gen);
		t[i + 1] = t[i] + tau;

		std::uniform_real_distribution<> dis(0, 1);
		float randomValue = dis(gen);
		if (randomValue * rate_sum > 0 && randomValue * rate_sum < rates[0]) {
			X[i + 1] = X[i] + 1;
		}
		else if (randomValue * rate_sum > rates[0] && randomValue * rate_sum < rates[0] + rates[1]) {
			X[i + 1] = X[i] - 1;
		}
	}
	std::cout << X[1000] << ", " << t[1000];
	Graph(algfunc, X, t, no_of_steps);
}