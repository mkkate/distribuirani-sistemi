/*
zad9. Napisati MPI program koji pronalazi i prikazuje minimalni neparan
broj sa zadatom osobinom i identifikator procesa koji ga sadrži.
Neparni brojevi se nalaze u intervalu [a,b](a i b su zadate konstante).
Osobina koju broj treba da poseduje je da je deljiv zadatom vrednošću x.
Prilikom ispitivanja (da li broj poseduje zadatu osobinu ili ne) svaki proces
generiše i ispituje odgovarajuće neparne brojeve na način prikazan na slici
(za primer broj_procesa=4 i a=3, b=31, x=5).
Konačne rezultate treba da prikaže proces koji sadrži najmanji broj takvih brojeva.
Zadatak rešiti korišćenjem grupnih operacija.
*/

#define MCW MPI_COMM_WORLD
#include <mpi.h>
#include <stdio.h>
#define a 3
#define b 31
#define x 5

int main(int argc, char** argv)
{
	int rank, size;
	int count = 0;

	struct {
		int value;
		int rank;
	}count_in, count_out, min_in, min_out;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	// inicijalne vrednosti za min su maksimalne vrednosti koje postoje
	min_in.value = b;
	min_in.rank = size - 1;

	for (int i = a + 2 * rank; i <= b; i += 2 * size)
	{
		count_in.value = i;
		count_in.rank = rank;

		if (i % x == 0)
		{
			count++;
			if (i < min_in.value)
			{
				min_in.value = i;
				min_in.rank = rank;
			}

		}
	}
	count_in.rank = rank;
	count_in.value = count; // svaki proces je izbrojao koliko ima brojeva deljivih sa x i taj count je u in.value od svakog procesa

	// sada reduce u out smesta najmanji count od svih count-ova, zajedno sa id-em odg. procesa
	MPI_Reduce(&count_in, &count_out, 1, MPI_2INT, MPI_MINLOC, 0, MCW);	// nalazi najmanji broj brojeva (najmanji count) deljivih sa x

	MPI_Bcast(&count_out, 1, MPI_2INT, 0, MPI_COMM_WORLD);	// ybog narednog reduce-a, da bi svi procesi mogli da vide count.rank

	MPI_Reduce(&min_in, &min_out, 1, MPI_2INT, MPI_MINLOC, count_out.rank, MCW);	// nalazi najmanji broj deljiv sa x

	if (rank == count_out.rank)
	{
		printf("Najmanji neparan broj sa osobinom da je deljiv sa %d je %d, sa P[%d]\n", x, min_out.value, min_out.rank);
		printf("Najmanje neparnih brojeva sa osobinom da su deljivi sa %d ima P[%d] = %d\n", x, count_out.rank, count_out.value);
	}

	MPI_Finalize();

	return 0;
}