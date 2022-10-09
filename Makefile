
build:
	gcc -Wall -g -c functiiCoada-V.c -o functiiCoada-V.o
	gcc -Wall -g -c functiiStiva-V.c -o functiiStiva-V.o
	gcc -Wall -g -c main.c -o main.o
	gcc -Wall -g functiiCoada-V.o functiiStiva-V.o main.o -o tema2

clean:
	rm -f *.o tema2