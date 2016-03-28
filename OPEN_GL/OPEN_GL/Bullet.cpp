#include "Diger.h"

Bullet::Bullet(float x_, float y_, direction dir_, float radius_, float speed_, bool is_)
{
	is=is_;
	x=x_;
	y=y_;
	dir=dir_;
	radius=radius_;
	speed=speed_;
}

void Bullet::move()
{
	if (dir == right && x < Width - radius) x+=speed;
	if (dir == left && x >= radius) x-=speed;
	if (dir == up && y < Height - radius) y+=speed;
	if (dir == down && y>=radius) y-=speed;
}

void Bullet::Draw()
{
	if (is)
	{
		glBegin( GL_TRIANGLE_FAN );
				glVertex2d(x, y);// вершина в центрі круга
				for(double i = 0.; i <= 6.401; i+=0.1 )
					glVertex2d(cos( i ) * radius + x, sin( i ) * radius + y);
		glEnd();
		
		if (x>=Width - radius) is=false;
		if (x<radius) is=false;
		if (y>=Height - radius) is=false;
		if (y<radius) is=false;
	}
}