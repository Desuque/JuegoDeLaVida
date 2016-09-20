CC = gcc
CFLAGS = -c
PROG = strlcpy
DEFAULT = generic

c_strlcpy: strlcpy.c
	$(CC) $(CFLAGS) strlcpy.c

as_strlcpy: strlcpy.S
	$(CC) $(CFLAGS) strlcpy.S

strlcpy_usage: strlcpy_usage.c
	$(CC) $(CFLAGS) strlcpy_usage.c

mips: as_strlcpy strlcpy_usage
	$(CC) strlcpy.o strlcpy_usage.o -o $(PROG)

generic: c_strlcpy strlcpy_usage
	$(CC) strlcpy.o strlcpy_usage.o -o $(PROG)

	
clean:
	rm -rf *.o $(PROG)
