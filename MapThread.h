//
// Created by Omri on 4/29/2016.
//

#ifndef EX3_MAPTHREAD_H
#define EX3_MAPTHREAD_H

#include "MapReduceClient.h"
#include "MapReduceFramework.h"

typedef struct MapThread{
public:
    void init(int threadId, bool done, IN_ITEMS_LIST inputPair);
    int id;
    bool isDone;
    IN_ITEMS_LIST pairContainer;

}MapThread;
#endif //EX3_MAPTHREAD_H

