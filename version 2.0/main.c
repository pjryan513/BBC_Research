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
	int arr_size = 10000;

	//if need to print raw
	int printRaw = 0;

	//if need to print BBC compress
	int printBBC = 1;

	//if need to print BBEC compress
	int printBBEC = 1;

	//the array of bytes to be compressed
	unsigned char * to_compress = (unsigned char *) malloc(sizeof(unsigned char) * arr_size);

	int fill = 0;
	int option = 0;
	int i = 0;
	int fill_percent = 100;

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
		for(i = arr_size-3; i < arr_size; i++)
		{
			int temp_boy = 255;
			printf("%u, ", temp_boy);
			to_compress[i] = temp_boy;
		}
		printf("\n");
	}
	else if(option == 1)
	{
		byte temp[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
		to_compress = temp;
	}
	else if(option == 2)
	{
		for(i = 0; i < arr_size; i++)
		{
			to_compress[i] = fill;
		}
	}
	else if(option == 3)
	{
		byte temp[] = {164, 0, 0, 30, 206, 0, 0, 0, 0, 0, 248, 51, 0, 85, 0, 0, 0, 49, 117, 0, 0, 164, 0, 243, 0, 18, 0, 115, 6, 245, 108, 75, 0, 160, 0, 0, 0, 148, 0, 0, 0, 0, 225, 0, 0, 11, 0, 212, 3, 155, 0, 0, 184, 0, 87, 0, 187, 0, 79, 140, 0, 141, 251, 0, 0, 169, 0, 125, 0, 0, 0, 0, 73, 0, 134, 0, 0, 0, 69, 0, 0, 0, 16, 0, 0, 247, 59, 102, 0, 229, 0, 0, 0, 166, 0, 0, 0, 0, 0, 0};
		to_compress = temp;
	}
	else if(option == 4)
	{
		for(i = 0; i < arr_size; i++)
		{
			to_compress[i] = fill;
		}
	}
	else if(option == 5)
	{
		byte temp[] = {6, 0, 0, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 187, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 58, 0, 0, 0, 20, 0, 0, 174, 0, 0, 127, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 78, 192, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 201, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 0, 254, 219, 0, 0, 1, 0, 168, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 107, 0, 214, 140, 0, 193, 0, 0, 0, 114, 0
};
		to_compress = temp;
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
	printf("size in bytes: %u\n\n", arr_size);


	printf("--------	bbc compression:	started	-----------\n\n");
	compressResult * og_compress = bbcCompress(to_compress, arr_size);
	double bbc_ratio = (double)og_compress->size / (double)arr_size;
	printf("--------	bbc compression:	done	-----------\n\n");

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
	printf("\n\n");	
	printf("total size in bytes: %u\ncompression ratio %f\n\n", og_compress->size, bbc_ratio);

	printf("--------	bbec compression:	started	-----------\n\n");
	compressResult * bbec = BBEC(to_compress, arr_size);
	double bbec_ratio = (double)bbec->size / (double)arr_size;
	printf("--------	bbec compression:	done	-----------\n\n");

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
	printf("\n\n");	
	printf("total size in bytes: %u\ncompression ratio %f\n", bbec->size, bbec_ratio);
}