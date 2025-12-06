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
	//cobweb + population = 7
	int mode = 1;

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
	if (mode == 7) {
		cobpopweb(3.5f);
	}
	if (mode == 8) {
		cobpoplog();
	}
}