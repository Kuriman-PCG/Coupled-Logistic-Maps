// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "implot.h"
#include "Graphing.h"
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <math.h>

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


/* initialising variables */

// mu
float mu_min = -2;
float mu_max = 4;
int const mu_step = 1000;

//starting value of x
float x_0 = 0.5;

//resolution
int const no_of_steps = 1000;
int const cutoff = 900;

//number of total points
int const number = (mu_step + 1) * (no_of_steps - cutoff - 1);


void logfunc(float* xs, float* exps, float a) {

	static float mus1[number], mus2[mu_step];
	int mudummy1 = 0;
	int mudummy2 = 0;
	for (float mu = mu_min; mu <= mu_max; mu += ((mu_max - mu_min) / (float)mu_step)) {
		mus2[mudummy2++] = mu;
		for (int t = 0; t < no_of_steps; t++) 
			if (t > cutoff) 
				mus1[mudummy1++] = mu;
	}

	ImPlot::PlotScatter("Data 1", mus1, xs, number);
	ImPlot::PlotLine("Data 2", mus2, exps, mu_step);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
	ImPlot::PopStyleVar();
}

void logistic() {

	static float xs[number], exps[mu_step];

	int mudummy = 0;
	int expdummy = 0;
	for (float mu = mu_min; mu <= mu_max; mu += ((mu_max - mu_min) / (float) mu_step)) {
		// simulating the network
		vector<float> blacklist = { 0 };
		float x[no_of_steps + 1] = {};
		x[0] = x_0;
		float exp = 0.0f;
		exp = 0.0f;
		for (int t = 0; t < no_of_steps; t++)
		{
			//std::cout << "x[" << t << "] = " << x[t] << ", mu = " << mu << std::endl;
			x[t + 1] = step(x[t], mu);

			if (t > cutoff /* && !search(blacklist, x[t])*/) {
				xs[mudummy] = x[t];
				mudummy++;
				blacklist.push_back(truncate(x[t]));
				exp += log(abs(mu * (1.0f - (2.0f * x[t]))));
			}
		}
		exps[expdummy++] = exp / (float)(no_of_steps - cutoff);
	}

	
	Graph(logfunc, xs, exps, number);
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

void stologfunc(float* xs, float* ts, float number) {

	static ScrollingBuffer sdata1, sdata2;
	static float xy[2] = { 0.5f, 0.5f };
	static float r = 2;
	static float a = 2;
	static float b = 2;
	static float alpha = 0.25;
	static float t = 0;
	static float history = 10.0f;
	static bool constraint = false;
	static bool coeffecient = false;

	std::random_device rd;
	std::mt19937 gen(rd());

	float current_X = xy[0];
	float current_Y = xy[1];
	float rates[4];
	if (!coeffecient) {
		rates[0] = abs(r * current_X - r * current_X * current_X);
		rates[1] = abs(r * current_Y - r * current_Y * current_Y);
		rates[2] = alpha * current_X;
		rates[3] = alpha * current_Y;
	}
	else {
		rates[0] = abs(a * current_X - a * current_X * current_X);
		rates[1] = abs(b * current_Y - b * current_Y * current_Y);
		rates[2] = alpha * current_X;
		rates[3] = alpha * current_Y;
	}
	float rate_sum = rates[0] + rates[1] + rates[2] + rates[3];

	std::exponential_distribution<float> d(rate_sum);
	float tau = d(gen);
	t += tau;

	std::uniform_real_distribution<> dis(0, 1);
	float randomValue = dis(gen);
	if (randomValue * rate_sum > 0 && randomValue * rate_sum < rates[0]) {
		xy[0] = step(current_X, coeffecient ? a : r);
	}
	else if (randomValue * rate_sum > rates[0] && randomValue * rate_sum < rates[0] + rates[1]) {
		xy[1] = step(current_Y, coeffecient ? b : r);
	}
	else if (randomValue * rate_sum > rates[0] + rates[1] && randomValue * rate_sum < rates[0] + rates[1] + rates[2]) {
		xy[0] = current_X - (alpha * current_X);
		xy[1] = current_Y + (alpha * current_X);
	}
	else if (randomValue * rate_sum > rates[0] + rates[1] + rates[2] && randomValue * rate_sum < rates[0] + rates[1] + rates[2] + rates[3]) {
		xy[0] = current_X + (alpha * current_Y);
		xy[1] = current_Y - (alpha * current_Y);
	}
	if (constraint) {
		xy[0] = xy[0] < 1.0f ? xy[0] : 1.0f;
		xy[0] = xy[0] > 0.0f ? xy[0] : 0.0f;
		xy[1] = xy[1] < 1.0f ? xy[1] : 1.0f;
		xy[1] = xy[1] > 0.0f ? xy[1] : 0.0f;
	}

	sdata1.AddPoint(t, xy[0]);
	sdata2.AddPoint(t, xy[1]);
	this_thread::sleep_for(std::chrono::milliseconds(75));

	if (!coeffecient) {
		ImGui::SliderFloat("r", &r, 0, 4, "%.2f");
	}
	else {
		ImGui::SliderFloat("a", &a, 0, 4, "%.2f");
		ImGui::SliderFloat("b", &b, 0, 4, "%.2f");
	}
	ImGui::SliderFloat("alpha", &alpha, 0, 1, "%.2f");
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
	if (ImGui::Button("Revive From Death")) {
		xy[0] = 0.5f;
		xy[1] = 0.5f;
	} 
	ImGui::SameLine();
	ImGui::Checkbox("Constrain", &constraint);
	ImGui::SameLine();
	ImGui::Checkbox("Separate coeffecients", &coeffecient);

	ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
	ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
	ImPlot::PlotLine("Population1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
}

void stochasticlogistic() {
	const int no_of_steps = -2.0f;
	float X[3], t[3];
	Graph(stologfunc, X, t, no_of_steps);
}