/*****************************************************************************/
/**                                                                         **/
/**     NAME:            Brendan Turner                                     **/
/**     PROJECT:         Assignment #2 - MPI Monte Carlo                    **/
/**     DATE:            07/16/2015                                         **/
/**                                                                         **/
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>


int main(int argc, char **argv) {
    int rank, size, i;
    int root = 0;
    int hits = 0; // index
    int total = 0; // index
    int msg_waiting = 0;
    double results[2] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Status status;
    MPI_Request request;

    // is master process
    if(rank == root) {
        double totalHits = 0;
        double totalPokes = 0;
        double area;

        while (1) {
            // check each slave process for results (non-blocking)
            for (i = 1; i < size; i++) {
                MPI_Iprobe(i, 0, comm, &msg_waiting, &status);
                // if slave process is sending results
                if (msg_waiting) {
                    MPI_Recv(&results, 2, MPI_DOUBLE, i, 0, comm, &status);
                    totalHits += results[hits];
                    totalPokes += results[total];
                }
            }
            if (totalPokes >= 25000000000) {
                area = (totalHits / totalPokes) * 4;
                printf("Area=%.12lf\n", area);
                // send terminating message to each slave process
                for (i = 1; i < size; i++) {
                    MPI_Isend(&area, 1, MPI_DOUBLE, i, 0, comm, &request);
                }
                break;
            }
        }
    // is slave process
    } else {
        double x, y, dx, dy;

        // seed random gen with process specific seed
        srand(rank * 999999999);

        while (1) {
            if (results[total] < 50000000) {
                x = ((double)rand()/(double)RAND_MAX);
                y = ((double)rand()/(double)RAND_MAX);
                dx = x + 2;
                dy = y + 2;
                // fast, basic region check
                if (dx + y <= 3 && x + dy <=3) {
                    results[hits] += 1;
                // slow, precise region check
                } else if (dx*dx + y*y <= 9 && x*x + dy*dy <= 9) {
                    results[hits] += 1;
                }
                results[total] += 1;
            } else {
                // check for terminating message
                MPI_Iprobe(root, 0, comm, &msg_waiting, &status);
                if (msg_waiting) {break;}
                // send round of results to master process
                MPI_Send(&results, 2, MPI_DOUBLE, root, 0, comm);
                results[hits] = 0;
                results[total] = 0;
            }
        }
    }

    MPI_Finalize();
}
