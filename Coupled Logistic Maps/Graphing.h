#pragma once

int Graph(void (*func)(float*, float*, float), float*, float*, float);

bool GraphToFile(void (*func)(float*, float*, float), float*, float*, float, const char* filename);