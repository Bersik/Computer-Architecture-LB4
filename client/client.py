import threading
import subprocess

__author__ = 'Bersik'

import argparse
import sys
import os
import time
import datetime
today = datetime.datetime.now()


args_parser = argparse.ArgumentParser(description='Runs clients.')

args_parser.add_argument('-frequency',metavar='frequency',type=int,default=20)

args_parser.add_argument('-count_vertex',metavar='count_vertex',type=int,default=3000)

args_parser.add_argument('-count_nodes',metavar='count_nodes',type=int,default=6000)

args_parser.add_argument('-sec',metavar='seconds',type=int,default=20)

args = args_parser.parse_args(sys.argv[1:])
count_ended_task = 0
count_ended_task_with_error = 0


def call_cpp():
    global count_ended_task,count_ended_task_with_error
    prog = './client %d %d' % (args.count_vertex,args.count_nodes)
    PIPE = subprocess.PIPE
    try:
        p = subprocess.Popen(prog, shell = True,stdout=PIPE)
        p.wait()
        if p.returncode == 0:
            count_ended_task += 1
        else:
            count_ended_task_with_error +=1
    except:
        pass

count_task = 0
start_t = time.time()
processes = []

while time.time() - start_t < args.sec:
    start = time.time()
    for i in xrange(0, args.frequency):
        t = threading.Thread(target=call_cpp)
        t.daemon = True
        t.start()
        processes.append(t)
        count_task += 1
        if time.time() - start >= 1.0:
            break

    try:
        t = time.time() - start
        if t< 1:
            time.sleep(1.0 - t)
    except:
        pass
    #count_task += args.frequency


print "time(%d): %f" % (args.sec, time.time() - start_t)
print "count task: " , str(count_task)
print "count ended task: " , str(count_ended_task)
print "count task error: %d" % count_ended_task_with_error
print "count vertex: " , str(args.count_vertex)
print "count nodes: " , str(args.count_nodes)


f = open('log.txt', 'a')
f.write("---------------------------------\n")
f.write("Test at %s\n" % str(today.ctime()))
f.write("time(%d): %f\n" % (args.sec, time.time() - start_t))
f.write("count task: %d\n" % count_task)
f.write("count ended task: %d\n" % count_ended_task)
f.write("count task error: %d\n" % count_ended_task_with_error)
f.write("count vertex: %s\n" % str(args.count_vertex))
f.write("count nodes: %s\n\n" % str(args.count_nodes))
f.close()

for i in range(len(processes)):
    try:
        processes[i].terminate()
        processes[i].join()
    except:
        pass