#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "BBEC.h"
#include "util.h"

//////////////////////////////////////////////////
//              helper function                 //
//////////////////////////////////////////////////


int updateRunEx(runData *param)
{
  if(param->run_type == TYPE_1)
  {
    if(param->fill_len > 3)
    {
      param->run_type = TYPE_3;
      return 0;
    }
    else if(param->byte_type == ONE_ODD_BYTE || param->byte_type == ZERO_ODD_BYTE)
    {
      param->run_type = TYPE_2;
      param->odd_pos = findOddPos(param->next_byte, param->fill_bit);
      return 0;
    }
  }
  else if(param->run_type == TYPE_3)
  {
    if(param->fill_len > 127)
    {
      param->run_type = TYPE_4;
      return 0;
    }
  }
  return 1;
}

void startNewRunEx(runData * param)
{
  param->fill_len = NEWRUN;
  param->tail_len = NEWRUN;

  int i;
  for(i = 0; i < TAIL_LIMIT; i++)
  {
    param->tail_store[i] = NEWRUN;  
  }

  param->fill_bit = 0;
  param->run_type = TYPE_1;
}

baseExpo * expoDecomp(int num)
{
  int x = num;
  int base = 1;
  int expo = 1;

  int i;
  for(i = 9; i > 1; i--)
  {
    int j = 1;
    while(pow(i,j) < x && j < 129)
    {
      j++;
    }
    j--; //we dec because we need to go until we are beyond x so take a step back and that is the closest value
    if(x-pow(i,j) < x-pow(base,expo))
    {
      base = i;
      expo = j;
    }
  }
  baseExpo *ret = (baseExpo*) malloc(sizeof(baseExpo));
  ret->base = base;
  ret->expo = expo;

  return ret;
}

void storeCompressEx(runData *param)
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
    //printf("------TYPE_3 store -----------\n");

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
    //printf("----------TYPE_4-----------\n");

    int comp_style = 0;

    if(comp_style == 0)
    {
      int num = param->fill_len;
      while(num > 127)
      {
        byte header = TYPE_4_HEADER;

        byte temp = param->fill_bit;
        temp <<= 3;
        header |= temp;

        baseExpo * bE = expoDecomp(num);

        //we substract two because for our purpose having base below 2 is useless.
        //Example base is 5 but we can store it as 3 because we know that 0 and 1 spots are not needed
        // so 0 means 2, 1 means 3 ect ect
        header |= (bE->base - 2);  

        addCompressSeq(param,header);

        addCompressSeq(param,(byte) (bE->expo -2));

        num -= pow(bE->base,bE->expo);

        if((bE->base - 2) < 0 || (bE->expo -2) < 0)
        {
          printf("HOLD ON: base and/or expo below zero, NOT GOOD\n");
        }
      }
      if(num > 3)
      {
        param->compress->compressed_seq[param->compress->size-1] |= 128;
        addCompressSeq(param,num);
        num = 0;
      }
      if(num > 0 || param->tail_len > 0)
      {
        runData * tempRun = (runData*) malloc(sizeof(runData));
        tempRun->fill_len = num;
        tempRun->tail_len = param->tail_len;
        tempRun->tail_store = param->tail_store;
        tempRun->byte_type = param->byte_type;
        tempRun->compress = (compressResult *) malloc(sizeof(compressResult));
        tempRun->compress->compressed_seq = param->compress->compressed_seq;
        tempRun->compress->size = param->compress->size;


        tempRun->run_type = TYPE_1;   
        
        storeCompressEx(tempRun); 

        param->compress->compressed_seq = tempRun->compress->compressed_seq;
        param->compress->size = tempRun->compress->size;
        free(tempRun);
      }
      
    }
    else if(comp_style == 1)
    {

    }
  }
}

int endRunEx(runData *param)
{
  if(param->run_type == TYPE_2)
  {
    //printf("------ending 1--------\n");
    storeCompressEx(param);

    //printf("---run is done--- \n");
    //printCompressData(param->compress);

    startNewRunEx(param);
    return 0;
  }
  if((param->run_type == TYPE_3 || param->run_type == TYPE_4) && (param->byte_type == ZERO_ODD_BYTE || param->byte_type == ONE_ODD_BYTE))
  {
    
    //printf("------ending 2--------\n");
    storeCompressEx(param);

    //printf("---run is done--- \n");
    //printCompressData(param->compress);

    startNewRunEx(param);
    
  }
  else if(param->byte_type == ONE_BYTE || param->byte_type == ZERO_BYTE  || param->byte_type == ZERO_ODD_BYTE || param->byte_type == ONE_ODD_BYTE)
  {
    if(param->comp_fill_bit != param->fill_bit)
    {
      //printf("------ending 3--------\n");
      storeCompressEx(param);

      //printf("---run is done---\n");
      //printCompressData(param->compress);

      startNewRunEx(param);
      return 0;
    }
    if(param->tail_len > 0)
    {
      //printf("------ending 4--------\n");
      storeCompressEx(param);

      //printf("---run is done---\n");
      //printCompressData(param->compress);

      startNewRunEx(param);
      return 0;
    }
  }
  if(param->tail_len > TAIL_LIMIT)
  {
    //printf("------ending 5--------\n");
    storeCompressEx(param);

    //printf("---run is done---\n");
    //printCompressData(param->compress);

    startNewRunEx(param);
    return 0;
  }
  return 1;
}

//////////////////////////////////////////////////
//                  main function               //
//////////////////////////////////////////////////

compressResult * BBEC(byte * to_compress, int size){

  runData * param = (runData *) malloc(sizeof(runData));

  param->size = size;
  param->toCompress = to_compress;
  param->run_type = TYPE_1;

  param->tail_store = (byte *) malloc(sizeof(byte) * 15);

  param->compress = (compressResult *) malloc(sizeof(compressResult));
  param->compress->compressed_seq = (byte *) malloc(sizeof(byte));
  param->compress->size = 0;

  startNewRunEx(param);

  int i;

  //int k = 0;

  for(i = 0; i < param->size; i++)
  {
    
    param->next_byte = param->toCompress[i];//get the next byte from the block sequence of bytes

    getByteType(param);//get the type of next_byte: zero byte, one byte, odd byte ect ect

    //If the run is new both fill and tail lens will be zero and we need to choose a new fill bit

    if(i > 0)
    { //done for an end case that only can occur when i == 0
      endRunEx(param);
    }

    if(param->fill_len <= 0 && param->tail_len <= 0)
    {
      if(param->byte_type == ONE_BYTE || param->byte_type == ONE_ODD_BYTE)
      {
        param->fill_bit = 1;
      }
      else
      {
        param->fill_bit = 0;
      }
    }
    
    /*if(param->fill_bit == 0 && param->byte_type == ONE_ODD_BYTE)
    {
      param->byte_type = MESSY_BYTE;
    }
    else if(param->fill_bit == 1 && param->byte_type == ZERO_ODD_BYTE)
    {
      param->byte_type = MESSY_BYTE;
    }*/

    /*if((param->run_type == TYPE_3 || param->run_type == TYPE_4) && (param->byte_type == ZERO_ODD_BYTE || param->byte_type == ONE_ODD_BYTE))
    {
      param->byte_type = MESSY_BYTE;
    }*/

    if(param->byte_type == ZERO_BYTE || param->byte_type == ONE_BYTE) //if we are a fill we need to increment fill_len
    {
      if(param->comp_fill_bit == param->fill_bit)
      {
        param->fill_len++;
      }
    }
    else if(param->byte_type == MESSY_BYTE) //if we are a messy we need to increment tail_len
    {
      param->tail_store[param->tail_len] = param->next_byte;
      param->tail_len++;

    }
    updateRunEx(param);


    /*if(k > 110)
    {
      printRunData(param);
    }
    k++;*/

  }

  storeCompressEx(param);

  return param->compress;
}
