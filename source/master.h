#ifndef _master_h
#define _master_h 

#include <stdio.h>
#include <mpi.h>

void master_node(MPI_Comm world_comm, int size);

#endif
