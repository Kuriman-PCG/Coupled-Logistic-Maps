// Coupled Logistic Maps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>

/* function to carry out one movement in the model */
float step(float x_n, float mu)
{
	float x_n1 = mu * x_n * (1 - x_n);
	return(x_n1);
}

int main()
{
	/* initialising variables */
	float mu = 2;
	float x_0 = 0.9;
	int no_of_steps = 10;

	/* simulating the network */
	float x[no_of_steps + 1] = {};
	x[0] = x_0;
	for (int t = 0; t < no_of_steps; t++)
	{
		x[t + 1] = step(x[t], mu);
	}
	
	/* printing results */
	for (int t = 0; t < no_of_steps + 1; t++)
	{
		std::cout << "x[" << t << "] = " << x[t] << std::endl;
	}

	system("pause");
	return(0);
}


// int main()
// {
//     std::cout << "Hello World!\n";
//     printf("Testdsokjhgkjd");
// }

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file