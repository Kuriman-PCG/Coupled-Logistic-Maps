#include <vector>
#include <iostream>
#include <stdio.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "implot.h"
#include "Graphing.h"
#include <string>
#include <sstream>
#include <numeric>

void matrixCouplingFunc(float* mus, float* xs, float num) {

	
	//holy block of variables
	using namespace boost::numeric::ublas;
	static const char* labels1[] = { "Population","Hogs","Dogs","Logs" };
	const static int startPop = 3;
	static float connectivityMatrix[20][20];
	static float initGrowthRates[20] = { 3.9f, 1.5f, 2.5f, 3.0f, 1.0f, 2.0f, 3.5f, 0.5f, 0.4f, 3.9f, 0.1f, 1.0f, 1.0f, 0.5f, 3.0f, 3.0f, 3.0f, 2.45f, 2.0f };
	static float initCapacities[startPop] = { 1.0f, 1.0f, 1.0f };
	static vector<float> growthVector(startPop);
	static vector<float> capacityVector(startPop);
	static vector<float> populationVector(startPop);
	static matrix<float> adjacencyMatrix(startPop, startPop);
	static vector<float> logisticVector(startPop);
	static int populations = startPop;
	static int populationMemory = startPop;
	static float alpha = 0.1;
	static bool hasRun = false;
	static bool logisticMap = false;

	//Check whether the population size has changed
	if (populationMemory != populations) {
		populationMemory = populations;
		growthVector.resize(populations);
		capacityVector.resize(populations);
		populationVector.resize(populations);
		adjacencyMatrix.resize(populations, populations);
		logisticVector.resize(populations);
		hasRun = false;
	}

	//Set initial conditions
	if (!hasRun) {
		hasRun = true;
		for (int i = 0; i < populations; i++) {
			growthVector[i] = 1.5f;
			capacityVector[i] = 1.0f;
			populationVector[i] = 0.3f;
		}

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (abs(i - j) == 1 && i < populations && j < populations)
					connectivityMatrix[i][j] = 1.0f;
				else
					connectivityMatrix[i][j] = 0.0f;
			}
		}
	}

	//Mirror the connectivity matrix
	for (int i = 0; i < populations; i++) {
		for (int j = i + 1; j < populations; j++) {
			connectivityMatrix[i][j] = connectivityMatrix[j][i];
		}
	}

	//Start population change
	for (int i = 0; i < populations; i++) {
		//Create the adjacency matrix
		for (int j = 0; j < populations; j++) {
			if (i == j)
				adjacencyMatrix(i, j) = -1;
			else
				adjacencyMatrix(i, j) = connectivityMatrix[j][i] / std::accumulate(std::begin(connectivityMatrix[j]), std::end(connectivityMatrix[j]), 0);
		}
		//Create the logistic vector
		logisticVector[i] = growthVector[i] * populationVector[i] * (capacityVector[i] - populationVector[i]);
	}
	adjacencyMatrix = adjacencyMatrix * alpha;

	//Change the population vector
	static vector<float> result(populations, populations);
	result = (logisticMap ? logisticVector : populationVector) + prod(adjacencyMatrix, populationVector);
	populationVector = result;


	const double pi2 = 6.2829;
	static ImPlotPieChartFlags flags = 0;

	//Plot the populations
	for (int i = 0; i < populations; i++) {
		static float data[1];
		data[0] = populationVector[i];
		ImPlot::PlotPieChart(labels1, data, 1, populations * cos(pi2 * i / populations), populations * sin(pi2 * i / populations), 1, "%.2f", 90, flags);
	}

	//Allow for modifications to the connectivity matrix
	if (ImPlot::BeginLegendPopup("Population")) {
		ImGui::BulletText("Connectivity Matrix");
		for (int i = 0; i < populations; i++) {
			for (int j = 0; j <= i; j++) {
				//This looks jank but imgui forces me to use a different char[] label for every input
				char arr[7] = "##0000";
				arr[2] = '0' + int(i / 10);
				arr[3] = '0' + int(i % 10);
				arr[4] = '0' + int(j / 10);
				arr[5] = '0' + int(j % 10);

				ImGui::SetNextItemWidth(50);
				ImGui::InputFloat(arr, &connectivityMatrix[i][j]);
				if (j < i) ImGui::SameLine();
			}
		}
		ImGui::BulletText("Growth Rates");
		for (int i = 0; i < populations; i++) {
			char arr[6] = "##a00";
			arr[3] = '0' + int(i / 10);
			arr[4] = '0' + int(i % 10);

			ImGui::SetNextItemWidth(50);
			ImGui::InputFloat(arr, &growthVector[i]);
			if (i < populations - 1) ImGui::SameLine();
		}

		ImPlot::EndLegendPopup();
	}

	//Variables
	ImGui::DragInt("Populations", &populations, 1.0F, 1, 20);
	ImGui::DragFloat("Alpha", &alpha, 0.02F, 0.0f, 1.0f, "%.2f");
	ImGui::Checkbox("LogisticMap", &logisticMap);
}

void matrixCoupling() {

	float a[3], b[3], c = -2.0f;
	Graph(matrixCouplingFunc, a, b, c);
}