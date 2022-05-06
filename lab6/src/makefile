CC=gcc
path=/home/runner/oslab2019/lab6/src

client: client.c
	$(CC) $(path)/client.c -L$(path) -lutils -pthread -o $(path)/client

server: server.c
	$(CC) $(path)/server.c -L$(path) -lutils -pthread -o $(path)/server

libutils.so: utils.o
	$(CC) -shared utils.o -o libutils.so

utils.o : utils.c
	$(CC) -c -fPIC utils.c -o utils.o

clean :
	rm client server