/*
 * queue.h
 *
 *  Created on: Jun 5, 2021
 *      Author: cjgree13
 */

#ifndef INCLUDE_MIXQUEUE_H_
#define INCLUDE_MIXQUEUE_H_

#include <bits/stdc++.h>
#include "audio.h"
using namespace std;


typedef struct
{
	WavFile file;
	float msec;
} MusicData;

class MixQueue {

private:
    int front_, rear_, capacity_, size_;
    MusicData* array_;

public:
    MixQueue(int capacity);
    bool isFull();
    bool isEmpty();
    void enqueue(MusicData item);
    MusicData dequeue();
    int getQueueSize();
#if DEAD_CODE
    MusicData front();
    MusicData rear();
#endif
};

#endif /* INCLUDE_MIXQUEUE_H_ */
