#include <vector>
#include <iostream>
#include <stdio.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "implot.h"
#include "Graphing.h"
#include <string>

void adjacency(int n) {

}

void piefunc(float* mus, float* xs, float num) {

	static const char* labels1[] = { "Frogs","Hogs","Dogs","Logs" };
	

	using namespace boost::numeric::ublas;
	const static int startPop = 3;
	static float connectivityMatrix[20][20];
	for (int i = 1; i < 20; i++) {
		for (int j = 1; j < 20; j++) {
			if (abs(i - j) == 1)
				connectivityMatrix(i, j) = 1.0f;
			else
				connectivityMatrix(i, j) = 0.0f;
		}
	}
	/*static float connectivityMatrix[20][20] = {
		{}
	};*/
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

	if (populationMemory != populations) {
		populationMemory = populations;
		growthVector.resize(populations);
		capacityVector.resize(populations);
		populationVector.resize(populations);
		adjacencyMatrix.resize(populations, populations);
		logisticVector.resize(populations);
		hasRun = false;
	}

	if (!hasRun) {
		hasRun = true;
		for (int i = 0; i < populations; i++) {
			growthVector[i] = initGrowthRates[i];
			capacityVector[i] = 1.0f;
			populationVector[i] = 0.3f;
		}



	}

	for (int i = 0; i < populations; i++) {
		for (int j = 0; j < populations; j++) {
			//Create the adjacency matrix
			//if (i == j)
			//	adjacencyMatrix(i, j) = -1.0f;
			//else if (abs(i-j) == 1 || abs(i-j) == populations - 1)
			//	adjacencyMatrix(i, j) = 0.5f;
			//else
			//	adjacencyMatrix(i, j) = 0.0f;
			if (i == j)
				adjacencyMatrix(i, j) = -1;
			else
				adjacencyMatrix(i, j) = connectivityMatrix(i, j) / sum(connectivityMatrix(i, ));
		}
		//Create the logistic vector
		logisticVector[i] = growthVector[i] * populationVector[i] * (capacityVector[i] - populationVector[i]);
	}
	adjacencyMatrix = adjacencyMatrix * alpha;

	static vector<float> result(populations, populations);
	result = (logisticMap ? logisticVector : populationVector) + prod(adjacencyMatrix, populationVector);
	populationVector = result;


	std::cout << adjacencyMatrix << std::endl;
	std::cout << populationVector << std::endl;


	//ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
	//ImPlot::SetupAxesLimits(0, 1, 0, 1);

	const double pi2 = 6.2829;
	static ImPlotPieChartFlags flags = 0;

	for (int i = 0; i < populations; i++) {
		static float data[1];
		data[0] = populationVector[i];
		ImPlot::PlotPieChart(labels1, data, 1, populations * cos(pi2 * i / populations), populations * sin(pi2 * i / populations), 1, "%.2f", 90, flags);
	}


	ImGui::SetNextItemWidth(250);
	ImGui::DragInt("Populations", &populations, 1.0F, 1, 20);
	ImGui::Checkbox("LogisticMap", &logisticMap);
}

void pie() {

	float a[3], b[3], c = -1.0f;
	Graph(piefunc, a, b, c);
}

void stepGraph(float* mus, float* xs, float num) {

	using namespace boost::numeric::ublas;
	const static int startPop = 3;
	static float initGrowthRates[startPop] = { 2.0f, 1.5f, 1.5f };
	static float initCapacities[startPop] = { 1.0f, 1.0f, 1.0f };
	static float initPopulation[startPop] = { 0.5f, 0.5f, 0.5f };
	static vector<float> growthVector(startPop);
	static vector<float> capacityVector(startPop);
	static vector<float> populationVector(startPop);
	static matrix<float> adjacencyMatrix(startPop, startPop);
	static vector<float> logisticVector(startPop);
	static int populations = startPop;
	static float alpha = 0.1;


	static bool hasRun = false;
	if (!hasRun) {
		hasRun = true;
		for (int i = 0; i < 3; i++) {
			growthVector[i] = initGrowthRates[i];
			capacityVector[i] = initCapacities[i];
			populationVector[i] = initPopulation[i];
		}
	}



	for (int i = 0; i < populations; i++) {
		for (int j = 0; j < populations; j++) {
			//Create the adjacency matrix
			if (i == j)
				adjacencyMatrix(i, j) = -1.0f;
			else if (i == 1)
				adjacencyMatrix(i, j) = 1.0f;
			else if (j == 1)
				adjacencyMatrix(i, j) = 0.5f;
			else
				adjacencyMatrix(i, j) = 0.0f;
		}
		//Create the logistic vector
		logisticVector[i] = growthVector[i] * populationVector[i] * (capacityVector[i] - populationVector[i]);
	}
	adjacencyMatrix = adjacencyMatrix * alpha;

	static vector<float> result(populations, populations);
	result = logisticVector + prod(adjacencyMatrix, populationVector);
	populationVector = result;

	std::cout << populationVector << std::endl;
}

void step() {

	float a[3], b[3], c = 3;
	Graph(stepGraph, a, b, c);
}