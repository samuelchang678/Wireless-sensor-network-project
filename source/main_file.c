#include "master.h"
#include "slave.h"
#define master_rank 0

int main (int argc, char *argv[]){
    int rank, size;
     
    double start_time = MPI_Wtime();

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    // splitting base station and wsn network
    MPI_Comm station_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank == master_rank , rank, &station_comm);
    
    if (rank == master_rank){
	   
        master_node(MPI_COMM_WORLD, size);
       
    }
    else {
	    slave_node(rank, MPI_COMM_WORLD, station_comm, start_time); 
    }

    MPI_Comm_free(&station_comm);
    MPI_Finalize();
    
    
    return 0;
}
