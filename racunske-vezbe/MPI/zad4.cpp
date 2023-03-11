/*
zad4. Napisati MPI program koji nalazi minimalnu i maximalnu vrednost zadate
promenljive za N procesa kao i identifikatore procesa koji sadrže te vrednosti.
*/

#define MCW MPI_COMM_WORLD

#include <mpi.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

int main(int argc, char** argv)
{
	int rank, size, root;
	struct {
		double value;
		int rank;
	}in, out;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	root = size - 1;

	srand(time(NULL) + rank);	// da bi svaki proces imao drugi broj, 
	// bez srand(time...) ce rand() dodeliti isti nasumicni broj svim procesima
	in.value = (double)(rand() % 15);
	in.rank = rank;
	printf("P%d: in.value = %lf, in.rank = %d\n", rank, in.value, in.rank);

	MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MAXLOC, root, MCW);
	if (rank == root)
		printf("MAX: P%d, value = %lf\n", out.rank, out.value);

	MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, root, MCW);
	if (rank == root)
		printf("MIN: P%d, value = %lf\n", out.rank, out.value);

	MPI_Finalize();

	return 0;
}