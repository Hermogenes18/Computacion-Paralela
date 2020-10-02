#include <stdio.h>
#include <string.h>
#include <mpi.h>


int MPI Allgather(
	void∗ 			send_buf_p,
	int 			send_count,
	MPI_Datatype 	send_type,
	void∗ 			recv_buf_p,
	int 			recv_count,
	MPI_Datatype 	recv_type,
	MPI_Comm 		comm );


void Mat vect mult(
		double local A[],
		double local x[],
		double local y[],
		int local m,
		int n,
		int local n,
		MPI_Comm comm) {
	double∗ x;
	int local_i, j;
	int local_ok = 1;

	x = malloc(n∗sizeof(double));
	MPI_Allgather(local x, local n, MPI DOUBLE,
	x, local n, MPI DOUBLE, comm);

	for (local i = 0; local i < local m; local i++) {
		local y[local i] = 0.0;
		for (j = 0; j < n; j++)
			local y[local i] += local A[local i∗n+j]∗x[j];
	}
	free(x);
}