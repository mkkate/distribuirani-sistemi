/*
Napisati program koji uzima podatke od nultog procesa i šalje ih svim
drugim procesima tako što proces i treba da primi podatke i pošalje ih procesu
i+1, sve dok se ne stigne do poslednjeg procesa. Unos podataka se završava
nakon što se prenese negativna vrednost podatka.
*/

#define _CRT_SECURE_NO_WARNINGS

#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank, size, value;

	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	do {
		if (rank == 0)
		{
			scanf_s("%d", &value);
			MPI_Send(&value, 1, MPI_INT, 1, 0, MCW);
		}
		else
		{
			MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MCW, &st);

			if (rank < size - 1)	// ne salje ako je poslednji proces
				MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MCW);
		}
		printf("Proces %d, rank %d\n", rank, value);
	} while (value >= 0);

	MPI_Finalize();

	return 0;
}