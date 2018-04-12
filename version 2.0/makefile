all : 
	gcc -Wall -c BBCCompressor.c
	gcc -Wall -c BBEC.c
	gcc -Wall -c util.c
	gcc -Wall -o main main.c BBCCompressor.o BBEC.o util.o -lpthread -lm

clean:
	rm -f *.0 main