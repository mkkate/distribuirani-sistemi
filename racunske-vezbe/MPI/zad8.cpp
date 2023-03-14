/*
zad8. Napisati MPI program koji pronalazi proizvod matrice Anxn i vektora bn.
Matrica A i vektor b se inicijalizuju u procesu 0.
Izračunavanje se obavlja tako što se svakom procesu distribuira po vrsta
matrice A i ceo vektor b.
Svi procesi učestvuju u izračunavanju. Rezultat se prikazuje u procesu 0.

	A={ 1, -1, 2,		b={2, 1, 0}
		0, -3, 1,
		4, 0, -2}

	A*b={1, -3, 8}
*/

#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int rank, size, n, * a, * b, * local_a;
	int part_result = 0, * result;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	n = size;

	a = (int*)malloc(n * n * sizeof(int));
	b = (int*)malloc(n * sizeof(int));
	if (a == NULL || b == NULL)
		return -1;

	local_a = (int*)malloc(n * sizeof(int));
	result = (int*)malloc(n * sizeof(int));
	if (local_a == NULL || result == NULL)
		return -1;

	if (rank == 0)
	{
		for (int i = 0; i < n * n; i++)
			scanf_s("%d", &a[i]);

		for (int i = 0; i < n; i++)
			scanf_s("%d", &b[i]);
	}

	MPI_Scatter(a, n, MPI_INT, local_a, n, MPI_INT, 0, MCW);
	MPI_Bcast(b, n, MPI_INT, 0, MCW);

	for (int i = 0; i < n; i++)
		part_result += local_a[i] * b[i];

	MPI_Gather(&part_result, 1, MPI_INT, result, 1, MPI_INT, 0, MCW);

	if (rank == 0)
	{
		printf("PO = { ");
		for (int i = 0; i < n; i++)
			printf("%d ", result[i]);
		printf(" }");
	}

	free(a);
	free(b);
	free(local_a);
	free(result);

	MPI_Finalize();

	return 0;
}