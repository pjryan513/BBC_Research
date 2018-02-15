#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "BBCUtil.h"
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

	int i = 0;
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

	compressResult * og_Compress = bbcCompress(to_compress, arr_size);

	compressResult * newCompress = patBBCCompress(to_compress, arr_size);

	printf("the compressed seq:\n");
	for(i=0; i < og_Compress->size; i++)
	{
		printf("%u", og_Compress->compressed_seq[i]);
		if(i < og_Compress->size -1)
		{
			printf(", ");
		}

	}
	printf("\n");	
}