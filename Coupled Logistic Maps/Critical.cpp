#include <iostream>
#include <fstream>
#include <stdio.h>
#include "implot.h"
#include "Graphing.h"
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <limits>
#define debugging false
using namespace std;

double* step(double r, double x, double y, double alpha) {
	double result[2];
	result[0] = r * x * (1 - x) - alpha * (x - y);
	result[1] = r * y * (1 - y) + alpha * (x - y);
	return(result);
}

int estimate(double x0, double y0, double r, double alpha, int cycles) {
	int steps = 20000000;
	double x = x0;
	double y = y0;
	for (int i = 0; i < steps; i++) {
		double* resulta = step(r, x, y, alpha);
		x = resulta[0];
		y = resulta[1];
	}

	vector<double> xs;
	for (int i = 0; i < 2 * cycles; i++) {
		double* resultb = step(r, x, y, alpha);
		x = resultb[0]; 
		y = resultb[1]; 
		xs.push_back(x);
	}
	sort(xs.begin(), xs.end(), greater<double>());
#if debugging
	cout << endl << "r : " << setprecision(15) << r << endl;
#endif
	for (int i = 0; i < cycles; i++) {
		if (abs(xs[2 * i] - xs[2 * i + 1]) > 0.0000000000000001) return -1;
		if (abs((i < cycles - 1) ? xs[2 * i] - xs[2 * i + 2] : 0.1) < 0.000000000001) return 1;
	}
	return 0;
}


double cvalue(double alpha, int cycles, double rguess = 2.5, double rchange = 0.1, double x = 0.4, double y = 0.5) {
	bool done = false;
	int lastresult = -2;
	do {
		int result = estimate(x, y, rguess, alpha, cycles); //2.798322329 3.448964208 3.629913118 3.629916600 3.629917078 3.629917087
		if (result != lastresult) rchange *= 0.8;
		if (result == 0) {
#if debugging 
			cout << "correct number of cycles, rchange: " << rchange << endl;
#endif
			rguess -= rchange;
		}
		if (result == 1) {
#if debugging
			cout << "too few cycles, rchange: " << rchange << endl;
#endif
			rguess += rchange;
		}
		if (result == -1) {
#if debugging
			cout << "too many cycles, rchange: " << rchange << endl;
#endif
			rguess -= rchange;
		}
		lastresult = result;
	} while (rchange > 0.0000000001);

	cout << "done, alpha: " << alpha << ", cycles: " << cycles << ", r: ";
	cout << setprecision(15) << rguess << endl;
	return rguess;
}

void criticalvalues() {
	double alphastart = 0;
	double alphachange = 0.02;
	double alphamax = 0.3;
	int cycles = 2;

	ofstream MyFile("data.csv");
	MyFile << "cycles,alpha,result" << endl;

	for (int cycles = 2; cycles < 33; cycles *= 2) {
		for (double alpha = alphastart; alpha <= alphamax; alpha += alphachange) {
			double result = cvalue(alpha, cycles, 2.9 - (3 * alpha));
			MyFile << cycles << "," << alpha << "," << setprecision(15) << result << endl;
		}
	}

	MyFile.close();
}