#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "BBC.h"
#include "BBEC.h"
#include "util.h"


//compressResult * BBEC(byte * to_compress, int size);


//testMain is used to debug the BBEC and BBC algorithms.  
void testMain()
{

	int k = 1 ;
	int resultNum = 20;
	compressResult * resultArrayOG = (compressResult*) malloc(sizeof(compressResult) * resultNum);
	compressResult * resultArrayBBEC = (compressResult*) malloc(sizeof(compressResult) * resultNum);

	int arr_size = 1000;

	//if need to print raw
	int printRaw = 1;

	//if need to print BBC compress
	int printBBC = 1;

	//if need to print BBEC compress
	int printBBEC = 1;



	//the array of bytes to be compressed
		unsigned char * to_compress = (unsigned char *) malloc(sizeof(unsigned char) * arr_size);

		int fill = 0;
		int option = 0;
		int i = 0;
		//printf("k is : %u\n", k);
		int fill_percent = 20;

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

			for(i = arr_size - 5; i < arr_size; i++)
			{
				to_compress[i] = rand() % 256;
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

		printf("\n\n");	
		//printf("total size in bytes: %u\ncompression ratio %f\n\n", og_compress->size, bbc_ratio);

		printf("--------	bbc compression:	started	-----------\n");
		compressResult * og_compress = bbcRun(to_compress, arr_size);
		resultArrayOG[k].size = og_compress->size;
		resultArrayOG[k].compressed_seq = og_compress->compressed_seq;
		//double bbc_ratio = (double)og_compress->size / (double)arr_size;
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

		printf("--------	bbec compression:	started	-----------\n");
		compressResult * bbec = bbecRun(to_compress, arr_size);
		resultArrayBBEC[k].size = bbec->size;
		resultArrayBBEC[k].compressed_seq = bbec->compressed_seq;
		//double bbec_ratio = (double)bbec->size / (double)arr_size;
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
		printf("\n");
}

void testMain2()
{

	int arr_size = 1000;
	//the array of bytes to be compressed
	unsigned char * to_compress = (unsigned char *) malloc(sizeof(unsigned char) * arr_size);

	int fill = 0;
	int option = 0;
	int i = 0;
	//printf("k is : %u\n", k);
	int fill_percent = 20;

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

		for(i = arr_size - 5; i < arr_size; i++)
		{
			to_compress[i] = rand() % 256;
		}
	}

	compressUsingBBEC(to_compress, arr_size);
}


//The main method for the BBC research version 3.  If there are no cmd line arguments then the algorithms will be run 20 times.
//With the probablity of there being a zero increasing from zero up to 100 percent.
//These results are then compared to each other
//If a cmd line arugment is given then testMain will be run
int main(int argc, char * args[])
{
	//seed the random number generator
	srand(time(NULL));

	int k ;
	int resultNum = 20;
	compressResult * resultArrayOG = (compressResult*) malloc(sizeof(compressResult) * resultNum);
	compressResult * resultArrayBBEC = (compressResult*) malloc(sizeof(compressResult) * resultNum);

	int arr_size = 10000000;


	//if need to print raw
	int printRaw = 0;

	//if need to print BBC compress
	int printBBC = 0;

	//if need to print BBEC compress
	int printBBEC = 0;

	if(argc > 1)
	{
		testMain2();
	}
	else
	{
	
	for(k = 0; k <= resultNum; k++)
	{

		//size of the array of bytes to be compressed
			
			

		//the array of bytes to be compressed
		unsigned char * to_compress = (unsigned char *) malloc(sizeof(unsigned char) * arr_size);

		int fill = 0;
		int option = 0;
		int i = 0;
		//printf("k is : %u\n", k);
		int fill_percent = k * 5;

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
		//printf("\n");
		//printf("size in bytes: %u\n", arr_size);


		printf("--------	bbc compression:	started	-----------\n");
		compressResult * og_compress = bbcRun(to_compress, arr_size);
		resultArrayOG[k].size = og_compress->size;
		resultArrayOG[k].compressed_seq = og_compress->compressed_seq;
		//double bbc_ratio = (double)og_compress->size / (double)arr_size;
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
		//printf("\n");	
		//printf("total size in bytes: %u\ncompression ratio %f\n\n", og_compress->size, bbc_ratio);

		printf("--------	bbec compression:	started	-----------\n");
		compressResult * bbec = bbecRun(to_compress, arr_size);
		resultArrayBBEC[k].size = bbec->size;
		resultArrayBBEC[k].compressed_seq = bbec->compressed_seq;
		//double bbec_ratio = (double)bbec->size / (double)arr_size;
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
		//printf("\n");	
		//printf("total size in bytes: %u\ncompression ratio %f\n", bbec->size, bbec_ratio);

		//printf("\n");

		free(to_compress);
		free(og_compress);
		free(bbec);
		
	}

	printf("-----Analyse of Results-----\n\n");
	int i;
	for(i = 0; i <= resultNum; i++)
	{
		double numOG = resultArrayOG[i].size;
		double numBBEC = resultArrayBBEC[i].size;


		printf("for fill percent of %u ", (i * 5));
		if(numOG == numBBEC)
		{
			printf("BBEC and BBC where equal in compression\n");
		}
		else if(numOG > numBBEC)
		{
			printf("BBEC was more effiecient in compression by: %f bytes\n", numOG - numBBEC);
		}
		else
		{
			printf("BBC was more eefiecient in compression by: %f\n", numOG/numBBEC);
		}
		printf("BBEC size: %f | BBC size: %f\n", numBBEC, numOG);
		printf("\n");
	}
	
	}


}