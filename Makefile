CFLAGS=-o2

all: potentiel simplexe

potentiel: main.o mdp.o
			$(CC) -o $@ $^

simplexe: simplexe.o
			$(CC) -o $@ $^

%.o: %.c 
		$(CC) -c -o $@ $<

mdp.o : protos.h

clean:
		rm -f *.o potentiel simplexe

.Phony: clean