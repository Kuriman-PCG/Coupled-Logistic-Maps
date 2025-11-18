#include <vector>
#include <iostream>
#include <stdio.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Graphing.h"

void adjacency(int n) {

}

void stepGraph(float* mus, float* xs, float num) {

	using namespace boost::numeric::ublas;
	const static int startPop = 3;
	static float initGrowthRates[startPop] = { 3.0f, 2.0f, 1.5f };
	static float initCapacities[startPop] = { 2.0f, 1.0f, 3.0f };
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

	float a[3], b[3], c;
	Graph(stepGraph, a, b, c);
}