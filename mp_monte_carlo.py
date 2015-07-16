from multiprocessing import Process, Event
from multiprocessing.sharedctypes import Array
from ctypes import c_double
from numpy.random import seed, rand
from sys import argv


# -selects points on the xy-plane bounded by 0<=x<=1 and 0<=y<=1
# -tests points to determine if they fall within a specified region
# -tallies results in a global array using an index unique to the
#					current process
def pokeGraph(results_arr, pid):
	# declare array indexes specific to current process
	hit_index = pid * 2
	miss_index = pid * 2 + 1
	l_hit = float(0)
	l_miss = float(0)
	counter = 0

	# seed unique to process
	seed(pid * 99999999)

	# loop until process is terminated
	while True:
		# store results locally until 500000 results are stored
		while counter < 2000000:
			x = rand()
			y = rand()
			dx = x + 2
			dy = y + 2
			# fast, basic check
			if dx + y <= 3 and x + dy <=3:
				l_hit += 1
			# slower, precise check
			elif  (dx**2 + (y)**2 <= 9 \
				and (x)**2 + dy**2 <= 9 ):
				l_hit += 1
			else:
				l_miss += 1
			counter += 1
		# write results to global array
		results_arr[hit_index] += l_hit
		results_arr[miss_index] += l_miss
		counter = 0
	return None


# -reads from global results array and determines area of the region
#       once a sufficient sample size has been reached
def regionArea(results_arr, qty_processes):
	event = Event()

	while True:
		total_hit = 0
		total_miss = 0
		# wait allows CPU to do things in interim
		event.wait(2)
		# iterate through processes and sum results
		for i in range(qty_processes):
			total_hit += results_arr[i * 2]
			total_miss += results_arr[i * 2 + 1]
		total_pokes = total_hit + total_miss
		if (total_pokes >= 50000000000):
			area = total_hit/total_pokes
			# area multiplied by 4 for radial symmetry
			return area * 4



if __name__ == '__main__':
	# if user inputs number of processes
	if (len(argv) > 1):
		qty_processes = int(argv[1])
	else:
		# try to determine system's cpu count
		try:
			from multiprocessing import cpu_count
			qty_processes = cpu_count()
		# default to 1 process
		except NotImplementedError:
			qty_processes = 1

	# init global array that will hold hit and miss counts
	res_arr = Array(c_double, qty_processes * 2, lock=False)
	processes = []

	# prep processes
	for i in range(qty_processes):
		processes.append(Process(target = pokeGraph, args=(res_arr, i)))
		processes[i].start()

	print "Area=%s" % (regionArea(res_arr, qty_processes))

	# once answer was determined, kill processes
	for i in range(qty_processes):
		processes[i].terminate()
