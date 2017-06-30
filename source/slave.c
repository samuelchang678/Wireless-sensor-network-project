#include "slave.h"

/*
 * random a number between Min and Max
 */
int random_number(int min, int max){
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void hopping_destination(int tmp, int tag, int bcast_list[4], MPI_Comm wsn_comm){
     if (tag  == GET_TOP){
            MPI_Send(&tmp, 1, MPI_INT, bcast_list[3], EXPAND_TAG, wsn_comm);
     }

     if (tag == GET_BOT){
            MPI_Send(&tmp, 1, MPI_INT, bcast_list[2], EXPAND_TAG, wsn_comm);
     }

     if (tag == GET_RGT){
            MPI_Send(&tmp, 1, MPI_INT, bcast_list[1], EXPAND_TAG, wsn_comm);
     }

     if (tag == GET_LFT){
            MPI_Send(&tmp, 1, MPI_INT, bcast_list[0], EXPAND_TAG, wsn_comm);
     }

}


void slave_node(int rank, MPI_Comm world_comm, MPI_Comm station_comm, double start_time){
    MPI_Comm wsn_comm;
    /*
     * establish a non-periodic cartesian topology  size 4x15
     */
    int dim[2] = {4, 15};
    int period[2] = {0, 0};
    int reorder = 1;
    int left_nbr, right_nbr, top_nbr, bot_nbr;
    int i;
    int local_rank;
    int tmp = 0;
    MPI_Status status;
    MPI_Cart_create(station_comm, 2, dim, period, reorder, &wsn_comm);
    
    //discover adjacent neighbors
    MPI_Cart_shift(wsn_comm, 0, 1, &left_nbr, &right_nbr);
    MPI_Cart_shift(wsn_comm, 1, 1, &bot_nbr, &top_nbr);
    int bcast_list[4] = {left_nbr, right_nbr, bot_nbr, top_nbr};

    srand(time(NULL) + rank); 
   
    MPI_Comm_rank(wsn_comm, &local_rank);

    int data = random_number(MIN, MAX);
    if (data > THRESHOLD) { 
     
        // send to all adjacent neighbors that are not null
        for (i = 0; i < 4; i++){
            if (bcast_list[i] != MPI_PROC_NULL)
                MPI_Send(&local_rank, 1, MPI_INT, bcast_list[i], NEIGHBOR_TAG, wsn_comm);
           
        }
        /*
        printf("rank %d ", local_rank);
        for (i = 0; i < 4; i ++){
            printf("bcast[%d]: %d -- ", i, bcast_list[i]);
        }
        printf("\n");
        */
        //left edge
        if (left_nbr == MPI_PROC_NULL){
            if (top_nbr == MPI_PROC_NULL) {
                //printf("top left rank %d\n", local_rank);
                /*ask for confirmation from neighbors of your bottom neighbor
                 * right node of your bottom neighbor = your lower diagonal neighbor
                 * bottom node of your bottom neighbor = node has 2-unit distance from your node
                 */
                MPI_Send(&tmp, 1, MPI_INT, bot_nbr, GET_RGT, wsn_comm);
                MPI_Send(&tmp, 1, MPI_INT, bot_nbr, GET_BOT, wsn_comm);
            }
            else if (bot_nbr == MPI_PROC_NULL){
                //printf("bot left rank %d\n", local_rank);
                /*
                 * ask for confirmation from neighbors of your top node
                 * right node of your top neighbor = your right diagonal neighbor
                 * top node of your top neighbor = node has 2-unit distance from your node
                 */
                MPI_Send(&tmp, 1, MPI_INT, top_nbr, GET_RGT, wsn_comm);
                MPI_Send(&tmp, 1, MPI_INT, top_nbr, GET_TOP, wsn_comm);
            }
            else {
                //printf("left edge rank %d\n", local_rank);
                /*
                 * ask for confirmation from your lower diagonal neighbor
                 */
                MPI_Send(&tmp, 1, MPI_INT, right_nbr, GET_BOT, wsn_comm);
            }
           
        }
        //right edge
        else if (right_nbr == MPI_PROC_NULL){
            
            if (top_nbr == MPI_PROC_NULL) {
                //printf("top right rank %d\n", local_rank);
                /*
                 * ask for confirmation from neighbors of your bottom neighbor
                 * left node of your bottom neighbor = your lower diagonal neighbor
                 * bottom node of your bottom neighbor = node has 2-unit distance from your node

                 */
                MPI_Send(&tmp, 1, MPI_INT, bot_nbr, GET_LFT, wsn_comm);
                MPI_Send(&tmp, 1, MPI_INT, bot_nbr, GET_BOT, wsn_comm);

            }
            else if (bot_nbr == MPI_PROC_NULL) {
                //printf("bot right rank %d\n", local_rank);
                /*
                 * ask for confirmation from neighbors of your top neighbor
                 * left node of your top neighbor = your lower diagonal neighbor
                 * top node of your top neighbor = node has 2-unit distance from your node

                 */
                MPI_Send(&tmp, 1, MPI_INT, top_nbr, GET_LFT, wsn_comm);
                MPI_Send(&tmp, 1, MPI_INT, top_nbr, GET_TOP, wsn_comm);
            }
            else {
                //printf("right edge rank %d\n", local_rank);
                /*
                 * ask for confirmation from your lower diagonal neighbor
                 */
                MPI_Send(&tmp, 1, MPI_INT, left_nbr, GET_BOT, wsn_comm);
            }
        }
        else if (top_nbr == MPI_PROC_NULL){
            //printf("top edge rank %d\n", local_rank);
            /*
             * ask for confirmation from your left diagonal neighbor
             */
            MPI_Send(&tmp, 1, MPI_INT, bot_nbr, GET_LFT, wsn_comm);
        }
        else if (bot_nbr == MPI_PROC_NULL){
            //printf("bot edge rank %d\n", local_rank);
            /*
             * ask for confirmation from your left diagonal neighbor
             */
            MPI_Send(&tmp, 1, MPI_INT, top_nbr, GET_LFT, wsn_comm);
        }

       
        int value, buffer, event = 0;
       
        for (i =0; i < 20; i++){       
      
        MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, wsn_comm, &status);
               
        if (status.MPI_TAG == CONFIRM_TAG)// counter when receiving confirmation
            event ++;
        
        if (status.MPI_TAG == NEIGHBOR_TAG){
            // receive confirmation requirement from reference node
            // value of buffer is stored for later confirmation
            // send CONFIRM_TAG back to the refernce node
            value = buffer;
            MPI_Send(&value, 1, MPI_INT, value, CONFIRM_TAG, wsn_comm);
        }
        if (status.MPI_TAG == EXPAND_TAG)
            // receive requirement for additional confirmation from bridge node 
            MPI_Send(&tmp, 1, MPI_INT, status.MPI_SOURCE, REPLY_TAG, wsn_comm);
        
        // receive confirmation from diagonal or 2-unit away neighbor
        // send CONFIRM_TAG to the reference node
        if (status.MPI_TAG == REPLY_TAG){
            MPI_Send(&tmp, 1, MPI_INT, value, CONFIRM_TAG, wsn_comm);
        }
        /* receive requirement to become a bridge, 
         * location diagonal neighbor or 2-unit away neighbor based on MPI_TAG
         */
        hopping_destination(tmp, status.MPI_TAG, bcast_list, wsn_comm);
       
        // complete confirmation
        if (event == 4){
            MPI_Send(&data, 1, MPI_INT, 0, 0, world_comm);
            printf("Confirm event for rank %02d with time %lf\n", rank, MPI_Wtime() - start_time);
        }
        }
        
        }
    else {
        // if lower than threshold, listen to data and send back what you receive
        int value, buffer;
       
        for (i =0; i < 20; i++){       
      
        MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, wsn_comm, &status);
        //printf("receive from rank %d data %d\n", status.MPI_SOURCE, value);
        
        if (status.MPI_TAG == NEIGHBOR_TAG){
            // receive confirmation requirement from reference node
            // value of buffer is stored for later confirmation
            // send CONFIRM_TAG back to the refernce node
            value = buffer;
            MPI_Send(&value, 1, MPI_INT, value, CONFIRM_TAG, wsn_comm);
        }
        if (status.MPI_TAG == EXPAND_TAG)
             // receive requirement for additional confirmation from bridge node 
            MPI_Send(&tmp, 1, MPI_INT, status.MPI_SOURCE, REPLY_TAG, wsn_comm);
        
        // receive confirmation from diagonal or 2-unit away neighbor
        // send CONFIRM_TAG to the reference node
        if (status.MPI_TAG == REPLY_TAG){
            MPI_Send(&tmp, 1, MPI_INT, value, CONFIRM_TAG, wsn_comm);
        }
        
        /* receive requirement to become a bridge, 
         * location diagonal neighbor or 2-unit away neighbor based on MPI_TAG
         */
        hopping_destination(tmp, status.MPI_TAG, bcast_list, wsn_comm);
    }
    
    MPI_Comm_free(&wsn_comm);
    }


}

