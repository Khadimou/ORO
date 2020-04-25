CFLAGS=-o2

all: potentiel 

potentiel: main.o mdp.o
			$(CC) -o $@ $^

%.o: %.c 
		$(CC) -c -o $@ $<

mdp.o : protos.h

clean:
		rm -f *.o potentiel 

.Phony: clean