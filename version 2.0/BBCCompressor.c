#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "BBCUtil.h"
#include "BBCCompressor.h"

//////////////////////////////////////////////////
//              helper function                 //
//////////////////////////////////////////////////

//gets the type of a RAW byte
void getByteType(runData *param){
  byte b = param->next_byte;
  if(param->next_byte == 0){
    param->byte_type = ZERO_BYTE;
    param->fill_match = 0;
  }
  else if(param->next_byte == 255){
    param->byte_type = ONE_BYTE;
    param->fill_match = 1;
    //param->byte_type = 1;
  }
  //if fill bit == 0, then we can use the ODD OBE BYTE. if the fill bit == 1, then we can use the ODD ZERO BYTE.
  //what if we are starting a new run? (NO FILL BIT DEFINED YET).
  //then we can attach either type of ODD BYTE to the end of a 1-bit-long type 2 run, and adjust the fill bit accordingly.
  //checks to see if there is
  //if(param->fill_bit == 0){
  else if(b == 1 || b == 2 || b == 4 || b == 8 || b == 16 || b == 32 || b == 64 || b == 128){
    param->byte_type =  ODD_BYTE;
    param->fill_match = 0;
  }
  //}
  else if(b == 254 || b == 253 || b == 251 || b ==247 || b ==239 || b ==223 || b == 191 || b == 127){
    param->byte_type = ODD_BYTE;
    param->fill_match = 1;
  }

  else{
    param->byte_type = MESSY_BYTE;
  }
}

int updateRun(runData *param)
{
  if(param->run_type == TYPE_1)
  {
    if(param->fill_len > 3)
    {
      param->run_type = TYPE_3;
      return 0;
    }
    else if(param->byte_type == ODD_BYTE)
    {
      param->run_type = TYPE_2;
      return 0;
    }
  }
  else if(param->run_type == TYPE_3)
  {
    if(param->byte_type == ODD_BYTE)
    {
      param->run_type = TYPE_4;
      return 0;
    }
  }
  return 1;
}

int endRun(runData *param)
{
  if(param->run_type == TYPE_2 || param->run_type == TYPE_4)
  {
    storeCompress(param);
    startNewRun(param);
    return 0;
  }
  else
  {
    
    if(param->byte_type == ONE_BYTE || param->byte_type == ZERO_BYTE)
    {

      if(param->byte_type != param->fill_bit)
      {
        storeCompress(param);
        startNewRun(param);
        return 0;
      }
      else if(param->tail_len > 0)
      {
        storeCompress(param);
        startNewRun(param);
        return 0;
      }
    }
    else if(param->tail_len >= TAIL_LIMIT)
    {
      storeCompress(param);
      startNewRun(param);
      return 0;
    }
  }
  return 1;
}

void storeCompress(runData *param)
{
  if(param->run_type == TYPE_1)
  {
    //blank type 1 header
    byte header = TYPE_1_HEADER;

    //add fill bit
    byte temp = fill_bit;
    temp <<= 7;
    header |= temp;

    //add fill len
    temp = fill_len;
    temp <<= 4;
    header |= temp;

    //add tail len
    header |= tail;

    param->compress->compressed_seq[param->compress->size]; //the reason we can use param->compress->size as the index is because it is not updated till after we store the current data
    param->compress->size++;
  }
  else if(param->run_type == TYPE_2)
  {
    //blank type 2 header
    byte header = TYPE_2_HEADER;

    //add fill bit
    byte temp = fill_bit;
    temp <<= 6;
    header |= temp;

    //add fill len
    temp = fill_len;
    temp <<= 2;
    header |= temp;

    //add odd pos
    int odd_pos = findOddPos(param->next_byte);
  }
}

//////////////////////////////////////////////////
//                  main function               //
//////////////////////////////////////////////////


compressResult * bbcCompress(unsigned char * to_compress, int size){

  //these methods gather information from the header

  runData * param = (runData *) malloc(sizeof(runData));

  param->size = size;
  param->toCompress = to_compress;

  param->compress = (compressResult *) malloc(sizeof(compressResult));
  param->compress->compressed_seq = (unsigned char *) malloc(sizeof(unsigned char));
  param->compress->size = 0;

  int i;

  //This array will hold the result of the compression algorithm
  param->curr_run = (byte*) malloc(sizeof(byte)*size);
  //The size of curr_run array
  
  for(i = 0; i < param->size; i++)
  {
    //printf("starting bbccompress\n");
    //these functions should go in rawbitmapreader.c, for each column there should be a new file.
    //sprintf(compfile, "compressed_%d", i);
    //param->colFile = fopen("filewrite/compressed%d.txt", i, "w");
    
    param->next_byte = param->toCompress[i];//get the next byte from the block sequence of bytes
    //printf("next byte (#%d): %x\n", i, param->next_byte);

    getByteType(param);//get the type of next_byte: zero byte, one byte, odd byte ect ect

    updateRun(param);

    endRun(param);

    if(param->byte_type == ZERO_BYTE)
    {

    }
    else if(param->byte_type == ONE_BYTE)
    {

    }
    else if(param->byte_type == ODD_BYTE)
    {

    }
    else if(param->byte_type == MESSY_BYTE)
    {

    }

  }
  free(param->curr_run);

  return param->compress;
}
