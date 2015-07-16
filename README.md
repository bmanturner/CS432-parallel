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
