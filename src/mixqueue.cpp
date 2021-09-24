/*
 * queue.cpp
 *
 *  Created on: Jun 5, 2021
 *      Author: cjgree13
 */

#include "mixqueue.h"
#include <mutex>

std::mutex qLock;

MixQueue::MixQueue(int capacity)
{
    capacity_ = capacity;
    front_ = size_ = 0;

    rear_ = capacity - 1;
    musicData = new MusicData[capacity_];
}

bool MixQueue::isFull()
{
    return (size_ == capacity_);
}

bool MixQueue::isEmpty()
{
    return (size_ == 0);
}

QueueStatus MixQueue::enqueue(MusicData item)
{
    if (isFull())
    {
        return FULL;
    }
    rear_ = (rear_ + 1) % capacity_;

    if (qLock.try_lock())
    {
        musicData[rear_] = item;
        size_ = size_ + 1;
        qLock.unlock();
        return LOCK_ACQUIRED;
    }
    else
    {
    	return LOCK_UNAVAILABLE;
    }
}

QueueStatus MixQueue::dequeue(MusicData * item)
{
	item->file = NONE;
	item->msec = 0;

    if (isEmpty())
    {
        return EMPTY;
    }

    if (qLock.try_lock())
    {
        *item = musicData[front_];
        front_ = (front_ + 1) % capacity_;
        size_ = size_ - 1;
        qLock.unlock();
        return LOCK_ACQUIRED;
    }
    else
    {
    	return LOCK_UNAVAILABLE;
    }
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

    item = musicData[front_];
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

    item = musicData[rear_];
    return item;
}
#endif
