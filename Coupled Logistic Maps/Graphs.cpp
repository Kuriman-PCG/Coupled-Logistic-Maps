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

void cobfunc(float* mus, float* xs, float num) {

	float static mu = 3.5;

	static float xs1[1001], ys1[1001];
	float div = 1001;
	for (int i = 0; i < 1001; ++i) {
		xs1[i] = i * 0.001f;
		ys1[i] = step(xs1[i], (float)mu);
	}

	float static x0 = 0.155;
	float static xs2[41], ys2[41];
	xs2[0] = x0;
	ys2[0] = x0;

	for (int i = 1; i < 21; i++) {
		xs2[2 * i - 1] = xs2[2 * i - 2];
		ys2[2 * i - 1] = step(xs2[2 * i - 2], mu);
		xs2[2 * i] = ys2[2 * i - 1];
		ys2[2 * i] = ys2[2 * i - 1];
	}

	ImPlot::SetupAxes("x", "y");
	ImGui::SliderFloat("Mu", &mu, 1, 5, "%.3f");
	ImGui::SliderFloat("x0", &x0, 0, 1, "%.3f");
	ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
	ImPlot::PlotLine("g(x)", xs1, xs, 1001);
	ImPlot::PlotLine("cobweb", xs2, ys2, 41);
}

void cobweb(float mu)
{
	static float xs2[1001], ys2[1001];
	for (int i = 0; i < 1001; ++i) {
		ys2[i] = (float)i / 1001;
	}

	Graph(cobfunc, xs2, ys2, -1.0f);
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
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
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


// utility structure for realtime plot
struct RollingBuffer {
	float Span;
	ImVector<ImVec2> Data;
	RollingBuffer() {
		Span = 10.0f;
		Data.reserve(2000);
	}
	void AddPoint(float x, float y) {
		float xmod = fmodf(x, Span);
		if (!Data.empty() && xmod < Data.back().x)
			Data.shrink(0);
		Data.push_back(ImVec2(xmod, y));
	}
};

struct ScrollingBuffer {
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	ScrollingBuffer(int max_size = 2000) {
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
		}
	}
};

void popfunc(float* mus, float* xs, float number) {
	
	static ScrollingBuffer sdata1, sdata2;
	static float t = 0;
	static float history = 10.0f;
	static float mu = 1.8f;
	static float x = 0.6;

	t += ImGui::GetIO().DeltaTime;
	x = step(x, mu);
	sdata2.AddPoint(t, x);

	ImGui::SliderFloat("Mu", &mu, 1, 4, "%.1f");
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

	static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

	ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
	ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
	ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
	ImPlot::PlotLine("Population", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
}

void population() {
	float mus[3], xs[3];
	float num = -1.0f;

	Graph(popfunc, mus, xs, num);
}