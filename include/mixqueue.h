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

typedef enum QueueStatus
{
	FULL = 0,
	EMPTY,
	LOCK_UNAVAILABLE,
	LOCK_ACQUIRED
} QueueStatus;

class MixQueue {

private:
    int front_, rear_, capacity_, size_;
    MusicData* musicData;

public:
    MixQueue(int capacity);
    bool isFull();
    bool isEmpty();
    QueueStatus enqueue(MusicData item);
    QueueStatus dequeue(MusicData * item);
    int getQueueSize();
#if DEAD_CODE
    MusicData front();
    MusicData rear();
#endif
};

#endif /* INCLUDE_MIXQUEUE_H_ */
