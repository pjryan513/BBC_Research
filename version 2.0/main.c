#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "BBCCompressor.h"


int main(int argc, char * args[])
{
	//seed the random number generator
	srand(time(NULL));


	//size of the array of bytes to be compressed
	int arr_size = 10;

	//the array of bytes to be compressed
	unsigned char * to_compress = (unsigned char *) malloc(sizeof(unsigned char) * arr_size);

	int fill = 0;
	int option = 0;
	int i = 0;

	if(option == 0)
	{
		
		for(;i < arr_size; i++)
		{
			if((rand() % 4) <= 2)
			{
				to_compress[i] = fill;
			}
			else
			{
				to_compress[i] = rand() % 256;	
			} 	
		}
	}
	else if(option == 1)
	{
		byte temp[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
		to_compress = temp;
	}

	printf("the uncompressed seq:\n");
	for(i = 0; i < arr_size; i++)
	{
		printf("%u",to_compress[i]);
		if(i < arr_size -1)
		{
			printf(", ");
		}
	}
	printf("\n");
	printf("size in bytes: %u\n", arr_size);


	printf("--------	original bbc comrpession:	started	-----------\n\n");
	compressResult * og_compress = bbcCompress(to_compress, arr_size);
	printf("--------	original bbc compression:	done	-----------\n\n");

	//compressResult * newCompress = patBBCCompress(to_compress, arr_size);

	printf("compressed seq done by original BBC:\n");
	for(i=0; i < og_compress->size; i++)
	{
		printf("%u", og_compress->compressed_seq[i]);
		if(i < og_compress->size -1)
		{
			printf(", ");
		}

	}
	printf("\n");	
	printf("total size in bytes: %u\n", og_compress->size);
}