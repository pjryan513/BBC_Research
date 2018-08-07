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

#include "BBCUtil.h"
#include "BBCCompressor.h"



compressResult * bbcCompress(byte *to_compress, int size);

#endif