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

bool MixQueue::isFull()
{
    return (size_ == capacity_);
}

bool MixQueue::isEmpty()
{
    return (size_ == 0);
}

void MixQueue::enqueue(MusicData item)
{
    if (isFull())
    {
        return;
    }
    rear_ = (rear_ + 1) % capacity_;
    array_[rear_] = item;
    size_ = size_ + 1;
}

MusicData MixQueue::dequeue()
{
	MusicData item;
	item.file = NONE;
	item.msec = 0;

    if (isEmpty())
    {
        return item;
    }

    item = array_[front_];
    front_ = (front_ + 1) % capacity_;
    size_ = size_ - 1;
    return item;
}

int MixQueue::getQueueSize()
{
	return size_;
}

#if DEAD_CODE
MusicData MixQueue::front()
{
	MusicData item;
	item.file = NONE;
	item.msec = 0;

    if (isEmpty())
    {
        return item;
    }

    item = array_[front_];
    return item;
}

MusicData MixQueue::rear()
{
	MusicData item;
	item.file = NONE;
	item.msec = 0;

    if (isEmpty())
    {
        return item;
    }

    item = array_[rear_];
    return item;
}
#endif
