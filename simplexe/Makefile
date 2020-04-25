CFLAGS=-o2

all: simplexe

simplexe: main.o simplexe.o
			$(CC) -o $@ $^

%.o: %.c 
		$(CC) -c -o $@ $<

simplexe.o : protos.h

clean:
		rm -f *.o simplexe 

.Phony: clean