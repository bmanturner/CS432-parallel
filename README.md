# CS432
UAB CS432 - Parallel Programming

CS432 covers Posix Threads and Distributed Computing (via MPI).

## mp_counter.py (Multiprocessing 'Hello World')
The 'hello world' of multiprocessing ported from an example C file discussed during class. Features the creation of multiple (4) threads that simultaneously run a method that counts to a specified integer. The results are then combined and stored under a global counter variable in shared memory. Also includes the use of a lock that prevents multiple threads from modifying the counter simultaneously.

# The Assignment
Determine the area of the region created by four overlapping circles of radius 3 centered at (-2,0), (2,0), (0,-2), and (0,2).

![](http://i.imgur.com/RGJyEAw.jpg)

## The Solution
Because the region is symmetrical across both the X and Y axis, I chose to find the area of the region constrained to the first quadrant (0 <= X <= 1 and 0 <= Y <= 1) and then multiply it by four to determine the total area.

![](http://i.imgur.com/tX8VXHc.jpg)

### mp_monte_carlo.py (Multiprocessing Monte Carlo Simulation)
Uses multiple processes to determine the area of an arbitrarily defined 2D region by means of generating and testing (x,y) coordinate pairs. An observer method intermittently tests the results until a satisfactory degree of accuracy can be obtained. Features lock-less multiprocessing by assigning each process unique indexes allowing for the modification of an array in shared memory without conflict.

#### Requirements
* Python 2.7.x
* [NumPy](http://www.numpy.org/)

#### Running
```
% python mp_monte_carlo.py N
[where N is an integer >= 1]
```


### mpi_monte_carlo.c (Message Passing Interface Monte Carlo Simulation)
Uses multiple hosts to determine the area of an arbitrarily defined 2D region by means of generating and testing (x,y) coordinate pairs. The master process intermittently tests the results until a satisfactory degree of accuracy can be obtained. Features asynchronous communication by means of MPI_Iprobe(), a non-blocking method that determines whether a message is waiting to be received. As a result, slower processes do not impede the performance of faster processes.

#### Requirements
* [MPICH](http://www.mpich.org/)

#### Running
```
% mpicc mpi_monte_carlo.c -o mpi_monte_carlo
% mpirun -np X mpi_monte_carlo
[where N is an integer >= 2]
```


### hybrid_monte_carlo.c (MPI/Threading Monte Carlo Simulation)
Uses multiple hosts to determine the area of an arbitrarily defined 2D region. Each sub-process spawns multiple threads that generate and test coordinate pairs; the results of which are stored in shared memory specific to the parent sub-process. The results are periodically read by the sub-process and sent to the master process. The threads never block and continue to run even while the sub-process is sending results to the master process.


#### Requirements
* [MPICH](http://www.mpich.org/)

#### Running
```
% mpicc hybrid_monte_carlo.c -o hybrid_monte_carlo
% mpirun -host nodeA,nodeB,nodeC,... hybrid_monte_carlo
[where nodeX is host address for each networked computer. List nodeA twice to also run calculations on master computer]
```


## The Exact Solution

This method is far more precise than the solution above, but the calculation is too quick to justify the use of parallel processing. The area of the region defined above can be broken up into the area of a square (green) and the area between the square and the curve (pink).

![](http://i.imgur.com/OWEjpUE.jpg)

### exact_solution.c
Uses Riemann sums to find the area under the curve by representing the curve as a row of rectangles whose height corresponds with the height of the curve. As the number of rectangles increases the width of each individual rectangle decreases, which in turn increases the precision of the result. This script uses 1,000,000,000 rectangles of width 8.7e-10 units to determine the area of the curve (3.63592791634722 units^2).
