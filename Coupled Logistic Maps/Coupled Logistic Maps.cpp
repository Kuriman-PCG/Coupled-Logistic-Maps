// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include "Graphs.h"
#include "gillespie.h"
#include "Coupling.h"
#include "Matrix.h"
#include "Critical.h"
#include <string>
#include <filesystem>

using namespace std;

string modes[15]{
	"cobweb",							// 0
	"logistic",							// 1
	"population",						// 2
	"gillespie",						// 3
	"coupling",							// 4
	"stochastic",						// 5
	"matrix coupling",					// 6
	"coupled cobweb",					// 7
	"separated step coupled",			// 8
	"time step coupled",				// 9
	"gauss population",					//10
	"coupled logistic map",				//11
	"phase diagram",					//12
	"coupled logistic map animated",	//13
	"critical points"					//14
};

int main()
{
	//Print all modes
	for (int i = 0; i < (sizeof(modes) / sizeof(*modes)); i++) {
		cout << modes[i] << ": " << i << endl;
	}

	//Allow for a selection in runtime
	cout << "Enter mode: ";
	string modeS;
	getline(cin, modeS);
	try {
		int mode = stoi(modeS);

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
			coupledcobweb();
		}
		if (mode == 8) {
			sepcouple();
		}
		if (mode == 9) {
			timecouple();
		}
		if (mode == 10) {
			gausspopulation();
		}
		if (mode == 11) {
			coupledlogistic();
		}
		if (mode == 12) {
			phasediagram();
		}
		if (mode == 13) {
			coupledlogisticanim();
		}
		if (mode == 14) {
			criticalvalues();
		}
	}
	catch(exception ex){
		cout << "Problem occured";
	}

}

// Critical point changes when coupling
// Cobweb diagram changes
// Coupled cobweb diagrams
// Separating the coupled logistic map population graph into its logistic term step and coupling term step