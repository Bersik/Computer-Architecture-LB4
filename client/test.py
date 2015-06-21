import os
import time

__author__ = 'Bersik'

frequency = 200
count = 0
count_vertex = 1000
count_nodes = 2000

for _ in xrange(0, 1):
    start = time.time()
    for __ in xrange(0, frequency):
        os.system('./client %d %d' % (count_vertex,count_nodes))
        count +=1
    try:
        time.sleep(1.0 - (time.time() - start))
    except:
        pass

print "count = " ,count