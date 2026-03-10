#include <iostream>
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

	cout << endl << "r : " << setprecision(15) << r << endl;

	for (int i = 0; i < cycles; i++) {
		if (abs(xs[2 * i] - xs[2 * i + 1]) > 0.00000000000001) return -1;
		if (abs((i < cycles - 1) ? xs[2 * i] - xs[2 * i + 2] : 0.1) < 0.000000000000001) return 1;
	}
	return 0;
}

void criticalvalues() {
	double x = 0.4;
	double y = 0.5;
	double rguess = 2.9;
	double rchange = 0.1;
	double alpha = 0.1;
	bool done = false;
	int lastresult = -2;
	do {
		int result = estimate(x, y, rguess, alpha, 4); //2.798322329 3.448964208 3.629913118 3.629916600 3.629917078 3.629917087
		if (result != lastresult) rchange *= 0.95;
		if (result == 0) {
			cout << "correct number of cycles, rchange: " << rchange << endl;
			rguess -= rchange;
		}
		if (result == 1) {
			cout << "too few cycles, rchange: " << rchange << endl;
			rguess += rchange;
		}
		if (result == -1) {
			cout << "too many cycles, rchange: " << rchange << endl;
			rguess -= rchange;
		}
		lastresult = result;
	} while (rchange > 0.0000000001);

	cout << "done, r: ";
	cout << setprecision(15) << rguess << endl;

}