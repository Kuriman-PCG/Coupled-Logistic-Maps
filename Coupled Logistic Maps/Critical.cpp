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

void plotfunc(float* mus, float* alphas, float toggle) {
	// the "toggle" float is just used to identify whether the graph is intended to be of r_max or r_inifinity, and since GraphToFile expects a float as an argument anyway
	// this isn't necessary in other graphs because we usually only prodce one graph per execution of the program

	//setting up style for the plot
	ImGui::StyleColorsLight();
	ImPlot::StyleColorsLight();
	ImPlotStyle& style = ImPlot::GetStyle();
	style.MajorGridSize = ImVec2(5.0f, 5.0f);
	style.MinorGridSize = ImVec2(5.0f, 5.0f);
	style.PlotBorderSize = 1;
	style.LineWeight = 1.5f;
	style.MarkerSize = 4;
	ImVec4 black = ImVec4(0.25, 0.25, 0.25, 0.75);
	ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 10, black, 1.0f, black);
	//ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 10, black, 0.0f, ImVec4(0, 0, 0, 0));
	if (toggle > 0) {
		ImPlot::SetupAxes("alpha", "r_max");
		ImPlot::SetupAxesLimits(-0.01, 0.51, 2.7, 4.05);
	}
	if (toggle < 0) {
		ImPlot::SetupAxes("alpha", "r_infinity");
		ImPlot::SetupAxesLimits(-0.01, 0.51, 2.45, 3.55);

	}

	double HorizontalAxisLabelPositions[6] = { 0, 0.1, 0.2, 0.3, 0.4, 0.5};
	double VerticalAxisLabelPositions[7] = {2.5, 2.75, 3, 3.25, 3.5, 3.75, 4};
	ImPlot::SetupAxisTicks(ImAxis_X1, HorizontalAxisLabelPositions, 6);
	ImPlot::SetupAxisTicks(ImAxis_Y1, VerticalAxisLabelPositions, 7);
	//ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(10, 10));
	//ImPlot::PushStyleVar(ImPlotStyleVar_LabelPadding, ImVec2(10, 10));

	ImPlot::PlotScatter("Data 1", alphas, mus, 201);

	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.00005f);
	ImPlot::PopStyleVar();
}

void criticalvalues() {


	double alphastart = 0;
	double alphachange = 0.01;
	double alphamax = 0.5;
	//int cycles = 2;

	ofstream MyFile("data.csv");
	MyFile << "alpha,r_max,r_infinity" << endl;

	//finding r_max
	int const no_of_steps = 1000;
	double x_0 = 0.1;
	double y_0 = 0.5;
	double xinf, yinf;
	//float xs[no_of_steps + 1] = { x_0 };
	//float ys[no_of_steps + 1] = { y_0 };
	double new_x, new_y, x, y = 0;
	float alphas[201], r_maxes[201], r_infinities[201];

	// per alpha
	for (int i = 0; i <= 200; i++) {
		double alpha = i / static_cast<double>(400);
		double r_infinity = 4;
		double r_max = 4;
		//per x_0
		for (int xi = 1; xi <= 50; xi++) {
			x_0 = xi / 100.0f;
			//per y_0
			for (int yi = 1; yi <= 50; yi++) {
				y_0 = yi / 100.0f;
				if (x_0 <= y_0) {break;} // system is symmetrical, so don't need (x_0,y_0) if (y_0,x_0) has already been computed, and definitely don't want x_0 = y_0 since thats a fixed point
				else {
					double r_max_dummy = 4;
					double r_infinity_dummy = 4;
					bool CheckForChaos = true;
					// per r
					for (int j = 0; j <= 4000; j++) {
						double r = j / static_cast<double>(1000);
						x = x_0;
						y = y_0;
						double exp = 0.0f;
						// per n
						for (int k = 0; k < no_of_steps; k++) {
							new_x = r * x * (1 - x) - alpha * (x - y);
							new_y = r * y * (1 - y) + alpha * (x - y);
							x = new_x;
							y = new_y;

							if (k > 50) exp += log(fabs((r * (1 - 2 * new_x) - alpha) * (r * (1 - 2 * new_y) - alpha) - alpha * alpha));
						}
						exp = exp / no_of_steps;
						//std::cout << "(" << r << ", " << alpha << ", " << exp << ")" << endl;
						if (CheckForChaos && exp > 0) {
							r_infinity_dummy = r;
							//xinf = x_0;
							//yinf = y_0;
							CheckForChaos = false;
						}
						if (!isinf(x) && !isnan(x)) { r_max_dummy = r; }
						else break;
					}
					r_max = min(r_max, r_max_dummy);
					r_infinity = min(r_infinity, r_infinity_dummy);
				}
			}
		}
		alphas[i] = alpha;
		r_maxes[i] = r_max;
		r_infinities[i] = r_infinity;
		MyFile << alpha << "," << r_max << "," << r_infinity << endl;
		std::cout << alphas[i] << "," << r_maxes[i] << "," << r_infinities[i] << endl;
	}
	// NOTE: iterating through all values of (x,y) in the range 0 < x,y < 1 is not suitable because we could be starting at, e.g. (0.99, 0.99), which will almost certainly be a state from which chaos can emerge. The decision has been made to cap x_0 and y_0 at 0.5. Hopefully this is good enough
	// It's telling that if we record the inital values for which r_infinity occurs, we often get maximal values, although this could be due to floating point imprecision in the min() function overwriting the same value over and over again

	/*
	for (int cycles = 2; cycles < 33; cycles *= 2) {
		for (double alpha = alphastart; alpha <= alphamax; alpha += alphachange) {
			double result = cvalue(alpha, cycles, 2.9 - (3 * alpha));
			MyFile << cycles << "," << alpha << "," << setprecision(15) << result << endl;
		}
	}
	*/

	MyFile.close();
	GraphToFile(plotfunc, r_maxes, alphas, 1.0f, "plot1.png");
	GraphToFile(plotfunc, r_infinities, alphas, -1.0f, "plot2.png");
}