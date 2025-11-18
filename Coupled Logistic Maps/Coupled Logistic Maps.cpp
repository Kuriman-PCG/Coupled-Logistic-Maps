// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "Graphs.h"
#include "gillespie.h"
#include "Coupling.h"
#include "Matrix.h"

int main()
{
	//cobweb = 0
	//logistic = 1
	//population = 2
	//gillespie = 3
	//coupling = 4
	//stochastic = 5
	//matrix coupling = 6
	int mode = 6;

	if (mode == 0) {
		cobweb(0.5f);
	}
	if (mode == 1) {
		logistic();
	}
	if (mode == 2) {
		population();
	}
	if (mode == 3) {
		algorithm();
	}
	if (mode == 4) {
		couple();
	}
	if (mode == 5) {
		stochasticlogistic();
	}
	if (mode == 6) {
		matrixCoupling();
	}
}