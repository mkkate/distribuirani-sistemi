/*
Napisati MPI program koji izračunava sumu N celih brojeva (N je
stepen 2) korišćenjem Point-to-point komunikacije.
U prvom koraku, procesi se grupišu u parove (P0, P1), (P2,
P3),…, (Pp-2, Pp-1). Zatim se izačunavaju parcijalne sume u
svim parovima korišćenjem P-to-P komunikacije i akumuliraju u
procesima (P0, P2,…, Pp-2). Npr. process Pi (i-parno) izračunava
parcijalne sume za par procesa (Pi , Pi+1).
U sledećem koraku razmatraju se parovi procesa (P0, P2), (P4,
P6),…, (Pp-4, Pp-2)pronalaze parcijalne sume i akumuliraju u
(P0, P4,…, Pp-4). Postupak se ponavlja dok ne ostanu 2 procesa
i rezultat se akumulira u P0.
*/

#define MCW MPI_COMM_WORLD

#include <mpi.h>
#include <stdio.h>
#include "math.h"

int main(int argc, char** argv)
{
	int rank, size, value, sum, level, num_levels;

	int dst, src;

	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	sum = rank;
	num_levels = (int)log2(size);
	for (int i = 0; i < num_levels; i++)
	{
		level = (int)pow(2, i);
		if ((rank % (2 * level)) == 0)
		{
			src = rank + level;
			MPI_Recv(&value, 1, MPI_INT, src, 0, MCW, &st);
			sum += value;
		}
		else
		{
			dst = rank - level;
			MPI_Send(&sum, 1, MPI_INT, dst, 0, MCW);
			break;
		}
	}

	if (rank == 0)
		printf("%d\n", sum);

	MPI_Finalize();

	return 0;
}