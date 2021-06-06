/*
 * queue.cpp
 *
 *  Created on: Jun 5, 2021
 *      Author: cjgree13
 */

#include "mixqueue.h"


MixQueue::MixQueue(int capacity)
{
    capacity_ = capacity;
    front_ = size_ = 0;

    rear_ = capacity - 1;
    array_ = new MusicData[capacity_];
}

bool MixQueue::isFull(MixQueue* queue)
{
    return (queue->size_ == queue->capacity_);
}

bool MixQueue::isEmpty(MixQueue* queue)
{
    return (queue->size_ == 0);
}

void MixQueue::enqueue(MixQueue* queue, MusicData item)
{
    if (isFull(queue))
    {
        return;
    }
    queue->rear_ = (queue->rear_ + 1) % queue->capacity_;
    queue->array_[queue->rear_] = item;
    queue->size_ = queue->size_ + 1;
}

MusicData MixQueue::dequeue(MixQueue* queue)
{
	MusicData item;
	item.file = NONE;
	item.msec = 0;

    if (isEmpty(queue))
    {
        return item;
    }

    item = queue->array_[queue->front_];
    queue->front_ = (queue->front_ + 1) % queue->capacity_;
    queue->size_ = queue->size_ - 1;
    return item;
}

MusicData MixQueue::front(MixQueue* queue)
{
	MusicData item;
	item.file = NONE;
	item.msec = 0;

    if (isEmpty(queue))
    {
        return item;
    }

    item = queue->array_[queue->front_];
    return item;
}

MusicData MixQueue::rear(MixQueue* queue)
{
	MusicData item;
	item.file = NONE;
	item.msec = 0;

    if (isEmpty(queue))
    {
        return item;
    }

    item = queue->array_[queue->rear_];
    return item;
}
