
#include <iostream>
#include <stdio.h>
#include "implot.h"
#include "Graphing.h"
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

float* step(float x_n, float y_n, float z_n, float a, float b, float c, float d) {
	float x_n1 = a * x_n * (1 - x_n) - c * x_n + c * 0.5f * y_n;
	float y_n1 = b * y_n * (1 - y_n) - c * y_n + c * x_n + c * z_n;
	float z_n1 = d * z_n * (1 - z_n) - c * z_n + c * 0.5f * y_n;
	float xyz[3] = { x_n1, y_n1, z_n1 };
	return xyz;
}

float* step2(float x_n, float y_n, float a, float b, float c, float d) {
	float x_n1 = a * x_n * (1 - x_n) - c * x_n + d * y_n;
	float y_n1 = b * y_n * (1 - y_n) - d * y_n + c * x_n;
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