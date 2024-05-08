#include <string>
#ifndef CIRCLE
#define CIRCLE
#define ARRAY_SIZE 64
class circularBuffer
{
    public:
    circularBuffer();
    void write(float num);
    float getValAtIndex(int n);
    float mValues[ARRAY_SIZE];

    int mHead;
};
#endif