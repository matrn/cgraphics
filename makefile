CFLAGS+=  -pedantic -Wall -std=c99 -O3 -g
ENDFLAGS+= -lm -limago

main: main.c utils.c utils.h matrix.c matrix.h
	${CC} ${CFLAGS} main.c utils.c matrix.c -o main ${ENDFLAGS}

smiley: smiley.c utils.c utils.h matrix.c matrix.h
	${CC} ${CFLAGS} smiley.c utils.c matrix.c -o smiley ${ENDFLAGS}

test:
	./main

smile:
	./smiley

clean:
	rm -f *.o
	rm -f main
	rm -f smiley