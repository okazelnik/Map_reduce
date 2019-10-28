//
// Created by Omri on 4/29/2016.
//

#include "MapThread.h"

void MapThread::init(int threadId, bool done, IN_ITEM inputPairList) {

    this->id = threadId;
    this->isDone = done;
    this->pairContainer = inputPairList;

}