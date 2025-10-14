#include "implot.h"
#include "math.h"
#include "Graphing.h"
#include "stdlib.h"
using namespace std;

void func() {

    static float xs1[100], ys1[100];
    for (int i = 0; i < 100; ++i) {
        xs1[i] = i * 0.01f;
        ys1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
    }
    static float xs2[50], ys2[50];
    for (int i = 0; i < 50; i++) {
        xs2[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
        ys2[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
    }

    ImPlot::PlotScatter("Data 1", xs1, ys1, 100);
    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
    ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6, ImPlot::GetColormapColor(1), IMPLOT_AUTO, ImPlot::GetColormapColor(1));
    ImPlot::PlotScatter("Data 2", xs2, ys2, 50);
    ImPlot::PopStyleVar();
}

int main() {
    Graph(func);
}