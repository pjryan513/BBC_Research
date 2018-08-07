//BBCCompressor.h

#ifndef BBEC_COMPRESSOR
#define BBEC_COMPRESSOR
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "BBECUtil.h"
#include "BBECCompressor.h"



compressResult * bbecCompress(byte *to_compress, int size);

#endif