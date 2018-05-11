//BBCCompressor.h

#ifndef BBC_COMPRESSOR
#define BBC_COMPRESSOR
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "util.h"

//#define ZERO_BYTE 0
//#define ONE_BYTE 1
//#define ODD_BYTE 2
//#define MESSY_BYTE 3

int updateRun(runData *param);

int endRun(runData *param);

void storeCompress(runData *param);

void startNewRun(runData *param);

compressResult * bbcCompress(byte *to_compress, int size);

#endif