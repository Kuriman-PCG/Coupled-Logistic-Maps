// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "Graphs.h"
#include "gillespie.h"

int main()
{
	//cobweb = 0
	//logistic = 1
	//population = 2
	//gillespie = 3
	int mode = 3;

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
}