all : 
	gcc -Wall -c BBC.c
	gcc -Wall -c BBEC.c
	gcc -Wall -c util.c
	gcc -Wall -o main main.c BBC.o BBEC.o util.o -lpthread -lm

clean:
	rm -f *.0 main