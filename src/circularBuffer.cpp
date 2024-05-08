#include <string>
#include "circularBuffer.h"

circularBuffer::circularBuffer()
{

    mHead = 0;
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        mValues[i] = 0.0;
    }
}
float circularBuffer::getValAtIndex(int n){
    return mValues[(mHead+n)%ARRAY_SIZE];
}

void circularBuffer::write(float num)
{
    mValues[mHead] = num;
    mHead = (mHead + 1) % ARRAY_SIZE;
}