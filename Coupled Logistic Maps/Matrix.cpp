#include <vector>
#include <iostream>
#include <stdio.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/io.hpp>

void adjacency(int n) {

}


void step() {
	using namespace boost::numeric::ublas;
	static matrix<float> adjacency(3, 3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j)
				adjacency(i, j) = -.1f;
			else if (i == 1)
				adjacency(i, j) = .1f;
			else if (j == 1)
				adjacency(i, j) = .05f;
			else
				adjacency(i, j) = .0f;
		}
	}

	static identity_matrix<float> identityMatrix(3);
	static matrix<float> positionMatrix(3,3);
	static matrix<float> growthMatrix(3,3);
	static matrix<float> capacityMatrix(3,3);
	static vector<float> positionVector(3);


	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			positionMatrix(i, j) = i == j ? 0.5f : 0;
			growthMatrix(i, j) = i == j ? (i == 0 ? 1.0f : (i == 1 ? 2.0f : 1.8f)) : 0;
			capacityMatrix(i, j) = i == j ? (i == 0 ? 2 : 1) : 0;
		}
		positionVector[i] = 0.5f;
	}

	static matrix<float> result(3, 3);
	result = prod(prod(growthMatrix, capacityMatrix - positionMatrix) + adjacency, positionMatrix);

	diagonal_matrix<double> m(3, &positionVector);

	std::cout << result << std::endl;
	/*
	static vector<float> growthVector = (2.0f, 2.0f, 2.0f );
	static vector<float> capacityVector = { 1.0f, 1.0f, 1.0f };
	static vector<float> populationVector = { 0.5f, 0.5f, 0.5f };
	growthVector + capacityVector*/
}