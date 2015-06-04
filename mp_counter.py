from multiprocessing import Process, Lock, Value


# counter method
def add(lock, counter):
	lcounter = 0
	for i in range(5000000):
		lcounter += 1
	# blocking/non-blocking Lock
	lock.acquire()
	counter.value += lcounter
	lock.release()
	return None


# this runs first
if __name__ == '__main__':
	processes = []
	lock = Lock()
	counter = Value('i', 0)

	for i in range(4):
		# create thread and store reference
		processes.append(Process(target=add, args=(lock, counter)))
		#start thread
		processes[i].start()

	for i in range(4):
		# wait on threads to finish
		processes[i].join()

	print "Counter: %s" % (counter.value)
