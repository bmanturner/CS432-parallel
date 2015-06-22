import multiprocessing as mp
from time import sleep
import random
import sys


# -returns true if provided coords are within region
# -else returns false
# -region: the area created by 4 overlapping circles of
#         radius 3 centered at (3,0),(7,0),(0,3),(0,7)
def isInRegion(x, y):
	r = 3
	if (  (3 - x)**2 + (5 - y)**2 <= r**2 \
		and (7 - x)**2 + (5 - y)**2 <= r**2 \
		and (5 - x)**2 + (3 - y)**2 <= r**2 \
		and (5 - x)**2 + (7 - y)**2 <= r**2 ):
		return True
	else:
		return False


# -selects points on the xy-plane bounded by 4<=x<=6 and 4<=y<=6
# -tests points to determine if they fall within a specified region
# -tallies results in a global array using an index unique to the
#					current process
def pokeGraph(results_arr, pid):
	# declare array indexes specific to current process
	hit_index = pid * 2
	miss_index = pid * 2 + 1

	random.seed()
	# jumpahead(n) ensures each thread is generating a uniquely random number
	random.jumpahead(pid * 999999999)

	# loop until process is terminated
	while True:
		x = random.uniform(4, 6)
		y = random.uniform(4, 6)
		# test if is in region
		if (isInRegion(x, y)):
			results_arr[hit_index] += 1
		else:
			results_arr[miss_index] += 1
	return None


# -reads from global results array and determines area of the region
# -returns area once it has been determined to sufficient accuracy
def regionArea(results_arr, qty_processes):
	area = 0
	# loop until answer found
	while True:
		print "sleep"
		sleep(2)
		total_hit = 0
		total_miss = 0
		# iterate through processes and sum results
		for i in range(qty_processes):
			total_hit += results_arr[i * 2]
			total_miss += results_arr[i * 2 + 1]
		total_pokes = total_hit + total_miss
		cmp_area = float(total_hit)/total_pokes
		# check difference for accuracy
		difference = abs(area - cmp_area)
		if( difference <= 0.0000001 ):
			return cmp_area * 4
		else:
			print "old %:      ", area
			print "new %:      ", cmp_area
			print "difference: ", difference
			area = cmp_area



if __name__ == '__main__':
	# if user inputs number of processes
	if (len(sys.argv) > 1):
		qty_processes = int(sys.argv[1])
	else:
		# try to determine system's cpu count
		try:
			qty_processes = mp.cpu_count()
		except NotImplementedError:
			# default to 1 process
			qty_processes = 1

	print "Initializing %s process(es)" % (qty_processes)

	# init global array that will hold hit and miss counts
	res_arr = mp.Array('i', range(qty_processes * 2), lock=False)
	processes = []

	for i in range(qty_processes):
		processes.append(mp.Process(target = pokeGraph, args=(res_arr, i)))
		processes[i].start()

	print "Approx Region Area: %s units" % (regionArea(res_arr, qty_processes))

	# once answer was determined, kill processes
	for i in range(qty_processes):
		processes[i].terminate()


