//
// Created by Omri on 4/29/2016.
//

#include "MapReduceFramework.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <iterator>
//#include <pthread.h>

#define chunkSize 10

using namespace std;

vector<IN_ITEM> v;
pthread_t threads[];
int incIndex = 0;
MapReduceBase* globMapReduce;

int getIndex(int start){
    //lock mutex
    //get the index
    //unlock the mutex
    //return the index.
}

int getAndIncrementIndex(int start){
    // lock mutex
    //start = incIndex+10
    //unlock the mutex
    //return the mutex
}
void *createChunk(void *threadId){

    int* threadIndexPtr = (int*) threadId;
    int threadIndex = (*threadIndexPtr);

    int runIndex = getIndex(incIndex);

    while((unsigned) runIndex != (v.size() - 1)){

        runIndex = getAndIncrementIndex(incIndex);
        for(int i = runIndex; i < chunkSize; i++){
            //calling the Map function.
        }

        runIndex = getIndex(incIndex);
    }
}

void createThread(int numOfThreads, MapReduceBase& mapFunc){

    for (int i = 0; i < numOfThreads; i++){

        pthread_create(&thread[i], NULL, createChunk,(void *)* i);
    }
}
OUT_ITEMS_LIST runMapReduceFramework(MapReduceBase& mapReduce,
                                     IN_ITEMS_LIST& itemsList, int multiThreadLevel){

    globMapReduce = mapReduce;
    // converts the itemsList from list to vector.
    v.reserve(itemsList.size());
    copy(begin(itemsList), end(itemsList), back_inserter(v));

    pthread_t threads[multiThreadLevel];
    createThread(multiThreadLevel, MapReduceBase);
}

