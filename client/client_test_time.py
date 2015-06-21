__author__ = 'Bersik'

import argparse
import sys
import os
import time
import datetime
today = datetime.datetime.now()
import threading
import subprocess


args_parser = argparse.ArgumentParser(description='Runs clients.')
args_parser.add_argument('-count_task',metavar='count_task',type=int,default=2500)
args_parser.add_argument('-count_vertex',metavar='count_vertex',type=int,default=1000)
args_parser.add_argument('-count_nodes',metavar='count_nodes',type=int,default=2000)

args = args_parser.parse_args(sys.argv[1:])



def call_cpp():
    global count_ended_task,count_ended_task_with_error
    prog = './client %d %d' % (args.count_vertex,args.count_nodes)
    PIPE = subprocess.PIPE
    p = subprocess.Popen(prog, shell = True,stdout=PIPE)
    p.wait()
    if p.returncode == 0:
        count_ended_task += 1
    else:
        count_ended_task_with_error +=1

start_t = time.time()
count_task = 0
processes = []
count_ended_task = 0
count_ended_task_with_error = 0

for __ in xrange(0, args.count_task):
    start = time.time()
    t = threading.Thread(target=call_cpp)
    t.daemon = True
    t.start()
    processes.append(t)
    count_task += 1
    if time.time() - start >= 1.0:
        break

for i in range(len(processes)):
    try:
        processes[i].join()
    except:
        pass

print "time: %f" % (time.time() - start_t)
print "count task: " , str(count_task)
print "count ended task: " , str(count_ended_task)
print "count task error: %d" % count_ended_task_with_error
print "count vertex: " , str(args.count_vertex)
print "count nodes: " , str(args.count_nodes)


f = open('log2.txt', 'a')
f.write("---------------------------------\n")
f.write("Test at %s\n" % str(today.ctime()))
f.write("time: %f\n" % (time.time() - start_t))
f.write("count task: %d\n" % count_task)
f.write("count ended task: %d\n" % count_ended_task)
f.write("count task error: %d\n" % count_ended_task_with_error)
f.write("count vertex: %s\n" % str(args.count_vertex))
f.write("count nodes: %s\n\n" % str(args.count_nodes))
f.close()
