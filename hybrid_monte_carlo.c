/*****************************************************************************/
/**                                                                         **/
/**     NAME:            Brendan Turner                                     **/
/**     PROJECT:         Assignment #3 - MPI/POSIX Monte Carlo              **/
/**     DATE:            07/23/2015                                         **/
/**                                                                         **/
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <pthread.h>

// passed to individual posix threads
typedef struct t_data {
    int rank;        // parent host's rank
    int id;          // thread id
    double* results; // shared memory results
    pthread_mutex_t* lock;
} t_data;

// host's processor count
int get_cpu_count() {
    char str[256];
    int procCount = 0;
    FILE *fp;
    // linux method
    if( (fp = fopen("/proc/cpuinfo", "r")) ) {
        while(fgets(str, sizeof str, fp))
        if( !memcmp(str, "processor", 9) ) procCount++;
        fclose(fp);
    }
    // mac fallback
    if ( !procCount ) {
        procCount= sysconf(_SC_NPROCESSORS_ONLN);
    }
    return procCount;
}

// thread generates and tests random coordinates (coords either hit or miss)
// stores results in shared memory using indexes unique to thread
void *throw_darts(void *arguments) {
    double x, y, dx, dy;
    double l_hits = 0;
    double l_total = 0;
    t_data *args = (t_data *)arguments;
    int tid = args->id;
    // thread-specific shared mem indexes
    int hits = tid * 2;
    int total = tid * 2 + 1;

    // create seed unique to each thread across all nodes
    unsigned int seed = (tid * args->rank * 99999999);

    args->results[hits] = 0;
    args->results[total] = 0;

    while (1) {
        // stores results locally, periodically updates shared memory
        if (l_total < 50000000) {
            x = ((double)rand_r(&seed)/(double)RAND_MAX);
            y = ((double)rand_r(&seed)/(double)RAND_MAX);
            dx = x + 2;
            dy = y + 2;
            // fast, basic region check
            if (dx + y <= 3 && x + dy <=3) {
                l_hits++;
            // slow, precise region check
            } else if (dx*dx + y*y <= 9 && x*x + dy*dy <= 9) {
                l_hits++;
            }
            l_total++;
        } else {
            pthread_mutex_lock(args->lock);
            // write local results to shared memory
            args->results[hits] += l_hits;
            args->results[total] += l_total;
            pthread_mutex_unlock(args->lock);
            l_hits = 0;
            l_total = 0;
        }
    }
}


int main(int argc, char **argv) {
    int rank, size, i;
    int root = 0;
    int hits = 0;  // index used for 'hits'
    int total = 1; // index used for 'total'
    int msg_waiting = 0;

    double results[2] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Status status;
    MPI_Request request;

    // is root process
    if(rank == root) {
        double area;
        double total_hits = 0;
        double total_pokes = 0;

        while (1) {
            // check each slave process for results (non-blocking)
            for (i = 1; i < size; i++) {
                MPI_Iprobe(i, 0, comm, &msg_waiting, &status);
                // if slave process is sending results
                if (msg_waiting) {
                    MPI_Recv(&results, 2, MPI_DOUBLE, i, 0, comm, &status);
                    total_hits += results[hits];
                    total_pokes += results[total];
                }
            }
            if (total_pokes >= 25000000000) {
                area = (total_hits / total_pokes) * 4;
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
        int cpu_count = get_cpu_count();
        double shared_results[cpu_count * 2];

        pthread_t threads[cpu_count];
        pthread_mutex_t lock;
        pthread_mutex_init(&lock, NULL);
        t_data thread_data[cpu_count];

        for (i = 0; i < cpu_count; i++) {
            thread_data[i].id = i;
            thread_data[i].rank = rank;
            thread_data[i].results = shared_results;
            thread_data[i].lock = &lock;
            pthread_create(&threads[i], NULL, &throw_darts, &thread_data[i]);
        }

        // periodically reads results from shared memory; sends to root process
        while(1) {
            sleep(3);
            // first checks for termination flag from root process
            MPI_Iprobe(root, 0, comm, &msg_waiting, &status);
            if (msg_waiting) {
                // terminate threads
                for (i = 0; i < cpu_count; i++) {
                    pthread_cancel(threads[i]);
                }
                break;
            } else {
                results[hits] = 0;
                results[total] = 0;
                pthread_mutex_lock(&lock);
                for (i = 0; i < cpu_count; i++) {
                    results[hits] += shared_results[i * 2];
                    results[total] += shared_results[i * 2 + 1];
                }
                memset(shared_results, 0, sizeof(shared_results));
                pthread_mutex_unlock(&lock);
                // send results to root process
                MPI_Send(&results, 2, MPI_DOUBLE, root, 0, comm);
            }
        }
    }

    MPI_Finalize();
}
