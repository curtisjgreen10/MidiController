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
    int front_, rear_;
    MusicData* array_;

public:
    int capacity_, size_;
    MixQueue(int capacity);
    bool isFull(MixQueue* queue);
    bool isEmpty(MixQueue* queue);
    void enqueue(MixQueue* queue, MusicData item);
    MusicData dequeue(MixQueue* queue);
    MusicData front(MixQueue* queue);
    MusicData rear(MixQueue* queue);
};

#endif /* INCLUDE_MIXQUEUE_H_ */
