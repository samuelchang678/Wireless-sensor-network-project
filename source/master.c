#include "master.h"

/*
 * param: world_comm - the communicator between the master and all nodes
 *                      in the wireless sensor network
 *        size - the size of the wireless sensor network
 *
 * master rank sits and listens to event detection message from nodes of WSN
 *
 */
void master_node(MPI_Comm world_comm, int size){
		int total_event = 0;
        int buf, i;
        MPI_Status status;
        for(i=0; i < size - 1; i++){
       
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, world_comm, &status);
            total_event ++;
            printf("event at node %02d with data %d - total: %02d\n",status.MPI_SOURCE, buf, total_event);

        }
}
