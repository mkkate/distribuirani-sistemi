/*
Napisati program koji nalazi sumu n celih brojeva koriscenjem point-to-point
komunikacije tako da svaki proces učestvuje u sumiranju.
*/

#define _CRT_SECURE_NO_WARNINGS

#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank, size, value, suma = 0;

	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	if (rank == 0)
	{
		value = 1;
		MPI_Send(&value, 1, MPI_INT, 1, 0, MCW);
	}
	else
	{

		if (rank < size - 1)	//ako nije poslednji proces
		{
			MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MCW, &st);
			suma = value + rank + 1;
			MPI_Send(&suma, 1, MPI_INT, rank + 1, 0, MCW);
		}
		else
		{
			// poslednji proces samo prima i prikazuje krajnju sumu
			MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MCW, &st);
			suma = value + rank + 1;
			printf("Proces %d, suma %d\n", rank, suma);
		}
	}

	//printf("Proces %d, suma %d\n", rank, suma);

	MPI_Finalize();

	return 0;
}