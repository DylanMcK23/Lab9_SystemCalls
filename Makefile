CFLAGS=-c -g -Wall -Werror
CC=gcc

all: info pinfo finfo pmod

info: info.o
	$(CC) -o info info.o

pinfo: pinfo.o
	$(CC) -o pinfo pinfo.o

finfo: finfo.o
	$(CC) -o finfo finfo.o

pmod: pmod.o
	$(CC) -o pmod pmod.o

clean:
	rm -f *.o info pinfo finfo pmod

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<