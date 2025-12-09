#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "grav.h"

void main()
{


    int n = 1; // rows
    int m = 7; // columns
    double w = 0;
    long long int iw = 0;


    // Allocate array of row pointers
    const double array[1][7] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0};

    double **res = malloc(n * sizeof(double *));
    // Allocate each row
    for (int i = 0; i < n; i++) {
        res[i] = malloc(m * sizeof(double));
    }

    const double *arg1 = (&(array[0][0]));
    const double **arg = &arg1;

    printf("Running G\n");

    for(int i = 0; i < m; i++)
	{
		printf("%f\n",array[0][i]);
	}

	G(arg, res, &iw, &w, 0);

	for(int i = 0; i < m; i++)
	{
		printf("%f\n",res[0][i]);
	}

	return;
	
	return;
}

