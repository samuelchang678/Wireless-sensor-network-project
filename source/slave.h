#ifndef _slave_h
#define _slave_h 

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define THRESHOLD 10
#define MIN 1
#define MAX 20

#define NEIGHBOR_TAG 0
#define CONFIRM_TAG 1
#define EXPAND_TAG 2
#define REPLY_TAG 3
#define GET_TOP 10
#define GET_BOT 11
#define GET_LFT 12
#define GET_RGT 13

int random_number(int min, int max);
void hopping_destination(int tmp, int tag, int bcast_list[4], MPI_Comm wsn_comm);
void slave_node(int rank, MPI_Comm world_comm, MPI_Comm station_comm, double start_time);

#endif
