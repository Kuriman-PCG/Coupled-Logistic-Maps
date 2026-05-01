
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


float og_step(float x_n, float mu)
{
	float x_n1 = mu * x_n * (1 - x_n);
	return(x_n1);
}


float* step(float x_n, float y_n, float z_n, float a, float b, float c, float d) {
	float x_n1 = a * x_n * (1 - x_n) - c * x_n + c * 0.5f * y_n;
	float y_n1 = b * y_n * (1 - y_n) - c * y_n + c * x_n + c * z_n;
	float z_n1 = d * z_n * (1 - z_n) - c * z_n + c * 0.5f * y_n;
	float xyz[3] = { x_n1, y_n1, z_n1 };
	return xyz;
}

float* step3(float x_n, float y_n, float a, float b, float c, float d) {
	float x_n1 = a * x_n * (1 - x_n) - c * x_n + d * y_n;
	float y_n1 = b * y_n * (1 - y_n) - d * y_n + c * x_n;
	float xy[2] = { x_n1, y_n1 };
	return xy;
}

float* step2(float x_n, float y_n, float a, float b, float c, float d) {
	float x_n1 = (1-c) * a * x_n * (1 - x_n) + d * b * y_n * (1 - y_n);
	float y_n1 = (1-d) * b * y_n * (1 - y_n) + c * a * x_n * (1 - x_n);
	float xy[2] = { x_n1, y_n1 };
	return xy;
}

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

void coupfunc(float* mus, float* xs, float number) {

	//static int i = 0;
	//static int j = 1;
	//
	//if (i % j != 0) {
	//	i++;
	//	j++;
	//	return;
	//}
	//i++;
	//j++;

	static ScrollingBuffer sdata1, sdata2, sdata3;
	static float t = 0;
	static float history = 10.0f;
	static float a = 1.8f;
	static float b = 1.8f;
	static float c = 0.1f;
	static float d = 0.1f;
	static float xyz[3] = { 0.6f, 0.4f, 0.0f };
	static float xy[2] = { 0.5f, 0.4f };
	static int timer = 100;

	t++;
	this_thread::sleep_for(std::chrono::milliseconds(timer));

	//float *output = step(xyz[0], xyz[1], xyz[2], a, b, c, d);
	//xyz[0] = output[0];
	//xyz[1] = output[1];
	//xyz[2] = output[2];

	float* output = step2(xy[0], xy[1], a, b, c, d);
	xy[0] = output[0];
	xy[1] = output[1];
	
	sdata1.AddPoint(t, xy[0]);
	sdata2.AddPoint(t, xy[1]);
	//sdata3.AddPoint(t, xyz[2]);

	ImGui::SliderFloat("a", &a, 0, 4, "%.2f");
	ImGui::SliderFloat("b", &b, 0, 4, "%.2f");
	ImGui::SliderFloat("c", &c, 0, 1, "%.2f");
	ImGui::SliderFloat("d", &d, 0, 1, "%.2f");
	ImGui::SliderInt("Timer", &timer, 0, 1000);
	//ImGui::SliderFloat("d", &d, 0, 4, "%.1f");
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
	if (ImGui::Button("Revive From Death")) {
		//xyz[0] = 0.5f;
		//xyz[1] = 0.5f;
		//xyz[2] = 0.5f;
		xy[0] = 0.5f;
		xy[1] = 0.5f;
	}

	//static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

	//ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
	ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
	ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
	ImPlot::PlotLine("Population1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
	//ImPlot::PlotLine("Population3", &sdata3.Data[0].x, &sdata3.Data[0].y, sdata3.Data.size(), 0, sdata3.Offset, 2 * sizeof(float));
}

void couple() {
	float mus[3], xs[3];
	float num = -2.0f;

	Graph(coupfunc, mus, xs, num);
}


float coupledstep(float x_n, float mu)
{
	float x_n1 = mu * x_n * (1 - x_n);
	return(x_n1);
}

float coupledcoupling(float x, float a, float alpha, float beta) {
	float coupling = beta * a - alpha * x;
	return coupling;
}

void coupledcobfunc(float* mus, float* xs, float num) {


	float static mu1 = 3.5;
	float static mu2 = 3.5;

	static float xs1[1001], ys1[1001], zs1[1001];
	float div = 1001;
	for (int i = 0; i < 1001; ++i) {
		xs1[i] = i * 0.001f;
		ys1[i] = coupledstep(xs1[i], (float)mu1);
		zs1[i] = coupledstep(xs1[i], (float)mu2);
	}

	float static x0 = 0.155;
	float static alpha = 0.1;
	float static beta = 0.1;
	const int number = 40;
	float static xs2[3 * number + 1], ys2[3 * number + 1], xs3[3 * number + 1], ys3[3 * number + 1];
	xs2[0] = x0;
	ys2[0] = x0;
	xs3[0] = x0;
	ys3[0] = x0;

	for (int i = 1; i < number + 1; i++) {
		xs2[3 * i - 2] = xs2[3 * i - 3];
		ys2[3 * i - 2] = coupledstep(xs2[3 * i - 3], mu1);
		xs3[3 * i - 2] = xs3[3 * i - 3];
		ys3[3 * i - 2] = coupledstep(xs3[3 * i - 3], mu2);

		xs2[3 * i - 1] = xs2[3 * i - 3];
		ys2[3 * i - 1] = coupledstep(xs2[3 * i - 3], mu1) + coupledcoupling(xs2[3 * i - 3], xs3[3 * i - 3], alpha, beta);
		xs3[3 * i - 1] = xs3[3 * i - 3];
		ys3[3 * i - 1] = coupledstep(xs3[3 * i - 3], mu2) - coupledcoupling(xs2[3 * i - 3], xs3[3 * i - 3], alpha, beta);

		xs2[3 * i] = ys2[3 * i - 1];
		ys2[3 * i] = ys2[3 * i - 1];
		xs3[3 * i] = ys3[3 * i - 1];
		ys3[3 * i] = ys3[3 * i - 1];
	}

	static float cratios[1] = { 1.0f };
	static float rratios[2] = { 3.5f,1.0f };
	static ImPlotSubplotFlags flagsa = ImPlotSubplotFlags_ShareItems;

	if (ImPlot::BeginSubplots("My Subplots", 2, 1, ImVec2(-1, -120), flagsa, rratios, cratios)) {
		if (ImPlot::BeginPlot("Cobweb 1", ImVec2())) {
			ImPlot::SetupAxes("x", "y");
			ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
			ImPlot::PlotLine("g(x)", xs1, xs, 1001);
			ImPlot::PlotLine("cobweb", xs2, ys2, 3 * number + 1);
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Cobweb 2", ImVec2())) {
			ImPlot::SetupAxes("x", "y");
			ImPlot::PlotLine("f(x)2", xs1, zs1, 1001);
			ImPlot::PlotLine("g(x)2", xs1, xs, 1001);
			ImPlot::PlotLine("cobweb2", xs3, ys3, 3 * number + 1);
			ImPlot::EndPlot();
		}
		ImPlot::EndSubplots();
	}

	ImGui::SliderFloat("Mu1", &mu1, 1, 5, "%.3f");
	ImGui::SliderFloat("Mu2", &mu2, 1, 5, "%.3f");
	ImGui::SliderFloat("x0", &x0, 0, 1, "%.3f");
	ImGui::SliderFloat("alpha", &alpha, 0, 1, "%.3f");
	ImGui::SliderFloat("beta", &beta, 0, 1, "%.3f");
}

void coupledcobweb()
{
	static float xs2[1001], ys2[1001];
	for (int i = 0; i < 1001; ++i) {
		ys2[i] = (float)i / 1000;
	}

	Graph(coupledcobfunc, xs2, ys2, -3.0f);
}


float* sepstep2(float x_n, float y_n, float a, float b, float c, float d) {
	float x_n05 = a * x_n * (1 - x_n);
	float x_n1 = x_n05 - c * x_n + d * y_n;
	float y_n05 = b * y_n * (1 - y_n);
	float y_n1 = y_n05 - d * y_n + c * x_n;
	float xy[4] = { x_n05, x_n1, y_n05, y_n1 };
	return xy;
}


void sepcoupfunc(float* mus, float* xs, float number) {


	static ScrollingBuffer sdata1, sdata2, sdata3, sdata4;
	static float t = 0;
	static float history = 10.0f;
	static float a = 1.8f;
	static float b = 1.8f;
	static float c = 0.1f;
	static float d = 0.1f;
	static float xy[2] = { 0.5f, 0.4f };
	static float xy05[2] = { 0.5f, 0.4f };
	static int timer = 250;

	t++;
	this_thread::sleep_for(std::chrono::milliseconds(timer));

	float* output = sepstep2(xy[0], xy[1], a, b, c, d);
	xy05[0] = output[0];
	xy[0] = output[1];
	xy05[1] = output[2];
	xy[1] = output[3];

	sdata1.AddPoint(t - 0.05, xy05[0]);
	sdata1.AddPoint(t, xy[0]);
	sdata2.AddPoint(t - 0.05, xy05[1]);
	sdata2.AddPoint(t, xy[1]);
	sdata3.AddPoint(t, xy[0]);
	sdata4.AddPoint(t, xy[1]);

	ImGui::SliderFloat("a", &a, 0, 4, "%.2f");
	ImGui::SliderFloat("b", &b, 0, 4, "%.2f");
	ImGui::SliderFloat("c", &c, 0, 1, "%.2f");
	ImGui::SliderFloat("d", &d, 0, 1, "%.2f");
	ImGui::SliderInt("Timer", &timer, 0, 1000);
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
	if (ImGui::Button("Revive From Death")) {
		xy[0] = 0.5f;
		xy[1] = 0.5f;
		xy05[0] = 0.5f;
		xy05[1] = 0.5f;
	}

	ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
	ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
	ImPlot::PlotLine("Population1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population1a", &sdata3.Data[0].x, &sdata3.Data[0].y, sdata3.Data.size(), 0, sdata3.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population2a", &sdata4.Data[0].x, &sdata4.Data[0].y, sdata4.Data.size(), 0, sdata4.Offset, 2 * sizeof(float));
}

void sepcouple() {
	float mus[3], xs[3];
	float num = -2.0f;

	Graph(sepcoupfunc, mus, xs, num);
}

float timestep(float x, float r) {
	return r * x * (1 - x);
}

float* timestep2(float x_n, float y_n, float a, float b, float c) {
	float x_n1 = (1 - c) * timestep(x_n, a) + c * timestep(y_n, b);
	float y_n1 = c * timestep(x_n, a) + (1 - c) * timestep(y_n, b);
	float xy[2] = { x_n1, y_n1 };
	return xy;
}


void timecoupfunc(float* mus, float* xs, float number) {


	static ScrollingBuffer sdata1, sdata2, sdata3, sdata4;
	static float t = 0;
	static float history = 10.0f;
	static float a = 1.8f;
	static float b = 1.8f;
	static float c = 0.1f;
	static float e = 1.8f;
	static float xy[2] = { 0.5f, 0.5f };
	static float xy2[2] = { 0.5f, 0.5f };
	static bool sep = false;
	static int timer = 50;

	t++;
	this_thread::sleep_for(std::chrono::milliseconds(timer));

	float* output = step2(xy[0], xy[1], sep ? a : e, sep ? b : e, c, c);
	xy[0] = output[0];
	xy[1] = output[1];
	float* output2 = timestep2(xy2[0], xy2[1], sep ? a : e, sep ? b : e, c);
	xy2[0] = output2[0];
	xy2[1] = output2[1];

	sdata1.AddPoint(t, xy[0]);
	sdata2.AddPoint(t, xy[1]);
	sdata3.AddPoint(t, xy2[0]);
	sdata4.AddPoint(t, xy2[1]);


	static float cratios[1] = { 1.0f };
	static float rratios[2] = { 1.0f ,1.0f };
	static ImPlotSubplotFlags flagsa = ImPlotSubplotFlags_ShareItems;

	if (ImPlot::BeginSubplots("My Subplots", 2, 1, ImVec2(-1, -120), flagsa, rratios, cratios)) {
		if (ImPlot::BeginPlot("Pop 1", ImVec2())) {
			ImPlot::SetupAxes("t", "x");
			ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
			ImPlot::PlotLine("Populati1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("Populati2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));

			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Pop 2", ImVec2())) {

			ImPlot::SetupAxes("t", "y");
			ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
			ImPlot::PlotLine("Populati1a", &sdata3.Data[0].x, &sdata3.Data[0].y, sdata3.Data.size(), 0, sdata3.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("Populati2a", &sdata4.Data[0].x, &sdata4.Data[0].y, sdata4.Data.size(), 0, sdata4.Offset, 2 * sizeof(float));

			ImPlot::EndPlot();
		}
		ImPlot::EndSubplots();
	}

	if (sep) {
		ImGui::SliderFloat("a", &a, 0, 4, "%.2f");
		ImGui::SliderFloat("b", &b, 0, 4, "%.2f");
	}
	else {
		ImGui::SliderFloat("e", &e, 0, 4, "%.2f");
	}
	ImGui::SliderFloat("c", &c, 0, 1, "%.2f");
	ImGui::SliderInt("Timer", &timer, 0, 1000);
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
	if (ImGui::Button("Revive From Death")) {
		xy[0] = 0.5f;
		xy[1] = 0.5f;
		xy2[0] = 0.5f;
		xy2[1] = 0.5f;
	}
	if (ImGui::Button("Nudge")) {
		std::random_device dev;
		std::mt19937 gen(dev());
		std::uniform_real_distribution<float> dist6(-0.1, 0.1);

		float ab = dist6(gen);

		xy[0] += dist6(gen);
		xy[1] += dist6(gen);
		xy2[0] += dist6(gen);
		xy2[1] += dist6(gen);
	}
	ImGui::SameLine(); ImGui::Checkbox("Separated", &sep);
}

void timecouple() {
	float mus[3], xs[3];
	float num = -3.0f;

	Graph(timecoupfunc, mus, xs, num);
}


float atruncate(float x) {
	return (trunc(x * 100000) / 100000);
}

bool asearch(vector<float> a, float b) {
	for (int i = 0; i < a.size(); i++)
		if (atruncate(a[i]) == atruncate(b))
			return true;
	return false;
}

/* initialising variables */

// mu
float amu_min = 2;
float amu_max = 4;
int const amu_step = 5000;

//starting value of x
float ax_0 = 0.1;
float ay_0 = 0.9;

// alpha
float alpha = 0.05;

//resolution
int const no_of_steps = 5000;
int const cutoff = 1000;

//number of total points
int const anumber = (amu_step + 1) * (no_of_steps - cutoff - 1);


void couplogfunc(float* xs, float* exps, float a) {


	static float mus1[anumber], mus2[amu_step];
	int mudummy1 = 0;
	int mudummy2 = 0;
	for (float mu = amu_min; mu <= amu_max; mu += ((amu_max - amu_min) / (float)amu_step)) {
		mus2[mudummy2++] = mu;
		for (int t = 0; t < no_of_steps; t++)
			if (t > cutoff)
				mus1[mudummy1++] = mu;
	}

	// find when chaos emerges
	bool CheckForChaos = true;
	for (int i = 1; i < no_of_steps; i++) {
		if (CheckForChaos && exps[i] > 0) {
			std::cout << "Lyapunov exponent > 0 first occurs at r = " << amu_min + i * ((amu_max - amu_min) / amu_step) << endl;
			CheckForChaos = false;
		}
	}

	//setting up style for the plot
	ImGui::StyleColorsLight();
	ImPlot::StyleColorsLight();
	ImPlotStyle& style = ImPlot::GetStyle();
	style.MajorGridSize = ImVec2(5.0f, 5.0f);
	style.MinorGridSize = ImVec2(5.0f, 5.0f);
	style.PlotBorderSize = 1;
	style.LineWeight = 1.5f;
	style.MarkerSize = 4;
	ImVec4 black = ImVec4(0.25, 0.25, 0.25, 0.75);
	ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 1, black, 0.0f, black);
	ImPlot::SetupAxes("r", "x"); //λ
	//ImPlot::SetupAxesLimits(amu_min, amu_max + 0.05, -5.1, 1.1);
	ImPlot::SetupAxesLimits(amu_min, amu_max + 0.05, -0.05, 1.05);

	double HorizontalAxisLabelPositions[9] = { 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4 };
	//double HorizontalAxisLabelPositions[7] = { 3.5, 3.6, 3.7, 3.8, 3.9, 4};
	//double VerticalAxisLabelPositions[7] = {-5, -4, -3, -3, -1, 0, 1};
	double VerticalAxisLabelPositions[6] = { 0, 0.2, 0.4, 0.6, 0.8, 1 };
	ImPlot::SetupAxisTicks(ImAxis_X1, HorizontalAxisLabelPositions, 9);
	ImPlot::SetupAxisTicks(ImAxis_Y1, VerticalAxisLabelPositions, 6);

	ImPlot::PlotScatter("Data 1", mus1, xs, anumber);
	//ImPlot::PlotLine("Data 2", mus2, exps, amu_step);
	//ImPlot::PlotLine("##", mus2, 0, amu_step);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
	ImPlot::PopStyleVar();

	/* envelope 
	float upper_envelope[amu_step], lower_envelope[amu_step], difference[amu_step] = {0};
	for (int i = 0; i < amu_step; i++) {
		float r = amu_min + i * (amu_max - amu_min) / (amu_step - 1);
		upper_envelope[i] = (og_step((r - alpha) / (2 * r), r) - alpha * ((r - alpha) / (2 * r))) / (1 - alpha);
		lower_envelope[i] = (og_step(upper_envelope[i], r) - alpha * upper_envelope[i]) / (1 - alpha);
	}
	ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 5.0f);
	ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0, 0, 0, 1));
	ImPlot::PlotLine("Upper Envelope", mus2, upper_envelope, amu_step);
	ImPlot::PlotLine("Lower Envelope", mus2, lower_envelope, amu_step);

	/* line for x-axis 
	float zeros[amu_step] = { 0 };
	ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 3.0f);
	ImPlot::PushStyleColor(ImPlotCol_Line, black);
	ImPlot::PlotLine("Data 1", mus2, zeros, amu_step);
	*/
}

void coupledlogistic() {

	static float xs[anumber], ys[anumber], exps[amu_step];

	int mudummy = 0;
	int expdummy = 0;
	for (float mu = amu_min; mu <= amu_max; mu += ((amu_max - amu_min) / (float)amu_step)) {
		// simulating the network
		vector<float> blacklist = { 0 };
		float x[no_of_steps + 1] = {};
		float y[no_of_steps + 1] = {};
		x[0] = ax_0;
		y[0] = ay_0;
		float exp = 0.0f;
		exp = 0.0f;
		for (int t = 0; t < no_of_steps; t++)
		{
			//std::cout << "x[" << t << "] = " << x[t] << ", mu = " << mu << std::endl;
			float* state = step2(x[t], y[t], mu, mu, alpha, alpha);
			x[t + 1] = state[0];
			y[t + 1] = state[1];

			if (t > cutoff /* && !search(blacklist, x[t])*/) {
				xs[mudummy] = x[t];
				mudummy++;
				blacklist.push_back(atruncate(x[t]));
				exp += log(abs( (mu * (1 - 2 * x[t]) - alpha) * (mu * (1 - 2 * y[t]) - alpha) - pow(alpha,2) ));
			}
		}
		exps[expdummy++] = exp / (float)(no_of_steps - cutoff);
	}


	GraphToFile(couplogfunc, xs, exps, anumber, "plot.png");
}

/* initialising variables */

// mu
float bmu_min = 0;
float bmu_max = 4;

//starting value of x
float bx_0 = 0.1;
float by_0 = 0.5;

//resolution
int const bno_of_steps = 50000;
int const bcutoff = 5000;

//number of total points
int const bnumber = bno_of_steps - bcutoff;


void phasefunc(float* xs, float* ys, float a) {

	//setting up style for the plot
	ImGui::StyleColorsLight();
	ImPlot::StyleColorsLight();
	ImPlotStyle& style = ImPlot::GetStyle();
	style.MajorGridSize = ImVec2(5.0f, 5.0f);
	style.MinorGridSize = ImVec2(5.0f, 5.0f);
	style.PlotBorderSize = 1;
	style.LineWeight = 1.5f;
	style.MarkerSize = 4;
	ImVec4 black = ImVec4(0.25, 0.25, 0.25, 0.75);
	ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 25, black, 0.0f, black);
	ImPlot::SetupAxes("x", "y");
	ImPlot::SetupAxesLimits(-0.05, 1.05, -0.05, 1.05);

	double HorizontalAxisLabelPositions[15] = { 0, 0.2, 0.4, 0.6, 0.8, 1};
	double VerticalAxisLabelPositions[15] = { 0, 0.2, 0.4, 0.6, 0.8, 1 };
	ImPlot::SetupAxisTicks(ImAxis_X1, HorizontalAxisLabelPositions, 15);
	ImPlot::SetupAxisTicks(ImAxis_Y1, VerticalAxisLabelPositions, 15);

	ImPlot::PlotScatter("Data 1", xs, ys, bnumber);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
	ImPlot::PopStyleVar();

	//for (int i = 0; i < bnumber; i++) std::cout << "(" << xs[i] << ", " << ys[i] << ")" << endl;
	for (int i = 0; i < bnumber; i++) if (xs[i] == 0) std::cout << i;
}

void phasediagram() {

	static float xs[bnumber], ys[bnumber];
	
	float alpha = 0.1;
	float mu = 3.5;

	// simulating the network12

	float x[bno_of_steps + 1] = {};
	float y[bno_of_steps + 1] = {};
	x[0] = bx_0;
	y[0] = by_0;
	for (int t = 0; t < bno_of_steps; t++)
	{
		float* state = step2(x[t], y[t], mu, mu, alpha, alpha);
		x[t + 1] = state[0];
		y[t + 1] = state[1];

		if (t >= bcutoff) {
			xs[t - bcutoff] = x[t];
			ys[t - bcutoff] = y[t];
		}
	}
	GraphToFile(phasefunc, xs, ys, bnumber, "plot.png");
}

/* initialising variables */

// mu
float cmu_min = 0;
float cmu_max = 4;
int const cmu_step = 1000;

//alpha
int const alpha_step = 200;

//starting values
float cx_0 = 0.55;
float cy_0 = 0.6;
//float alpha = 0.1;

//resolution
int const cno_of_steps = 7400;
int const ccutoff = 5400;

//number of total points
int const cnumber = (cmu_step + 1) * (cno_of_steps - ccutoff - 1);


void coupledlogfuncanim(float* axs, float* ays, float a) {

	static float xs[cnumber], ys[cnumber];

	static float alpha = 0;
	static int alphadummy = 0;
	static bool anim = false;

	static float mus1[cnumber], mus2[cmu_step], exps[cmu_step];
	static float alphas[alpha_step], values[alpha_step];
	int mudummy1 = 0;
	int mudummy2 = 0;
	for (float mu = cmu_min; mu <= cmu_max; mu += ((cmu_max - cmu_min) / (float)cmu_step)) {
		mus2[mudummy2++] = mu;
		for (int t = 0; t < cno_of_steps; t++)
			if (t > ccutoff)
				mus1[mudummy1++] = mu;
	}


	static float cratios[1] = { 1.0f };
	static float rratios[2] = { 1.0f ,1.0f };
	static ImPlotSubplotFlags flagsa = ImPlotSubplotFlags_None;

	if (ImPlot::BeginSubplots("My Subplots", 2, 1, ImVec2(-1, -60), flagsa, rratios, cratios)) {
		if (ImPlot::BeginPlot("Pop 1", ImVec2(/*-1, -30*/))) {
			ImPlot::SetupAxes("x", "r");
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 1.0f, ImVec4(0, 0, 1, 1), 1.0f, ImVec4(0, 0, 0, 0));
			ImPlot::PlotScatter("Data 1", mus1, xs, cnumber);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}
		/* this produces a second bifurcation diagram for y, but they are always identical so it has been removed for performance reasons*/
		if (ImPlot::BeginPlot("Pop 2", ImVec2())) {

			ImPlot::SetupAxes("y", "s");
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 1.0f, ImVec4(1, 0, 0, 1), 1.0f, ImVec4(0, 0, 0, 0));
			ImPlot::PlotScatter("Data 2", alphas, values, alpha_step);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}
		ImPlot::EndSubplots();
	}
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat("##Alpha", &alpha);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat("##x0", &cx_0);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat("##y0", &cy_0);
	ImGui::SameLine();
	if (ImGui::Button("Anim")) {
		alpha = 0.0f;
		alphadummy = 0;
		anim = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Toggle")) anim = !anim;
	if (anim) {
		int mudummy = 0;
		float value = 0.0f;
		float muvalue = 0.0f;
		for (float mu = cmu_min; mu <= cmu_max; mu += ((cmu_max - cmu_min) / (float)cmu_step)) {
			// simulating the network
			vector<float> blacklistx = { 0 };
			vector<float> blacklisty = { 0 };
			float x[cno_of_steps + 1] = {};
			float y[cno_of_steps + 1] = {};
			x[0] = cx_0;
			y[0] = cy_0;
			for (int t = 0; t < cno_of_steps; t++)
			{
				//std::cout << "x[" << t << "] = " << x[t] << ", mu = " << mu << std::endl;
				float* output = step2(x[t], y[t], mu, mu, alpha, alpha);
				x[t + 1] = output[0];
				y[t + 1] = output[1];


				if ((t % 10 == 0) && t < 00) {
					std::random_device dev;
					std::mt19937 gen(dev());
					std::uniform_real_distribution<float> dist6(-0.1, 0.1);

					float ab = dist6(gen);

					x[t + 1] += dist6(gen);
				}

				if (t > ccutoff /* && !asearch(blacklist, x[t])*/) {
					if (x[t] > value && mu > 2.8f - 2.0f * alpha) {
						value = x[t];
						muvalue = mu;
					}
					xs[mudummy] = x[t];
					ys[mudummy] = y[t];
					mudummy++;
					blacklistx.push_back(atruncate(x[t]));
					blacklisty.push_back(atruncate(x[t]));
				}
			}
		}
		alphas[alphadummy] = alpha;
		values[alphadummy++] = value;
		alpha += 1.0f / alpha_step;
		if (alpha >= 1.0f) anim = false;
	}
}

void coupledlogisticanim() {
	static float axs[1], ays[1];

	Graph(coupledlogfuncanim, axs, ays, -3.0f);
}