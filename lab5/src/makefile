CC=gcc

mutex: mutex.c
	$(CC) mutex.c -pthread -o mutex

mutex_on: mutex_on.c
	$(CC) mutex_on.c -pthread -o mutex_on

parallel_factorial: parallel_factorial.c
	$(CC) parallel_factorial.c -pthread -o parallel_factorial

deadlock_example: deadlock_example.c
	$(CC) deadlock_example.c -pthread -o deadlock_example

clean :
	rm mutex mutex_on parallel_factorial deadlock_example