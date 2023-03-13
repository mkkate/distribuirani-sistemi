/*
zad6. Napisati MPI program koji izračunava vrednost broja PI kao vrednost
integrala funkcije f(x)=4/(1+x^2) na intervalu [0,1] .
*/

#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

int main(int argc, char** argv)
{
	int rank, size, intervals = 0, i;
	double x, h = 0, sum = 0.0, pi = 0, rankPi;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	if (rank == 0)
	{
		printf("Unesi broj intervala:\n");
		scanf_s("%d", &intervals);
	}

	MPI_Bcast(&intervals, 1, MPI_INT, 0, MCW);

	h = 1.0 / intervals;

	for (i = rank; i < intervals; i += size)
	{
		x = h * (i + 0.5);
		sum += 4.0 / (1 + x * x);
	}

	rankPi = h * sum;

	MPI_Reduce(&rankPi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MCW);

	if (rank == 0)
		printf("PI = %.16f, Error = %1.6f\n", pi, fabs(pi - M_PI));

	MPI_Finalize();

	return 0;
}