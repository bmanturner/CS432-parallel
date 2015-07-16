# CS432
UAB CS432 - Parallel Programming

CS432 covers Posix Threads and Distributed Computing (MPI). 

## mp_counter.py
The 'hello world' of multiprocessing ported from an example C file discussed during class. Features the creation of multiple (4) threads that simultaneously run a method that counts to a specified integer. The results are then combined and stored under a global counter variable in shared memory. Also includes the use of a lock that prevents multiple threads from modifying the counter simultaneously.

## mp_area.py
Uses multiple processes to determine the area of an arbitrarily defined 2D region by means of generating and testing (x,y) coordinate pairs. An observer method intermittently tests the results until a satisfactory degree of accuracy can be obtained. Features lock-less multiprocessing by assigning each process unique indexes allowing for the modification of an array in shared memory without conflict.
