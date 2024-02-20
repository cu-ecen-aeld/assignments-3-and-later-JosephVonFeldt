CC = $(CROSS_COMPILE)gcc
writer: writer.o

writer.o: writer.c

all : writer.c
	$(CC) -o writer writer.c

clean:
	rm -f *.o writer