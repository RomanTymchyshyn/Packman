#include <cmath>
#include "supFunc.h"

bool CircleIntersectRect(float x1, float y1, float width, float height, float x, float y, float r)
{
	if(y > y1)   // ���� ����� ������
	{
		if(x < x1+width/2.0)        // ���� ����� � ����� ���
			return ((x1-x)*(x1-x) + (y1-y)*(y1-y)) <= r*r;
		if(x > x1+width/2.0)          // ���� ����� � ������� ���
			return ((x1+width-x)*(x1+width-x) + (y1-y)*(y1-y)) <= r*r;
		return (y1-y) <= r; // ���� ����� ���������
	}

	if(y < y1-height)   // ���� ����� �����
	{
		if(x < x1+width/2.0)          // ���� ����� ���� �� �������� ��������
			return ((x1-x)*(x1-x) + (y1-height-y)*(y1-height-y)) <= r*r;
		if(x > x1+width/2.0)          // ���� ����� � ����� ���
			return ((x1+width-x)*(x1+width-x) + (y1-height-y)*(y1-height-y)) <= r*r;
		return (y1-height-y) <= r; // ���� ����� ���������
	}

	if(x < x1)   // ���� ����� ����
		return (x1-x) <= r;
	
	if(x > x1+width)   // ���� ����� ������
		return (x1+width-x) <= r;

	return true; // ���� ����� ��������
}

