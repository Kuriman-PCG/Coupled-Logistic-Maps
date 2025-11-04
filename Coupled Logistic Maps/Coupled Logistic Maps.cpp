// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "Graphs.h"

int main()
{
	//cobweb = 0
	//logistic = 1
	//population = 2
	int mode = 1;

	if (mode == 0) {
		cobweb(0.5f);
	}
	if (mode == 1) {
		logistic();
	}
	else {
		population();
	}
}