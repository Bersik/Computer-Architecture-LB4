import threading

__author__ = 'Bersik'

import argparse
import sys
import os
import time

args_parser = argparse.ArgumentParser(description='Runs clients.')

args_parser.add_argument('-count_task',
                         metavar='frequency',
                         type=int,
                         default=1000)

args_parser.add_argument('-count_vertex',
                         metavar='count_vertex',
                         type=int,
                         default=100)

args_parser.add_argument('-count_nodes',
                         metavar='count_nodes',
                         type=int,
                         default=500)


args = args_parser.parse_args(sys.argv[1:])

start_t = time.time()

for __ in xrange(0, args.count_task):
    os.system('./client %d %d' % (args.count_vertex,args.count_nodes))


print "time: " ,time.time() - start_t
print "count task: " , str(args.count_task)
print "count vertex " , str(args.count_vertex)
print "count nodes : " , str(args.count_nodes)