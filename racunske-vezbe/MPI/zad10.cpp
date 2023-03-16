/*
zad10.Napisati MPI program koji pronalazi proizvod matrice Amxn i vektora bn.
Matrica A i vektor b se inicijalizuju u procesu 0.
Izračunavanje se obavlja tako što se svakom procesu distribuira po kolona matrice A
i po 1 element vektora b.
Za distribuciju kolona po procesima koristiti P-t-P operacije, za sve ostalo grupne operacije.
Svi procesi učestvuju u izračunavanju.
Rezultat se prikazuje u procesu koji, nakon distribuiranja kolona matrice A,
sadrži minimum svih elemenata matrice A.
	A={ 1, -1, 2,		b={5, 6, 7}
		0, -3, 4}

	A*b={5, -24, 42} = 23

NAPOMENA: Broj procesa == broj kolona (n) !!!
*/

#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int rank, size;
	int* a, * b = nullptr, n = 3, m = 2; // n=size !!!
	int* local_a, local_b;
	int* kolone = nullptr, k = 0, v = 0;
	int proces_result = 0, result = 0;

	struct {
		int value;
		int rank;
	}min_proces_out, min_proces_in, min_in, min_out;

	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	a = (int*)malloc(m * n * sizeof(int));
	b = (int*)malloc(n * sizeof(int));
	local_a = (int*)malloc(n * sizeof(int));
	kolone = (int*)malloc(n * sizeof(int));

	if (a == NULL || b == NULL)
		return -1;
	if (local_a == NULL || kolone == NULL)
		return -1;

	if (rank == 0)
	{
		printf("Unesi elemente matrice A:\n");
		fflush(stdout);

		for (int i = 0; i < m * n; i++)
			scanf_s("%d", &a[i]);

		printf("Unesi elemente vektora B:\n");
		fflush(stdout);

		for (int i = 0; i < n; i++)
			scanf_s("%d", &b[i]);		
	}


	if (rank == 0)
	{
		//PO "salje" samom sebi prvu kolonu (indeks 0)
		for (v = 0; v < m; v++)
			local_a[v] = a[v * n];

		//P0 salje elemente ostalim procesima (P1 do Psize-1)
		for (k = 1; k < size; k++)	// k je rank procesa kome se salje, zato je k<size
		{
			for (v = 0; v < m; v++)
				kolone[v] = a[v * n + k];

			MPI_Send(kolone, m, MPI_INT, k, 0, MCW);
		}
	}
	else
		MPI_Recv(local_a, m, MPI_INT, 0, 0, MCW, &st);

	//P0 salje po 1 element iz b ostalim procesima
	MPI_Scatter(b, 1, MPI_INT, &local_b, 1, MPI_INT, 0, MCW);

	//nalazenje minimuma svih elemenata matrice A
	min_in.value = INT_MAX;
	for (int i = 0; i < m; i++)
		if (local_a[i] < min_in.value)
		{
			min_in.value = local_a[i];
			min_in.rank = rank;
		}
	MPI_Reduce(&min_in, &min_out, 1, MPI_2INT, MPI_MINLOC, 0, MCW);
	// salje svim procesima min_out, da bi oni mogli konacni rezultat da posalju u min_out.rank
	MPI_Bcast(&min_out, 1, MPI_2INT, 0, MCW);

	//izracunavanje
	for (int i = 0; i < m; i++)
		proces_result += local_a[i] * local_b;

	//konacni rezultat
	// da nije bilo Bcast, procesi ne bi znali kome treba da posalju result
	MPI_Reduce(&proces_result, &result, 1, MPI_INT, MPI_SUM, min_out.rank, MCW);

	if (rank == min_out.rank)
	{
		printf("Rank procesa koji sadrzi minimum svih elemenata matrice, tj. %d, je [%d]\n", min_out.value, rank);
		printf("Result = %d\n", result);
	}

	MPI_Finalize();

	return 0;
}