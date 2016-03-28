#include <cmath>
#include "supFunc.h"

bool CircleIntersectRect(float x1, float y1, float width, float height, float x, float y, float r)
{
	if(y > y1)   // Якщо центр зверху
	{
		if(x < x1+width/2.0)        // Якщо центр в лівому куті
			return ((x1-x)*(x1-x) + (y1-y)*(y1-y)) <= r*r;
		if(x > x1+width/2.0)          // Якщо центр в правому куті
			return ((x1+width-x)*(x1+width-x) + (y1-y)*(y1-y)) <= r*r;
		return (y1-y) <= r; // Якщо центр посередині
	}

	if(y < y1-height)   // Якщо центр знизу
	{
		if(x < x1+width/2.0)          // Якщо центр лівіше за середину квадрата
			return ((x1-x)*(x1-x) + (y1-height-y)*(y1-height-y)) <= r*r;
		if(x > x1+width/2.0)          // Якщо центр в лівому куті
			return ((x1+width-x)*(x1+width-x) + (y1-height-y)*(y1-height-y)) <= r*r;
		return (y1-height-y) <= r; // Якщо центр посередині
	}

	if(x < x1)   // Якщо центр зліва
		return (x1-x) <= r;
	
	if(x > x1+width)   // Якщо центр справа
		return (x1+width-x) <= r;

	return true; // Якщо центр всередині
}

