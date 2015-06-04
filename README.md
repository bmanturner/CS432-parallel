# CS432-python
UAB CS432 - Parallel Programming (Python)

This course is traditionally taught using C/C++. It covers Posix Threads and Distributed Computing (MPI). Python is my language of choice and has access to the same libraries, so I've opted to complete the assignments in Python instead.

## mp_counter.py
The 'hello world' of multiprocessing ported from an example C file discussed during class. Features the creation of multiple (4) threads that simultaneously run a method that counts to a specified integer. The results are then combined and stored under a global counter variable in shared memory. Also includes the use of a lock that prevents multiple threads from modifying the counter simultaneously.
