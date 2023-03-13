/*
zad7. Napisati MPI program koji izračunava vrednost skalarnog proizvoda
dva vektora dimenzije N. Pretpostaviti da je N deljivo sa brojem procesa.
Vrednosti vektora a i b se učitavaju u procesu P0.

	a*b = a1*b1 + a2*b2 + ... + an*bn

	a={1,2,3,4,5,6}, b={7,8,9,10,11,12}
	a*b=217

NAPOMENA: svaki komentar je primer linije ispod u slucaju kada bi se radilo
sa MPI_Bcast umesto MPI_Scatter
*/

#define N 6
#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int rank, size, local_size, partial_result = 0, final_result = 0;
	int* a, * b, * local_a, * local_b;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	a = (int*)malloc(N * sizeof(int));
	b = (int*)malloc(N * sizeof(int));

	local_size = N / size;
	local_a = (int*)malloc(local_size * sizeof(int));
	local_b = (int*)malloc(local_size * sizeof(int));

	if (rank == 0)
	{
		printf("Unesi vektor a:\n");
		for (int i = 0; i < N; i++)
			scanf_s("%d", &a[i]);
	}
	//MPI_Bcast(a, N, MPI_INT, 0, MCW);
	MPI_Scatter(a, local_size, MPI_INT, local_a, local_size, MPI_INT, 0, MCW);

	if (rank == 0)
	{
		printf("Unesi vektor b:\n");
		for (int i = 0; i < N; i++)
			scanf_s("%d", &b[i]);
	}
	//MPI_Bcast(b, N, MPI_INT, 0, MCW);
	MPI_Scatter(b, local_size, MPI_INT, local_b, local_size, MPI_INT, 0, MCW);

	//for (int i = rank; i < N; i += size)
	for (int i = 0; i < local_size; i++)
		//partial_result += a[i] * b[i];
		partial_result += local_a[i] * local_b[i];

	MPI_Reduce(&partial_result, &final_result, 1, MPI_INT, MPI_SUM, 0, MCW);

	if (rank == 0)
		printf("Skalarni proizvod = %d\n", final_result);


	free(a);
	free(b);

	MPI_Finalize();

	return 0;
}