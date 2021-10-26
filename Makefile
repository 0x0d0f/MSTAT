CFLAGS = -Wall -Werror

BINS = mstat

all: $(BINS)

mstat:
	gcc $(CFLAGS) mstat.c -o $@

clean:
	rm -f *.o $(BINS)