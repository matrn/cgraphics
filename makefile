CFLAGS+=  -pedantic -Wall -std=c99 -O3 -g
ENDFLAGS+= -lm

main: main.c utils.c utils.h matrix.c matrix.h easyppm/easyppm.c
	${CC} ${CFLAGS} main.c utils.c matrix.c easyppm/easyppm.c -o main ${ENDFLAGS}

test:
	./main

clean:
	rm -f *.o
	rm -f main