/*
zad5. Svaki od N procesa sadrži 30 realnih brojeva. Napisati MPI
program koji nalazi maksimalnu vrednost na svakoj od 30 lokacija, kao
i identifikator procesa koji sadrži tu vrednost.
*/

#define MCW MPI_COMM_WORLD

#include <mpi.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

int main(int argc, char** argv)
{
	int rank, size, root = 0;
	struct {
		double value;
		int rank;
	}in[3], out[3];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	srand(time(NULL) + rank);
	for (int i = 0; i < 3; i++)
	{
		in[i].value = (double)(rand() % 25);
		in[i].rank = rank;
		printf("P%d: in[%d].value = %lf, in[%d].rank = %d\n", rank, i, in[i].value, i, in[i].rank);
	}

	MPI_Reduce(&in, &out, 3, MPI_DOUBLE_INT, MPI_MAXLOC, root, MCW);

	if (rank == root)
		for (int i = 0; i < 3; i++)
			printf("MAX(%d): P = %d, value = %lf\n", i, out[i].rank, out[i].value);

	MPI_Finalize();

	return 0;
}