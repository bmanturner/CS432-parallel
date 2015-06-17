import multiprocessing as mp
import random


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

# -selects points on the xy-plane bounded by 0<=x<=10 and 0<=y<=10
# -tests points to determine if they fall within a specified region
# -tallies results in a global array using an index unique to the
#					current process
def pokeGraph(results_arr, pid):
	# declare array indexes specific to current process
	hit_index = pid * 2
	miss_index = pid * 2 + 1

	random.seed()
	# jumpahead(n) ensures each thread is generating a uniquely random number
	random.jumpahead(pid)

	# loop until process is terminated
	while True:
		x = random.uniform(0, 10)
		y = random.uniform(0, 10)
		# if coords are obviously not within region, increment miss counter
		if (x < 4 or x > 6 or y < 4 or y > 6):
			results_arr[miss_index] += 1
		# otherwise, test using exact formula
		elif (isInRegion(x, y)):
			results_arr[hit_index] += 1
		else:
			results_arr[miss_index] += 1

	return None

# -reads from global results array and determines area of the region
# -returns area once it has been determined to sufficient accuracy
def regionArea(results_arr, qty_processes):
	area = 0
	total_pokes = 0

	# loop until answer found
	while True:
		total_hit = 0
		total_miss = 0
		# iterate through processes and sum results
		for i in range(qty_processes):
			total_hit += results_arr[i * 2]
			total_miss += results_arr[i * 2 + 1]
		# ensure total pokes has changed
		if (total_pokes != total_hit + total_miss):
			total_pokes = total_hit + total_miss
			cmp_area = float(total_hit)/total_pokes
			# check difference for accuracy
			if( abs(area - cmp_area) <= 0.00000001 ):
				return cmp_area * 100
			else:
				area = cmp_area



if __name__ == '__main__':
	# try to determine system's cpu count
	try:
		qty_processes = mp.cpu_count()
	except NotImplementedError:
		# default to 1 process
		qty_processes = 1

	# init global array that will hold hit and miss counts
	res_arr = mp.Array('i', range(qty_processes * 2))
	processes = []

	for i in range(qty_processes):
		processes.append(mp.Process(target = pokeGraph, args=(res_arr, i)))
		processes[i].start()

	print "Approx Region Area: %s units" % (regionArea(res_arr, qty_processes))

	# once answer was determined, kill processes
	for i in range(qty_processes):
		processes[i].terminate()


