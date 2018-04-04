#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "BBCCompressor.h"

//////////////////////////////////////////////////
//              helper function                 //
//////////////////////////////////////////////////

//gets the type of a RAW byte
void getByteType(runData *param){
  byte b = param->next_byte;
  if(param->next_byte == 0){
    param->byte_type = ZERO_BYTE;
  }
  else if(param->next_byte == 255){
    param->byte_type = ONE_BYTE;
  }
  //if fill bit == 0, then we can use the ODD OBE BYTE. if the fill bit == 1, then we can use the ODD ZERO BYTE.
  //what if we are starting a new run? (NO FILL BIT DEFINED YET).
  //then we can attach either type of ODD BYTE to the end of a 1-bit-long type 2 run, and adjust the fill bit accordingly.
  //checks to see if there is
  //if(param->fill_bit == 0){
  else if(b == 1 || b == 2 || b == 4 || b == 8 || b == 16 || b == 32 || b == 64 || b == 128){
    param->byte_type =  ODD_BYTE;
  }
  //}
  else if(b == 254 || b == 253 || b == 251 || b ==247 || b ==239 || b ==223 || b == 191 || b == 127){
    param->byte_type = ODD_BYTE;
  }

  else{
    param->byte_type = MESSY_BYTE;
  }
}

int findOddPos(byte oddByte, unsigned int fill_bit)
{
  int i;
  int expo = 1;
  for(i = 0; i < 8; i++)
  {
    if(oddByte == expo || oddByte == (255 - expo))
    {
      return i;
    }

     expo = expo *2;
  }
  return -1;
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
      param->odd_pos = findOddPos(param->next_byte, param->fill_bit);
      return 0;
    }
  }
  else if(param->run_type == TYPE_3)
  {
    if(param->byte_type == ODD_BYTE)
    {
      param->run_type = TYPE_4;
      param->odd_pos = findOddPos(param->next_byte, param->fill_bit);
      return 0;
    }
  }
  return 1;
}

compressResult * fillStore(unsigned int fill_len, byte fill_bit)
{
  int i = 0;
  byte * fill = malloc(sizeof(byte*));
  while(fill_len > FILL_LIMIT_TYPE_3)
  {
    realloc(fill, sizeof(byte *) * (i +1));
    fill[i] = FULL_FILL;

    fill_len -= FILL_LIMIT_TYPE_3; //Even though FULL_FILL is 255 we subtract 127 because only 7-bits are used for storage FSB is used to tell if more fill storage follows
    i++;
  }

  if(fill_len > 0)
  {
    realloc(fill, sizeof(byte*) * (i+1));
    fill[i] = fill_len;
    i++;
  }

  compressResult * fill_done = (compressResult *) malloc(sizeof(compressResult *));
  fill_done->compressed_seq = fill;
  fill_done->size = i;
  return fill_done;
}

void addCompressSeq(runData *param, byte toAdd)
{
  realloc(param->compress->compressed_seq, sizeof(byte*) * (param->compress->size + 1));
  param->compress->compressed_seq[param->compress->size] = toAdd; //the reason we can use param->compress->size as the index is because it is not updated till after we store the current data
  param->compress->size++;
}

void startNewRun(runData * param)
{
  param->fill_len = NEWRUN;
  param->tail_len = NEWRUN;

  int i;
  for(i = 0; i < TAIL_LIMIT; i++)
  {
    param->tail_store[i] = NEWRUN;  
  }

  param->run_type = TYPE_1;
}

void storeCompress(runData *param)
{
  if(param->run_type == TYPE_1)
  {
    //blank type 1 header
    byte header = TYPE_1_HEADER;

    //add fill bit
    byte temp = param->fill_bit;
    temp <<= 6;
    header |= temp;

    //add fill len
    temp = param->fill_len;
    temp <<= 4;
    header |= temp;

    //add tail len
    header |= param->tail_len;

    addCompressSeq(param, header);

    int i;
    for(i = 0; i < param->tail_len; i++)
    {
      addCompressSeq(param, param->tail_store[i]);
    }
  }
  else if(param->run_type == TYPE_2)
  {
    //blank type 2 header
    byte header = TYPE_2_HEADER;

    //add fill bit
    byte temp = param->fill_bit;
    temp <<= 5;
    header |= temp;

    //add fill len
    temp = param->fill_len;
    temp <<= 3;
    header |= temp;

    //add odd pos
    header |= param->odd_pos;

    addCompressSeq(param, header);
  }
  else if(param->run_type == TYPE_3)
  {
    //blank type 3 header
    byte header = TYPE_3_HEADER;

    //add fill bit
    byte temp = param->fill_bit;
    //001x | xxxx
    temp <<= 4;
    header |= temp;

    //add tail
    header |= param->tail_len;

    addCompressSeq(param, header);

    compressResult *three_fill = fillStore(param->fill_len, param->fill_bit);
    int i;
    for(i = 0; i < three_fill->size; i++)
    {
      addCompressSeq(param, three_fill->compressed_seq[i]);
    }
    free(three_fill->compressed_seq);
    free(three_fill);

    for(i = 0; i < param->tail_len; i++)
    {
      addCompressSeq(param, param->tail_store[i]);
    }
  }
  else if(param->run_type == TYPE_4)
  {
    byte header = TYPE_4_HEADER;

    byte temp = param->fill_bit;
    temp <<= 3;
    header |= temp;

    header |= param->odd_pos;

    compressResult *four_fill = fillStore(param->fill_len, param->fill_bit);
    int i;
    for(i = 0; i < four_fill->size; i++)
    {
      addCompressSeq(param, four_fill->compressed_seq[i]);
    }
    free(four_fill->compressed_seq);
    free(four_fill);
  }
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
    else if(param->tail_len > TAIL_LIMIT)
    {
      storeCompress(param);
      startNewRun(param);
      return 0;
    }
  }
  return 1;
}

//////////////////////////////////////////////////
//                  main function               //
//////////////////////////////////////////////////


compressResult * bbcCompress(byte * to_compress, int size){

  //these methods gather information from the header

  runData * param = (runData *) malloc(sizeof(runData));

  param->size = size;
  param->toCompress = to_compress;
  param->run_type = TYPE_1;

  param->tail_store = (byte *) malloc(sizeof(byte) * 15);

  param->compress = (compressResult *) malloc(sizeof(compressResult));
  param->compress->compressed_seq = (byte *) malloc(sizeof(byte));
  param->compress->size = 0;

  int i;

  //This array will hold the result of the compression algorithm
  //param->curr_run = (byte*) malloc(sizeof(byte)*size);
  
  //The size of curr_run array
  
  for(i = 0; i < param->size; i++)
  {
    //printf("starting bbccompress\n");
    //these functions should go in rawbitmapreader.c, for each column there should be a new file.
    //sprintf(compfile, "compressed_%d", i);
    //param->colFile = fopen("filewrite/compressed%d.txt", i, "w");
    
    param->next_byte = param->toCompress[i];//get the next byte from the block sequence of bytes

    getByteType(param);//get the type of next_byte: zero byte, one byte, odd byte ect ect

    if(i > 0)
    { //done for an end case that only can occur when i == 0
      endRun(param);
    }
    
    //If the run is new both fill and tail lens will be zero and we need to choose a new fill bit
    if(param->fill_len <= 0 && param->tail_len <= 0)
    {
      if(param->byte_type == ONE_BYTE)
      {
        param->fill_bit = param->byte_type;
      }
      else
      {
        param->fill_len = ZERO_BYTE;
      }
    }

    if(param->byte_type == ZERO_BYTE || param->byte_type == ONE_BYTE) //if we are a fill we need to increment fill_len
    {
      if(param->byte_type == param->fill_bit)
      {
        param->fill_len++;
      }
    }
    else if(param->byte_type == MESSY_BYTE) //if we are a messy we need to increment tail_len
    {
      param->tail_store[param->tail_len] = param->next_byte;
      param->tail_len++;

    }

    updateRun(param);

  }

  return param->compress;
}
