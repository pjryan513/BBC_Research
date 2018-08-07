all : 
	gcc -Wall -c BBCUtil.c
	gcc -Wall -c BBCCompressor.c
	gcc -Wall -o main main.c BBCUtil.o BBCCompressor.o -lpthread -lm

clean:
	rm -f *.0 main