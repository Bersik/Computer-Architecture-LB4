import threading

__author__ = 'Bersik'

import argparse
import sys
import os
import time

args_parser = argparse.ArgumentParser(description='Runs clients.')

args_parser.add_argument('-frequency',
                         metavar='frequency',
                         type=int,
                         default=200)

args_parser.add_argument('-count_vertex',
                         metavar='count_vertex',
                         type=int,
                         default=100)

args_parser.add_argument('-count_nodes',
                         metavar='count_nodes',
                         type=int,
                         default=500)

args_parser.add_argument('-sec',
                         metavar='seconds',
                         type=int,
                         default=10)


args = args_parser.parse_args(sys.argv[1:])


def call_cpp():
    os.system('./client %d %d' % (args.count_vertex,args.count_nodes))

count_task = 0
start_t = time.time()

while time.time() - start_t < args.sec:
    start = time.time()
    for __ in xrange(0, args.frequency):
        t = threading.Thread(target=call_cpp)
        t.start()
        #os.system('./client %d %d' % (args.count_vertex,args.count_nodes))
    try:
        time.sleep(1.0 - (time.time() - start))
    except:
        pass
    count_task += args.frequency


print "time: " ,time.time() - start_t
print "count task: " , str(count_task)
print "count vertex " , str(args.count_vertex)
print "count nodes : " , str(args.count_nodes)
