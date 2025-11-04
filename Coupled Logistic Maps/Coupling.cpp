
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
	static float c = 0.3f;
	static float d = 1.8f;
	static float xyz[3] = { 0.6f, 0.4f, 0.0f };

	t++;
	this_thread::sleep_for(std::chrono::milliseconds(500));
	//t += ImGui::GetIO().DeltaTime;

	float *output = step(xyz[0], xyz[1], xyz[2], a, b, c, d);
	xyz[0] = output[0];
	xyz[1] = output[1];
	xyz[2] = output[2];
	
	sdata1.AddPoint(t, xyz[0]);
	sdata2.AddPoint(t, xyz[1]);
	sdata3.AddPoint(t, xyz[2]);

	ImGui::SliderFloat("a", &a, 0, 4, "%.1f");
	ImGui::SliderFloat("b", &b, 0, 4, "%.1f");
	ImGui::SliderFloat("c", &c, 0, 1, "%.2f");
	ImGui::SliderFloat("d", &d, 0, 4, "%.1f");
	//ImGui::SliderInt("j", &j, 1, 10, "%.0f");
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
	if (ImGui::Button("Revive From Death")) {
		xyz[0] = 0.5f;
		xyz[1] = 0.5f;
		xyz[2] = 0.5f;
	}

	//static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

	//ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
	ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
	ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
	ImPlot::PlotLine("Population1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
	ImPlot::PlotLine("Population3", &sdata3.Data[0].x, &sdata3.Data[0].y, sdata3.Data.size(), 0, sdata3.Offset, 2 * sizeof(float));
}

void couple() {
	float mus[3], xs[3];
	float num = -2.0f;

	Graph(coupfunc, mus, xs, num);
}