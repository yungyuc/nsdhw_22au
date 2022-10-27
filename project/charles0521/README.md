# Gaussian-Elimination and QR-decomposition
## Basic Information
In this project, I am going to solve over-constraint problems by using the Least Square Methods, employing the Gaussian elimination and QR-decomposition solvers.

Github repository: https://github.com/charles0521/Gaussian-Elimination-and-QR-decomposition

## Problem to Solve(example)
*  Define $p(x) = x^0 + x^1 + x^2 + x^3 + x^4 + x^5 + x^6 + x^7$
*  Generate a data set, there are 11 sample points. ${(x_i, y_i)}, y = p(x_i), 0 \leq i \leq 10. x_i = 2.0, 2.2, ...,4.0$

*  Fitting the data by using a polynomial of degree 7 $q(x) = a_0x^0 + a_1x^1 + a_2x^2 + a_3x^3 + a_4x^4 + a_5x^5 + a_6x^6 + a_7x^7$. At first form the following system $A\vec{c} = \vec{y} $, where
![image](https://user-images.githubusercontent.com/56105794/197404654-510ec5e3-8851-4387-8485-0b1d68d3ba56.png)

* Form a new system ${A^T}A\vec{c} = {A^T}\vec{y} $, which is theoretically solvable. Call it $B\vec{c} = \vec{d}$, which is an 8 by 8 system.
* I will implement three solutions and compare the precision and speed of three solutions.

    1.  Solve the new system by using the Gaussian elimination solver.
    2.  Solve the new system by using the QR-decomposition solver.
    3.  Solve the original system by using the QR-decomposition solver.

## Goal
* Implement Gaussian-elimination method.
* Implement QR-decomposition method.
* Comparing the precision and speed of the three solutions

## System Architecture
![image](https://user-images.githubusercontent.com/56105794/198303409-4f84d384-d547-446b-bb73-364d8bd0b036.png)

## API Description
1. Gaussian Elimination
2. QR decomposition
```c++
# This is the pseudo code

void gauss_elm(double **A, double *b, int n)
{
    int     p, i, j, k;
    double  maxEntry, t, r;

    for(i=0;i<n-1;i++)
    {
	// Partial pivoting
	maxEntry = fabs(A[i][i]);
	p = i;
	for(k=i;k<n;k++)
	    if(fabs(A[k][i])>maxEntry)
	    {
		p = k;
		maxEntry = fabs(A[k][i]);
	    }
	if(p!=i)
	{
	    for(j=i;j<n;j++)
	    {
		t = A[p][j];
		A[p][j] = A[i][j];
		A[i][j] = t;
	    }
	    t = b[p];
	    b[p] = b[i];
	    b[i] = t;
	}
		//Forward elimination.
        for(k=i+1;k<n;k++)
	{
	    if(A[k][i]==0.0) continue;
			
	    r = A[k][i]/A[i][i];
	    for(j=i;j<n;j++)
	    A[k][j] = A[k][j] - r*A[i][j];
	    b[k] = b[k] - r*b[i];
	}
    }
}

void QR_reflect(double **A, double *b, int n)
{
    // Procedure to reflect matrix A into an upper triangular
}

void back_substitute(double **U, double *x, double *b, int n)
{
    // Procedure to solve a lower triangular system
}



void QR_solver(double **A, double *x, double *b, int n)
{
    // Reflect A into an upper triangular matrix.
    QR_reflect(A, b, n);
    // Solve the upper triangular system by using backward
    // substitution.
    back_substitute(A, x, b, n);
}


```

## Engineering Infrastructure
* Automatic build system: make
* Version control: git
* Testing framework: pytest
* Documentation: GitHub [[README.md]](https://github.com/charles0521/Gaussian-Elimination-and-QR-decomposition/blob/main/README.md)

## Schedule

Week 1 (10/31): Study Linear Algebra

Week 2 (11/7):  Implement the simple QR-decomposition and Gaussian-Elimination with C++

Week 3 (11/14): Finish C++, and start visualizing some data(time) in python

Week 4 (11/21): Implement some simple parallel programs for matrix

Week 5 (11/28): Compare time with other libraries

Week 6 (12/5): Finish up, debug, and write the documentation

Week 7 (12/12): Buffer time for further testing and debugging

Week 8 (12/19): Make slides and prepare for the presentation

