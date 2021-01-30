CFLAGS+=  -pedantic -Wall -O3 -g #-std=c99
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