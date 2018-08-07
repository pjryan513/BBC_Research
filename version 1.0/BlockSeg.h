#ifndef BLOCKSEG_H_
#define BLOCKSEG_H_

#include "Control.h"

//struct that holds uncompressed data to send to compressor
typedef struct blockSeg{
	int size;//number of words in toCompress
	FILE *colFile;//where all the compressed words are going
	
	byte *compressBytes; //the current block of uncompressed bytes
	//struct blockBytes *nextBlock; //the next block of bytes to compress
	unsigned int fill_len;
	unsigned int tail_len;
	unsigned int run_type;
	byte fill_match; //either 0 or 1 depending on if we're a
	byte fill_bit;
	byte next_byte;
	unsigned int byte_type;
	byte header;
	byte *curr_run; /*the current run (an array of bytes, in case we have literals, counter bytes, etc.)*/
	unsigned int curr_size; //the length of the curr_run array

} blockSeg;

#endif /* BLOCKSEG_H_ */
