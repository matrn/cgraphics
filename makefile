CFLAGS+=  -pedantic -Wall -std=c99 -O3 -g
ENDFLAGS+= -lm

main: main.c utils.c utils.h matrix.c matrix.h
	${CC} ${CFLAGS} main.c utils.c matrix.c -o main ${ENDFLAGS}

test:
	./main

clean:
	rm -f *.o
	rm -f main