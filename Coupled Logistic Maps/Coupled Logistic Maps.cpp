// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "Graphs.h"

int main()
{
	bool cob = true;

	if (cob) {
		float mu = 3.5;
		cobweb(mu);
	}
	else {
		logistic();
	}
}