#include <iostream>
using namespace std;
#include "RangeArray.h"

RangeArray::RangeArray(int i,int j):Array(j-i+1)
{
	low = i;
	high = j;
}

RangeArray::~RangeArray()
{
	delete(data);
    data = NULL;
}

int RangeArray::baseValue()
{
	return low;
}
int RangeArray::endValue()
{
	return high;
}
int& RangeArray::operator [](int i)
{
	return Array::operator [](i-low);
}
int RangeArray::operator [](int i) const
{
	return Array::operator [](i-low);
}
