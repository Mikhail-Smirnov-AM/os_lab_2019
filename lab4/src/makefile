CC=gcc
path=/home/runner/oslab2019/lab4/src

parallel_min_max: libutils.so
	$(CC) $(path)/parallel_min_max.c -L$(path) -lutils -o $(path)/parallel_min_max

zombie_example: zombie_example.c
	$(CC) zombie_example.c -o zombie_example

process_memory: process_memory.c
	$(CC) process_memory.c -o process_memory

parallel_sum : parallel_sum.c utils.o
	$(CC) $(path)/parallel_sum.c -L$(path) -lutils  -pthread -o parallel_sum

libutils.so: utils.o
	$(CC) -shared utils.o libutils.so

utils.o : utils.c
	$(CC) -c -fPIC utils.c utils.o

clean :
	rm parallel_min_max.o parallel_min_max parallel_synch.txt zombie_example.o zombie_example process_memory.o process_memory parallel_sum.o parallel_sum