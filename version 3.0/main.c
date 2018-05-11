#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "BBCCompressor.h"
#include "BBEC.h"
#include "util.h"


int main(int argc, char * args[])
{
	//seed the random number generator
	srand(time(NULL));


	//size of the array of bytes to be compressed
	int arr_size = 10000000;

	//if need to print raw
	int printRaw = 0;

	//if need to print BBC compress
	int printBBC = 0;

	//if need to print BBEC compress
	int printBBEC = 0;

	//the array of bytes to be compressed
	unsigned char * to_compress = (unsigned char *) malloc(sizeof(unsigned char) * arr_size);

	int fill = 0;
	int option = 0;
	int i = 0;
	int fill_percent = 98;

	if(option == 0)
	{
		
		for(;i < arr_size; i++)
		{
			if((rand() % 100) <= fill_percent)
			{
				to_compress[i] = fill;
			}
			else
			{
				to_compress[i] = rand() % 256;	
			} 	
		}
	}
	
	if(printRaw)
	{
		printf("the uncompressed seq:\n");
		for(i = 0; i < arr_size; i++)
		{
			printf("%u",to_compress[i]);
			if(i < arr_size -1)
			{
				printf(", ");
			}
		}
	}
	printf("\n");
	printf("size in bytes: %u\n", arr_size);


	printf("--------	bbc compression:	started	-----------\n\n");
	compressResult * og_compress = bbcCompress(to_compress, arr_size);
	double bbc_ratio = (double)og_compress->size / (double)arr_size;
	printf("--------	bbc compression:	done	-----------\n");

	//compressResult * newCompress = patBBCCompress(to_compress, arr_size);

	if(printBBC)
	{
		printf("BBC output:\n");
		for(i=0; i < og_compress->size; i++)
		{
			printf("%u", og_compress->compressed_seq[i]);
			if(i < og_compress->size -1)
			{
				printf(", ");
			}
		}
	}
	printf("\n");	
	printf("total size in bytes: %u\ncompression ratio %f\n\n", og_compress->size, bbc_ratio);

	printf("--------	bbec compression:	started	-----------\n\n");
	compressResult * bbec = BBEC(to_compress, arr_size);
	double bbec_ratio = (double)bbec->size / (double)arr_size;
	printf("--------	bbec compression:	done	-----------\n");

	if(printBBEC)
	{
		printf("BBEC output:\n");
		for(i=0; i < bbec->size; i++)
		{
			printf("%u", bbec->compressed_seq[i]);
			if(i < bbec->size -1)
			{
				printf(", ");
			}
		}
	}
	printf("\n");	
	printf("total size in bytes: %u\ncompression ratio %f\n", bbec->size, bbec_ratio);
}