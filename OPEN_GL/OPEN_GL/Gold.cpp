#include "Diger.h"

Gold::Gold(float x_, float y_, float radius_)
{
	x=x_;
	y=y_;
	radius=radius_;
}

void Gold::Draw()
{
	glBegin( GL_TRIANGLE_FAN );
				glVertex2d(x, y);// вершина в центрі круга
				for(double i = 0.; i <= 6.401; i+=0.1 )
					glVertex2d(cos( i ) * radius + x, sin( i ) * radius + y);
	glEnd();
}