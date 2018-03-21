//bbcUtil.h

#ifndef BBC_UTIL
#define BBC_UTIL



void startNewRun(blockSeg *param);

void changeRunType(unsigned int run_type, blockSeg *param);

void placeOddBit(blockSeg *param);

void incrementFill(blockSeg *param);



void setExpoStore(blockSeg *param);

void incrementTail(blockSeg *param);

void storeCompress(blockSeg *param);

void printBlock(blockSeg *param);

#endif
