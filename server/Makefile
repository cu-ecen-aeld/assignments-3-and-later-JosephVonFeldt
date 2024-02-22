CC = $(CROSS_COMPILE)gcc

.DEFAULT_GOAL:= aesdsocket
aesdsocket: aesdsocket.o

aesdsocket.o: aesdsocket.c

all : aesdsocket.c
	$(CC) -o aesdsocket aesdsocket.c

clean:
	rm -f *.o aesdsocket